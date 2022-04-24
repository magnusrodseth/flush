#include "../include/command_line_parser.h"
#include "../include/background_tasks.h"

int get_argument_count(char input[INPUT_COMMAND_LENGTH], char *delimiter)
{
    char copy[INPUT_COMMAND_LENGTH];
    strncpy(copy, input, sizeof(copy));

    int count = 0;
    char *argument;

    argument = strtok(copy, delimiter);

    while (argument != NULL)
    {
        argument = strtok(NULL, delimiter);
        count++;
    }

    return count;
}

void parse_args(char *input, char **result, int result_length, char *delimiter)
{
    // Because strtok is destructive, we copy the input in order to not manipulate it
    char copy[INPUT_COMMAND_LENGTH] = {0};
    strncpy(copy, input, INPUT_COMMAND_LENGTH);

    char *argument;

    int i = 0;

    argument = strtok(copy, delimiter);
    while (argument != NULL)
    {
        result[i] = malloc(strnlen(argument, MAX_ALLOWED_ARG_LENGTH));
        strncpy(result[i], argument, MAX_ALLOWED_ARG_LENGTH);
        i++;

        argument = strtok(NULL, delimiter);
    }
    result[result_length] = NULL;
}


command_t *initialize_command(char *input)
{
    int argument_count = get_argument_count(input, " \t\r");
    char **args = malloc(sizeof(char *) * (argument_count + 1));
    parse_args(input, args, argument_count, " \t\r");
    char *command_input = args[0];

    command_t *command = malloc(sizeof(command_t));
    command->command = command_input;
    command->args = args;

    return command;
}


void delete_command(command_t *command)
{
    free(command->args);
    free(command);
}
