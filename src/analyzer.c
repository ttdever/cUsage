#include "analyzer.h"

unsigned int *cpu_pload = NULL;

void calculate_core_pload(SharedCoreData *shared_core_data, const unsigned int core_num)
{

    unsigned int core_id;
    system("clear");
    for (core_id = 0; core_id < core_num; ++core_id)
    {
        unsigned long long int idle = shared_core_data->core_data_array_current[core_id].idle + shared_core_data->core_data_array_current[core_id].iowait;
        unsigned long long int prev_idle = shared_core_data->core_data_array_previous[core_id].idle + shared_core_data->core_data_array_previous[core_id].iowait;

        unsigned long long int non_idle = shared_core_data->core_data_array_current[core_id].user + shared_core_data->core_data_array_current[core_id].nice +
                                          shared_core_data->core_data_array_current[core_id].system + shared_core_data->core_data_array_current[core_id].irq +
                                          shared_core_data->core_data_array_current[core_id].soft_irq + shared_core_data->core_data_array_current[core_id].steal;

        unsigned long long int prev_non_idle = shared_core_data->core_data_array_previous[core_id].user + shared_core_data->core_data_array_previous[core_id].nice +
                                               shared_core_data->core_data_array_previous[core_id].system + shared_core_data->core_data_array_previous[core_id].irq +
                                               shared_core_data->core_data_array_previous[core_id].soft_irq + shared_core_data->core_data_array_previous[core_id].steal;

        unsigned long long int total = idle + non_idle;
        unsigned long long int prev_total = prev_idle + prev_non_idle;

        unsigned long long int total_delta = total - prev_total;
        unsigned long long int idle_delta = idle - prev_idle;

        int core_percentage_load = total_delta == 0 ? 0 : (float)(((float)total_delta - (float)idle_delta) / (float)total_delta) * 100;

        cpu_pload[core_id] = core_percentage_load;

        printf("%d -> %d%%\n", core_id, core_percentage_load);
    }
}

void *analyzer_task(void *args)
{
    (void)args;

    cpu_pload = (unsigned int *)calloc(core_num, sizeof(unsigned int));
    if (cpu_pload == NULL)
    {
        perror("Wasn't able to allocate meomory for cpu load procantage info...\n");
        exit(1);
    }

    while (1)
    {
        pthread_mutex_lock(&shared_core_data.mutex);
        pthread_cond_wait(&reader_cond, &shared_core_data.mutex);
        calculate_core_pload(&shared_core_data, core_num);
        pthread_mutex_unlock(&shared_core_data.mutex);
    }
}