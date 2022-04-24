set(PROJECT flush)

cmake_minimum_required(VERSION 3.21)

project(${PROJECT} C)

set(CMAKE_C_STANDARD 99)

add_executable(
        ${PROJECT}
        src/main.c
        src/app.c
        src/logger.c
        src/command_line_parser.c
        include/app.h
        include/logger.h
        include/command_line_parser.h
        include/constants.h
        include/commands.h src/commands.c include/io_redirection.h src/io_redirection.c src/background_tasks.c include/background_tasks.h src/linked_list.c include/linked_list.h src/pipelines.c include/pipelines.h src/process.c include/process.h)

