#include "fileio.hpp"

#include "alchemystdint.h"

#include "Addresses.h"



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
	_igReportPrintf("%d:%s", _handle, data);
	return len;
}

