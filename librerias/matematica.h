double potencia (double,int);

double potencia (double,double);

int vabs (int vl)
	{
   if (vl>=0)
   	return vl;
   else
   	return (vl-2*vl);
   }


float raiz (float vl,ind,x,prc)
int ind=2;
float x=0;
float prc=0.00000001;

	{
   // Utilizando el metodo de newton para encontrar raices de ecuacion.
   // Permite calcular cualquier raiz con indice entero
   float x2;

   if (ind==1)
   	return vl;
   if (x==0)
   	x=vl;

   if ( (vabs(potencia(x,ind)-prc)<vl) )
   	return x;

   x2=raiz(vl,ind,((ind-1)*potencia(x,ind)+vl)/(ind*potencia(x,ind-1)));
   return x2;
   }

double potencia (double a,double exp)
	{
   int num,den=1;
   double aux=exp;
   while ( ((aux-(int)aux)!=0) && den<1000 )
   	{
      aux*=10;
      den*=10;
      }
   num=(int)aux;
   // Obtengo el numerador y el denominador.
	return (raiz((float)potencia(a,num),den));
   }


double potencia (double a,int exp)
    {
    double acum=1;
    char inv=0;
    int i=0;
    if (exp==0)
      	return 1;
    if (exp<0)
      	{
	exp=((-1)*exp);
        inv=1;
        }
     for (i=0;i<exp;i++)
      	acum*=a;
      if (inv==1)
      	return (1/acum);
      else
      	return acum;
      }


