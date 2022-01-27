all: cuenta_trozo cuenta

cuenta: cuenta.o funciones.o
	gcc -o cuenta cuenta.o funciones.o

funciones.o: funciones.c
	gcc -c funciones.c

cuenta.o:
	gcc -c cuenta.c

cuenta_trozo: cuenta_trozo.c
	gcc -o cuenta_trozo cuenta_trozo.c

clean:
	rm *.o cuenta_basic cuenta_trozo cuenta
