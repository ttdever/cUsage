#ifndef WATCHDOG_H
#define WATCHDOG_H

#define THREAD_TIMEOUT_SEC 2

#include "structs.h"
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

extern size_t watched_threads_count;
_Noreturn void finish_program(void);
int is_thread_alive(time_t thread_timestamp);
void check_threads_timeout(void);
void *watchdog_task(void *args);

#endif
