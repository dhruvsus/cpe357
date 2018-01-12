#include <stdio.h>
#include "checkit.h"
#include "part6.h"

/* define testing functions */
void test_create_point1(void)
{
   struct point p = create_point(1.9, -2.7);

   checkit_double(p.x, 1.9);
   checkit_double(p.y, -2.7);
}

void test_create_point2(void)
{
   struct point p = create_point(0.2, 12.1);

   checkit_double(p.x, 0.2);
   checkit_double(p.y, 12.1);
}

void test_create_point(void)
{
   test_create_point1();
   test_create_point2();
}
void test_is_square1(void){
   rect testRec;
   testRec.bottomRight.x=10;
   testRec.bottomRight.y=0;
   testRec.topLeft.x=0;
   testRec.topLeft.y=10;
   checkit_int(is_a_square(testRec),1);
}
void test_is_square2(void){
   rect testRec2;
   testRec2.bottomRight.x=11;
   testRec2.bottomRight.y=0;
   testRec2.topLeft.x=0;
   testRec2.topLeft.y=10;
   checkit_int(is_a_square(testRec2),0);
}
void test_is_a_square(void){
   test_is_square1();
   test_is_square2();
}
int main(int arg, char *argv[])
{
   /* call testing function(s) here */
   test_create_point();
   test_is_a_square();
   return 0;
}