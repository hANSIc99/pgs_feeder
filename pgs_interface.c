#include "pgs_interface.h"

#define CHECK_TABLE "SELECT relname FROM pg_class WHERE relname = '_edition_cnn_com2';"
#define CREATE_TABLE_1 

static void exit_nicely(PGconn *conn);

uint8_t run_db(){

	char *s_create_table_1 = "CREATE TABLE ";
	char *s_create_table_2 = "(	"
		"ID INT PRIMARY KEY NOT NULL,"
		"NAME TEXT NOT NULL,"	
		"AGE INT NOT NULL,"	
		"ADDRESS CHAR(50),"		
		"SALARY REAL"			
		");";



	char *s_table_name = "_edition_cnn_com2";
	char *s_sql_cmd;
	uint16_t u16_sql_lenght;
	PGresult *exec;
	PGconn *conn = connect_db();

	exec = PQexec(conn, CHECK_TABLE );

	printf("\npg_class result: %d\n", PQresultStatus(exec));
	printf("\npg_status as string: %s\n", PQresStatus(PQresultStatus(exec)));

	/* check is the table excist */
	if(PQntuples(exec) == 1){
		printf("\nTable excist: %d\n", PQntuples(exec));
		PQclear(exec);
	} else {

		u16_sql_lenght = strlen(s_create_table_1)+strlen(s_create_table_2)+strlen(s_table_name) +1;


		s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

		strcpy(s_sql_cmd, s_create_table_1);
		strcat(s_sql_cmd, s_table_name);
		strcat(s_sql_cmd, s_create_table_2); 

		printf("\nstring lenght: %d\n", u16_sql_lenght);
		printf("\nSQL COMMAND: \n%s\n", s_sql_cmd );


		exec = PQexec(conn, s_sql_cmd);

		if((*PQresStatus(PQresultStatus(exec))) != PGRES_COMMAND_OK){

			printf("\nERROR: Couldnt create table: %s\n%s\n", PQresStatus(PQresultStatus(exec)),PQresultErrorMessage(exec));
			/* log error message here */
		} else {

			printf("\nTable created!\n");

		}

		PQclear(exec);
	}

}




PGconn *connect_db(){

	char *conninfo = "host=localhost port=5432 dbname=mydb user=stephan password=rakete connect_timeout=10";
	PGconn *conn;
	PGresult *res;
	conn = PQconnectdb(conninfo);

	if(PQstatus(conn) != CONNECTION_OK){
		printf("\nConnection failed: %s\n", PQerrorMessage(conn));
		exit_nicely(conn);
	} else {
		printf("\nConnected to %s\n", PQdb(conn));

		return conn;


	}




}


static void exit_nicely(PGconn *conn){

	PQfinish(conn);
	exit(1);

}
