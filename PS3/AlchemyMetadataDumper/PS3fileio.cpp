#include "fileio.hpp"

#include <cell/fs/cell_fs_file_api.h>

#include "Addresses.h"



#define FILE_PREFIX "/dev_hdd0/tmp/"



void FileWriter::platformOpenFile(const char* filepath)
{
	char prefixedPath[255];
	__stubs::__sprintf(prefixedPath, FILE_PREFIX "%s", filepath);

	cellFsOpen(prefixedPath, CELL_FS_O_CREAT|CELL_FS_O_WRONLY, &_handle, NULL, 0);
}

void FileWriter::platformTruncateFile(const char* filepath)
{
	char prefixedPath[255];
	__stubs::__sprintf(prefixedPath, FILE_PREFIX "%s", filepath);

	cellFsTruncate(filepath, 0);
}

void FileWriter::platformCloseHandle()
{
	cellFsClose(_handle);
}

int32_t FileWriter::platformWriteFile(uint8_t* data, int32_t len)
{
	uint64_t written = 0;
	cellFsWrite(_handle, text, len, &written);
	return written;
}
