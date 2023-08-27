#include "reader.h"
#include "shared.h"

static struct timespec reader_sleep_time = {0, 500000000L};
static time_t *execute_time_pointer;
static FILE *stat_file;

unsigned int core_num = 0;
SharedCoreData shared_core_data;
pthread_cond_t reader_cond = PTHREAD_COND_INITIALIZER;

void terminate_reader(int signal)
{
    (void)signal;
    fclose(stat_file);
    pthread_mutex_lock(&shared_core_data.mutex);
    free(shared_core_data.core_data_array_previous);
    free(shared_core_data.core_data_array_current);

    shared_core_data.core_data_array_previous = NULL;
    shared_core_data.core_data_array_current = NULL;
    pthread_mutex_unlock(&shared_core_data.mutex);
}

unsigned int count_core_num()
{
    return count_core_num_from_file("/proc/stat");
}

unsigned int count_core_num_from_file(const char *cpu_info_file)
{
    char line[4];
    unsigned int core_counter = 0;

    stat_file = fopen(cpu_info_file, "r");

    if (stat_file == NULL)
    {
        printf("Error opening \"%s\" file...\n", cpu_info_file);
        exit(1);
    }

    while (fgets(line, sizeof(line), stat_file))
    {
        if (strncmp(line, "cpu", 3) == 0)
        {
            ++core_counter;
        }
        else if (strncmp(line, "int", 3) == 0)
        {
            core_counter = core_counter == 0 ? core_counter : core_counter - 1;
            break;
        }
    }

    fclose(stat_file);
    return core_counter;
}

void read_proc_stat(SharedCoreData *core_data, const unsigned int num_of_cores, const int previous)
{
    const char reading_format[] = "cpu%d %llu %llu %llu %llu %llu %llu %llu %llu";
    char line[256];
    unsigned int core_id;

    stat_file = fopen("/proc/stat", "r");

    if (stat_file == NULL)
    {
        perror("Error opening \"/proc/stat\" file...\n");
        exit(1);
    }

    fgets(line, sizeof(line), stat_file); // Skip first line of file with general cpu information
    pthread_mutex_lock(&core_data->mutex);
    for (core_id = 0; core_id < num_of_cores; ++core_id)
    {
        fgets(line, sizeof(line), stat_file);
        if (previous == 0)
        {
            sscanf(line, reading_format,
                   &core_id, &core_data->core_data_array_previous[core_id].user, &core_data->core_data_array_previous[core_id].nice,
                   &core_data->core_data_array_previous[core_id].system, &core_data->core_data_array_previous[core_id].idle,
                   &core_data->core_data_array_previous[core_id].iowait, &core_data->core_data_array_previous[core_id].irq,
                   &core_data->core_data_array_previous[core_id].soft_irq, &core_data->core_data_array_previous[core_id].steal);
        }
        else
        {
            sscanf(line, reading_format,
                   &core_id, &core_data->core_data_array_current[core_id].user, &core_data->core_data_array_current[core_id].nice,
                   &core_data->core_data_array_current[core_id].system, &core_data->core_data_array_current[core_id].idle,
                   &core_data->core_data_array_current[core_id].iowait, &core_data->core_data_array_current[core_id].irq,
                   &core_data->core_data_array_current[core_id].soft_irq, &core_data->core_data_array_current[core_id].steal);
        }
    }
    pthread_mutex_unlock(&core_data->mutex);

    fclose(stat_file);
}

void *reader_task(void *arg)
{

    execute_time_pointer = (time_t *)arg;
    *execute_time_pointer = time(NULL);

    core_num = count_core_num();
    if (core_num == 0)
    {
        perror("Wasn't able to count CPU cores...\n");
        exit(1);
    }

    if (pthread_mutex_init(&shared_core_data.mutex, NULL) != 0)
    {
        perror("Failed to init reader-analyzer mutex...\n");
        exit(1);
    }

    pthread_mutex_lock(&shared_core_data.mutex);
    shared_core_data.core_data_array_previous = (CoreData *)calloc(core_num, sizeof(CoreData));
    shared_core_data.core_data_array_current = (CoreData *)calloc(core_num, sizeof(CoreData));
    if (shared_core_data.core_data_array_previous == NULL || shared_core_data.core_data_array_current == NULL)
    {
        perror("Failed to allocate memory for core_data array...\n");
        exit(1);
    }
    pthread_mutex_unlock(&shared_core_data.mutex);

    signal(SIGTERM, terminate_reader);
    while (1)
    {
        *execute_time_pointer = time(NULL);
        read_proc_stat(&shared_core_data, core_num, 0);
        nanosleep(&reader_sleep_time, NULL);
        read_proc_stat(&shared_core_data, core_num, 1);
        pthread_cond_signal(&reader_cond);
        nanosleep(&reader_sleep_time, NULL);
    }
}
