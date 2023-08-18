#include "reader.h"

struct timespec reader_sleep_time = {1, 0};
unsigned int core_num = 0;

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

void *reader_task()
{
    core_num = count_core_num();
    if (core_num == 0)
    {
        perror("Wasn't able to count CPU cores...\n");
        exit(1);
    }

    while (1)
    {
        nanosleep(&reader_sleep_time, NULL);
    }
}