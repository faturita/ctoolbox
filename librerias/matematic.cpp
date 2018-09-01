double potencia (double a,int exp)
   	{double acum=1;
      char inv=0;
      if (exp==0)
      	return 1;
      if (exp<0)
      	{
         exp=((-1)*exp);
         inv=1;
         }
      for (int i=0;i<exp;i++)
      	acum*=a;
      if (inv==1)
      	return (1/acum);
      else
      	return acum;
      }


