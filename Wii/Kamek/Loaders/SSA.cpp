#include "AlchemyKamek.h"

int loadIntoSSA();

#define ALCHEMY_KAMEK_DEFAULT_POOL 6

//kmCondWritePointer(0x804CF8D0, 0x00000000, loadIntoSSA); // EU rev3, adds to the end of the initialisers array
kmBranchDefAsm(0x800D69E8, NULL)
{
	nofralloc
	//save registers
	stwu        r1, -0x1A0(r1)
	stw         r0,   0x1C(r1)
	mfspr       r0,  LR
	stw         r0,  0x1A4(r1)
	stw         r31, 0x19C(r1)
	stw         r30, 0x198(r1)
	stw         r29, 0x194(r1)
	stw         r28, 0x190(r1)
	stw         r27, 0x18C(r1)
	stw         r26, 0x188(r1)
	stw         r25, 0x184(r1)
	stw         r24, 0x180(r1)
	stw         r23, 0x17C(r1)
	stw         r22, 0x178(r1)
	stw         r21, 0x174(r1)
	stw         r20, 0x170(r1)
	stw         r19, 0x16C(r1)
	stw         r18, 0x168(r1)
	stw         r17, 0x164(r1)
	stw         r16, 0x160(r1)
	stw         r15, 0x15C(r1)
	stw         r14, 0x158(r1)
	stw         r13, 0x154(r1)
	stw         r12, 0x150(r1)
	stw         r11, 0x14C(r1)
	stw         r10, 0x148(r1)
	stw         r9,  0x144(r1)
	stw         r8,  0x140(r1)
	stw         r7,  0x13C(r1)
	stw         r6,  0x138(r1)
	stw         r5,  0x134(r1)
	stw         r4,  0x130(r1)
	stw         r3,  0x12C(r1)
	stw         r2,  0x128(r1)
	stw         r1,  0x124(r1)
	stw         r0,  0x120(r1)
	stfd        f31, 0x118(r1)
	stfd        f30, 0x110(r1)
	stfd        f29, 0x100(r1)
	stfd        f28, 0x100(r1)
	stfd        f27,  0xF8(r1)
	stfd        f26,  0xF0(r1)
	stfd        f25,  0xE8(r1)
	stfd        f24,  0xE0(r1)
	stfd        f23,  0xD8(r1)
	stfd        f22,  0xD0(r1)
	stfd        f21,  0xC8(r1)
	stfd        f20,  0xC0(r1)
	stfd        f19,  0xB8(r1)
	stfd        f18,  0xB0(r1)
	stfd        f17,  0xA8(r1)
	stfd        f16,  0xA0(r1)
	stfd        f15,  0x98(r1)
	stfd        f14,  0x90(r1)
	stfd        f13,  0x88(r1)
	stfd        f12,  0x80(r1)
	stfd        f11,  0x78(r1)
	stfd        f10,  0x70(r1)
	stfd        f9,   0x68(r1)
	stfd        f8,   0x60(r1)
	stfd        f7,   0x58(r1)
	stfd        f6,   0x50(r1)
	stfd        f5,   0x48(r1)
	stfd        f4,   0x40(r1)
	stfd        f3,   0x38(r1)
	stfd        f2,   0x30(r1)
	stfd        f1,   0x28(r1)
	stfd        f0,   0x20(r1)

	// call C++ method
	bl          loadIntoSSA

	// Reload registers
	lwz         r0,  0x1A4(r1)
	lwz         r31, 0x19C(r1)
	lwz         r30, 0x198(r1)
	lwz         r29, 0x194(r1)
	lwz         r28, 0x190(r1)
	lwz         r27, 0x18C(r1)
	lwz         r26, 0x188(r1)
	lwz         r25, 0x184(r1)
	lwz         r24, 0x180(r1)
	lwz         r23, 0x17C(r1)
	lwz         r22, 0x178(r1)
	lwz         r21, 0x174(r1)
	lwz         r20, 0x170(r1)
	lwz         r19, 0x16C(r1)
	lwz         r18, 0x168(r1)
	lwz         r17, 0x164(r1)
	lwz         r16, 0x160(r1)
	lwz         r15, 0x15C(r1)
	lwz         r14, 0x158(r1)
	lwz         r13, 0x154(r1)
	lwz         r12, 0x150(r1)
	lwz         r11, 0x14C(r1)
	lwz         r10, 0x148(r1)
	lwz         r9,  0x144(r1)
	lwz         r8,  0x140(r1)
	lwz         r7,  0x13C(r1)
	lwz         r6,  0x138(r1)
	lwz         r5,  0x134(r1)
	lwz         r4,  0x130(r1)
	lwz         r3,  0x12C(r1)
	lwz         r2,  0x128(r1)
	lwz         r1,  0x124(r1)
	lwz         r0,  0x120(r1)
	lfd         f31, 0x118(r1)
	lfd         f30, 0x110(r1)
	lfd         f29, 0x100(r1)
	lfd         f28, 0x100(r1)
	lfd         f27,  0xF8(r1)
	lfd         f26,  0xF0(r1)
	lfd         f25,  0xE8(r1)
	lfd         f24,  0xE0(r1)
	lfd         f23,  0xD8(r1)
	lfd         f22,  0xD0(r1)
	lfd         f21,  0xC8(r1)
	lfd         f20,  0xC0(r1)
	lfd         f19,  0xB8(r1)
	lfd         f18,  0xB0(r1)
	lfd         f17,  0xA8(r1)
	lfd         f16,  0xA0(r1)
	lfd         f15,  0x98(r1)
	lfd         f14,  0x90(r1)
	lfd         f13,  0x88(r1)
	lfd         f12,  0x80(r1)
	lfd         f11,  0x78(r1)
	lfd         f10,  0x70(r1)
	lfd         f9,   0x68(r1)
	lfd         f8,   0x60(r1)
	lfd         f7,   0x58(r1)
	lfd         f6,   0x50(r1)
	lfd         f5,   0x48(r1)
	lfd         f4,   0x40(r1)
	lfd         f3,   0x38(r1)
	lfd         f2,   0x30(r1)
	lfd         f1,   0x28(r1)
	lfd         f0,   0x20(r1)
	mtspr       LR,  r0
	lwz         r0,   0x1C(r1)
	lwzu        r1,  0x1A0(r1)

	// Return
	blr
}

void* allocAdapter(u32 size, bool isForCode, const loaderFunctions* funcs)
{
	const alchemyLoaderFuncs* alchemyFuncs = reinterpret_cast<const alchemyLoaderFuncs*>(funcs);
	void* memoryPool = alchemyFuncs->igGetMemoryPool(ALCHEMY_KAMEK_DEFAULT_POOL);
	void* mem = alchemyFuncs->igMemoryPool_mallocAligned(memoryPool, size, 0x100);
	funcs->OSReport("Allocated %x bytes at %p\n", size, mem);
	return mem;
}

void freeAdapter(void* buffer, bool isForCode, const loaderFunctions *funcs)
{
	const alchemyLoaderFuncs* alchemyFuncs = reinterpret_cast<const alchemyLoaderFuncs*>(funcs);
	void* memoryPool = alchemyFuncs->igGetMemoryPool(ALCHEMY_KAMEK_DEFAULT_POOL);
	alchemyFuncs->igMemoryPool_free(memoryPool, buffer);
}


const alchemyLoaderFuncs functions_p = {
	{
		reinterpret_cast<OSReport_t>(0x8000b398),
		reinterpret_cast<OSFatal_t>(OSFatal_placeholder), // Unknown
		reinterpret_cast<DVDConvertPathToEntrynum_t>(0x803659c0),
		reinterpret_cast<DVDFastOpen_t>(0x80365cd0),
		reinterpret_cast<DVDReadPrio_t>(0x803660a0),
		reinterpret_cast<DVDClose_t>(0x80365e60),
		reinterpret_cast<sprintf_t>(0x803571cc),
		allocAdapter,
		freeAdapter,
	},
	reinterpret_cast<igGetMemoryPool_t>(0x80103f68),
	reinterpret_cast<igMemoryPool_mallocAligned_t>(0x801031c8),
	reinterpret_cast<igMemoryPool_free_t>(0x800dca08)
};

int loadIntoSSA()
{
	char region = 'P';
	int version = 3;
	const alchemyLoaderFuncs* funcs = &functions_p;

	char path[64];
	if (version == 0)
	{
		funcs->base.sprintf(path, "/Code/%c.bin", region);
	}
	else
	{
		funcs->base.sprintf(path, "/Code/%c%d.bin", region, version);
	}

	loadKamekBinaryFromDisc(&funcs->base, path);

	return 1;
}

