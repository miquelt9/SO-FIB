#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int
main (int argc, char *argv[])
{
  // pipe1[0] llegir, pipe1[1] escriure
  int pipe1[2];
  char buff[256];
  if (pipe(pipe1) == -1)
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
    close(pipe1[0]);
    dup2(pipe1[1], 1);
    execlp("cat", "cat", argv[1], (char*)NULL);
  }
  else
  {
    close(pipe1[1]);
    dup2(pipe1[0], 0);
    sprintf(buff, "El número de palabras es ");
    write(1, buff, strlen(buff));
    waitpid(-1,NULL,0);
    execlp("wc", "wc", "-w", (char*)NULL);
  }
}
