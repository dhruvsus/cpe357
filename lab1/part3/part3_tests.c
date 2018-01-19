#include <stdio.h>

#include "checkit.h"
#include "part3.h"
#define TESTARRAYSIZE 15
void test_sum_1()
{
   int test1[TESTARRAYSIZE]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
   checkit_int(sum(test1,2),3);
   checkit_int(sum(test1,0),0);
   checkit_int(sum(test1,15),120);
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
