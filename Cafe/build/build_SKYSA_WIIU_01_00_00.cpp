extern "C" {
    extern void* _IMPORT_COREINIT_memcpymemcpy(void* dest, const void* src, unsigned int count);

    void* memcpy(void* dest, const void* src, unsigned int count) {
        return _IMPORT_COREINIT_memcpymemcpy(dest, src, count);
    }
}

// 0x0205486c tfbSprite::spriteArtImp::handleMovie
PATCH_WRITE(0x0205486c, "bl _Z20MetadataDumperThreadv");

#define TARGET_CAFE 1
#define TARGET_PPC 1

// SSA Wii U
#define TARGET_GAME GameCode(2013, 7, 12, 1, 0, 0)
#include "Games.h"

#include "asleep.cpp"
#include "CafeCommon.cpp"
#include "Cafefileio.cpp"
#include "fileio.cpp"
#include "igMetaField.cpp"
#include "MetadataDumper.cpp"
#include "SKYSA_WIIU_01_00_00.cpp"
