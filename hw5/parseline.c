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
  command *last, *trav;
  char *tok, **buf, temp[2];
  int x;
  char in[] = "original stdin";
  char out[] = "original stdout";
  char toString[] = pipeout;
  char fromString[] = pipein;
  static int to=0,from=0,current=0;
  if(head == NULL) {
      head = safe_malloc(sizeof(*head));
      last = NULL;
      trav = head;
  } else {
      trav = head;
      while(trav->next){
          trav = trav->next;
      }
      last = trav;
      trav = trav->next;
      trav = safe_malloc(sizeof(*trav));
  }
  if(line[strlen(line)-1] == '\n') {
      line[strlen(line)-1] = '\0';
  }
  if(!last) {
      strcpy(trav->input, in);
  } else {
      strcpy(trav->input, fromString);
      sprintf(temp, "%d", current-1);
      strcat(trav->input, temp);
      if(last) {
          strcpy(last->output, toString);
          sprintf(temp, "%d", current);
          strcat(last->output, temp);
      }
      last->next = trav;
  }
  strcpy(trav->output, out);
  strcpy(trav->line, line);
  buf = &line;
  tok = line;
  x = 0;
  while((tok = strsep(buf, " "))) {
      if(strlen(tok) == 0) {
          continue;
      }
      if(*tok == '>') {
          if(to) {
              if(last) {
                  fprintf(stderr,"%s: ambiguous input\n",trav->argv[0]);
                  exit(1);
              }
              fprintf(stderr,"%s: bad input redirection\n", trav->argv[0]);
              exit(1);
          }
          tok = strsep(buf, " ");
          strcpy(trav->output, tok);
          to = 1;
      } else if(*tok == '<') {
          if(from) {
              if(last) {
                  fprintf(stderr,"%s: ambiguous input\n",trav->argv[0]);
                  exit(1);
              }
              fprintf(stderr,"%s: bad input redirection\n", trav->argv[0]);
              exit(1);
          }
          tok = strsep(buf, " ");
          strcpy(trav->input, tok);
          from = 1;
      } else {
          trav->argv[x++] = tok;
      }
  }
  if(!trav->argv[0]) {
       fprintf(stderr,"invalid null command\n");
       exit(1);
  }
  trav->numcommand = current++;
  trav->argc = x;
  trav->next = NULL;
  return head;
}
void printCommands(command * head) {
  int x;
  command *trav = head;
  x = 0;
  while(trav) {
      printf("--------\n");
      printf("Stage %d: \"%s\"\n", trav->numcommand, trav->line);
      printf("--------\n");
      printf("\t input: %s\n", trav->input);
      printf("\t output: %s\n", trav->output);
      printf("\t argc: %d\n", trav->argc);
      printf("\t argv: \"%s\"", trav->argv[0]);
      for(x = 1; trav->argv[x]; x++) {
          printf(",\"%s\"", trav->argv[x]);
      }
      trav = trav->next;
      printf("\n");
  }
  printf("\n");
}
