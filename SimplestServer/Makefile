client:client.o
	cc -o client client.o
server:main.o
	cc -o server main.o

client.o:client.c SimplestServer.h
	cc -c client.c
server.o:main.c SimplestServer.h
	cc -c main.c

clean:
	rm client server main.o client.o
