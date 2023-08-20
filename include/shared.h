#ifndef SHARED_H
#define SHARED_H

#include "structs.h"

extern unsigned int core_num;
extern SharedCoreData shared_core_data;

extern unsigned int *cpu_pload;

extern pthread_cond_t reader_cond;

#endif
