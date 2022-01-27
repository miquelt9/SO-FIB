#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int pid_pare;

void do_nothing()
{
}

void avisar_pare()
{
  kill(pid_pare, SIGUSR1);
  exit(0);
}

void usage()
{
	// Esta funcion escribe un mensaje mas o menos detallado sobre como se
	// usa el programa
	char buff[128];
	sprintf(buff,"Usage: signals N\n");
	write(1,buff,strlen(buff));
	exit(1);
}

int main(int argc, char **argv)
{
  char buff[128];
  int n = atoi(argv[1]);
  struct sigaction sa;
  if (argc != 2) usage();
  int pid;
  pid_pare = getpid();

  sigset_t mask;

  sa.sa_handler = &avisar_pare;
  sa.sa_flags = SA_RESTART;
  sigfillset (&sa.sa_mask);
  if (sigaction (SIGALRM, &sa, NULL) < 0)
    perror("Sigaction");

  sa.sa_handler = &do_nothing;
  sa.sa_flags = SA_RESTART;
  sigfillset (&sa.sa_mask);
  if (sigaction (SIGUSR1, &sa, NULL) < 0)
    perror("Sigaction");

  for (int i = 0; i < n; ++i)
  {
    pid = fork();
    if (pid == 0)
    {
      alarm(1);
      sprintf(buff,"El meu PID és: %d\n", getpid());
      write(1,buff,strlen(buff));
      while(1);
    }
    else
    {
      sigfillset (&mask);
      sigdelset (&mask, SIGUSR1);
      sigsuspend (&mask);
    }
  }
  while(waitpid (-1, NULL, 0) > 0);
  exit(0);
}
