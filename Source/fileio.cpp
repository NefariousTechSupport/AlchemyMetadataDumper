#include "fileio.hpp"

#include "Addresses.h"
#if TARGET_WII
#include "support.h"
#else
#include <stdarg.h>
#endif // TARGET_WII

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

#if !TARGET_WII // Neffy - Codwarriors compiler inserts calls to operator delete, which we lack
FileWriter::~FileWriter()
{
	if(_handle)
	{
		platformCloseHandle();
	}
}
#endif // !TARGET_WII

int FileWriter::WriteText(int len, const char* text)
{
	auint64_t written = platformWriteFile(text, len);

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
	int res = __stubs::sprintf(text, fmt, args);

	va_end(args);

	auint64_t written = platformWriteFile(text, res);

	if (written != res)
	{
		_igReportPrintf("pain and suffering, didn't write enough data...\n");
	}
	return res;
}