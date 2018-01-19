#include<stdio.h>
#include"mytr.h"
#include<ctype.h>
#include<string.h>

int main(int argc, char* argv[]){
    int result;
    result=check_arguments(argc,argv);
    if(result==1){
        return result;
    }
    else{
        init_array();
        if(strcmp(argv[1],"-d")==0){
            del_build_arr(argv[2]);
        }
        else{
            rep_build_arr();
        }
    }
    return 0;
}
int check_arguments(int num_arguments, char* argument_array[]){
    if(num_arguments<3){
        printf("tr: missing operand\nTry 'tr --help' for more information.\n");
        return 1;
    }
    else if(strcmp(argument_array[1],"-d")==0 && num_arguments>3){
        printf("tr: extra operand '%s'\nOnly one string may be given when deleting without squeezing repeats.\nTry 'tr --help' for more information.\n", argument_array[3]);
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
void init_array(){
    int iter;
    for(iter=0;iter<256;iter++){
        arr[iter]=iter;
    }
}
void del_build_arr(char* del_set){
    while(*del_set){
        if(*del_set == '\\' && *(del_set + 1)){
            del_set++;
            *del_set = find_escape_characters(*del_set);
        }

        arr[(int) *del_set] = '\0';
        del_set++;
    }
}
char find_escape_characters(char c){

    switch (c)
    {
        case 't':
            return '\t';
        case 'n':
            return '\n';
        case '\\':
            return '\\';
        default:
            return c;
    }
}