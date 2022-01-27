#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//Dado un nombre de fichero, calcula el tamaño del contenido del fichero
//-Resultado: Devuelve el tamaño del fichero
//-Parametros de Entrada:
//  char *name: nombre del fichero
int FileSize(char *name)
{
        int fd,size;
        fd=open(name,O_RDONLY);
        if (fd<0){
            perror("Opening file");
            exit(1);
        }
        size=(int)lseek(fd,0,SEEK_END);
        close(fd);
        return size;
        
}

//Dado un nombre de fichero y un PID, lee el fichero generado por la funcion CountCharacter
// con el contador parcial
//-Resultado: Devuelve un integer con el valor del contador leido
//-Parametros de Entrada:
//  char *name: nombre del fichero
//  int pid: PID del proceso
int ReadPartialCount(char *name,int pid)
{
        int fd,count;
        char buff[128];
        sprintf(buff,"%s.%d",name,pid);
        if ((fd=open(buff,O_RDONLY))<0){
            perror("Error opening partial count file");
            exit(1);
        }
        if (read(fd,&count,sizeof(int))!=sizeof(int)){
            perror("Error reading partial count");
            exit(1);
        }
        close(fd);
        return count;
        
}

//Lee una parte de un fichero y cuenta cuantas veces aparece el caracter "c_tocount".
//-Resultado: El resultado se escribe en un fichero, pero no devuelve nada.
//-Parametros de entrada:
//  char *fileName: nombre del fichero
//  char c_tocount: caracter a buscar
//  int i: identificador local (0..num_procs-1)
//  int num_procs: numero total de procesos
//  int size: tamaño del fichero
void CountCharacter(char *fileName,char c_tocount,int i,int num_procs,int size)
{
    char newfile[128];
    int fd,fd2,count=0,pos_init,my_size;
    char c;
    sprintf(newfile,"%s.%d",fileName,getpid());
    if ((fd=open(fileName,O_RDONLY))<0){
        perror("Error opening file");
        exit(1);
    }
    if ((fd2=open(newfile,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU))<0){
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

