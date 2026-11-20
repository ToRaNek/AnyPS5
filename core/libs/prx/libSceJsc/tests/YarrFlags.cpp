#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libSceJsc/libSceJsc.hpp"

#include <bit>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

using Jsc::WtfAbi::StringView;
using Jsc::YarrAbi::OptionalFlags;

namespace {

using RawParse = unsigned char* (APS5_VABI *)(unsigned char* result, StringView);
const auto rawParse = std::bit_cast<RawParse>(&_ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE);

void Check(bool value, int line) {
    if (!value) {
        std::fprintf(stderr, "Yarr flags check failed at line %d\n", line);
        std::abort();
    }
}

#define Require(value) Check((value), __LINE__)

StringView View8(const char* text) {
    return {text, static_cast<unsigned>(std::strlen(text)), true};
}

StringView View16(const std::u16string& text) {
    return {text.data(), static_cast<unsigned>(text.size()), false};
}

bool Parsed(StringView view, std::uint8_t expected) {
    const OptionalFlags result = _ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE(view);
    unsigned char raw[4] = {0xcd, 0xcd, 0xcd, 0xcd};
    if (rawParse(raw, view) != raw) return false;
    if (raw[0] != 1 || raw[1] != expected || raw[2] != 0xcd || raw[3] != 0xcd) return false;
    return result.engaged && result.value == expected;
}

bool Rejected(StringView view) {
    unsigned char raw[4] = {0xcd, 0xcd, 0xcd, 0xcd};
    if (rawParse(raw, view) != raw) return false;
    if (raw[0] != 0 || raw[1] != 0 || raw[2] != 0xcd || raw[3] != 0xcd) return false;
    return !_ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE(view).engaged;
}

bool Unknown(StringView view) {
    try {
        _ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE(view);
    } catch (const std::invalid_argument&) {
        return false;
    } catch (const std::runtime_error&) {
        return true;
    }
    return false;
}

void TestLatin1() {
    Require(Parsed(View8(""), 0));
    Require(Parsed(View8("g"), 0x01));
    Require(Parsed(View8("i"), 0x02));
    Require(Parsed(View8("m"), 0x04));
    Require(Parsed(View8("y"), 0x08));
    Require(Parsed(View8("u"), 0x10));
    Require(Parsed(View8("s"), 0x20));
    Require(Parsed(View8("gimsuy"), 0x3F));
    Require(Parsed(View8("yusmig"), 0x3F));
    Require(Rejected(View8("gg")));
    Require(Rejected(View8("gimsuyg")));
    Require(Rejected(View8("v")));
    Require(Rejected(View8("uv")));
    Require(Rejected(View8("vu")));
    Require(Rejected(View8("x")));
    Require(Rejected(View8("G")));
    Require(Rejected(View8("g ")));
    const char withNul[] = {'g', '\0', 'i'};
    Require(Rejected(StringView{withNul, 3, true}));
    Require(Parsed(StringView{"gix", 2, true}, 0x03));
}

void TestHasIndices() {
    Require(Unknown(View8("d")));
    Require(Unknown(View8("dg")));
    Require(Unknown(View8("gimsuyd")));
    Require(Rejected(View8("dd")));
    Require(Rejected(View8("dgd")));
    Require(Rejected(View8("dx")));
    Require(Rejected(View8("xd")));
    Require(Rejected(View8("dgg")));
    Require(Rejected(View8("dv")));
    const std::u16string wide = u"dy";
    Require(Unknown(View16(wide)));
}

void TestUtf16() {
    const std::u16string gi = u"gi";
    Require(Parsed(View16(gi), 0x03));
    const std::u16string wide = u"g\u0100";
    Require(Rejected(View16(wide)));
    const std::u16string highByte = u"\u0167";
    Require(Rejected(View16(highByte)));
    const std::u16string all = u"gimsuy";
    Require(Parsed(View16(all), 0x3F));
}

void TestNullCharacters() {
    Require(Parsed(StringView{nullptr, 0, true}, 0));
    bool threw = false;
    try {
        _ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE(StringView{nullptr, 1, true});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    Require(threw);
}

}

int main() {
    TestLatin1();
    TestHasIndices();
    TestUtf16();
    TestNullCharacters();
    return 0;
}
