CC=gcc
LIBS=libs
PROGRAM_ARGS= -DDEBUG_FLAG=1
CFLAGS=$(pkg-config --cflags libsodium)
LDFLAGS=$(pkg-config --libs libsodium)
TARGET_DIR = ./../target/

default: ssh-history

ssh-history: ssh-history.c logger.o crypt.o connection_repository.o
	$(CC) -o ${TARGET_DIR}$@ ${TARGET_DIR}logger.o ${TARGET_DIR}crypt.o ${TARGET_DIR}connection_repository.o ssh-history.c ${CFLAGS} ${LDFLAGS} ./../sqlite3.dll -static -lsodium
	
logger.o: logger.c
	$(CC) -o ${TARGET_DIR}$@ ${PROGRAM_ARGS} -c $^

crypt.o: crypt.c
	$(CC) -o ${TARGET_DIR}$@ ${PROGRAM_ARGS} -c $^

connection_repository.o: connection_repository.c
	$(CC) -o ${TARGET_DIR}$@ ${PROGRAM_ARGS} -c $^
