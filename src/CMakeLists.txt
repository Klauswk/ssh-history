PROJECT(ssh-history)

add_library(logger logger.c)
add_library(connection_repository connection_repository.c)
add_library(crypt crypt.c)

add_executable(ssh-history ssh-history.c)

target_link_libraries( ssh-history logger connection_repository crypt sqlite3.a sodium.a)
