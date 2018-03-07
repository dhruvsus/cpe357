#ifndef PARSELINE_H
#define PARSELINE_H
#define MAX_COMMAND_LINE_LENGTH 512
#define MAX_COMMANDS_TO_PIPELINE 10
#define MAX_ARGUMENTS_TO_COMMAND 10
#define cmdlenerror "command too long"
#define pplengtherror "pipeline too deep"
#define cmdargnoerror "cmd: too many arguments"
#define ppemptyerror "invalid null command"
#define cmdbadinputerror "cmd: bad input redirection"
#define cmdbadoutputerror "cmd: bad output redirection"
#define cmdambinputerror "cmd: ambiguous input"
#define cmdamboutputerror "cmd: ambiguous output"
#define pipein "pipe from stage"
#define pipeout "pipe to stage"
#endif
