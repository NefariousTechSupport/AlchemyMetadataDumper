#include <3ds.h>
#include <string.h>
#include <stdio.h>
#include "plgldr.h"
#include "csvc.h"
#include "common.h"

#include "MetadataDumper.hpp"

static Handle       thread;
static Handle       onProcessExitEvent, resumeExitEvent;
static u8           stack[STACK_SIZE] CTR_ALIGN(8);



// Plugin main thread entrypoint
void ThreadMain(void *arg)
{
    plgLdrInit();

    // Dump metadata
    MetadataDumperThread();

    // This is executed when the game is about to exit
    // useful to save config, properly deinit stuff etc
    plgLdrExit();
    srvExit();

    // We're done with our exit code, so we let the game exit
    svcSignalEvent(resumeExitEvent);

    svcExitThread();
}

extern char* fake_heap_start;
extern char* fake_heap_end;
extern u32 __ctru_heap;
extern u32 __ctru_linear_heap;

u32 __ctru_heap_size        = 0;
u32 __ctru_linear_heap_size = 0;

void __system_allocateHeaps(PluginHeader *header)
{
    __ctru_heap_size = header->heapSize;
    __ctru_heap = header->heapVA;

    // Set up newlib heap
    fake_heap_start = (char *)__ctru_heap;
    fake_heap_end = fake_heap_start + __ctru_heap_size;
}

// Entrypoint, game will starts when you exit this function
int main(void)
{
    PluginHeader *header = (PluginHeader *)0x07000000;

    if (header->magic != HeaderMagic)
        return 1; ///< Abort plugin as something went wrong

    // Init heap
    __system_allocateHeaps(header);

    // Init services
    srvInit();

    // Get the event triggered  when the game will exit
    svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_GET_ON_EXIT_EVENT, (u32)&onProcessExitEvent, (u32)&resumeExitEvent);

    // Create the plugin's main thread
    svcCreateThread(&thread, ThreadMain, 0, (u32 *)(stack + STACK_SIZE), 30, -1);

    return 0;
}
