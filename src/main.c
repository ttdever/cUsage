#include <pthread.h>
#include "reader.h"

int main()
{
    pthread_t reader_thread;
    pthread_create(&reader_thread, NULL, *reader_task, NULL);
    pthread_join(reader_thread, NULL);
}