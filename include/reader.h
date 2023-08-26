#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include "structs.h"

unsigned int count_core_num(void);
void read_proc_stat(SharedCoreData *shared_core_data, const unsigned int num_of_cores, const int previous);
void *reader_task(void *arg);

#endif
