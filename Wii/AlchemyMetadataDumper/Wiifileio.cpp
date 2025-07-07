#include "fileio.hpp"

#include "alchemystdint.h"

#include "Addresses.h"



void FileWriter::platformOpenFile(const char* filepath)
{
	_handle = 1;
}

void FileWriter::platformTruncateFile(const char* filepath)
{

}

void FileWriter::platformCloseHandle()
{

}

aint32_t FileWriter::platformWriteFile(const char* data, aint32_t len)
{
	return _igReportPrintf("%s", data);
}

