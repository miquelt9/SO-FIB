#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>




int main(int argc, char *argv[])
{
  int pid, ret;
  int pfd1[2], pfd2[2];
  char msg[256];
  char palabras[10];

  pipe(pfd1);
  pid = fork();
  if (pid == 0)
  {
    dup2(pfd1[1], 1); close(pfd1[1]);       // L'output s'enviara per pfd1, i no per stdout
    close(pfd1[0]);                         // Com que no haura de llegir per pfd1, tanca
    execlp("cat", "cat", argv[1], (char*)0);// Muta a cat
  }
  close(pfd1[1]);                           // Ja no s'haurà d'enviar res per pfd1, es tanca

  pipe(pfd2);

  if (pid == 0)
  {
    dup2(pfd1[0], 0); close(pfd1[0]);       // Es sibstitueix el que arriba de stdin per el que arriba de pfd1
    dup2(pfd2[1], 1); close(pfd2[1]);       // L'output s'enviara per pfd2, i no per stdout
    close(pfd2[0]);                         // No haurem de llegir de pfd2, el tanquem
    execlp("wc", "wc", "-w", (char*)0);     // Muta a contar paraules
  }
  close(pfd1[0]);                           // Tanquem pfd1, qui havia de llegir ja ho ha fet
  close(pfd2[1]);                           // Tanquem pfd2, qui havia d'escriure ja ho ha fet

  ret = read(pfd2[0], palabras, sizeof(palabras));

  close(pfd2[0]);                           // Ja s'ha llegit el contingut de pfd2
  strcpy(msg, "El numero de palabras es "); // Similar a sprintf()
  write(1, msg, strlen(msg));
  write(1, palabras, ret);

  while(waitpid(-1, NULL, 0) > 0);
}
