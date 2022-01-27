#include "funciones.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void usage()
{
	// Esta funcion escribe un mensaje mas o menos detallado sobre como se
	// usa el programa
	char buff[128];
	sprintf(buff,"Usage: cuenta num_procs fileName char_to_find\n");
	write(1,buff,strlen(buff));
	exit(1);
}

int main(int argc, char **argv)
{
    // argv[1] = num_procs
    // argv[2] = fileName
    // argv[3] = char_to_find
		int pare = getpid();
    char buff[128];
  	if (argc!=4) usage();
    int num_procs = atoi(argv[1]);
    int file_size = FileSize(argv[2]);

    if (file_size%num_procs != 0)
    {
      sprintf(buff,"El tamaño del archivo no es divisible por el número de procesos\n");
      write(1,buff,strlen(buff));
      exit(1);
    }
    int i;
    for (i = 0; i < num_procs && fork() == 0; ++i);
    CountCharacter(argv[2], argv[3][0], i, num_procs, file_size);
    if (getpid() != pare) exit(1);
    int child;
    int suma = 0;
    while((child = waitpid(-1, NULL, 0))>0)
    {
      suma += ReadPartialCount(argv[2], child);
    }
    sprintf(buff,"%d\n", suma);
    write(1,buff,strlen(buff));
    exit(1);
}
