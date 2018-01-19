#include<stdio.h>
#include"mytr.h"
#include<ctype.h>
#include<string.h>

int main(int argc, char* argv[]){
   int result;
   int delete=0;
   result=check_arguments();
   if(result==1){
       return result;
   }
   else if(strcmp(argv[1],"-d")==0){
       delete=1;
   }
   init_array(arr);
   if(delete){
       delete_tr(argv);
   }
   else{
       replace_tr(argv);
   }
}
int check_arguments(int num_arguments, char* argument_array[]){
	if(num_arguments<3){
        printf("tr: missing operand\nTry 'tr --help' for more information.\n");
        return 1;
    }
    else if(strcmp(argument_array[1],"-d")==0 && num_arguments>3){
        printf("tr: extra operand '%s'\nOnly one string may be given when deleting without squeezing repeats.\nTry 'tr --help' for more information.\n", argv[3]);
        return 1;
    }
    else if(num_arguments>3){
        printf("tr: extra operand ‘%s’\nTry 'tr --help' for more information.\n", argument_array[3]);
        return 1;
    }
    else{
        return 0;
    }
}
void init_array(int* arr){
    int i=0;
    for(i=0;i<256;i++){
        arr[i]=(int)(i);
    }
}
void delete_tr(char* argument_array[]){
    char* del_set=argument_array[2];
    while(*del_set!='\0'){
        if(*del_set=='\' && *del_set!='\0'){
            switch(*(del_set+1)){
                
            }
        }
    }
}