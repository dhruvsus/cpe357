#include "part6.h"
#include <stdio.h>
#include <stdlib.h>

struct point create_point(double x, double y)
{
   struct point p = {x, y};
   return p;
}
int is_a_square(rect h){
	return(abs((h.bottomRight.x-h.topLeft.x)-(h.topLeft.y-h.bottomRight.y))<0.1);
}