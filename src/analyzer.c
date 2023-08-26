#include "analyzer.h"

unsigned int *cpu_pload = NULL;
pthread_mutex_t printer_reader_mutex;
static time_t *execute_time_pointer;

void terminate_analyzer(int signal)
{
    (void)signal;
    free(cpu_pload);
    cpu_pload = NULL;
}

void calculate_core_pload(const SharedCoreData *core_data, const unsigned int num_of_cores)
{

    unsigned int core_id;
    for (core_id = 0; core_id < num_of_cores; ++core_id)
    {
        unsigned long long int idle = core_data->core_data_array_current[core_id].idle + core_data->core_data_array_current[core_id].iowait;
        unsigned long long int prev_idle = core_data->core_data_array_previous[core_id].idle + core_data->core_data_array_previous[core_id].iowait;

        unsigned long long int non_idle = core_data->core_data_array_current[core_id].user + core_data->core_data_array_current[core_id].nice +
                                          core_data->core_data_array_current[core_id].system + core_data->core_data_array_current[core_id].irq +
                                          core_data->core_data_array_current[core_id].soft_irq + core_data->core_data_array_current[core_id].steal;

        unsigned long long int prev_non_idle = core_data->core_data_array_previous[core_id].user + core_data->core_data_array_previous[core_id].nice +
                                               core_data->core_data_array_previous[core_id].system + core_data->core_data_array_previous[core_id].irq +
                                               core_data->core_data_array_previous[core_id].soft_irq + core_data->core_data_array_previous[core_id].steal;

        unsigned long long int total = idle + non_idle;
        unsigned long long int prev_total = prev_idle + prev_non_idle;

        unsigned long long int total_delta = total - prev_total;
        unsigned long long int idle_delta = idle - prev_idle;

        unsigned int core_percentage_load = total_delta == 0 ? 0 : (unsigned int)((total_delta - idle_delta) * 100 / total_delta);

        cpu_pload[core_id] = core_percentage_load;
    }
}

void *analyzer_task(void *args)
{
    execute_time_pointer = (time_t *)args;
    *execute_time_pointer = time(NULL);

    cpu_pload = (unsigned int *)calloc(core_num, sizeof(unsigned int));
    if (cpu_pload == NULL)
    {
        perror("Wasn't able to allocate meomory for cpu load procantage info...\n");
        exit(1);
    }

    if (pthread_mutex_init(&printer_reader_mutex, NULL) != 0)
    {
        perror("Failed to init printer-reader mutex...\n");
        exit(1);
    }

    signal(SIGTERM, terminate_analyzer);
    while (1)
    {
        *execute_time_pointer = time(NULL);
        pthread_mutex_lock(&shared_core_data.mutex);
        pthread_cond_wait(&reader_cond, &shared_core_data.mutex);
        pthread_mutex_lock(&printer_reader_mutex);
        calculate_core_pload(&shared_core_data, core_num);
        pthread_mutex_unlock(&printer_reader_mutex);
        pthread_cond_signal(&printer_cond);
        pthread_mutex_unlock(&shared_core_data.mutex);
    }
}
