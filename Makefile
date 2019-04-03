CC=gcc
LIBS=libs

default: ssh-history

ssh-history: ssh-history.c
	$(CC) -o $@ ssh-history.c -static -lsodium -L${LIBS} 
	
crypt: crypt.c
	$(CC) -o $@ crypt.c -static -lsodium -L${LIBS} 

connection_repository: connection_repository.c
	$(CC) -o $@ connection_repository.c

clean :
	rm -f ssh-history ssh-history.exe crypt crypt.exe
