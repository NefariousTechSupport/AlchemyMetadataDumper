#include "asleep.h"

#include <3ds.h>

void asleep(auint64_t milliseconds)
{
    svcSleepThread(milliseconds * 1000);
}
