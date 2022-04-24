#pragma once

#include <unistd.h>
#include <stdlib.h>

#include "../include/command_line_parser.h"

typedef struct node_t
{
    pid_t pid;
    char *command;
    bool is_running;
    struct node_t *next;
} node_t;

typedef struct linked_list_t
{
    node_t *head;
} linked_list_t;

linked_list_t *initialize_linked_list();

void delete_linked_list(linked_list_t *linked_list);

void print(linked_list_t *linked_list);

void insert_first(linked_list_t *linked_list, pid_t pid, char *command);

void delete(linked_list_t *linked_list, pid_t pid);

bool is_empty(linked_list_t *linked_list);

/**
 * Deletes a node from the linked list based on the identifier (PID).
 *
 * Inspired by https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
 * @param linked_list is the linked list
 * @param pid is the PID of the node to delete
 * @return the deleted node, or NULL if no node with the identifier was found.
 */
void delete(linked_list_t *linked_list, pid_t pid);
