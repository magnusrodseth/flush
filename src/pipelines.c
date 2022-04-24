#include <sys/fcntl.h>
#include "../include/pipelines.h"
#include "../include/process.h"
#include "../include/io_redirection.h"

int pipeline(const char *input)
{
    int status = 0;

    if (!is_valid_pipeline(input))
    {
        return ILLEGAL_IO_REDIRECTION;
    }

    char copy[INPUT_COMMAND_LENGTH] = {0};
    strncpy(copy, input, sizeof(copy));

    char *delimiter = "|>";

    int commands_count = get_argument_count(copy, delimiter);

    char **commands = malloc(sizeof(char *) * (commands_count + 1));
    parse_args(copy, commands, commands_count, delimiter);

    bool redirect_out = strchr(input, '>') != NULL;
    status = redirect_pipeline(commands, commands_count, redirect_out);

    free(commands);

    return status;
}

int redirect_pipeline(char **commands, int commands_count, bool redirect_out)
{
    int status = OK;

    int pipes_count = commands_count + 1;

    // Pipe for interprocess communication between N amount of processes
    int pipes[pipes_count][2];
    pid_t pid_buffer[commands_count];

    // Create pipes
    for (int i = 0; i < pipes_count; i++)
    {
        int pipe_status = pipe(pipes[i]);
        int pipe_success = pipe_status == 0;

        if (!pipe_success)
        {
            char message[LOG_MESSAGE_SIZE];
            snprintf(message, sizeof(message), "An error occurred when creating pipe %d.", i);
            log_error(message);

            for (int j = 0; j < i; j++)
            {
                close(pipes[j][READ]);
                close(pipes[j][WRITE]);

                char prior_pipes_message[LOG_MESSAGE_SIZE];
                snprintf(prior_pipes_message, sizeof(prior_pipes_message), "Closing previously opened pipe %d", j);
                log_trace(prior_pipes_message);
            }

            return ERROR;
        }
    }

    // Handle redirection of standard input and output per command
    for (int i = 0; i < commands_count; i++)
    {
        pid_buffer[i] = fork();
        if (pid_buffer[i] < 0)
        {
            char message[LOG_MESSAGE_SIZE];
            snprintf(message, sizeof(message), "An error occurred when creating process number %d\n.", i);
            log_error(message);
            return ERROR;
        }

        // The child executes the command
        if (is_child_process(pid_buffer[i]))
        {
            // Write to the next pipe
            int write_index = i + 1;

            for (int j = 0; j < pipes_count; j++)
            {
                // Close read end of all pipes that are not the current pipe
                if (i != j)
                {
                    close(pipes[j][READ]);
                }

                // Close write end all pipes that are not the next pipe
                if (write_index != j)
                {
                    close(pipes[j][WRITE]);
                }
            }

            if (i == 0)
            {
                // The first pipe writes to the next pipe
                dup2(pipes[write_index][WRITE], STDOUT_FILENO);

                close(pipes[write_index][WRITE]);
                close(pipes[pipes_count - 1][READ]);
            } else if (i == commands_count - 2 && redirect_out)
            {
                // The second to last pipe should write to file if ">" appears as a redirection
                dup2(pipes[i][READ], STDIN_FILENO);

                // Clean up filename that is the last item in the array of commands
                char *filename = commands[i + 1];
                if (filename[0] == ' ' || filename[0] == '\t')
                {
                    filename++;
                }

                // Redirect output to file
                redirect_fd(filename, O_CREAT | O_TRUNC | O_WRONLY,
                            STDOUT_FILENO,
                            0644,
                            &status);

                close(pipes[i][READ]);
            } else if (i == commands_count - 1)
            {
                // The last pipe reads from the previous pipe
                dup2(pipes[i][READ], STDIN_FILENO);

                close(pipes[i][READ]);
            } else
            {
                // The middle pipes read from the previous pipe, and write to the next pipe
                dup2(pipes[i][READ], STDIN_FILENO);
                dup2(pipes[write_index][WRITE], STDOUT_FILENO);

                close(pipes[i][READ]);
                close(pipes[write_index][WRITE]);
            }

            // Execute current command
            command_t *command = initialize_command(commands[i]);
            execvp(command->command, command->args);
            delete_command(command);

            return status;
        }
    }

    // ----- Parent process -----

    // Close unused pipes
    for (int i = 0; i < pipes_count; i++)
    {
        close(pipes[i][READ]);
        close(pipes[i][WRITE]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < commands_count; i++)
    {
        wait(NULL);
    }

    return OK;
}

bool includes_pipe(const char *input)
{
    return (strchr(input, '|') != NULL);
}

bool is_valid_pipeline(const char *input)
{
    bool out = strchr(input, '>') != NULL;
    bool in = strchr(input, '<') != NULL;

    if (out)
    {
        char *out_symbol = strchr(input, '>');
        char *remaining = out_symbol + 1;

        char substring[strlen(remaining)];
        strncpy(substring, remaining, strlen(remaining));

        char *split;
        char *delimiter = "<>|";

        split = strtok(substring, delimiter);
        int count = 0;

        while (split != NULL)
        {
            count++;
            split = strtok(NULL, delimiter);
        }

        // If count == 1,
        // then the input does not have any character that appears in the delimiter
        // after the first stdout redirection (>).
        return count == 1;
    }

    if (in)
    {
        // If we're using pipe, it does not make sense to use stdin redirection (<).
        return false;
    }

    // In this case, the input consists solely of pipe operators.
    return true;
}