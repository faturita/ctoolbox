#include <stdio.h>
#include <stdlib.h>

void main()
{
srand(1);
printf ("%d",( (int) (3.0*rand()/(RAND_MAX+1.0))));
printf ("%d",( (int) (3.0*rand()/(RAND_MAX+1.0))));
exit(0);
}

