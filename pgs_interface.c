#include "pgs_interface.h"


uint8_t connect_db(){
char *conninfo = "host=localhost port=5432 dbname=mydb user=stephan password=rakete connect_timeout=10";
PGconn *conn;
PGresult *res;


conn = PQconnectdb(conninfo);

if(PQstatus(conn) != CONNECTION_OK){
printf("\nConnection failed: %s\n", PQerrorMessage(conn));
exit_nicely(conn);
} else {
printf("\nConnected to %s\n", PQdb(conn));
}
}


static void exit_nicely(PGconn *conn){

PQfinish(conn);
exit(1);

}
