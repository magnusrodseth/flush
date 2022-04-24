#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.h"
#include "constants.h"

typedef struct command_t
{
    char *command;
    char **args;
} command_t;

int get_argument_count(char input[INPUT_COMMAND_LENGTH], char *delimiter);

void parse_args(char *input, char **result, int result_length, char *delimiter);

command_t *initialize_command(char *input);

void delete_command(command_t *command);
