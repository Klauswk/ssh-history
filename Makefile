CC=gcc
LIBS=libs

default: ssh-history

ssh-history: ssh-history.c crypt.o connection_repository.o
	$(CC) -o $@ $^ sqlite3.dll -static -lsodium -L${LIBS}
	
crypt.o: crypt.c

connection_repository.o: connection_repository.c

clean :
	rm -f ssh-history ssh-history.exe crypt crypt.exe
