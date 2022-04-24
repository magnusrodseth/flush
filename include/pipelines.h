#pragma once

#include <stdbool.h>
#include <string.h>
#include "constants.h"
#include "command_line_parser.h"

int redirect_pipeline(char **commands, int commands_count, bool redirect_out);

int pipeline(const char *input);

bool includes_pipe(const char *input);

/**
 * Checks if the redirection for the pipe is valid. \n\n
 *
 * The redirection is valid if the input consists solely of pipe redirection.\n
 * Example: "ls -l | grep .md | cat" \n\n
 *
 * If the input has a stdout redirection, it is valid if no other redirection appears after that stdout redirection. \n
 * Example: "ls -l | grep .md | cat > /tmp/file" \n\n
 *
 * @param input is the input to validate.
 * @return a boolean value indicating if the pipeline redirection is valid.
 */
bool is_valid_pipeline(const char *input);
