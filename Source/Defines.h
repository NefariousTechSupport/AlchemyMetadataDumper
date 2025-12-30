#pragma once





#ifndef TARGET_PS3
#define TARGET_PS3 0
#endif // TARGET_PS3

#ifndef TARGET_GC
#define TARGET_GC 0
#endif // TARGET_GC

#ifndef TARGET_WII
#define TARGET_WII 0
#endif // TARGET_WII

#ifndef TARGET_XBOX360
#define TARGET_XBOX360 0
#endif // TARGET_XBOX360

#ifndef TARGET_CAFE
#define TARGET_CAFE 0
#endif // TARGET_CAFE

#ifndef TARGET_IOS
#define TARGET_IOS 0
#endif // TARGET_IOS

#ifndef TARGET_WIN
#define TARGET_WIN 0
#endif // TARGET_WIN

#ifndef TARGET_OSX
#define TARGET_OSX 0
#endif // TARGET_OSX

#ifndef TARGET_3DS
#define TARGET_3DS 0
#endif // TARGET_3DS

#if (TARGET_PS3 + TARGET_GC + TARGET_WII + TARGET_XBOX360 + TARGET_CAFE + TARGET_IOS + TARGET_WIN + TARGET_OSX + TARGET_3DS) != 1
#error Failed to determine target
#endif // (TARGET_PS3 + TARGET_GC + TARGET_WII + TARGET_XBOX360 + TARGET_CAFE + TARGET_IOS + TARGET_WIN + TARGET_OSX + TARGET_3DS) != 1





#if TARGET_PS3 || TARGET_GC || TARGET_WII || TARGET_XBOX360 || TARGET_CAFE
#define TARGET_PPC 1
#elif TARGET_IOS || TARGET_OSX || TARGET_3DS
#define TARGET_ARM 1
#elif TARGET_WIN
#define TARGET_X86 1
#endif // TARGET_PS3 || TARGET_GC || TARGET_WII || TARGET_XBOX360 || TARGET_CAFE + TARGET_IOS || TARGET_OSX || TARGET_3DS + TARGET_WIN





#ifndef TARGET_PPC
#define TARGET_PPC 0
#endif // TARGET_PPC

#ifndef TARGET_X86
#define TARGET_X86 0
#endif // TARGET_X86

#ifndef TARGET_ARM
#define TARGET_ARM 0
#endif // TARGET_ARM

#if (TARGET_PPC + TARGET_X86 + TARGET_ARM) != 1
#error Failed to determine architecture
#endif // (TARGET_PPC + TARGET_X86 + TARGET_ARM) != 1

