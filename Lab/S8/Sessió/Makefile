all: exServerSocket exClientSocket sin_nombre lector escriptor
	lector_socket escritor_v2 escriptor_socket

exServerSocket: exServerSocket.c socketMng.o
	gcc -o exServerSocket exServerSocket.c socketMng.o

exClientSocket: exClientSocket.c socketMng.o
	gcc -o exClientSocket exClientSocket.c socketMng.o

socketMng.o: socketMng.c
	gcc -c socketMng.c

lector: lector.c
	gcc -o lector lector.c

escriptor: escriptor.c
	gcc -o escritor escriptor.c

escritor: escritor_v2.c
	gcc -o escritor escritor_v2.c

lector_socket: lector_socket.c
	gcc -o lector_socket lector_socket.c

escriptor_socket: escriptor_socket.c
	gcc -o escriptor_socket escriptor_socket.c

sin_nombre: sin_nombre.c
	gcc -o sin_nombre sin_nombre.c

clean:
	rm -f exServerSocket exClientSocket socketMng.o
