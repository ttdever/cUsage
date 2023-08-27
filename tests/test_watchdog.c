#include <assert.h>
#include <time.h>
#include "watchdog.h"

static void test_is_thread_alive(time_t last_thread_activity, int expected_result)
{
    assert(is_thread_alive(last_thread_activity) == expected_result);
}

int main()
{
    test_is_thread_alive(time(NULL), 0);
    test_is_thread_alive(time(NULL) - THREAD_TIMEOUT_SEC, 1);
    return 0;
}
