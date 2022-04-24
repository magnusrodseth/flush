#pragma once

#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "linked_list.h"
#include "logger.h"

bool is_cd_command(const char *command);

int change_directory(const char *path);

bool is_jobs_command(const char *command);

void jobs(linked_list_t *background_tasks);
