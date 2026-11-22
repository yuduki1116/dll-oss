#include "Translation.hpp"
#include "Utils.hpp"
#include <json/json.hpp>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <mutex>

namespace {
    std::unordered_map<std::string, std::string> g_map;
    std::mutex g_mutex;
    bool g_loaded = false;

    void loadLocked() {
        g_map.clear();
        g_loaded = true;
        try {
            std::filesystem::path p(Utils::getClientPath());
            p /= "Lang";
            p /= "ja.json";
            if (!std::filesystem::exists(p)) return;
            std::ifstream f(p, std::ios::binary);
            if (!f) return;
            nlohmann::json j = nlohmann::json::parse(f, nullptr, true, true);
            if (!j.is_object()) return;
            for (auto it = j.begin(); it != j.end(); ++it) {
                if (it.value().is_string()) g_map[it.key()] = it.value().get<std::string>();
            }
        } catch (...) {
            g_map.clear();
        }
    }
}

std::string Translation::tr(const std::string& text) {
    if (text.empty()) return text;
    std::lock_guard<std::mutex> lock(g_mutex);
    if (!g_loaded) loadLocked();
    if (g_map.empty()) return text;
    auto it = g_map.find(text);
    return it != g_map.end() ? it->second : text;
}

void Translation::reload() {
    std::lock_guard<std::mutex> lock(g_mutex);
    loadLocked();
}
