
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void Usage()
{
	char buff[128];
	sprintf(buff,"Usage: ./signals fichero1 fichero2 ... fichero10\n");
	write(1,buff,strlen(buff));
	exit(1);
}

void matar_all(int *pids_fills)
{
  for(int i = 0; i < 10; ++i) kill(pids_fills[i], SIGKILL);
	exit(1);
}

void matar(int pid)
{
	kill(pid, SIGUSR1);
}

void do_nothing(){}

int main(int argc, char **argv)
{
  char buff[128];
  int n_fills = argc - 1;
  int pids_fills[n_fills];
  if (argc < 2 || argc > 10) Usage();
  int i = 1;
  int pid;
  struct sigaction sa;
  sigset_t mask;
  while (i <= n_fills)
  {
    pid = fork();
    char *fichero = argv[i];
    if (pid == 0)
    {
			sa.sa_handler = &do_nothing;
		  sa.sa_flags = SA_RESTART;
		  sigfillset (&sa.sa_mask);
		  if (sigaction (SIGUSR1, &sa, NULL) < 0)
		    perror("Error al modificar SIGUSR1");
		    exit(1);

      sigfillset (&mask);
      sigdelset (&mask, SIGUSR1);
      sigsuspend (&mask);

      sprintf(buff, "Soy el hijo con pid %d y mi fichero es %s\n", getpid(), fichero);
      write(1, buff, strlen(buff));
      exit(i);
    }
    else if (pid < 0)
    {
      perror("Error en el fork\n");
      exit(1);
    }
    pids_fills[i-1] = pid;
    ++i;
  }

  i = 1;
  sa.sa_handler = &matar;
  sa.sa_flags = SA_RESTART;
  sigfillset (&sa.sa_mask);
  if (sigaction (SIGINT, &sa, NULL) < 0)
    perror("Error al modificar SIGINT");
    exit(1);

  sa.sa_handler = &matar_all;
  sa.sa_flags = SA_RESTART;
  sigfillset (&sa.sa_mask);
  if (sigaction (SIGALRM, &sa, NULL) < 0) // Modifiquem el tractament de SIGALRM p.e.
    perror("Error al modificar SIGALRM");						// Tractament d'errors
    exit(1);

  while (i < n_fills)
  {
    alarm(2);
    sigfillset (&mask);
    sigdelset (&mask, SIGALRM);
    sigdelset (&mask, SIGINT);
    sigsuspend (&mask);
		sprintf(buff,"Se ha enviado un SIGUSR1 al hijo %d con pid %d\n", i, pids_fills[i]);
		write(1,buff,strlen(buff));
		++i;
  }

  i = 1;
  int res;
  while (n_fills > 0)
  {
    pid = waitpid (-1, &res, 0);
    res = WEXITSTATUS(res);
    if (res < 0)
    {
      sprintf(buff,"Error en la muerte del hijo con la salida %d\n", res);
      write(1,buff,strlen(buff));
      exit(1);
    }
    sprintf(buff,"Ha muerto el hijo con la salida %d\n", res);
    write(1,buff,strlen(buff));
    --n_fills;
    ++i;
  }
}
