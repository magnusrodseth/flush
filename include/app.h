#pragma once


#include "command_line_parser.h"
#include "commands.h"
#include "logger.h"
#include "command_line_parser.h"
#include "io_redirection.h"
#include "background_tasks.h"
#include "constants.h"
#include "linked_list.h"

#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>


typedef struct app_t
{
    bool is_running;
    linked_list_t *background_tasks;
} app_t;

app_t *initialize_app();

void run_app(app_t *app);

void delete_app(app_t *app);
