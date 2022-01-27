#include <string.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_SIZE 8

int esNumero(char *str)
{
  if(str == NULL) return 0;
  if (*str == '-') ++str;
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] < '0' || str[i] > '9' || i >= MAX_SIZE) return 0;
    ++i;
  }
  return 1;
}

int main(int argc,char *argv[])
{
char buf[80];
	for (int i=1; i<argc; i++)
	{
		if (esNumero(argv[i])){
			sprintf(buf,"%s es un numero\n", argv[i]);
		}else{
			sprintf(buf,"%s no és un numero\n", argv[i]);
		}
		write(1,buf,strlen(buf));
	}
	return 0;
}
