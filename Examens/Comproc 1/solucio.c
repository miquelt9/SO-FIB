#include "funciones.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void Usage()
{
  char buff[128];
  sprintf(buff,"Usage: cuenta num_procs fileName char_to_find\n");
  write(1,buff,strlen(buff));
  exit(1);
}

int main(int argc, char *argv[])
{
  char buff[80];
  int num_proc, i, file_size, ret;
  char ctofind;
  char *file_name;
  if (argc != 4) Usage();
  num_proc = atoi(argv[1]);
  file_name = argv[2];
  ctofind = argv[3][0];
  file_size = FileSize(file_name);

  if (file_size%num_proc != 0)
  {
    sprintf(buff,"El tamaño del archivo no es divisible por el número de procesos\n");
    write(1,buff,strlen(buff));
    exit(1);
  }
  for (i = 0; i < num_proc; ++i)
  {
    ret = fork();
    if (ret == -1)
    {
      perror("Error creando hijo");
      exit(1);
    }
    else if (ret == 0)
    {
      CountCharacter(file_name, ctofind, i, num_proc, file_size);
      exit(1);
    }
  }
  int suma = 0;
  while ((ret = waitpid(-1, NULL, 0)) > 0)
  {
    suma += ReadPartialCount(file_name, ret);
  }
  sprintf(buff,"El numero de ocurrencias en %s es %d\n", file_name, suma);
  write(1,buff,strlen(buff));
  exit(1);
}
