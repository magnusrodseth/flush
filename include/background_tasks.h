#pragma once

#include <unistd.h>
#include <stdbool.h>
#include <string.h>

#include "linked_list.h"

bool is_background_task(const char *input);

void collect_background_tasks(linked_list_t *background_tasks);
