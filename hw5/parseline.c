#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include <stdlib.h>
#include"parseline.h"
#include"safe.h"
int main (int argc, char * argv[]) {
  command * head;

  head = NULL;
  fprintf(stdout,"line: ");
  head = parse(head);
  printCommands(head);
  return 0;
}

command* parse(command* head){
  char *line, *token;
  char **buf;
  line=(char*) safe_malloc(sizeof(char)*MAX_COMMAND_LINE_LENGTH);
  fgets(line,MAX_COMMAND_LINE_LENGTH,stdin);
  buf=&line;
  token=line;
  while(token=strsep(buf,"|")){
    head=makeCommands(head,token);
  }
  return head;
}
command* makeCommands(command *head, char *line){
  
}