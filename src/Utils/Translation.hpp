#pragma once
#include <string>

// 日本語化(ja.json)用の翻訳ルックアップ。
// %appdata%\Flarial\(Client\)Lang\ja.json があれば有効。無ければ英語のまま。
namespace Translation {
    // 訳があれば訳を、無ければ元の文字列をそのまま返す(UTF-8)
    std::string tr(const std::string& text);
    // ja.json を読み直す
    void reload();
}
