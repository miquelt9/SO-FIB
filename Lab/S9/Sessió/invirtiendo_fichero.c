#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void error_y_exit(char* msg, int error_status) {
	perror(msg);
	exit(error_status);
}

int main(int argc, char *argv[]) {
	char buff[256];
	char buf[256];

	int f1 = open(argv[1], O_RDONLY);
	sprintf(buf,"%s.inv", argv[1]);

	int f2 = creat(buf, 666);
	int f3 = lseek(f2,0,SEEK_END);
	lseek(f2, 0, SEEK_SET);

	while(f3!= 0) {
		lseek(f1,-1,SEEK_CUR);
		read(f1,buff,1);
		write(f2,buff,1);
		--f3;
	}
	close(f1);
	close(f2);
}
