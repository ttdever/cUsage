#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "shared.h"

void terminate_analyzer(int signal);
void calculate_core_pload(const SharedCoreData *core_data, const unsigned int core_num);
void *analyzer_task(void *args);

#endif
