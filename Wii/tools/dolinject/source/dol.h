#ifndef DOL_H
#define DOL_H
//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#include <cstdint>
#include <stddef.h>





//=============================================================================
// Macros
//-----------------------------------------------------------------------------
#define ALIGN_FOR_SECTION(value) (((value) + 0xFFFF) & ~0xFFFF)





//=============================================================================
// Constants
//-----------------------------------------------------------------------------
static constexpr uint8_t sTextSectionMax = 7;
static constexpr uint8_t sDataSectionMax = 11;





//=============================================================================
// Struct declarations
//-----------------------------------------------------------------------------
struct DolHeader
{
	uint32_t textOffsets[sTextSectionMax];
	uint32_t dataOffsets[sDataSectionMax];
	uint32_t textLdAddrs[sTextSectionMax];
	uint32_t dataLdAddrs[sDataSectionMax];
	uint32_t textLengths[sTextSectionMax];
	uint32_t dataLengths[sDataSectionMax];
	uint32_t bssLdAddr;
	uint32_t bssLength;
	uint32_t entryPoint;
	uint8_t padding[0x1C];
};





//=============================================================================
// Struct assertions
//-----------------------------------------------------------------------------
void ByteSwapDolHeader(DolHeader* header);





//=============================================================================
// Struct assertions
//-----------------------------------------------------------------------------
static_assert(offsetof(DolHeader, textOffsets) == 0x00);
static_assert(offsetof(DolHeader, dataOffsets) == 0x1C);
static_assert(offsetof(DolHeader, textLdAddrs) == 0x48);
static_assert(offsetof(DolHeader, dataLdAddrs) == 0x64);
static_assert(offsetof(DolHeader, textLengths) == 0x90);
static_assert(offsetof(DolHeader, dataLengths) == 0xAC);
static_assert(offsetof(DolHeader, bssLdAddr)   == 0xD8);
static_assert(offsetof(DolHeader, bssLength)   == 0xDC);
static_assert(offsetof(DolHeader, entryPoint)  == 0xE0);
static_assert(offsetof(DolHeader, padding)     == 0xE4);
static_assert(sizeof(DolHeader)                == 0x100);


#endif // DOL_H
