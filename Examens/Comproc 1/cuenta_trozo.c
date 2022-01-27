#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void Usage()
{
	char buff[128];
	sprintf(buff,"Usage: cuenta_trozo fileName char_to_count local_id (0..num_procs) num_procs_in_the_team file size\n");
	write(1,buff,strlen(buff));
	exit(1);
}
void main(int argc,char *argv[])
{
	char newFile[128];
	int fd,fd2,count=0,pos_init,my_size;
	char c;
	char *fileName,c_tocount;
	int i,num_procs,size;
	if (argc!=6) Usage();
	fileName=argv[1];
	c_tocount=argv[2][0];
	i=atoi(argv[3]);
	num_procs=atoi(argv[4]);
	size=atoi(argv[5]);
	sprintf(newFile,"%s.%d",fileName,getpid());
	if ((fd=open(fileName,O_RDONLY))<0){
		perror("Error opening file");
		exit(1);
	}
	if ((fd2=open(newFile,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU))<0){
		perror("Error creating new file");
                exit(1);
        }
	my_size=size/num_procs;
	pos_init=i*(size/num_procs);
	lseek(fd,pos_init,SEEK_SET);
	while(my_size>0){
		read(fd,&c,sizeof(char));
		if (c==c_tocount) count++;
		my_size--;
	}
	write(fd2,&count,sizeof(int));
	close(fd);
	close(fd2);
}
