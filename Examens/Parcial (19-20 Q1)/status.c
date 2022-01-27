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
	sprintf(buff,"Usage: status pid1, pid2, ...\n");
	write(1,buff,strlen(buff));
	exit(1);
}

int main(int argc, char **argv)
{
  char buff[128];
  int fills = argc - 1;
  if (argc < 2) usage();
  for (int i = 1; i <= fills; ++i)
  {
    int pid = fork();
    if (pid == 0)
    {
      sprintf(buff,"/proc/%s/status", argv[i]);
      execlp("grep", "grep" , "State", buff, (char *)NULL);
      sprintf(buff,"Error in function excelp\n");
      write(1,buff,strlen(buff));
      exit(1);
    }
  }
  while(waitpid (-1, NULL, 0) > 0);
  exit(0);
}
