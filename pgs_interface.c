#include "pgs_interface.h"

#define CHECK_TABLE_1 "SELECT relname FROM pg_class WHERE relname = '"
#define CHECK_TABLE_2 "';"
#define CREATE_TABLE_1 

static void exit_nicely(PGconn *conn);

uint8_t run_db(struct_data *sd_data){

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
	char *s_tablename, *s_ptr, *s_tmp_ptr;
	uint16_t u16_sql_lenght;
	PGresult *exec;

	/* establish a connection to a database */

	PGconn *conn = connect_db();




	/* remove dots from source string */

	s_table_name = malloc((strlen(sd_data->s_source) * sizeof(char)) +1);
	s_table_name[0] = '_' ;  
	strcpy(s_table_name+1, sd_data->s_source);
	s_tmp_ptr = s_table_name;

	while((s_ptr=strpbrk(s_tmp_ptr, ".")) != NULL){


		*s_ptr = '_' ;
		s_tmp_ptr = s_ptr + 1;		
		printf("\n dot found\n");

	}
	printf("\nNew table name: %s\n", s_table_name);	

	/* check is the table excist */

	u16_sql_lenght = strlen(CHECK_TABLE_1) + strlen(CHECK_TABLE_2) + strlen(s_table_name) +1;
	s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

	strcpy(s_sql_cmd, CHECK_TABLE_1);
	strcat(s_sql_cmd, s_table_name);
	strcat(s_sql_cmd, CHECK_TABLE_2);
	printf("\nSQL command before execution: \n%s\n\n", s_sql_cmd);
	exec = PQexec(conn, s_sql_cmd );

	free(s_sql_cmd);

	printf("\ntest source = %s\n", sd_data->s_source);
	printf("\npg_class result: %d\n", PQresultStatus(exec));
	printf("\npg_status as string: %s\n", PQresStatus(PQresultStatus(exec)));

	if(PQntuples(exec)){
		printf("\nTable excist: %d\n", PQntuples(exec));
		PQclear(exec);
	} else {
		/* create table */

		u16_sql_lenght = strlen(s_create_table_1)+strlen(s_create_table_2)+strlen(s_table_name) +1;


		s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

		strcpy(s_sql_cmd, s_create_table_1);
		strcat(s_sql_cmd, s_table_name);
		strcat(s_sql_cmd, s_create_table_2); 

		printf("\nstring lenght: %d\n", u16_sql_lenght);
		printf("\nSQL COMMAND: \n%s\n", s_sql_cmd );


		exec = PQexec(conn, s_sql_cmd);

		free(s_sql_cmd);

		if((PQresultStatus(exec)) != PGRES_COMMAND_OK){

			printf("\nERROR: Couldnt create table: %s\n%s\n", PQresStatus(PQresultStatus(exec)),PQresultErrorMessage(exec));
			/* log error message here */
		} else {

			printf("\nTable created!\n");

		}

		PQclear(exec);
	}




	free(s_table_name);
	return 0;
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
return 0;
}


static void exit_nicely(PGconn *conn){

	PQfinish(conn);
	exit(1);

}
