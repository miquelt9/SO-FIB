#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main()
{
  char c[1];
  int df = open("mi_pipe", O_RDONLY);
  if (df == -1) perror("error en obrir la pipe");
  while (read(0,c,1) > 0) write(df,c,1);
  close(df);
}
