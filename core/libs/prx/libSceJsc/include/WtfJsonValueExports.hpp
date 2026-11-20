#pragma once

#include "prx/libc/include/general/VabiMacros.hpp"

#include <cstddef>
#include <cstdint>

namespace Jsc::JsonAbi {

inline constexpr std::uint32_t ValueTypeBoolean = 1;

struct Value;

struct ValueVtable {
    void (APS5_VABI* completeDestructor)(Value*);
    void (APS5_VABI* deletingDestructor)(Value*);
};

struct Value {
    const ValueVtable* vtable;
    std::uint32_t refCount;
    std::uint32_t type;
    union {
        bool boolean;
        double number;
        void* string;
    };
};

struct ValueReference {
    Value* value;
};

static_assert(sizeof(Value) == 24);
static_assert(offsetof(Value, refCount) == 8);
static_assert(offsetof(Value, type) == 12);
static_assert(offsetof(Value, boolean) == 16);
static_assert(sizeof(ValueReference) == 8);

}

extern "C" {

bool APS5_VABI _ZNK3WTF8JSONImpl5Value9asBooleanERb(const Jsc::JsonAbi::Value* self, bool* output);
bool APS5_VABI _ZN3WTF8JSONImpl5Value7asValueERNS_6RefPtrIS1_NS_13DumbPtrTraitsIS1_EEEE(Jsc::JsonAbi::Value* self, Jsc::JsonAbi::ValueReference* output);

}
