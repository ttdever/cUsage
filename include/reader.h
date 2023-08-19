#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <regex.h>

typedef struct CoreData
{
    unsigned long long int user;
    unsigned long long int nice;
    unsigned long long int system;
    unsigned long long int idle;
    unsigned long long int iowait;
    unsigned long long int irq;
    unsigned long long int soft_irq;
    unsigned long long int steal;
} CoreData;

typedef struct SharedCoreData
{
    pthread_mutex_t mutex;
    CoreData *core_data_array;
} SharedCoreData;

extern struct timespec reader_sleep_time;
extern unsigned int core_num;
extern SharedCoreData shared_core_data;

unsigned int count_core_num(void);
void read_proc_stat(SharedCoreData *shared_core_data, unsigned int num_of_cores);
void print_core_stat_array(SharedCoreData *shared_core_data, unsigned int num_of_cores);
void *reader_task(void *arg);

#endif
