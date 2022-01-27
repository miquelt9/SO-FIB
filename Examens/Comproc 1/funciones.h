
//Dado un nombre de fichero, calcula el tamaño del contenido del fichero
//-Resultado: Devuelve el tamaño del fichero
//-Parametros de Entrada:
//  char *name: nombre del fichero
int FileSize(char *name);

//Dado un nombre de fichero y un PID, lee el fichero generado por la funcion CountCharacter
// con el contador parcial
//-Resultado: Devuelve un integer con el valor del contador leido
//-Parametros de Entrada:
//  char *name: nombre del fichero
//  int pid: PID del proceso
int ReadPartialCount(char *name,int pid);

//Lee una parte de un fichero y cuenta cuantas veces aparece el caracter "c_tocount".
//-Resultado: El resultado se escribe en un fichero, pero no devuelve nada.
//-Parametros de entrada:
//  char *fileName: nombre del fichero
//  char c_tocount: caracter a buscar
//  int i: identificador local (0..num_procs-1)
//  int num_procs: numero total de procesos
//  int size: tamaño del fichero
void CountCharacter(char *fileName,char c_tocount,int i,int num_procs,int size);
