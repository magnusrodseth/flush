#include "../include/logger.h"

void log_info(const char *message)
{
    fprintf(stdout, "> [INFO]: %s\n", message);
}

void log_trace(const char *message)
{
    fprintf(stdout, "> [TRACE]: %s\n", message);
}

void log_warning(const char *message)
{
    fprintf(stdout, "> [WARNING]: %s\n", message);
}

void log_error(const char *message)
{
    fprintf(stderr, "> [ERROR]: %s\n", message);
}

void log_exit_status(const char *command, int status)
{
    fprintf(stdout, "Exit status [%s] = %d\n", command, status);
}
