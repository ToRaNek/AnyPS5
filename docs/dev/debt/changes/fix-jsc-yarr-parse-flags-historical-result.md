# libSceJsc

## change [JSC::Yarr::parseFlags]

- [JSC::Yarr::parseFlags](../../../core/libs/prx/libSceJsc/src/YarrFlags.cpp) (libSceJsc) - not compared with a console module. The firmware's WebKit predates d7549ecd9331 (May 2021): libSceNKWebKit still exports `parseDoViCodecParameters(const String&)`. So this follows [YarrFlags.cpp](https://github.com/WebKit/WebKit/blob/c55364d53f971f63a709976f168d932d558b8909/Source/JavaScriptCore/yarr/YarrFlags.cpp#L34-L80): `g i m y u s` set bits 0 to 5 of an 8-bit set, and the `Optional<OptionSet<Flags>>` result is the flag at byte 0 and the set at byte 1, through the hidden result pointer. `d` was added by 7e63c11fb1f6 (February 2021), inside the window the exports leave open, so a string that would be valid with it throws `NotImplemented`. A null character pointer with a nonzero length throws where WebKit faults. Closes when: compared with a console module.
