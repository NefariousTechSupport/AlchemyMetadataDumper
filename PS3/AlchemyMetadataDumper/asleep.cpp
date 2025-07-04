#include "asleep.h"

#include <sys/timer.h>





void asleep(auint64_t milliseconds)
{
	sys_timer_usleep(milliseconds * 1000);
}
