#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <regex.h>

extern struct timespec reader_sleep_time;
extern unsigned int core_num;

typedef struct
{
    double *core_usage;
} CPUData;

unsigned int count_core_num();
void *reader_task();

#endif