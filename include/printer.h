#ifndef PRINTER_H
#define PRINTER_H

#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "shared.h"

void terminate_printer(int signal);
void print_core_data(const unsigned int *cores_percentage_load, const unsigned int cores_number);
void *printer_task(void *args);

#endif
