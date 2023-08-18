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

extern struct timespec reader_sleep_time;
extern unsigned int core_num;
extern CoreData *core_data;

unsigned int count_core_num();
void *reader_task();

#endif