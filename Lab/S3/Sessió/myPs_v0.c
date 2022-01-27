#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void error_y_exit(char *msg,int exit_status)
{
  perror(msg);
  exit(exit_status);
}


/* Ejecuta el comando ps –u username mediante la llamada al sistema execlp */
/* Devuelve: el código de error en el caso de que no se haya podido mutar */
void muta_a_PS(char *username)
{
  execlp("ps", "ps", "-u", username, (char*)NULL);
  error_y_exit("Ha fallado la mutación al ps", 1);
}

int main (int argc, char *argv[])
{
  char buffer[80];

  int pid = fork();

  switch (pid)
    {
    case 0:                    /* Proceso hijo */
      sprintf (buffer, "HIJO: Soy el proceso %d, el parámetro es %s\n", getpid (), argv[1]);
      write (1, buffer, strlen (buffer));
      muta_a_PS(argv[1]);
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
