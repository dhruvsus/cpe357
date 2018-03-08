#ifndef PARSELINE_H
#define PARSELINE_H
#define MAX_COMMAND_LINE_LENGTH 512
#define MAX_COMMANDS_TO_PIPELINE 10
#define MAX_ARGUMENTS_TO_COMMAND 10
#define cmdlenerror "command too long\n"
#define pplengtherror "pipeline too deep\n"
#define cmdargnoerror "too many arguments\n"
#define ppemptyerror "invalid null commandn\n"
#define cmdbadinputerror "bad input redirection\n"
#define cmdbadoutputerror "bad output redirection\n"
#define cmdambinputerror "ambiguous input\n"
#define cmdamboutputerror "ambiguous output\n"
#define pipein "pipe from command\n"
#define pipeout "pipe to command\n"
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
