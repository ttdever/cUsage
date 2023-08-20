#ifndef ANALYZER_H
#define ANALYZER_H

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "structs.h"
#include "shared.h"

void calculate_core_pload();
void *analyzer_task(void *args);

#endif
