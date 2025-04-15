#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H

// Takes command and arguments, returns a pointer to the response string
// Response must be a null-terminated string
const char* handle_command(const char *cmd, const char *args);

#endif
