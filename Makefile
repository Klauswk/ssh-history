CC=gcc
LIBS=libs
PROGRAM_ARGS= -DDEBUG_FLAG=1
CFLAGS=$(pkg-config --cflags libsodium)
LDFLAGS=$(pkg-config --libs libsodium)

default: ssh-history

ssh-history: ssh-history.c logger.o crypt.o connection_repository.o
	$(CC) -o $@ $^ ${CFLAGS} ${LDFLAGS} sqlite3.dll -static -lsodium
	
logger.o: logger.c
	$(CC) -o $@ ${PROGRAM_ARGS} -c $^

crypt.o: crypt.c

connection_repository.o: connection_repository.c

clean :
	rm -f ssh-history ssh-history.exe crypt crypt.exe
