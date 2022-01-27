// multiD s'ha d'utilitzar redireccionant el fitxer que es vols computar
// des de terminal, d'aquesta dorma es definira l'stdin com al fitxer

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int n_fills_error = 0;

void Usage()
{
	char buff[80];
	sprintf(buff, "Usage: ./multiA numero_hijos archivo\n");
	write(2,buff,strlen(buff));
}

void error_y_exit(char *s) {
	perror(s);
	exit(1);
}

void missatge_sortida(int fd)
{
  // Imprimim el missatge obtingut per la pipe
  char c;
  while (read(fd, &c, 1) > 0) {
    if (write (2, &c, 1) < 0) {
      perror("Error en escriure a la stderr");
      exit(2);
    }
    if(c == '\n') return;
  }
}

int print_child(int pid, int status) {

  char buff[80];
	if (WIFEXITED(status))
    {
        int exitcode = WEXITSTATUS(status);
        if (exitcode == 0) return 1;
        else {
          ++n_fills_error;
          sprintf(buff, "%d wrong execution (exit code %d)\n", pid,  exitcode);
          write(2,buff,strlen(buff));
          return 0;
        }
    }
    else {
      int signalcode = WTERMSIG(status);
      sprintf(buff, "%d wrong execution (signal: %d)\n", pid,  signalcode);
      write(2,buff,strlen(buff));
    }
    return 0;
}


int main (int argc, char *argv[])
{
  if (argc != 2) Usage();

  char buff[80];
  int pid;
  int n_fills = atoi(argv[1]);
  int pids[n_fills], fd_pipe[2];
  pipe(fd_pipe);

  for (int i = 0; i < n_fills; ++i)
  {
    pid = fork();
    if (pid == 0)
    {
      // El canal stdout passarà a ser l'entrada de la pipe
      close(fd_pipe[0]);
      execlp("./hijo2", "hijo2", (char*)0);
      error_y_exit("Error en el execlp\n");
  	}
    else if (pid < 0) error_y_exit("Error en el fork\n");
    pids[i] = pid;
  }
  close(fd_pipe[1]);

  int status, finished = 0;
  int tracted[n_fills];
  for (int i = 0; i < n_fills; ++i) tracted[i] = 0;

  while(!finished && (pid = waitpid (-1, &status, 0)) > 0) {
    finished = print_child(pid, status);
    for (int i = 0; i < n_fills; ++i) if(pids[i] == pid) ++tracted[i];
  }
  if (finished) missatge_sortida(fd_pipe[0]);
  close(fd_pipe[0]);

  for (int i = 0; i < n_fills; ++i)
  {
    if (pids[i] != pid && !tracted[i])
    {
      kill(pids[i], SIGKILL);
      print_child(pids[i], SIGKILL);
    }
  }

  if (n_fills == n_fills_error)
  {
    sprintf(buff, "Incalculable\n");
    write(2,buff,strlen(buff));
  }

}
