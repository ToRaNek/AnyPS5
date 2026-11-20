#include "prx/libc/include/general/VabiMacros.hpp"
#include "prx/libSceJsc/libSceJsc.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

using Jsc::JsonAbi::Value;
using Jsc::JsonAbi::ValueReference;
using Jsc::JsonAbi::ValueVtable;

namespace {

const auto AsBoolean = &_ZNK3WTF8JSONImpl5Value9asBooleanERb;
const auto AsValue = &_ZN3WTF8JSONImpl5Value7asValueERNS_6RefPtrIS1_NS_13DumbPtrTraitsIS1_EEEE;

void Check(bool value, int line) {
    if (!value) {
        std::fprintf(stderr, "JSON value check failed at line %d\n", line);
        std::abort();
    }
}

#define Require(value) Check((value), __LINE__)

int deleted = 0;
Value* lastDeleted = nullptr;
std::uint32_t countAtDeletion = 0;

void APS5_VABI CompleteDestructor(Value*) {
    std::abort();
}

void APS5_VABI DeletingDestructor(Value* value) {
    ++deleted;
    lastDeleted = value;
    countAtDeletion = value->refCount;
}

const ValueVtable TestVtable{&CompleteDestructor, &DeletingDestructor};

Value Make(std::uint32_t type, std::uint32_t refCount = 1) {
    Value value{};
    value.vtable = &TestVtable;
    value.refCount = refCount;
    value.type = type;
    return value;
}

template<typename TAction> bool ThrowsInvalidArgument(TAction action) {
    try {
        action();
    } catch (const std::invalid_argument&) {
        return true;
    } catch (...) {
        return false;
    }
    return false;
}

void TestLayout() {
    Value value = Make(1, 3);
    value.boolean = true;
    const auto* bytes = reinterpret_cast<const unsigned char*>(&value);
    std::uint32_t count = 0;
    std::uint32_t type = 0;
    std::memcpy(&count, bytes + 8, 4);
    std::memcpy(&type, bytes + 12, 4);
    Require(count == 3 && type == 1 && bytes[16] == 1);
}

void TestAsBoolean() {
    for (const bool stored : {false, true}) {
        Value value = Make(1);
        value.boolean = stored;
        bool output = !stored;
        Require(AsBoolean(&value, &output) && output == stored);
    }
    for (const std::uint32_t type : {0u, 2u, 3u, 4u, 5u, 6u, 7u, 0xFFFFFFFFu}) {
        Value value = Make(type);
        value.number = 1.0;
        bool output = true;
        Require(!AsBoolean(&value, &output) && output);
        output = false;
        Require(!AsBoolean(&value, &output) && !output);
    }
    Value value = Make(1);
    bool output = false;
    Require(ThrowsInvalidArgument([&] { AsBoolean(nullptr, &output); }));
    Require(ThrowsInvalidArgument([&] { AsBoolean(&value, nullptr); }));
}

void TestAsValue() {
    Value self = Make(5, 1);
    ValueReference output{nullptr};
    Require(AsValue(&self, &output));
    Require(output.value == &self && self.refCount == 2 && deleted == 0);

    Require(AsValue(&self, &output));
    Require(output.value == &self && self.refCount == 2 && deleted == 0);

    Value shared = Make(2, 2);
    ValueReference holdsShared{&shared};
    Require(AsValue(&self, &holdsShared));
    Require(holdsShared.value == &self && self.refCount == 3 && shared.refCount == 1 && deleted == 0);

    Value last = Make(4, 1);
    ValueReference holdsLast{&last};
    Require(AsValue(&self, &holdsLast));
    Require(holdsLast.value == &self && self.refCount == 4);
    Require(deleted == 1 && lastDeleted == &last && countAtDeletion == 1 && last.refCount == 1);

    Value single = Make(0, 1);
    ValueReference holdsSingle{&single};
    Require(AsValue(&single, &holdsSingle));
    Require(holdsSingle.value == &single && single.refCount == 1 && deleted == 1);

    Value noDestructor = Make(0, 1);
    noDestructor.vtable = nullptr;
    ValueReference holdsNoDestructor{&noDestructor};
    Require(ThrowsInvalidArgument([&] { AsValue(&self, &holdsNoDestructor); }));
    Require(holdsNoDestructor.value == &noDestructor && self.refCount == 4 && noDestructor.refCount == 1);
    Require(ThrowsInvalidArgument([&] { AsValue(nullptr, &output); }));
    Require(ThrowsInvalidArgument([&] { AsValue(&self, nullptr); }));
    Require(self.refCount == 4 && deleted == 1);
}

}

int main() {
    TestLayout();
    TestAsBoolean();
    TestAsValue();
    return 0;
}
