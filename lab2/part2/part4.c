#include<stdio.h>
#include<ctype.h>
#include"part4.h"
/*str_lower
 * two arguments: the original input string and a buffer to store the
 * lowercase version of the input string2. Write the str lower function to copy into the buffer the lowercase
 * version of each character in the original string
 *input: input to function: string to be converted to lowercase
 *buffer: output buffer to store lowercased letters
 */
char* str_lower(char* input,char* buffer){
	char* temp=buffer;
	while(*input!='\0'){
		if(isalpha(*input)&& islower(*input)){
			*buffer=(char)(toupper(*input));
		}
		else{
			*buffer=(char)*input;
		}
		input++;
		buffer++;
	}
	*buffer=(char)*input;
	return temp;
}
char* str_lower_mutate(char* mutInput){
	char* temp=mutInput;
	while(*mutInput!='\0'){
		if(isalpha(*mutInput)&& islower(*mutInput)){
			*mutInput=(char)(toupper(*mutInput));
		}
		mutInput++;
	}
	return temp;
}
