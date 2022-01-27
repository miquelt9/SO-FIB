#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "mis_funciones.h"

int main(int argc,char *argv[])
{
  if (argc < 3) Usage();
  else
  {
    char buf[80];
    int suma = 0;
    int valid = 1;
  	for (int i=1; i<argc && valid; i++)
  	{
  		// if (esNumero(argv[i])){
  		// 	sprintf(buf,"%s es un numero\n", argv[i]);
  		// }else{
  		// 	sprintf(buf,"%s no és un numero\n", argv[i]);
      if (!esNumero(argv[i]))
      {
        valid = 0;
        sprintf(buf,"Error: el parámetro “%s” no es un número\n", argv[i]);
        write(1,buf,strlen(buf));
      }
      suma = suma + mi_atoi(argv[i]);
  	}
    if(valid)
    {
      sprintf(buf,"La suma es %d\n", suma);
      write(1,buf,strlen(buf));
    }
  }
	return 0;
}
