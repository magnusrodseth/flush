#include "../include/background_tasks.h"


bool is_background_task(const char *input)
{
    const char *last = &input[strlen(input) - 1];

    return strcmp(last, "&") == 0;
}

void collect_background_tasks(linked_list_t *background_tasks)
{
    node_t *current = background_tasks->head;
    while (current != NULL)
    {
        int status = -1;
        if (current->is_running)
        {
            waitpid(current->pid, &status, WNOHANG);
            if (WIFEXITED(status))
            {
                wait(&status);
                log_exit_status(current->command, status);
                current->is_running = false;

                // Free the allocated memory for the node's command
                delete(background_tasks, current->pid);
            }
        }

        current = current->next;
    }
}