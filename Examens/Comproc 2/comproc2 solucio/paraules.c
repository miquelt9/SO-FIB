#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <fcntl.h>
#include <time.h>



int main(int argc, char *argv[])
{
    execlp("cat", "cat", argv[1], "|", "wc", "-w", (char*)0);
    //else { perror("ERROR: Only one file accepted"); exit(1); }
}
