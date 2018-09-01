#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc,char *argv[])
{
char inf='A';
char vec[15]={"Chupamedias"};
int cont=0,regrabar=1;
FILE *pf,*pf2;
pf=fopen(argv[1],"rb");
if (pf==NULL)
    {
    printf ("Se ha producido un error.");
    exit (1);
    }
pf2=fopen(argv[2],"wb+");
if (pf2==NULL)
    {
    printf ("Se ha producido un error.de escritura");
    exit (1);
    }

rewind(pf);
rewind(pf2);
while (!feof(pf))
    {
    fread(&inf,sizeof(inf),1,pf);
    /* Tratamiento del caracter */
    if ( (!feof(pf)) && (regrabar==1) )
	fwrite(&inf,sizeof(inf),1,pf2);	
    if ( (inf==vec[cont]) )
	{
	cont++;
	}
    else
	cont=0;
	
    if (cont==10)
	{
	fseek(pf2,ftell(pf2)-sizeof(inf)*10,0);
	cont=0;
	}
	
    }
    

fclose(pf);
fclose(pf2);
}

