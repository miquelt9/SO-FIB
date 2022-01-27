////////////////////////////////////////////////////
//																								//
//	    	Plantilla pel parcial de SO							//
//	--> Hi trobaràs les bases pel teu programa		//
//	--> Pot contenir alguna errata, compte! 			//
//	--> Crèdits: @miquelt9												//
//																								//
////////////////////////////////////////////////////

// Llibreries per incloure

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>						// Pels signals
#include <signal.h>							// Pels signals

// ____ Funció Usage || Com escriure __

void Usage()
{
	char buff[120];													// Valors per cada tipus (https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm)
	sprintf(buff, "Usage: ./ejemple n\n");		//  Char    Int   String
	write(1,buff,strlen(buff));								//  (%c)   (%d)    (%s)
}

// _________ Definicions ______________

int main (int argc, char *argv[]){} //argc = nº elements de argv (argc == argv.size())

int n;
char *string; // O bé string[n]
char **vector_string; // O bé *vector_string[n], vector_string[n][n]

// _________ Recordatoris ______________

exit(0); // --> El programa acaba normalment (bé)
exit(1); // --> El programa acaba per algun error
atoi(char *s); // --> Converteix un string a numero
getpid(); // --> Retorna el pid del programa
alarm(0); // --> Elimina el temporitzador i retorna qual li quedava a l'anterior
alarm(n); // --> En n (>0) s'enviarè un SIGALRM al procés

// __________ Mutar procés ____________

//      [1]   [2]    [3]    [4]
execlp ("ls", "ls", "-l", (char *) 0);
// [1] -> Que executeries a la terminal directament
// [2] -> Path (com que usem execlp només cal repetir [1] sense ./ si procedeix)
// [3] -> Arguments que li passariem a la funció (pot no haver-n'hi)
// [4] -> Confia

//_______________ Forks _______________

/* Base */
int pid;
pid = fork(); // Recorda, el fill rep 0 i el pare el pid del fill
if{...}

/* Fills sequencial */

for (int i = 0; i < n_fills and fork() == 0; ++i) //Nota que si fiquessim fork != 0 fariem un recurrencia
{
	//El pare no executa això, només els seus fills, nets...
}
// Tots executen aquesta part

/* Fills recurrents */
for (int i = 0; i < n_fills; ++i)
{
  pid = fork();
  if (pid == 0)
  {
    // Això ho executa el fill
    exit(0); // "Matem" el fil aquí
	}
  else if (pid < 0)
  {
    // S'ha produit un error
    perror("Error en el fork\n");
    exit(1);
  }
  else
  {
    // Això ho executa el pare
  }
}

/* Esperar fills */
while(waitpid (-1, NULL, 0) > 0) // waitpid (-1, NULL, 0) == wait()
//						[1]  [2]  [3]
// [1] -> Normalment -1, no preguntis
// [2] -> NULL o &res (per saber com ha mort el fill - mirar exemple de sota)
// [3] -> 0 o WNOHANG (comprova si hi ha algun fill zombie, if(fills zombie): es 'recull' el fill i retorna l'estat de sortida)
//																													else(si no hi ha fills 'acabats') retorna 0
//																													else retorna 1
// El waitpid(-1, NULL, WNOHANG) és útil per saber si ha mort algun fill recentment sense a que morin

// +info [WNOHANG](https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these#34845669)


/* Motiu pel qual ha mort un fill */
int res
pid = waitpid (-1, &res, 0); // Ara avaluem si hi ha hagut error o no
if (WIFEXITED(res) && WEXITSTATUS(res) != 0) // WIFEXITED (res) returns true si el fill ha acabat normalment (exit())
{...error...}                                // WEXITSTATUS(res) returna el valor d'exit (exit(valor))
                                             // https://www.ibm.com/docs/en/zos/2.4.0?topic=functions-waitpid-wait-specific-child-process-end
																						 // Per poder cridar WIFEXITSTATUS(res) és necessari haver cridat abans WIFEXITED(res) i ha d'haver retornat un numero diferent de 0

// _____________ Senyals ______________

struct sigaction sa;
sigset_t mask;

/* Evitar que ens arribi una senyal */
sigemptyset (&mask);                  // Buidem la mascara
sigaddset (&mask, SIGALRM);           // Afegim les senyals que volguem bloquejar
sigprocmask (SIG_BLOCK, &mask, NULL); // Bloquejem les senyals (també serveix per desbloquejar-les)


/* Que només pugui arribi una senyal */

sigfillset (&mask);                 // Emplenem la mascara
sigdelset (&mask, SIGINT);          // En aquest cas només podria arribar SIGINT

/* Reprogramació d'una senyal */
sa.sa_handler = &my_function;      // Pots indicar la teva funció, SIG_IGN per, no fer res o SIG_DFL (funcio per defecte)
sa.sa_flags = SA_RESTART;          // 0 (default) || SA_RESETHAND (restaurarà tractament antic) || SA_RESTART (reinicia la call que l'ha bloquejat)
sigfillset (&sa.sa_mask);          // Afegim els signals que hem modificat a la màscara

if (sigaction (SIGALRM, &sa, NULL) < 0) // Modifiquem el tractament de SIGALRM p.e.
  perror("Missatge d'error");						// Tractament d'errors
  exit(1);

/* Esperar fins una senyal en concret */

sigfillset (&mask);
sigdelset (&mask, SIGALRM);
sigsuspend (&mask);             // Ens esperem aquí

/* Enviar una senyal a un pid */

kill (pid, SIGINT); // Per fer-ho des de la terminal: #kill -SIGUSR1 pid

/* __________ Taula de senyañs i tractaments _________
 __________________________________________________________________
|___Nom___|__Acció per defecte_|___________Esdeveniment____________|
| SIGCHLD | IGNORAR	  				 | Un fill ha acabat o l'han matat	 |
| SIGCONT |						  			 | Continua si està parat (NB)			 |
| SIGSTOP | STOP 				  		 | Para un procés (Ctrl+Z) 					 |
| SIGINT  | TERMINAR (Acabar)  | Interrupció desde teclat (Ctrl+C) |
| SIGALRM | TERMINAR (Acabar)  | El temporitzador ha acabat        |
| SIGKILL | TERMINAR (Acabar)  | Acaba un procés (NB)              |
| SIGSEGV | CORE							 | Referència invàlida a memòria     |
| SIGUSR1 | TERMINAR (Acabar)  | Definit per l'usuari              |
| SIGUSR2 | TERMINAR (Acabar)  | Definit per l'usuari              |
¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯	¯
NB = No és bloquejable

// ___________ Què s'hereda quan? __________________________________________________________________

FORK ----> El fill:
		 		----> Hereda:
								- Copia de les variables
								- Taula d'accions dels signals
								- Màscara de signals bloquejats

			 	----> No hereda:
								- Signals pendents ni alarmes
								- PID

EXECLP ----> Muta:
		 		----> Hereda:
								- Senyals pendents
								- Màscara de signals bloquejats
								- PID

				----> No hereda:
								- Taula d'accions dels signals ni alarmes
								- Les variables (canvien de dirrecció)

+info [Introducció](https://docencia.ac.upc.edu/FIB/grau/SO/enunciados/Teoria/T1-Introduccion.pdf)
			[Processos](https://docencia.ac.upc.edu/FIB/grau/SO/enunciados/Teoria/T2-Procesos.pdf)

///////////////////////////////////////////////////////////////////*/
