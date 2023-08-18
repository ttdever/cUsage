#include "reader.h"

struct timespec reader_sleep_time = {1, 0};
unsigned int core_num = 0;
CoreData *core_data = NULL;

unsigned int count_core_num()
{
    FILE *stat_file = fopen("/proc/stat", "r");
    char line[4];
    unsigned int core_counter = 0;

    if (stat_file == NULL)
    {
        perror("Error opening \"/proc/stat\" file...\n");
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

void read_proc_stat(CoreData *core_array, unsigned int num_of_cores)
{
    FILE *stat_file = fopen("/proc/stat", "r");
    const char reading_format[] = "cpu%d %llu %llu %llu %llu %llu %llu %llu %llu";
    char line[256];
    unsigned int core_id;

    if (stat_file == NULL)
    {
        perror("Error opening \"/proc/stat\" file...\n");
        exit(1);
    }

    fgets(line, sizeof(line), stat_file); // Skip first line of file with general cpu information
    for (core_id = 0; core_id < num_of_cores; ++core_id)
    {
        fgets(line, sizeof(line), stat_file);
        sscanf(line, reading_format,
               &core_id, &core_array[core_id].user, &core_array[core_id].nice,
               &core_array[core_id].system, &core_array[core_id].idle,
               &core_array[core_id].iowait, &core_array[core_id].irq,
               &core_array[core_id].soft_irq, &core_array[core_id].steal);
    }

    fclose(stat_file);
}

void print_core_stat_array(CoreData *core_array, unsigned int num_of_cores)
{
    unsigned int core_id;

    printf("%2s | %4s | %4s | %6s | %10s | %6s | %3s | %8s | %4s\n", "ID", "User", "Nice", "System", "Idle", "IOwait", "irq", "soft_irq", "steal");
    for (core_id = 0; core_id < num_of_cores; ++core_id)
    {
        printf("%2u | %4llu | %4llu | %6llu | %10llu | %6llu | %3llu | %8llu | %4llu\n",
               core_id, core_array[core_id].user, core_array[core_id].nice, core_array[core_id].system, core_array[core_id].idle,
               core_array[core_id].iowait, core_array[core_id].irq, core_array[core_id].soft_irq, core_array[core_id].steal);
    }
}

void *reader_task(void *arg)
{
    (void)arg;
    core_num = count_core_num();
    if (core_num == 0)
    {
        perror("Wasn't able to count CPU cores...\n");
        exit(1);
    }

    core_data = (CoreData *)calloc(core_num, sizeof(CoreData));
    if (core_data == NULL)
    {
        perror("Failed to allocate memory for core_data array...\n");
        exit(1);
    }

    while (1)
    {
        system("clear");
        read_proc_stat(core_data, core_num);
        print_core_stat_array(core_data, core_num);
        system("cat /proc/stat");
        nanosleep(&reader_sleep_time, NULL);
    }
}
