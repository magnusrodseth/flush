#include "../include/process.h"

bool is_child_process(pid_t pid)
{
    return pid == 0;
}
