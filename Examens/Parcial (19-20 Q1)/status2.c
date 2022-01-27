#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int index_of_int_arr(const int arr[], int x, int size_arr){
    for (int i = 0; i < size_arr; ++i){
        if (arr[i] == x) return i;
    }
    return -1;
}

void usage()
{
	// Esta funcion escribe un mensaje mas o menos detallado sobre como se
	// usa el programa
	char buff[128];
	sprintf(buff,"Usage: status pid1, pid2, ...\n");
	write(1,buff,strlen(buff));
	exit(1);
}

int main(int argc, char **argv)
{
  char buff[128];
  int fills = argc - 1;
  int pids_fills[fills];
  int pids_buscar[fills];
  int pid;
  if (argc < 2) usage();
  for (int i = 1; i <= fills; ++i)
  {
    pid = fork();
    if (pid == 0)
    {
      sprintf(buff,"/proc/%s/status", argv[i]);
      execlp("grep", "grep" , "State", buff, (char *)NULL);
      sprintf(buff,"Error in function excelp\n");
      write(1,buff,strlen(buff));
      exit(1);
    }
    if (pid < 0)
    {
      perror("Error en el fork\n");
      exit(1);
    }
    pids_fills[i-1] = pid;
    pids_buscar[i-1] = atoi(argv[i]);
  }
  int res;
  while(fills > 0)
  {
    pid = waitpid (-1, &res, 0);
    if (WIFEXITED(res) && WEXITSTATUS(res) != 0)
    {
      int pid_error = pids_buscar[index_of_int_arr(pids_fills, pid, fills)];
      sprintf(buff,"Proceso %d no existe\n", pid_error);
      write(1,buff,strlen(buff));
      exit(0);
    }
    --fills;
  }
  exit(0);
}
