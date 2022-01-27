#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main (int argc, char *argv[])
{
  // fd[0] llegir, fd[1] escriure
  int fd[2];
  char buff[256];
  if (pipe(fd) == -1)
  {
    sprintf(buff, "Error on pipe creation\n");
    write(1, buff, strlen(buff));
  }
  int pid = fork();
  if (pid < 0)
  {
    sprintf(buff, "Error on fork creation\n");
    write(1, buff, strlen(buff));
  }
  else if (pid == 0)
  {/* Child reads from pipe */
    dup2(fd[0], 0);
    close(fd[0]);
    close(fd[1]);
    execlp("cat", "cat", (char*)NULL);
  }
  else
  {
    close(fd[0]);
    sprintf(buff,"Inicio\n");
    write(fd[1],buff,strlen(buff));
    //close(fd[1]);
    waitpid(-1,NULL,0);
    sprintf(buff,"Fin\n");
    write(1,buff,strlen(buff));
  }


}
