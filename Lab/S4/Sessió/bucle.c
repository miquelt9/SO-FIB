#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

int main()
{
  char buff[80];
  while(1)
  {
    printf (buff, "BUCLE INFINIT!!!");
    write (1, buff, strlen (buff));
  }
}
