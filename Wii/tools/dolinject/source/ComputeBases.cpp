//=============================================================================
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <errno.h>
#include <algorithm>
#include <inttypes.h>

#include "dol.h"





//=============================================================================
// Internal constants
//-----------------------------------------------------------------------------
// This needs to not be hardcoded but this is the best we got rn
static constexpr uint32_t sTextBudget = 0x28000;
static constexpr uint32_t sDataBudget = 0x10000;





//=============================================================================
// ComputeBases: Computes the base addresses to compile with
//-----------------------------------------------------------------------------
int ComputeBases(const char* inDol, const char* outMakefile)
{
	FILE* fInDol       = fopen(inDol, "rb");

	if (fInDol == NULL)
	{
		printf("Failed to open input dol file %s with error %" PRId32 "\n", inDol, errno);
		return errno;
	}

	FILE* fOutMakefile = fopen(outMakefile, "w");

	if (fOutMakefile == NULL)
	{
		printf("Failed to open output makefile file %s with error %" PRId32 "\n", outMakefile, errno);
		fclose(fInDol);
		return errno;
	}

	DolHeader header;

	size_t amountRead = fread(&header, 1, sizeof(header), fInDol);
	if (amountRead != sizeof(header))
	{
		printf("Read %z" PRIu32 " out of %z" PRIu32 " bytes from input dol %s. error number %" PRId32 "\n", amountRead, sizeof(header), inDol, errno);
		fclose(fInDol);
		fclose(fOutMakefile);
		return errno;
	}

	ByteSwapDolHeader(&header);

	// Compute the largest .text vaddr + size
	uint32_t highestTextLdAddr = 0;
	uint32_t highestTextLength = 0;
	for (uint8_t i = 0; i < sTextSectionMax; i++)
	{
		if (highestTextLdAddr <= header.textLdAddrs[i])
		{
			highestTextLdAddr = header.textLdAddrs[i];
			highestTextLength = header.textLengths[i];
		}
	}

	// Compute the largest .data vaddr + size
	uint32_t highestDataLdAddr = 0;
	uint32_t highestDataLength = 0;
	for (uint8_t i = 0; i < sDataSectionMax; i++)
	{
		if (highestDataLdAddr <= header.dataLdAddrs[i])
		{
			highestDataLdAddr = header.dataLdAddrs[i];
			highestDataLength = header.dataLengths[i];
		}
	}

	// Compute the base addresses and align to 0x100
	uint32_t textBaseAddress = ALIGN_FOR_SECTION(highestTextLdAddr + highestTextLength);
	uint32_t dataBaseAddress = ALIGN_FOR_SECTION(highestDataLdAddr + highestDataLength);
	uint32_t  bssBaseAddress = ALIGN_FOR_SECTION(header.bssLdAddr  + header.bssLength);

	uint32_t realBaseAddress = std::max(textBaseAddress, std::max(dataBaseAddress, bssBaseAddress));

	char output[1024];
	int outputLength = snprintf(output, sizeof(output),
		"export BASE_TEXT=0x%08X\n"
		"export BASE_DATA=0x%08X\n"
		"export BASE_BSS=0x%08X\n"
		"\n"
		"include Makefile.wii\n",
		realBaseAddress,
		realBaseAddress + sTextBudget,
		realBaseAddress + sTextBudget + sDataBudget
	);

	if (outputLength < 0)
	{
		printf("Internal encoding error\n");
		fclose(fInDol);
		fclose(fOutMakefile);
		return 1;
	}

	if (static_cast<uint32_t>(outputLength) > sizeof(output))
	{
		printf("Ran out of space writing to internal output buffer\n");
		fclose(fInDol);
		fclose(fOutMakefile);
		return 1;
	}

	size_t amountWritten = fwrite(output, 1, outputLength, fOutMakefile);
	if (amountWritten != static_cast<uint32_t>(outputLength))
	{
		printf("Wrote %z" PRIu32 " out of %" PRId32 " bytes to output makefile %s. error number %" PRId32 "\n", amountWritten, outputLength, outMakefile, errno);
		fclose(fInDol);
		fclose(fOutMakefile);
		return errno;
	}

	fclose(fInDol);
	fclose(fOutMakefile);
	return 0;
}
