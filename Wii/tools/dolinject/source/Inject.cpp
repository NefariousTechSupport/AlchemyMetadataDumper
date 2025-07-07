//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>
#include <inttypes.h>
#include <assert.h>
#include <algorithm>
#include <cstring>

#include "dol.h"





//=============================================================================
// Internal Constants 
//-----------------------------------------------------------------------------
constexpr uint32_t kNumItems = sTextSectionMax + sDataSectionMax + 1;  





//=============================================================================
// Function declarations
//-----------------------------------------------------------------------------
static void Validate(DolHeader* header);





//=============================================================================
// Inject: Does the code injection
//-----------------------------------------------------------------------------
int Inject(const char* gameDol, const char* injectDol, const char* outDol)
{
	// File reading


	FILE* fGameDol = fopen(gameDol, "rb");

	if (fGameDol == NULL)
	{
		printf("Failed to open input dol file %s with error %" PRId32 "\n", gameDol, errno);
		return errno;
	}

	FILE* fInjectDol = fopen(injectDol, "rb");

	if (fInjectDol == NULL)
	{
		printf("Failed to open input dol file %s with error %" PRId32 "\n", injectDol, errno);
		fclose(fGameDol);
		return errno;
	}

	FILE* fOutDol = fopen(outDol, "wb");

	if (fOutDol == NULL)
	{
		printf("Failed to open output dol file %s with error %" PRId32 "\n", outDol, errno);
		fclose(fGameDol);
		fclose(fInjectDol);
		return errno;
	}

	DolHeader gameHeader;
	DolHeader injectHeader;

	size_t amountRead = fread(&gameHeader, 1, sizeof(gameHeader), fGameDol);
	if (amountRead != sizeof(gameHeader))
	{
		printf("Read %z" PRIu32 " out of %z" PRIu32 " bytes from input dol %s. error number %" PRId32 "\n", amountRead, sizeof(gameHeader), gameDol, errno);
		fclose(fGameDol);
		fclose(fInjectDol);
		fclose(fOutDol);
		return errno;
	}

	amountRead = fread(&injectHeader, 1, sizeof(injectHeader), fInjectDol);
	if (amountRead != sizeof(injectHeader))
	{
		printf("Read %z" PRIu32 " out of %z" PRIu32 " bytes from input dol %s. error number %" PRId32 "\n", amountRead, sizeof(injectHeader), injectDol, errno);
		fclose(fGameDol);
		fclose(fInjectDol);
		fclose(fOutDol);
		return errno;
	}

	ByteSwapDolHeader(&gameHeader);
	ByteSwapDolHeader(&injectHeader);



	// Merging



	uint8_t gameFreeText = 0xFF;
	uint8_t injectText = 0xFF;
	for (uint8_t i = 0; i < sTextSectionMax; i++)
	{
		if (gameFreeText == 0xFF && gameHeader.textOffsets[i] == 0)
		{
			gameFreeText = i;
		}

		if (injectText == 0xFF && injectHeader.textOffsets[i] != 0)
		{
			injectText = i;
		}
	}

	uint8_t gameFreeData = 0xFF;
	uint8_t injectData = 0xFF;
	for (uint8_t i = 0; i < sDataSectionMax; i++)
	{
		if (gameFreeData == 0xFF && gameHeader.dataOffsets[i] == 0)
		{
			gameFreeData = i;
		}

		if (injectData == 0xFF && injectHeader.dataOffsets[i] != 0)
		{
			injectData = i;
		}
	}

	if (gameFreeText == 0xFF)
	{
		printf("There is no free .text section in the game binary\n");
		fclose(fGameDol);
		fclose(fInjectDol);
		fclose(fOutDol);
		return 1;
	}

	if (gameFreeData == 0xFF)
	{
		printf("There is no free .data section in the game binary\n");
		fclose(fGameDol);
		fclose(fInjectDol);
		fclose(fOutDol);
		return 1;
	}

	// Output

	fseek(fGameDol, 0, SEEK_END);
	uint32_t gameDolSize = ftell(fGameDol);
	memcpy(&gameHeader.textOffsets[1], &gameHeader.textOffsets[0], gameFreeText * sizeof(uint32_t));
	memcpy(&gameHeader.textLengths[1], &gameHeader.textLengths[0], gameFreeText * sizeof(uint32_t));
	memcpy(&gameHeader.textLdAddrs[1], &gameHeader.textLdAddrs[0], gameFreeText * sizeof(uint32_t));
	gameHeader.textOffsets[0] = gameDolSize;
	gameHeader.textLengths[0] = injectHeader.textLengths[injectText];
	gameHeader.textLdAddrs[0] = injectHeader.textLdAddrs[injectText];

	memcpy(&gameHeader.dataOffsets[1], &gameHeader.dataOffsets[0], gameFreeData * sizeof(uint32_t));
	memcpy(&gameHeader.dataLengths[1], &gameHeader.dataLengths[0], gameFreeData * sizeof(uint32_t));
	memcpy(&gameHeader.dataLdAddrs[1], &gameHeader.dataLdAddrs[0], gameFreeData * sizeof(uint32_t));
	gameHeader.dataOffsets[0] = gameDolSize + injectHeader.textLengths[injectText];
	gameHeader.dataLengths[0] = injectHeader.dataLengths[injectData];
	gameHeader.dataLdAddrs[0] = injectHeader.dataLdAddrs[injectData];

	gameHeader.bssLength = injectHeader.bssLdAddr + injectHeader.bssLength - gameHeader.bssLdAddr;

	Validate(&gameHeader);

	ByteSwapDolHeader(&gameHeader);

	size_t res = 0;

	uint8_t* buffer = new uint8_t[gameDolSize];
	fseek(fGameDol, 0, SEEK_SET);
	res = fread(buffer, 1, gameDolSize, fGameDol);
	assert(res == gameDolSize);
	res = fwrite(buffer, 1, gameDolSize, fOutDol);
	assert(res == gameDolSize);
	delete[] buffer;

	fseek(fOutDol, 0, SEEK_SET);
	res = fwrite(&gameHeader, 1, sizeof(gameHeader), fOutDol);
	assert(res == sizeof(gameHeader));
	
	fseek(fOutDol, 0, SEEK_END);

	buffer = new uint8_t[injectHeader.textLengths[injectText]];
	fseek(fInjectDol, injectHeader.textOffsets[injectText], SEEK_SET);
	res = fread(buffer, 1, injectHeader.textLengths[injectText], fInjectDol);
	assert(res == injectHeader.textLengths[injectText]);
	res = fwrite(buffer, 1, injectHeader.textLengths[injectText], fOutDol);
	assert(res == injectHeader.textLengths[injectText]);
	delete[] buffer;

	buffer = new uint8_t[injectHeader.dataLengths[injectText]];
	fseek(fInjectDol, injectHeader.dataOffsets[injectText], SEEK_SET);
	res = fread(buffer, 1, injectHeader.dataLengths[injectText], fInjectDol);
	assert(res == injectHeader.dataLengths[injectText]);
	res = fwrite(buffer, 1, injectHeader.dataLengths[injectText], fOutDol);
	assert(res == injectHeader.dataLengths[injectText]);
	delete[] buffer;

	fflush(fOutDol);
	fclose(fGameDol);
	fclose(fInjectDol);
	fclose(fOutDol);
	return 0;
}





//=============================================================================
// MakeIndexReadable: Converts the index for the arrays in Validate to
// something readable
//-----------------------------------------------------------------------------
static void MakeIndexReadable(uint32_t input, const char** sectionName, uint32_t* index)
{
	int32_t textIndex = (input < sTextSectionMax ? input : -1);
	int32_t bssIndex  = static_cast<int32_t>(input == (kNumItems - 1)) - 1;
	int32_t dataIndex = (input >= sTextSectionMax && bssIndex == -1 ? input - sTextSectionMax : -1);

	*index       = textIndex != -1 ? textIndex : dataIndex != -1 ? dataIndex : bssIndex;
	*sectionName = textIndex != -1 ? ".text"   : dataIndex != -1 ? ".data"   : ".bss";
}





//=============================================================================
// Validate: Validates the dol header and warns of overlapping sections 
//-----------------------------------------------------------------------------
static void Validate(DolHeader* header)
{
	std::array<uint32_t, kNumItems> addresses =
	{
		header->textLdAddrs[0],
		header->textLdAddrs[1],
		header->textLdAddrs[2],
		header->textLdAddrs[3],
		header->textLdAddrs[4],
		header->textLdAddrs[5],
		header->textLdAddrs[6],

		header->dataLdAddrs[0],
		header->dataLdAddrs[1],
		header->dataLdAddrs[2],
		header->dataLdAddrs[3],
		header->dataLdAddrs[4],
		header->dataLdAddrs[5],
		header->dataLdAddrs[6],
		header->dataLdAddrs[7],
		header->dataLdAddrs[8],
		header->dataLdAddrs[9],
		header->dataLdAddrs[10],

		header->bssLdAddr
	};

	std::array<uint32_t, kNumItems> lengths =
	{
		header->textLengths[0],
		header->textLengths[1],
		header->textLengths[2],
		header->textLengths[3],
		header->textLengths[4],
		header->textLengths[5],
		header->textLengths[6],

		header->dataLengths[0],
		header->dataLengths[1],
		header->dataLengths[2],
		header->dataLengths[3],
		header->dataLengths[4],
		header->dataLengths[5],
		header->dataLengths[6],
		header->dataLengths[7],
		header->dataLengths[8],
		header->dataLengths[9],
		header->dataLengths[10],

		header->bssLength
	};

	for (uint x = 0; x < kNumItems; x++)
	{
		if (addresses[x] == 0)
		{
			continue;
		}

		uint32_t testMin = addresses[x];
		uint32_t testMax = addresses[x] + lengths[x];

		uint32_t    testIndex = 0;
		const char* testSectionName = 0;
		MakeIndexReadable(x, &testSectionName, &testIndex);

		for (uint y = 0; y < kNumItems; y++)
		{
			if (x == y || addresses[y] == 0)
			{
				continue;
			}

			uint32_t    thisIndex = 0;
			const char* thisSectionName = 0;
			MakeIndexReadable(y, &thisSectionName, &thisIndex);

			if ((addresses[y] <= testMin && testMin <  addresses[y] + lengths[y])
			 || (addresses[y] <  testMax && testMax <  addresses[y] + lengths[y]))
			{
				printf("Validation Error: address range %08x:%08x (%s, %d) overlaps range %08x:%08x (%s, %d)\n", addresses[x], addresses[x] + lengths[x], testSectionName, testIndex, addresses[y], addresses[y] + lengths[y] - 1, thisSectionName, thisIndex);
			}
		}
	}
}
