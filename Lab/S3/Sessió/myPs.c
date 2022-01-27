#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
  char buffer[80];

  int pid = fork();

  switch (pid)
    {
    case 0:                    /* Proceso hijo */
      sprintf (buffer, "HIJO: Soy el proceso %d, el parámetro es %s\n", getpid (), argv[1]);
      write (1, buffer, strlen (buffer));
      break;

    case -1:                   /* Se ha producido un error */
      sprintf (buffer, "Se ha prodcido une error\n");
      write (1, buffer, strlen (buffer));
      break;

    default:                   /* Proceso padre */
      sprintf (buffer, "PADRE: Soy el proceso %d\n", getpid ());
      write (1, buffer, strlen (buffer));
      break;
    }
    while(1);
}
