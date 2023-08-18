#include "reader.h"

struct timespec reader_sleep_time = {1, 0};
unsigned int core_num = 0;
CoreData *core_data = NULL;

unsigned int count_core_num()
{
    FILE *stat_file = fopen("/proc/stat", "r");
    if (stat_file == NULL)
    {
        perror("Error opening \"/proc/stat\" file...\n");
        exit(1);
    }

    char line[4];
    unsigned int core_counter = 0;

    while (fgets(line, sizeof(line), stat_file))
    {
        if (strncmp(line, "cpu", 3) == 0)
        {
            core_counter++;
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
    if (stat_file == NULL)
    {
        perror("Error opening \"/proc/stat\" file...\n");
        exit(1);
    }

    const char general_reading_format[54] = "cpu%d %%llu %%llu %%llu %%llu %%llu %%llu %%llu %%llu\0";
    char line[256];
    unsigned int core_id;

    fgets(line, 256, stat_file); // Skip first line of file with general cpu information
    for (core_id = 0; core_id < num_of_cores; ++core_id)
    {
        char indexed_reading_format[50];
        snprintf(indexed_reading_format, sizeof(indexed_reading_format), general_reading_format, core_id);

        fgets(line, 256, stat_file);
        printf("%d: %s | %s\n", core_id, indexed_reading_format, line);

        sscanf(line, indexed_reading_format,
               &core_array[core_id].user, &core_array[core_id].nice, &core_array[core_id].system, &core_array[core_id].idle,
               &core_array[core_id].iowait, &core_array[core_id].irq, &core_array[core_id].soft_irq, &core_array[core_id].steal);
    }

    fclose(stat_file);
}

void *reader_task()
{
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
        read_proc_stat(core_data, core_num);
        nanosleep(&reader_sleep_time, NULL);
    }
}