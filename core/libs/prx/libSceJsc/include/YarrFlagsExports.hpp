#pragma once

#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libSceJsc/src/WtfStringAbi.hpp"

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace Jsc::YarrAbi {

enum class Flags : std::uint8_t {
    Global = 1 << 0,
    IgnoreCase = 1 << 1,
    Multiline = 1 << 2,
    Sticky = 1 << 3,
    Unicode = 1 << 4,
    DotAll = 1 << 5,
};

struct OptionalFlags {
    bool engaged;
    std::uint8_t value;
    OptionalFlags() : engaged(false), value(0) {}
    explicit OptionalFlags(std::uint8_t flags) : engaged(true), value(flags) {}
    OptionalFlags(const OptionalFlags& other) : engaged(other.engaged), value(other.value) {}
};

static_assert(sizeof(OptionalFlags) == 2);
static_assert(alignof(OptionalFlags) == 1);
static_assert(offsetof(OptionalFlags, engaged) == 0);
static_assert(offsetof(OptionalFlags, value) == 1);
static_assert(!std::is_trivially_copyable_v<OptionalFlags>);

} // namespace Jsc::YarrAbi

extern "C" {

Jsc::YarrAbi::OptionalFlags APS5_VABI _ZN3JSC4Yarr10parseFlagsEN3WTF10StringViewE(Jsc::WtfAbi::StringView string);

}
