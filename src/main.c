#include <pthread.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"
#include "watchdog.h"
#include "structs.h"

int main()
{
    WatchedThread reader_watched_thread = {.last_execute_time = 0};
    WatchedThread analyzer_watched_thread = {.last_execute_time = 0};
    WatchedThread printer_watched_thread = {.last_execute_time = 0};
    pthread_t watchdog_thread;

    WatchedThread *watched_threads_args[3] = {&reader_watched_thread, &analyzer_watched_thread, &printer_watched_thread};
    watched_threads_count = 3;

    system("clear");
    pthread_create(&reader_watched_thread.thread, NULL, *reader_task, &reader_watched_thread.last_execute_time);
    pthread_create(&analyzer_watched_thread.thread, NULL, *analyzer_task, &analyzer_watched_thread.last_execute_time);
    pthread_create(&printer_watched_thread.thread, NULL, *printer_task, &printer_watched_thread.last_execute_time);
    pthread_create(&watchdog_thread, NULL, *watchdog_task, watched_threads_args);

    pthread_join(reader_watched_thread.thread, NULL);
    pthread_join(analyzer_watched_thread.thread, NULL);
    pthread_join(printer_watched_thread.thread, NULL);
    pthread_join(watchdog_thread, NULL);

    return 0;
}
