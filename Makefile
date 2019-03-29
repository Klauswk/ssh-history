CC=gcc
LIBS=libs

ssh-history: ssh-history.c
	$(CC) -o ssh-history ssh-history.c -static -lsodium -L${LIBS} 
	
crypt: crypt.c
	$(CC) -o crypt crypt.c -static -lsodium -L${LIBS} 

clean :
	rm -f ssh-history ssh-history.exe crypt crypt.exe
