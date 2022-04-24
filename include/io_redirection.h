#pragma once

#include <stdbool.h>
#include <string.h>
#include <sys/fcntl.h>
#include <unistd.h>

#include "constants.h"
#include "command_line_parser.h"
#include "background_tasks.h"
#include "pipelines.h"


/**
 * Handles IO redirection based on input.
 * @param input is the input used when performing the redirection
 * @return the exit code of the entire IO redirection operation.
 */
int io_redirection(char *input);


/**
 * Redirects a file descriptor to a new file descriptor.
 * @param filename is the filename of the file to redirect.
 * @param file_options is the file options of the file to redirect.
 * @param new_fd is the new file descriptor.
 * @param permissions is the permissions of the file to redirect.
 * @param status is a reference to the status code.
 */
void redirect_fd(const char *filename, int file_options, int new_fd, mode_t permissions, int *status);

/**
 * Checks if the input includes a chevron, also called an angle bracket ('<' or '>').
 *
 * @param input is the input to check
 * @return a boolean indicating if the input includes a chevron.
 */
bool includes_chevron(const char *input);


/**
 * Checks if the input includes a pipe (|).
 *
 * @param input is the input to check
 * @return a boolean indicating if the input includes a pipe.
 */
bool includes_pipe(const char *input);


/**
 * Checks if a chevron for redirecting stdout appears before the chevron for redirecting stdin.
 *
 * For instance, this function would return true for the input "A > B < C", and false for the input "A < B > C".
 * @param input is the input to check.
 * @return a boolean value indicating whether the chevron for redirecting stdout appears before the chevron for redirecting stdin.
 */
bool out_before_in(const char *input);

/**
 * Checks if the provided input has multiple chevrons.
 * @param input is the input to check.
 * @return a boolean value indicating if the input has multiple chevrons.
 */
bool has_multiple_chevrons(const char *input);

/**
 * Gets the filename when redirecting stdin.
 *
 * For instance, the substring "cat < in.txt" would return "in.txt".
 * @param substring is the substring to clean.
 * @return the filename to be used when redirecting stdin.
 */
char *get_filename_for_stdin(char *substring);

/**
 * Gets the filename when redirecting stdout.
 *
 * For instance, the substring "ls > out.txt" would return "out.txt".
 *
 * @param substring is the substring to clean.
 * @return the filename to be used when redirecting stdout.
 */
const char *get_filename_for_stdout(char *substring);

/**
 * Wrapper function for redirecting stdout.
 *
 * @param input is the input to parse when redirecting stdout.
 * @param permissions is the file permissions.
 * @param status is a reference to the status code.
 */
void redirect_out(const char *input, mode_t permissions, int *status);

/**
 * Wrapper function for redirecting stdin.
 *
 * @param input is the input to parse when redirecting stdin.
 * @param permissions is the file permissions.
 * @param status is a reference to the status code.
 */
void redirect_in(const char *input, mode_t permissions, int *status);


/**
 * Wrapper function for redirecting both stdin and stdout.
 *
 * @param input is the input to parse when redirecting stdin and stdout.
 * @param permissions is the file permissions.
 * @param status is a reference to the status code.
 */
void redirect_in_and_out(const char *input, mode_t permissions, int *status);

/**
 * Wrapper function for executing a command.
 * @param command_input is the command input to parse.
 * @param status is a reference to the status code.
 */
void execute_command(char *command_input, int *status);
