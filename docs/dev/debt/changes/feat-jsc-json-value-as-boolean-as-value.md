# libSceJsc

## add Functional

- [WTF::JSONImpl::Value::asBoolean and asValue](../../../core/libs/prx/libSceJsc/src/WtfJsonValue.cpp) (libSceJsc) - not compared with a console module. Both are in the 3.20 inventory only and follow [JSONValues.cpp](https://github.com/WebKit/WebKit/blob/c55364d53f971f63a709976f168d932d558b8909/Source/WTF/wtf/JSONValues.cpp) on the 24-byte `Value` of that revision: vtable pointer, 32-bit count at 8, type at 12, payload at 16. `asValue` assigns `this` to the caller's `RefPtr`; when that drops the last reference of the value it held, the value is deleted through slot 1 of its own vtable, the deleting destructor, without the count being written. Closes when: compared with a console module.

## change [Older callable NIDs]

- [Older callable NIDs](../../../core/libs/prx/libSceJsc/LegacyExports.cpp): 196 exports have unverified signatures and behavior. 195 are named after the symbols the [verified NID name database](https://github.com/zecoxao/sce_symbols/blob/2883963a0a514ba6e77407a08ef4730a65e88254/aerolib.csv) gives for their NIDs, and the NID of each name matches; `NKE5vAmeA-o` has an unverified name. These are throwing import-resolution placeholders; no JavaScriptCore behavior is implemented. No matching NIDs were found in the later module inventory.
