#include "watchdog.h"
#include <stdio.h>

static struct timespec watchdog_sleep_time = {2, 0};
static WatchedThread **watched_threads;
size_t watched_threads_count = 0;

void finish_program(int signal)
{
    size_t thread_index;
    (void)signal;

    for (thread_index = 0; thread_index < watched_threads_count; ++thread_index)
    {
        pthread_kill(watched_threads[thread_index]->thread, SIGTERM);
    }
    exit(1);
}

int is_thread_alive(time_t thread_timestamp)
{
    return (time(NULL) - thread_timestamp) < THREAD_TIMEOUT_SEC ? 0 : 1;
}

void check_threads_timeout()
{
    size_t thread_index;
    for (thread_index = 0; thread_index < watched_threads_count; ++thread_index)
    {
        time_t therad_last_activity = watched_threads[thread_index]->last_execute_time;
        if (is_thread_alive(therad_last_activity) != 0)
        {
            finish_program(SIGTERM);
        }
    }
}

void *watchdog_task(void *args)
{
    watched_threads = (WatchedThread **)args;

    signal(SIGINT, finish_program);
    while (1)
    {
        check_threads_timeout();
        nanosleep(&watchdog_sleep_time, NULL);
    }
}
