#include<stdio.h>
#include"mytr.h"
#include<string.h>

int main(int argc, char* argv[]){
    int result;
    result=check_arguments(argc,argv);
    if(result==1){
        return 1;
    }
    else{
        init_array();
        if(strcmp(argv[1],"-d")==0){
            del_build_arr(argv[2]);
        }
        else{
            rep_build_arr(argv[1],argv[2]);
        }
        translate();
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
    else if(num_arguments>3) {
        printf("tr: extra operand ‘%s’\nTry 'tr --help' for more information.\n", argument_array[3]);
        return 1;
    }
    return 0;
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
            *del_set = identify_escaped_character(*del_set);
        }

        arr[(int)*del_set] = -1;
        del_set++;
    }
}
void rep_build_arr(char* set1, char* set2){
    while(*set1){
        if(*set1 == '\\' && *(set1 + 1) && (*(set1+1)=='t'||*(set1+1)=='n'||*(set1+1)=='\\')){
            set1++;
            arr[(int) *set1] = *set1;
            set1++;
        }else{
            if(*set2 == '\\' && *(set2 + 1) && (*(set2+1)=='t'||*(set2+1)=='n'||*(set2+1)=='\\') ){
                set2++;
                *set2 = identify_escaped_character(*set2);
            }

            if(*set1 == '\\' && *(set1 + 1) && (*(set1+1)=='t'||*(set1+1)=='n'||*(set1+1)=='\\') ){
                set1++;
                *set1 = identify_escaped_character(*set1);
            }
            arr[(int) *set1] = *set2;
            set1++;
        }

        if(*(set2 + 1)){
            set2++;
        }
    }
}
char identify_escaped_character(char c){

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
void translate(){
    char c;
    while((c=getchar())!= EOF){
        if((char)arr[(int)c]!=-1){
            putchar((char)arr[(int)c]);
        }
    }
}