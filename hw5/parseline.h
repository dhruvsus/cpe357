#ifndef PARSELINE_H
#define PARSELINE_H
#define MAX_COMMAND_LINE_LENGTH 512
#define MAX_COMMANDS_TO_PIPELINE 10
#define MAX_ARGUMENTS_TO_COMMAND 10
#define pipein "pipe from stage"
#define pipeout "pipe to stage"
typedef struct command{
    char line[MAX_COMMAND_LINE_LENGTH];
    char input[50];
    char output[50];
    int argc;
    char * argv[MAX_ARGUMENTS_TO_COMMAND];
    int numcommand;
    struct command * next;
} command;
command * parse(command * head);
void printCommands(command * head);
command * makeCommands(command * head, char * line);
#endif
