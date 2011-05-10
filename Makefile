all: cliente servidor

cliente: cliente.c
	gcc cliente.c -w -std=c99 -o cliente

servidor: servidor.c
	gcc servidor.c -w -std=c99 -o servidor

clean:
	rm -rf *c todos
