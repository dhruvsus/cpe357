#include <stdio.h>
#include "checkit.h"
#include "part4.h"
#define TESTARRAYSIZE 10
void test_sum_1()
{
   char test1[TESTARRAYSIZE]="a_c2ef7h@";
   char test1buff[TESTARRAYSIZE]="00000000";
   char test2[TESTARRAYSIZE]="a_c2ef7h@";
   checkit_string(str_lower(test1,test1buff),"A_C2EF7H@");
   checkit_string(str_lower_mutate(test2),"A_C2EF7H@");
}
void test_sum()
{
   test_sum_1();
}

int main(void)
{
   test_sum();

   return 0;
}
