#include "fileio.hpp"

#include "alchemystdint.h"

#include "Addresses.h"

#define WRITE_HEX 1


void FileWriter::platformOpenFile(const char* filepath)
{
	_handle = reinterpret_cast<aint32_t>(filepath);
}

void FileWriter::platformTruncateFile(const char* filepath)
{

}

void FileWriter::platformCloseHandle()
{

}

aint32_t FileWriter::platformWriteFile(const char* data, aint32_t len)
{
#if WRITE_HEX
	static const char kHexMap[] = "0123456789ABCDEF";

	// Write out as hex
	char tmp[1024 + 1];
	if (len >= (sizeof(tmp) / 2))
	{
		_igReportPrintf("Failed to write data, limit of %d", sizeof(tmp) / 2);
		return -1;
	}

	for (int i = 0; i < len; i++)
	{
		tmp[i*2 + 0] = kHexMap[(data[i] >> 4) & 0xF];
		tmp[i*2 + 1] = kHexMap[ data[i]       & 0xF];
	}

	// null terminator
	tmp[len * 2] = 0;

	_igReportPrintf("metadatadumper file %s:%s", reinterpret_cast<const char*>(_handle), tmp);
#else // WRITE_HEX
	_igReportPrintf("metadatadumper file %s:%s", reinterpret_cast<const char*>(_handle), data);
#endif // WRITE_HEX

	return len;
}

