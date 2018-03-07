#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include"parseline.h"
int main(){
  handleInput();
  return 0;
}
int handleInput(){
  char stringInput[MAX_COMMAND_LINE_LENGTH];
  scanf("%s",stringInput);
  printf("%s",stringInput);
}
