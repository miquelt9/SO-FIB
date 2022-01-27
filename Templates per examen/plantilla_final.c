////////////////////////////////////////////////////
//																								//
//	    	Plantilla pel final de SO		   					//
//	--> Hi trobaràs les bases pel teu programa		//
//	--> Pot contenir alguna errada, compte! 			//
//	--> Crèdits: @paubosch__	i @miquelt9 				//
//																								//
////////////////////////////////////////////////////

//includes

#include <stdlib.h>         //    Miscellanious (https://pubs.opengroup.org/onlinepubs/009604599/basedefs/stdlib.h.html)
#include <stdio.h>          //    I/O and miscellanious (https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdio.h.html)
#include <string.h>         //    strcpy, strlen (https://pubs.opengroup.org/onlinepubs/7908799/xsh/string.h.html)
#include <unistd.h>         //    fork, pipe, dup2, dup, close, open, execlp, read, write (and many others) (https://pubs.opengroup.org/onlinepubs/7908799/xsh/unistd.h.html)
#include <sys/wait.h>       //    waitpid, wait... (https://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/wait.h.html)
#include <sys/types.h>      //    Data types (https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/types.h.html#tag_13_67)
#include <sys/stat.h>       //    S_IRUSR, S_IWUSR, S_IXUSR (https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html) (https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/stat.h.html)
#include <sys/un.h>         //    Main socket functions (https://pubs.opengroup.org/onlinepubs/009696699/basedefs/sys/un.h.html)
#include <fcntl.h>          //    O_TRUNC, O_CREATE, O_RDONLYM O_RDWR, O_WRONLY... (https://pubs.opengroup.org/onlinepubs/009695399/basedefs/fcntl.h.html)
#include <time.h>           //    Clock instruccions (https://pubs.opengroup.org/onlinepubs/009695399/basedefs/sys/time.h.html)
#include <errno.h>          //    errno, EEXIST (https://pubs.opengroup.org/onlinepubs/009696899/basedefs/errno.h.html)

//////////////////////////
//    Functions use     //
//////////////////////////

int fd[2];
int fd, fd_substituit, ret;
char buffer[128];

pipe(fd);                           // Crea una pipe sense nom i indica la sortida d'aquesta a df[0] i l'entrada a fd[1]

dub2(fd, fd_substituit);            // Sobreescriu fd a fd_substituit (mirar "Conectar pipe amb terminal o altres pipes" més avall)

dub(fd);      // Ocupa el primer canal lliure amb una copia de fd

close(fd);    // Tanca el canal fd, això es important sobretot per tancar els escriptors i així
              // evitar que els lectors es quedin en un bucle infinit

ret = read(fd, buffer, sizeof(buffer));   // fd -> canal d'on volem llegir, buffer -> on volem guardar lo llegit,
                                          // sizeof(buffer) -> nº bytes a llegir, ret -> bits llegits
                                          // Si s'intenta llegir una pipe buida es quedarà bloquejat fins que hi hagin dades
                                          // En una pipe read només retornarà 0 quan no hi hagin escriptors d'aquesta (per això és important tancar-los)

ret = write(fd, buffer, sizeof(buffer));  // fd -> canal on volem escriure, buffer -> el que volem escriure,
                                          // sizeof(buffer) -> nº bytes a escriuer, ret -> bits escrits
                                          // Si s'intenta escriure en una pipe plena es quedarà bloquejat fins que es buida suficientment

fd = open("nom", acces_mode, permision_flags);    // El "nom" pot ser un fitxer o una pipe (amb nom clar), fd -> canal al que s'assgina el fitxer
                                                  // acces_mode:  O_RDONLY, O_WRONLY, O_RDWR (un d'aquest és sempre necessari)
                                                  //              O_CREAT -> crea; O_TRUNC -> Sobreescriu; O_APPEND -> afageix al final
                                                  //              Podem conquetenar els modes d'accés amb una '|'
                                                  // permision_flags: si renim que <rwxrwxrwx> (en grups de tres fan referencia a usuari, grup i others)
                                                  //                  podem ficar un 1 als que volem activar i convertir-ho en octal
                                                  // per exemple si volem read & write per l'usuari, fariem: 110000000 -> 600

int nova_posicio = lseek(fd, desplaçament, relatiu_a);  // Només per fitxers, fd és el canal del fitxer
                                                        // Posició accedida (nova_posicio) = relatiu_a + desplaçament (pot ser negatiu)
                                                        // relatiu_a: SEEK_SET (com un .begin()), SEEK_CUR (posició actual), SEEK_END (com un .end())

mknod("nompipe", S_IFIFO | 600, 0);                     // "nompipe" (autodescriptiu);  S_IFIFO (volem una pipe basicament, si vols +info ves al segon
                                                        //                              link de <sys/stat.h> per veure les altres possibilitats)
                                                        // 600 -> fa referencia als permisos, obtinguts de la mateixa forma que "permision_flags"
                                                        // El 0 del final no sé que vol dir pero no el tocaria, si tu ho saps pots canviar aquesta frase (o contectar amb nosaltres)


//////////////////////////
//  Creació pipe bàsica //
//////////////////////////

{
  int fd[2];

  pipe (fd);  //Retorna -1 si hi ha hagut algun error en crear la pipe

  write (fd[1], "Hola", 4); //Escriure a la pipe

  char buff[128];
  read (fd[0], buff, strlen(buff)); //Llegir de la pipe (buff = "Hola"). Retorna 0 si la pipe no té escriptors
}


////////////////////////////
//  Creació pipe amb nom  //
////////////////////////////

{

if (mknod("npipe", S_IFIFO | 600, 0) < 0 && errno != EEXIST)
{
  perror("Error creant la pipe");
  exit(1);
}

int fd = open("nom", O_RDONLY); //Guardem el canal de lectura de la pipe a fd;
//{...}
close (fd);
}


//////////////////////
//  Pipe amb Forks  //
//////////////////////

{
  int pid = fork(); //Retorna -1 si hi ha hagut error;

  //Procés del fill
  if (pid == 0) {
    close (fd[1]); //Tanquem el canal d'escriptura a la pipe per poder llegir
    char c;
    while (read (fd[0], &c, sizeof(c)) < 0) {
      write (1, &c, sizeof(c)); //Escrivim a la terminal el que anem llegint de la pipe
    }
    close (fd[0]);
    exit (0);
  }
  close (fd[1]); //Tanquem el canal d'escriptura a la pipe perquè el fill pugui llegir!!
  close (fd[0]); //Tanquem el canal de lectura ja que no el necessitem
  wait (NULL); //Esperem el fill
  exit (0);
}


//////////////////////////////////////////////
//  Obrir un fitxer per llegir i/o escriure //
//////////////////////////////////////////////

fd = open ("Nom_fitxer", O_RDONLY);   //Canal de lectura al fitxer
fd = open ("Nom_fitxer", O_WRONLY);   //Canal d'escriptura al fitxer
fd = open ("Nom_fitxer", O_RDWR);     //Canal d'escriptura al fitxer


////////////////////////////////////////////////
//  Conectar pipe amb terminal o altres pipes //
////////////////////////////////////////////////

dup2 (fd[0], 0); //El canal de lectura de la pipe passa a ser el canal de lectura de la terminal
close (fd[0]);

dup2 (fd[1], 1); //El canal d'escriptura de la pipe passa a ser el canal d'escriptura de la terminal
close (fd[1]);

dup2 (fd1[0], fd2[1]); //El canal de lectura de la pipe 1 passa a ser el canal d'escriptura de la pipe 2

int newfd = dup(fd); //Copia el canal fd al primer canal disponible i indica quin és aquest (ident. newfd)


////////////////////////////////////////////
//  Accés directe a posicions de fitxers  //
////////////////////////////////////////////

fd = open ("Nom_fitxer", O_RDWR);
lseek (fd, desplaçament, [*]);      // Mira a sota

//[*]:
    // SEEK_SET: punter = desplaçament
    // SEEK_CUR: punter = punter + desplaçament
    // SEEK_END: punter = file.size() + desplaçament
    // Si tenim:
              lseek (fd, -1, SEEK_END);
              read(fd, buffer, sizeof(char));
    // Llegirem l'últim char del fitxer

//________________________________________________________________________________//

    ////////////////////////////////////////
    //          Exemples de pipes         //
    ////////////////////////////////////////

// Un procés escriu per una pipe i llegeix el contingut d'aquesta (una mica inutil pero bé)
//-----------------------------------------------------------------------------------------

int main()
{
  int pipefd[2], r;
  char c, buff[8];
  if (pipe(pipefd) < 0) {
    perror("Error en crear la pipe");
    exit(1);
  }
  if ((r = write(pipefd[1], "hola!\n", 6)) < 0) {
    perror("Error en escriure a la pipe");
    exit(2);
  }
  if ((r = read(pipefd[0], buff, r)) < 0) {
    perror("Error en llegir de la pipe");
    exit(3);
  }
  write (1, buff, r);
  exit (0) ;
}

// El pare escriu a la pipe, el fill llegeix de la pipe i escriu per stdout
//--------------------------------------------------------------------------

int main () {
  int pipefd[2] , pid;
  char c;
  char buff[]= "Codificacio Cesar de 3.\n Text pla : hola !\nText xifrat : ";
  write(1, buff, strlen(buff));
  if (pipe(pipefd) < 0) {
    perror(" Error en crear la pipe");
    exit (1) ;
  }
  if (write(pipefd[1], "hola!", 5) < 0) {
    perror("Error en escriure a la pipe") ;
    exit(2);
  }

  pid = fork() ;
  if (pid < 0) {
    perror("Error de fork");
    exit(1);
  }

  if (pid == 0) {
    close(pipefd[1]); /* important !!!! */
    while (read(pipefd[0], &c, 1) > 0) {
      c +=3;
      if (write (1, &c, 1) < 0) {
        perror("Error en escriure a la stdout");
        exit(2);
      }
    }
    close(pipefd[0]);
    write (1, "\n", 1) ;
    exit(0);
  }
  close(pipefd[1]); /* important !!!! */
  close(pipefd[0]);
  wait(NULL);
  exit(0);
}


//
