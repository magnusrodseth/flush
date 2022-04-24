#include "../include/linked_list.h"


linked_list_t *initialize_linked_list()
{
    linked_list_t *linked_list = malloc(sizeof(linked_list_t));
    return linked_list;
}


void delete_linked_list(linked_list_t *linked_list)
{
    node_t *current = NULL;

    while (linked_list->head != NULL)
    {
        current = linked_list->head;
        linked_list->head = linked_list->head->next;

        free(current);
    }
}

void print(linked_list_t *linked_list)
{
    node_t *current = linked_list->head;
    printf("\n");

    while (current != NULL)
    {
        if (current->is_running)
        {
            printf("[%d, %s]\n", current->pid, current->command);
            current = current->next;
        }
    }

    printf("\n");
}

void insert_first(linked_list_t *linked_list, pid_t pid, char *command)
{
    node_t *node = malloc(sizeof(node_t));
    node->pid = pid;
    node->command = command;
    node->is_running = true;

    node->next = linked_list->head;

    linked_list->head = node;
}

bool is_empty(linked_list_t *linked_list)
{
    return linked_list->head == NULL;
}

void delete(linked_list_t *linked_list, pid_t pid)
{
    node_t *current = linked_list->head;
    node_t *previous = NULL;

    if (is_empty(linked_list))
    {
        return;
    }

    while (current->pid != pid)
    {
        if (current->next == NULL)
        {
            return;
        }

        previous = current;
        current = current->next;
    }

    // At this point, we found a match
    if (current == linked_list->head)
    {
        linked_list->head = linked_list->head->next;
    } else
    {
        previous->next = current->next;
    }

    free(current);
}