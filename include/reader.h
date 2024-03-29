#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include "structs.h"

void terminate_reader(int signal);
unsigned int count_core_num(void);
unsigned int count_core_num_from_file(const char *cpu_info_file);
void read_proc_stat(SharedCoreData *shared_core_data, const unsigned int num_of_cores, const int previous);
void *reader_task(void *arg);

#endif
