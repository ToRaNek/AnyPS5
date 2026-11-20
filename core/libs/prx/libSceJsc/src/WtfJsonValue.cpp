// Derived from Source/WTF/wtf/JSONValues.cpp@c55364d53f971f63a709976f168d932d558b8909
/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 * Copyright (C) 2014 University of Washington. All rights reserved.
 * Copyright (C) 2017-2019 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "prx/libSceJsc/include/WtfJsonValueExports.hpp"

#include <stdexcept>

using Jsc::JsonAbi::Value;
using Jsc::JsonAbi::ValueReference;

extern "C" {

bool APS5_VABI _ZNK3WTF8JSONImpl5Value9asBooleanERb(const Value* self, bool* output) {
    if (!self) throw std::invalid_argument("WTF::JSONImpl::Value::asBoolean: null this");
    if (!output) throw std::invalid_argument("WTF::JSONImpl::Value::asBoolean: null output");
    if (self->type != Jsc::JsonAbi::ValueTypeBoolean) return false;
    *output = self->boolean;
    return true;
}

bool APS5_VABI _ZN3WTF8JSONImpl5Value7asValueERNS_6RefPtrIS1_NS_13DumbPtrTraitsIS1_EEEE(Value* self, ValueReference* output) {
    if (!self) throw std::invalid_argument("WTF::JSONImpl::Value::asValue: null this");
    if (!output) throw std::invalid_argument("WTF::JSONImpl::Value::asValue: null output");
    Value* const previous = output->value;
    if (previous && previous != self && previous->refCount == 1 && (!previous->vtable || !previous->vtable->deletingDestructor))
        throw std::invalid_argument("WTF::JSONImpl::Value::asValue: last reference to a value with no destructor");
    ++self->refCount;
    output->value = self;
    if (!previous) return true;
    if (previous->refCount == 1) previous->vtable->deletingDestructor(previous);
    else --previous->refCount;
    return true;
}

}
