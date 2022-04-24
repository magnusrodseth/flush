#include "../include/app.h"
#include "../include/process.h"

app_t *initialize_app()
{
    app_t *app = malloc(sizeof(app_t));
    linked_list_t *background_tasks = initialize_linked_list();

    app->is_running = false;
    app->background_tasks = background_tasks;

    log_trace("Initialized application.");
    return app;
}

void run_app(app_t *app)
{
    app->is_running = true;
    while (app->is_running)
    {
        collect_background_tasks(app->background_tasks);

        // Output current working directory
        char current_directory[WORKING_DIRECTORY_LENGTH];
        getcwd(current_directory, WORKING_DIRECTORY_LENGTH);
        printf("%s: ", current_directory);

        // Read input until new line
        char input[INPUT_COMMAND_LENGTH];
        int input_status = scanf(" %[^\n]s", input);

        // Quit application on CTRL + D
        if (input_status == EOF)
        {
            app->is_running = false;
            return;
        }

        if (is_cd_command(input))
        {
            command_t *command = initialize_command(input);
            char *target = command->args[1];
            // The cd command needs to be an internal shell command
            int status = change_directory(target);
            bool success = (status == 0);

            if (!success)
            {
                char message[LOG_MESSAGE_SIZE];
                snprintf(message, sizeof(message), "No such file or directory: %s",
                         target);
                log_info(message);
            }

            log_exit_status(input, status);
            delete_command(command);
            continue;
        }

        if (is_jobs_command(input))
        {
            jobs(app->background_tasks);
            continue;
        }

        // Pipe for interprocess communication
        int fd[2];
        int pipe_status = pipe(fd);
        int pipe_success = pipe_status == 0;

        if (!pipe_success)
        {
            log_error("An error occurred when creating pipe.");
            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            log_error("An error occurred when creating child process.");
            continue;
        }

        if (!is_child_process(pid))
        {
            // ----- Parent process -----
            close(fd[WRITE]);

            char buffer[FOUR_KILOBYTES];
            exit_status_t status = ERROR;

            if (is_background_task(input))
            {
                // This command holds the input without &.
                // This is freed when background processes are collected.
                // For reference, see the method collect_background_tasks.
                char *command = malloc(strlen(input) - 1);
                strncpy(command, input, strlen(input) - 1);

                char message[LOG_MESSAGE_SIZE];
                snprintf(message, sizeof(message), "Started process with PID %d as background task.", pid);
                log_info(message);

                // Store PID of background process in a linked list
                insert_first(app->background_tasks, pid, command);
            } else
            {
                // Redirect stdout from child process to parent process
                while (!WIFEXITED(status))
                {
                    ssize_t read_status = read(fd[READ], buffer, sizeof(buffer));

                    if (read_status != 0)
                    {
                        printf("%s", buffer);
                    }

                    waitpid(pid, &status, 0);
                }

                // At this point, the child process has terminated with a status code
                log_exit_status(input, WEXITSTATUS(status));
            }
        } else
        {
            // ---- Child process -----
            exit_status_t status = ENOENT;

            dup2(fd[WRITE], STDIN_FILENO);
            close(fd[WRITE]);

            if (is_background_task(input))
            {
                // Clean the input
                input[strlen(input) - 1] = '\0';
                printf("\n");
            }

            if (includes_pipe(input))
            {
                // Pipeline redirection
                status = pipeline(input);
            } else if (includes_chevron(input))
            {
                // IO redirection
                status = io_redirection(input);
            } else
            {
                // System call
                command_t *command = initialize_command(input);
                status = execvp(command->command, command->args);
                delete_command(command);
            }

            // Validate status
            if (status == ILLEGAL_IO_REDIRECTION)
            {
                printf("\n");
                log_warning(
                        "Illegal IO redirection!\n"
                        "This shell only accepts input and output redirections as the last parameter of the command line input.\n");
            } else if (status != OK)
            {
                char message[LOG_MESSAGE_SIZE];
                snprintf(message, sizeof(message),
                         "An error occurred when executing command: %s", input);
                printf("\n");
                log_error(message);
            }

            exit(status);
        }

    }
}

void delete_app(app_t *app)
{
    delete_linked_list(app->background_tasks);
    free(app);
    printf("\n");
    log_trace("Successfully terminated application.");
}
