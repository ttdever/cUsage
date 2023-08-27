#include <assert.h>
#include <stdio.h>
#include "analyzer.h"

static void test_calculate_core_pload()
{
    unsigned int expected_loads[] = {42, 42, 42, 42, 42, 42, 42, 42};
    SharedCoreData core_data;
    unsigned int num_of_cores = 8;

    core_data.core_data_array_previous = (CoreData *)calloc(num_of_cores, sizeof(CoreData));
    core_data.core_data_array_current = (CoreData *)calloc(num_of_cores, sizeof(CoreData));
    cpu_pload = (unsigned int *)calloc(num_of_cores, sizeof(unsigned int));
    assert(core_data.core_data_array_previous != NULL);
    assert(core_data.core_data_array_current != NULL);
    assert(cpu_pload != NULL);

    for (unsigned int core_id = 0; core_id < num_of_cores; ++core_id)
    {
        core_data.core_data_array_previous[core_id].user = 100;
        core_data.core_data_array_previous[core_id].nice = 20;
        core_data.core_data_array_previous[core_id].system = 200;
        core_data.core_data_array_previous[core_id].idle = 1000;
        core_data.core_data_array_previous[core_id].iowait = 200;
        core_data.core_data_array_previous[core_id].irq = 10;
        core_data.core_data_array_previous[core_id].soft_irq = 5;
        core_data.core_data_array_previous[core_id].steal = 5;

        core_data.core_data_array_current[core_id].user = 150;
        core_data.core_data_array_current[core_id].nice = 25;
        core_data.core_data_array_current[core_id].system = 220;
        core_data.core_data_array_current[core_id].idle = 1100;
        core_data.core_data_array_current[core_id].iowait = 210;
        core_data.core_data_array_current[core_id].irq = 12;
        core_data.core_data_array_current[core_id].soft_irq = 6;
        core_data.core_data_array_current[core_id].steal = 7;
    }

    calculate_core_pload(&core_data, num_of_cores);

    for (unsigned int core_id = 0; core_id < num_of_cores; ++core_id)
    {
        assert(cpu_pload[core_id] == expected_loads[core_id]);
    }

    free(core_data.core_data_array_previous);
    free(core_data.core_data_array_current);
    free(cpu_pload);
}

int main()
{
    test_calculate_core_pload();
    return 0;
}
