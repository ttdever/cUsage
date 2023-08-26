#include "printer.h"

pthread_cond_t printer_cond = PTHREAD_COND_INITIALIZER;
static time_t *execute_time_pointer;

void print_core_data(const unsigned int *cores_percentage_load, const unsigned int cores_number)
{
    unsigned int core_id;
    unsigned int percentage_load_avg = 0;

    printf("\033[H");
    printf("%12s | %8s\n", "Core Index", "%-Load");
    for (core_id = 0; core_id < cores_number; ++core_id)
    {
        printf("core #%6d | %8d%%\n", core_id, cores_percentage_load[core_id]);
        percentage_load_avg += cores_percentage_load[core_id];
    }
    percentage_load_avg /= cores_number;
    printf("Avg %%-Load = %3u%%\n", percentage_load_avg);
    fflush(stdout);
}

void *printer_task(void *args)
{
    execute_time_pointer = (time_t *)args;
    *execute_time_pointer = time(NULL);

    while (1)
    {
        *execute_time_pointer = time(NULL);
        pthread_mutex_lock(&printer_reader_mutex);
        pthread_cond_wait(&printer_cond, &printer_reader_mutex);
        print_core_data(cpu_pload, core_num);
        pthread_mutex_unlock(&printer_reader_mutex);
    }
}
