#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <funciones.h>

int main(int argc,char *argv[])
{
	if(argc < 2)
	{
		Usage();
	}
	else
	{
		char buf[80];
		for (int i=0; i<argc; i++)
		{
			if (i==0){
				sprintf(buf,"El argumento %d es listaParametros\n",i);
			}else{
				sprintf(buf,"El argumento %d es %s\n",i,argv[i]);
			}
			write(1,buf,strlen(buf));
		}
	}
	return 0;
}
