#include <stdio.h>
#include <g++/streambuf.h>
#include <g++/iostream.h>


void main()
{
char c;
char d[20];
int i=0;
c=fgetc(stdin);
printf ("%d",c);
printf ("Aca estoy");
cout<<"Hola";

/*
while ((c=getchar())!=32)
    {
    printf ("%d",i);
    d[i++]=c;
    }
    
printf ("\n%d\n",i);
for (;i>=0;i--) printf ("%c",d[i]);
*/
}
