#include "../include/commands.h"

bool is_cd_command(const char *command)
{
    char first_chars[3];
    strncpy(first_chars, command, 2);
    first_chars[2] = '\0';

    return strcmp(first_chars, "cd") == 0;
}

int change_directory(const char *path)
{
    int status;

    bool has_arguments = (path != NULL);

    if (has_arguments)
    {
        bool is_root_directory = (strcmp(path, "~") == 0);
        if (is_root_directory)
        {
            status = chdir(getenv("HOME"));
        } else
        {
            status = chdir(path);
        }
    } else
    {
        status = chdir(getenv("HOME"));
    }

    return status;
}

bool is_jobs_command(const char *command)
{
    char first_chars[5];
    strncpy(first_chars, command, 4);
    first_chars[4] = '\0';

    return strcmp(first_chars, "jobs") == 0;
}

void jobs(linked_list_t *background_tasks)
{
    if (is_empty(background_tasks))
    {
        log_info("No background tasks.");
    } else
    {
        print(background_tasks);
    }
}