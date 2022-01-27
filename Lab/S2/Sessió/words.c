#include <string.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
  char buf[80];
  int i = 1;
  int count = 0;


  while (i < argc && argv[i][strlen(argv[i])-1] != '”')
  {
    sprintf(buf, "Char %c\n", argv[i][strlen(argv[i])-1]);
    write(1,buf,strlen(buf));
    ++count;
    ++i;
  }
  sprintf(buf,"%d palabras\n", count);
  write(1,buf,strlen(buf));
}
