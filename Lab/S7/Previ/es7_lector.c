#include <unistd.h>
#include <stdio.h>

int main ()
{
  int numero;
	read(0, &numero, sizeof(int));
	printf("%d\n", numero);
}
