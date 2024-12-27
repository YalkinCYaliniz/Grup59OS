CC = gcc
CFLAGS = -Wall -Wextra -g

all: shell # konsoldan ./shell olarak calisir

shell: main.o shell.o
	$(CC) $(CFLAGS) -o shell main.o shell.o

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o shell 