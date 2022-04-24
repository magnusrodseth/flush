#pragma once

#define WORKING_DIRECTORY_LENGTH 256
#define MAX_ALLOWED_ARG_LENGTH 64
#define ARRAY_SIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define INPUT_COMMAND_LENGTH 256
#define FOUR_KILOBYTES 4096

typedef enum exit_status_t
{
    ILLEGAL_IO_REDIRECTION = -2,
    ERROR = -1,
    OK = 0,
    ENOENT = 2,
} exit_status_t;

typedef enum pipe_t
{
    READ = 0, WRITE = 1
} pipe_t;
