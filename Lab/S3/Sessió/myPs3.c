#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
  char buffer[80];
  int pid;
  int num_hijos = atoi(argv[1]);
  //Ejemplo esquema secuencial
  for (int i=0;i<num_hijos;i++){
    pid=fork();

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
      if (pid==0) {
        // código hijo
        exit(0);// Solo si el hijo no muta y queremos que termine
      }
      // Esperamos a que termine antes de crear el siguiente
      while (waitpid(-1, NULL, 0)>0);
    }
}
