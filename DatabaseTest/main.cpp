#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <iostream>
#include <vector>
using std::vector;
#include <string>
using std::string;

const int PLAYLIST_NUM = 5;
vector<string> file_locations;


static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;

   
    for(i = 0; i < argc; i++)
    {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      
      file_locations.push_back(argv[i]);
    }
    
    printf("\n");
    return 0;
}

int main()
{
    sqlite3 *db;

    char *zErrMsg = 0;
    int db_handle;

    const char *sql;
    
    //Open database
    db_handle = sqlite3_open("/home/jess/Documents/MusicDatabase/musicdb", &db);

    if( db_handle )
    {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }
    
          
    string sql_str = "SELECT location FROM mp3 " \
                     "WHERE mp3_pk IN (SELECT mp3_fk FROM mp3_playlists " \
                                      "WHERE playlists_fk = " + std::to_string(PLAYLIST_NUM) + ");";
          
    
    sql = sql_str.c_str();
    
    //Execute SQL statement
    db_handle = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if(db_handle != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    
    
    
    
    
    sqlite3_close(db);
    
    return 0;
}
