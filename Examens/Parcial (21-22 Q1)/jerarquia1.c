
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void Usage()
{
	char buff[128];
	sprintf(buff,"Usage: ./jerarquia1 fichero1 fichero2 ... fichero10\n");
	write(1,buff,strlen(buff));
	exit(1);
}

int main(int argc, char **argv)
{
  char buff[128];
  int n_fills = argc - 1;
  int pids_fills[n_fills];
  if (argc < 2 || argc > 10) Usage();
  int i = 1;
  int pid;
  while (i <= n_fills)
  {
    pid = fork();
    char *fichero = argv[i];
    if (pid == 0)
    {
      sprintf(buff, "Soy el hijo con pid %d y mi fichero es %s\n", getpid(), fichero);
      write(1, buff, strlen(buff));
      exit(i);
    }
    else if (pid < 0)
    {
      perror("Error en el fork\n");
      exit(1);
    }
    pids_fills[i-1] = pid;
    ++i;
  }
  i = 1;
  int res;
  while (n_fills > 0)
  {
    pid = waitpid (pids_fills[i-1], &res, 0);
    res = WEXITSTATUS(res);
    if (res < 0)
    {
      sprintf(buff,"Error en la muerte del hijo %d con la salida %d\n", pids_fills[i-1], res);
      write(1,buff,strlen(buff));
      exit(1);
    }
    sprintf(buff,"Ha muerto el hijo con pid %d con la salida %d\n", pids_fills[i-1], res);
    write(1,buff,strlen(buff));
    --n_fills;
    ++i;
  }
}
