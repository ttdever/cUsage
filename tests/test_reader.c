#include <assert.h>
#include "reader.h"

static void test_count_core_num(unsigned int expected_cores, const char *filename)
{
    unsigned int actual_cores = count_core_num_from_file(filename);
    assert(actual_cores == expected_cores);
}

int main()
{
    test_count_core_num(8, "tests/testfiles/cpu_info_8_cores.txt");
    test_count_core_num(2, "tests/testfiles/cpu_info_2_cores.txt");
    return 0;
}
