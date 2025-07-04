#include <cellstatus.h>
#include <sys/prx.h>
#include <sys/ppu_thread.h>

#include "MetadataDumper.hpp"
#include "Addresses.h"

sys_ppu_thread_t dumperThread;

SYS_MODULE_INFO( AlchemyMetadataDumper, 0, 1, 1);
SYS_MODULE_START( _AlchemyMetadataDumper_prx_entry );

SYS_LIB_DECLARE_WITH_STUB( LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME );
SYS_LIB_EXPORT( _AlchemyMetadataDumper_export_function, LIBNAME );

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _AlchemyMetadataDumper_export_function(void)
{
    return CELL_OK;
}

sys_ppu_thread_t create_thread(void(*entry)(uint64_t), int priority, size_t stacksize, const char* threadname, sys_ppu_thread_t tid)
{
	if (sys_ppu_thread_create(&tid, entry, 0, priority, stacksize, 0, threadname) != CELL_OK)
	{
		_igReportPrintf("THREAD: Thread creation failed...\n");
	}
	else
	{
		_igReportPrintf("THREAD: Thread created...\n");
	}
	return tid;
}

// Wrapper around MetadataDumperThread
static void MetadataDumperThreadPS3(uint64_t threadId)
{
	MetadataDumperThread();
}

extern "C" int _AlchemyMetadataDumper_prx_entry(void)
{
	create_thread(MetadataDumperThreadPS3, 0x4AA, 0x6000, "Metadata Dumper Thread", dumperThread);
    return SYS_PRX_RESIDENT;
}
