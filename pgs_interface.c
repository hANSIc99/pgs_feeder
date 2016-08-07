#include "pgs_interface.h"


uint8_t connect_db(){
char *conninfo = "dbname = postgres";
PGconn *conn;
PGresult *res;


conn = PQconnectdb(conninfo);

if(PQstatus(conn) != CONNECTION_OK){
printf("\nConnection failed: %s\n", PQerrorMessage(conn));
exit_nicely(conn);
}
printf("\nConnected !\n");

}


static void exit_nicely(PGconn *conn){

PQfinish(conn);
exit(1);

}
