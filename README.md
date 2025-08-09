# Alchemy Metadata Dumper

A "platform independent" dynamic library for dumping runtime reflection metadata from games made with the alchemy game engine

Supported:
- Skylanders Spyro's Adventure (Wii SSPP52 Rev3)
- Skylanders Trap Team v1.1.0 (PS3)
- Skylanders Superchargers v1.6.0 (PS3)
- Skylanders Imaginators v1.1.0 (PS3)


## Developer Notes:

- Add platform specific code into platform specific folders
- Use C++98 for Wii compatibility
- Do not link against standard libraries, certain platforms have really tight memory constaints, and alchemy tends to allocate all its memory up front on consoles, leaving us with very little room to work with.
	- This means that you cannot do heap allocation, `malloc` requires linking against libstdc, `operator new` requires linking against libstdc++, rely on local variables and try and be smart with how you use them 
- Please put platform specific implementations behind conditional compilation, the guards are
	- `TARGET_ANDROID32`: 32-bit Android
	- `TARGET_ANDROID64`: 64-bit Android
	- `TARGET_ASPEN32`: 32-bit iOS
	- `TARGET_ASPEN64`: 64-bit iOS
	- `TARGET_CAFE`: Wii U
	- `TARGET_GC`: Game Cube
	- `TARGET_PS2`: PS2
	- `TARGET_PS3`: PS3
	- `TARGET_PS4`: PS4
	- `TARGET_WII`: Wii
	- `TARGET_WIN32`: 32-bit Windows
	- `TARGET_WIN64`: 64-bit Windows
	- `TARGET_XBOX360`: Xbox 360
	- `TARGET_XBOXONE`: Xbox One
- Additionally there are architecture specific guards:
	- `TARGET_ARM`: Arm (32-bit and 64-bit)
	- `TARGET_ARM32`: 32-bit arm
	- `TARGET_ARM64`: 64-bit arm
	- `TARGET_PPC`: PowerPC
	- `TARGET_X86`: x86 (32-bit and 64 bit)
	- `TARGET_X86_32`: 32-bit x86
	- `TARGET_X86_64`: 64-bit x86
