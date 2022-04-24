#pragma once

#include <stdio.h>

#define LOG_MESSAGE_SIZE 256

/**
 * @brief Logs an information message.
 *
 * @param message is the message to log.
 */
void log_info(const char *message);


/**
 * @brief Logs a trace message.
 * Trace messages are used to improve the developer experience by providing the developer
 * with better information throughout the lifecycle of the application.
 *
 * @param message is the message to log.
 */
void log_trace(const char *message);

/**
 * @brief Logs a warning message.
 *
 * @param message is the message to log.
 */
void log_warning(const char *message);

/**
 * @brief Logs an error message.
 *
 * @param message is the message to log.
 */
void log_error(const char *message);

/**
 * @brief Prints the exit status of a command.
 *
 * @param command is the command that has exited.
 * @param status is the status of the command.
 */
void log_exit_status(const char *command, int status);
