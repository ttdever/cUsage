#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <regex.h>
#include <stdbool.h>
#include "structs.h"

extern struct timespec reader_sleep_time;

unsigned int count_core_num(void);
void read_proc_stat(SharedCoreData *shared_core_data, const unsigned int num_of_cores, const bool previous);
void print_core_stat_array(SharedCoreData *shared_core_data, const unsigned int num_of_cores);
void *reader_task(void *arg);

#endif
