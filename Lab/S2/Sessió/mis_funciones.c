#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "mis_funciones.h"

#define MAX_SIZE 8

void Usage()
{
	char buf[120];
	sprintf(buf, "Usage:listaParametros arg1 [arg2..argn]\nEste programa escribe por su salida la lista de argumentos que recibe\n");
	write(1,buf,strlen(buf));
}


unsigned int char2int(char c)
{
  return c - '0';
}

int mi_atoi(char *s)
{
  int r, i;
  r = i = 0;
  int negative = 0;
  if (s[i] == '-')
  {
    negative = 1;
    ++i;
  }
  while (i < (int)strlen(s))
  {
    r = (r*10) + char2int(s[i]);
    ++i;
  }
  if (negative) r = r*(-1);
  return r;
}

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
