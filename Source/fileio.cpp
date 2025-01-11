#include "fileio.hpp"

#include "Addresses.h"

#include <string.h>
#include <stdarg.h>

#define WRITE_BUFFER_SIZE 0x200

FileWriter::FileWriter(const char* filepath)
{
	platformOpenFile(filepath);

	if (!_handle)
	{
		_igReportPrintf("failed to open file handle for path %s\n", filepath);
		return;
	}

	platformTruncateFile(filepath);
}

FileWriter::~FileWriter()
{
	if(_handle)
	{
		platformCloseHandle();
	}
}

int FileWriter::WriteText(int len, const char* text)
{
	uint64_t written = platformWriteFile(text, len);

	if (written != len)
	{
		_igReportPrintf("pain and suffering, didn't write enough data...\n");
	}
	return len;
}

int FileWriter::WriteTextf(int n, const char* fmt, ...)
{
	va_list args;
	va_start(args, n);

	char text[WRITE_BUFFER_SIZE];
	int res = __stubs::__sprintf(text, fmt, args);

	va_end(args);

	uint64_t written = platformWriteFile(text, res);

	if (written != res)
	{
		_igReportPrintf("pain and suffering, didn't write enough data...\n");
	}
	return res;
}