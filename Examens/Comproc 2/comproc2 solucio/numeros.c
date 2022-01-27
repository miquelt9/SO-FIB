#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[])
{
  int fd, pfd;
  int pid;
  int num;
  int ret;
  char num_ascii[10];

  if (mknod("npipe", S_IFIFO | 600, 0) < 0 && errno != EEXIST)
  {
    perror("Error creant la pipe");
    exit(1);
  }

  pid = fork();
  if (pid == 0)
  {

    fd = open("enteros.bin", O_RDONLY);
    pfd = open("npipe", O_WRONLY);
    while (read(fd, &num, sizeof(num)) > 0) {
      write(pfd, &num, sizeof(num));
    }
    exit(0);
  }

  pid = fork();
  if (pid == 0)
  {
    pfd = open("npipe", O_RDONLY);
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0600);

    while ((ret = read(pfd, &num, sizeof(num))) > 0) {
      sprintf(num_ascii, "%d\n", num);
      write(fd, num_ascii, strlen(num_ascii));
    }
    exit(0);
  }

  while (waitpid(-1, NULL, 0) > 0);
}
