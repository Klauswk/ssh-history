#include <unistd.h>
#include <stdio.h>
#include <sqlite3.h>

int createTable(sqlite3 *db)
{
    int rc;

    char *err_msg = 0;

    char *sql = "CREATE TABLE IF NOT EXISTS Users(Id INT, User TEXT, Ip TEXT);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {

        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    else
    {

        fprintf(stdout, "Table created successfully\n");
    }
}


int main(void) {
    
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    //rc = sqlite3_open(":memory:", &db);
    
    rc = sqlite3_open("gg", &db);

    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    if(createTable(db) == 1){
        return 1;
    }

    sqlite3_close(db);
    
    return 0;
}