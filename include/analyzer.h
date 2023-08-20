#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "structs.h"
#include "shared.h"

void calculate_core_pload(SharedCoreData *core_data, const unsigned int core_num);
void *analyzer_task(void *args);

#endif
