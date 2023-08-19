#include "analyzer.h"

unsigned short calculate_core_pload(SharedCoreData *shared_core_data, const unsigned int core_num)
{

    

    return 0;
}

void *analyzer_task(void *args)
{
    (void)args;

    while (1)
    {
        pthread_mutex_lock(&shared_core_data.mutex);
        pthread_cond_wait(&reader_cond, &shared_core_data.mutex);
        calculate_core_pload(&shared_core_data, core_num);
        pthread_mutex_unlock(&shared_core_data.mutex);
    }
}