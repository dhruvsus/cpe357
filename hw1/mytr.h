#ifndef MYTR_H
#define MYTR_H
int check_arguments(int num_arguments, char* argument_array[]);
int arr[256];
void init_array();
void del_build_arr(char* del_set);
void rep_build_arr();
char identify_escaped_character(char ch);
void translate();
#endif