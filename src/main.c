#include <pthread.h>
#include "reader.h"
#include "analyzer.h"
#include "printer.h"

int main()
{
    pthread_t reader_thread;
    pthread_t analyzer_thread;
    pthread_t printer_thread;

    system("clear");
    pthread_create(&reader_thread, NULL, *reader_task, NULL);
    pthread_create(&analyzer_thread, NULL, *analyzer_task, NULL);
    pthread_create(&printer_thread, NULL, *printer_task, NULL);

    pthread_join(analyzer_thread, NULL);
    pthread_join(reader_thread, NULL);
    pthread_join(printer_thread, NULL);
}
