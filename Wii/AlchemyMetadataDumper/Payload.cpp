#include "MetadataDumper.hpp"

static void MetadataDumperEntry()
{
	MetadataDumperThread();
}

__declspec (section ".ctors") void(*MetadataDumperEntryCTor)() = MetadataDumperEntry;
