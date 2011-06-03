all: cliente servidor

cliente: cliente.c
	gcc cliente.c -w -std=c99 -o cliente -arch x86_64

servidor: servidor.c
	gcc servidor.c -w -std=c99 -o servidor -arch x86_64

clean:
	rm -rf *c todos
