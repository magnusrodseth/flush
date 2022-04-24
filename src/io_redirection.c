#include "../include/io_redirection.h"
#include "../include/process.h"


int io_redirection(char *input)
{
    // Clean the input if needed
    if (input[strlen(input) - 1] == ' ')
    {
        input[strlen(input) - 1] = '\0';
    }

    // For more information on permissions: https://stackoverflow.com/questions/18415904/what-does-mode-t-0644-mean
    mode_t permissions = 0644;

    exit_status_t status = OK;

    bool in = strchr(input, '<') != NULL;
    bool out = strchr(input, '>') != NULL;

    pid_t pid = fork();

    if (pid < 0)
    {
        log_error("An error occurred when creating child process.");
        return ERROR;
    }

    if (!is_child_process(pid))
    {
        // ----- Parent process -----
        waitpid(pid, &status, 0);
    } else
    {
        // ----- Child process -----
        if (in && out)
        {
            if (out_before_in(input))
            {
                exit(ILLEGAL_IO_REDIRECTION);
            }

            redirect_in_and_out(input, permissions, &status);
        } else if (in)
        {
            if (has_multiple_chevrons(input))
            {
                exit(ILLEGAL_IO_REDIRECTION);
            }

            redirect_in(input, permissions, &status);
        } else if (out)
        {
            if (has_multiple_chevrons(input))
            {
                exit(ILLEGAL_IO_REDIRECTION);
            }

            redirect_out(input, permissions, &status);
        }

        exit(status);
    }

    return status;
}

void redirect_fd(const char *filename, int file_options, int new_fd, mode_t permissions, int *status)
{
    int fd = open(filename, file_options, permissions);
    bool error = fd == -1;

    if (error)
    {
        *status = fd;

        char message[LOG_MESSAGE_SIZE];
        snprintf(message, sizeof(message), "An error occurred while redirecting.");
        printf("\n");
        log_error(message);

        return;
    } else
    {
        dup2(fd, new_fd);
        close(fd);
    }
}

bool includes_chevron(const char *input)
{
    return (strchr(input, '<') != NULL) || (strchr(input, '>') != NULL);
}

bool out_before_in(const char *input)
{
    const char *out = strchr(input, '>');
    int out_index = (int) (out - input);
    const char *in = strchr(input, '<');
    int in_index = (int) (in - input);

    return out_index < in_index;
}

bool has_multiple_chevrons(const char *input)
{
    // Count will hold the number of tokens after tokenizing the input
    int count = 0;

    char copy[strlen(input)];
    strncpy(copy, input, strlen(input));

    char *split;
    char *delimiter = "<>";

    split = strtok(copy, delimiter);

    while (split != NULL)
    {
        count += 1;
        split = strtok(NULL, delimiter);
    }

    // An input with one chevron will split twice; before and after the chevron
    return count > 2;
}

char *get_filename_for_stdin(char *substring)
{
    char *filename;
    char *delimiter = "<>";

    filename = strtok(substring, delimiter);

    if (filename[0] == ' ')
    {
        filename += 1;
    }
    if (filename[strlen(filename) - 1] == ' ')
    {
        filename[strlen(filename) - 1] = '\0';
    }

    return filename;
}

void execute_command(char *command_input, int *status)
{
    command_t *command = initialize_command(command_input);
    *status = execvp(command->command, command->args);
    delete_command(command);
}

void redirect_out(const char *input, mode_t permissions, int *status)
{
    char *redirection_symbol = strrchr(input, '>');

    // // Parse input to a usable filename and command
    const char *filename = get_filename_for_stdout(redirection_symbol);

    int command_length = (int) ((redirection_symbol - input));
    char command_input[command_length];
    strncpy(command_input, input, command_length);
    command_input[command_length] = '\0';

    redirect_fd(filename,
                O_CREAT | O_TRUNC | O_WRONLY,
                STDOUT_FILENO,
                permissions,
                status);

    execute_command(command_input, status);
}

void redirect_in(const char *input, mode_t permissions, int *status)
{
    char *redirection_symbol = strchr(input, '<');

    if (is_background_task(input))
    {
        printf("\n");
    }

    // Parse input to a usable filename and command
    char substring_copy[MAX_ALLOWED_ARG_LENGTH] = {0};
    strncpy(substring_copy, redirection_symbol, sizeof(substring_copy));
    const char *filename = get_filename_for_stdin(substring_copy);

    int command_length = (int) ((redirection_symbol - input));
    char command_input[command_length];
    strncpy(command_input, input, command_length);
    command_input[command_length] = '\0';

    redirect_fd(filename, O_RDONLY, STDIN_FILENO, permissions, status);

    execute_command(command_input, status);
}


void redirect_in_and_out(const char *input, mode_t permissions, int *status)
{
    // ----- Parse in redirection -----
    char *in_redirection_symbol = strchr(input, '<');

    if (is_background_task(input))
    {
        printf("\n");
    }

    // Parse input to a usable filename and command
    char substring_copy[strlen(in_redirection_symbol)];
    strncpy(substring_copy, in_redirection_symbol, strlen(in_redirection_symbol));
    const char *in_filename = get_filename_for_stdin(substring_copy);

    int command_length = (int) ((in_redirection_symbol - input));
    char command_input[command_length];
    strncpy(command_input, input, command_length);
    command_input[command_length] = '\0';

    redirect_fd(in_filename, O_RDONLY, STDIN_FILENO, permissions, status);

    // ----- Parse out redirection -----
    char *out_redirection_symbol = strrchr(input, '>');

    // Parse input to a usable filename and command
    const char *out_filename = get_filename_for_stdout(out_redirection_symbol);

    redirect_fd(out_filename,
                O_CREAT | O_TRUNC | O_WRONLY,
                STDOUT_FILENO,
                permissions,
                status);

    execute_command(command_input, status);
}

const char *get_filename_for_stdout(char *substring)
{
    char *filename = substring[1] == ' ' ? substring + 2 : substring + 1;

    // Remove & if needed
    if (is_background_task(filename))
    {
        filename[strlen(filename) - 1] = '\0';
    }

    return filename;
}