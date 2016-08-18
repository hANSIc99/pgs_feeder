#include "pgs_interface.h"

#define CHECK_TABLE_1 "SELECT relname FROM pg_class WHERE relname = '"
#define CHECK_TABLE_2 "';"
#define CREATE_TABLE_1

#define DEBUG 1

static void exit_nicely(PGconn * conn);

uint8_t run_db(struct_data * sd_data)
{
	struct_db_info *s_db_info;
	uint8_t u8_count = 0;
	s_db_info = malloc(sizeof(struct_db_info));

	/* initialisation of the struct_db_info */

	for( ; u8_count < MAX_SEARCHKEYWORDS; u8_count++){

		s_db_info->s_column_names[u8_count] = NULL;

	}
	/* establish a connection to a database */

	s_db_info->conn = connect_db();
	/* error checking has to be implemented */
	s_db_info = check_create_table(sd_data, s_db_info);


	if(!write_data(s_db_info, sd_data)){
		/* log error here */
	}

	free_dbinfo(s_db_info);

	return 0;
}

PGconn *connect_db()
{

	char *conninfo =
		"host=localhost port=5432 dbname=mydb user=stephan password=rakete connect_timeout=10";
	PGconn *conn;

	conn = PQconnectdb(conninfo);

	if (PQstatus(conn) != CONNECTION_OK) {
		printf("\nConnection failed: %s\n", PQerrorMessage(conn));
		exit_nicely(conn);
	} else {
		printf("\nConnected to %s\n", PQdb(conn));

		return conn;

	}
	return 0;
}

static void exit_nicely(PGconn * conn)
{

	PQfinish(conn);
	exit(1);

}

struct_db_info *check_create_table(struct_data * sd_data,
		struct_db_info * db_data)
{

	const char *s_create_table_1 = "CREATE TABLE ";
	const char *s_create_table_2 = " ( ";
	const char *s_create_table_3 = " ); ";


	/* the keyword stands before SMALLINT */
	/* the data type for the result can be changed here */
	const char *s_create_table_key_col = " SMALLINT NULL, ";
	const char *s_create_table_timestamp_1 =
		" _timestamp";
	const char *s_create_table_timestamp_2 = " TIMESTAMPTZ PRIMARY KEY" ;

	char *s_ptr, *s_tmp_ptr, *s_sql_cmd, *s_table_column;
	uint16_t u16_column_lenght;
	uint8_t u8_count;
	PGresult *exec;

	/* remove dots from source string */
	/* +3 byte for the leading and the middel '_' & for the '\0' */

	db_data->s_tablename_data =
		malloc(((strlen(sd_data->s_source) +
						strlen(sd_data->s_customer)) * sizeof(char)) + 3);

	db_data->s_tablename_data[0] = '_';
	strcpy((db_data->s_tablename_data) + 1, sd_data->s_customer);
	strcat(db_data->s_tablename_data, "_");
	strcat(db_data->s_tablename_data, sd_data->s_source);

	/* table name created */
	s_tmp_ptr = db_data->s_tablename_data;

	while ((s_ptr = strpbrk(s_tmp_ptr, "."))) {

		*s_ptr = '_';
		s_tmp_ptr = s_ptr + 1;
		printf("\n dot found\n");

	}
	printf("\nNew table name: %s\n", db_data->s_tablename_data);

	/* DATA TABLE */
	/* check is the data table excist */


	s_sql_cmd = malloc((sizeof(char)) * MAX_SQL_LENGHT);

	strcpy(s_sql_cmd, CHECK_TABLE_1);
	strcat(s_sql_cmd, db_data->s_tablename_data);
	strcat(s_sql_cmd, CHECK_TABLE_2);
	printf("\nSQL command before execution: \n%s\n\n", s_sql_cmd);
	exec = PQexec(db_data->conn, s_sql_cmd);

	free(s_sql_cmd);

	if (DEBUG) {
		printf("\ntest source = %s\n", sd_data->s_source);
		printf("\npg_status as string: %s\n",
				PQresStatus(PQresultStatus(exec)));
	}

	/* if PQntubles returns a 1 (for 1 table), the table doenst have to be created */
	if (PQntuples(exec)) {
		printf("\nTable excist: %d\n", PQntuples(exec));
		PQclear(exec);

		for(u8_count = 0; u8_count < sd_data->u8_keywords_present; ++u8_count){
			u16_column_lenght = strlen(sd_data->s_search_keyword[u8_count]) + 2;
			s_table_column = malloc(u16_column_lenght);
			s_table_column[0] = '_' ;
			strcpy(s_table_column+1, sd_data->s_search_keyword[u8_count]);
			db_data->s_column_names[u8_count] = strdup(s_table_column);
			free(s_table_column);	
		}
		db_data->s_column_names[u8_count] = strdup(s_create_table_timestamp_1);


	} else {
		/* create table for the data */


		s_sql_cmd = malloc((sizeof(char)) * MAX_SQL_LENGHT);

		/* creating the string for one column */

		strcpy(s_sql_cmd, s_create_table_1);
		strcat(s_sql_cmd, db_data->s_tablename_data);
		strcat(s_sql_cmd, s_create_table_2);
		/* begin to add the columns to the table */
		if (1) {
			printf("\nSQL COMMAND: \n%s\n", s_sql_cmd);
		}

		for (u8_count = 0; u8_count < sd_data->u8_keywords_present;
				++u8_count) {
			/* add add keyword columns to the table */
			u16_column_lenght = 0;
			/* +1 for the leading '_' */

			u16_column_lenght =
				strlen(sd_data->s_search_keyword[u8_count]) + 2;

			u16_column_lenght += strlen(s_create_table_key_col);

			s_table_column =
				malloc(sizeof(char) * u16_column_lenght);
			/* add the leading '_' */

			s_table_column[0] = '_' ;
			strcpy(s_table_column+1,
					sd_data->s_search_keyword[u8_count]);

			/* add the column-name to the struct_db_info */
			db_data->s_column_names[u8_count] = strdup(s_table_column); 

			strcat(s_table_column, s_create_table_key_col);

			/* add the column command to the SQL command */

			strcat(s_sql_cmd, s_table_column);
			printf("\nCOLUMN FOR SQL:\n\t %s \n", s_table_column);


			free(s_table_column);

		}
		/* add the timestamp-column to the table */

		strcat(s_sql_cmd, s_create_table_timestamp_1);
		strcat(s_sql_cmd, s_create_table_timestamp_2);

		/* add the timestamp to the column names in struct_db_info */
		printf("\nu8 count ist hier %d\n", u8_count);
		db_data->s_column_names[u8_count] = strdup(s_create_table_timestamp_1);

		/* add the end to the SQL command */

		strcat(s_sql_cmd, s_create_table_3);

		/* execute SQL command */

		printf("\nSQL COMMAND: \n%s\n", s_sql_cmd);
		exec = PQexec(db_data->conn, s_sql_cmd);

		free(s_sql_cmd);

		/* test if the SQL command was succesful */
		if ((PQresultStatus(exec)) != PGRES_COMMAND_OK) {

			printf("\nERROR: Couldnt create table: %s\n%s\n",
					PQresStatus(PQresultStatus(exec)),
					PQresultErrorMessage(exec));
			/* log error message here */
		} else {
			if (DEBUG) {
				printf("\nTable created!\n");
			}
		}

		PQclear(exec);
	}

	return db_data;
}

void free_dbinfo(struct_db_info * db_info)
{
	uint8_t u8_count = 0;

	free((db_info->s_tablename_data));


	while((db_info->s_column_names[u8_count])){
		free(db_info->s_column_names[u8_count]);
		u8_count++;
	}


	exit_nicely(db_info->conn);
	free(db_info);

}
uint8_t write_data(struct_db_info *db_data, struct_data *sd_data){

	const char *s_sql_1 = "INSERT INTO ";
	const char *s_sql_2 = "( ";
	const char *s_sql_3 = " ) VALUES (" ;
	const char *s_sql_4 = " to_timestamp('";
	const char *s_sql_5 = "')); " ;

	PGresult *exec;
	char *s_sql_cmd, *s_tmp_value;

	uint8_t u8_count = 0;



	s_sql_cmd = malloc(MAX_SQL_LENGHT * sizeof(char));

	strcpy(s_sql_cmd, s_sql_1);
	strcat(s_sql_cmd,db_data->s_tablename_data);
	strcat(s_sql_cmd, s_sql_2);

	while((db_data->s_column_names[u8_count])){
		
		strcat(s_sql_cmd, db_data->s_column_names[u8_count]);
		if(u8_count < sd_data->u8_keywords_present){
			strcat(s_sql_cmd, ",");
		}
		u8_count++;
	}

	strcat(s_sql_cmd, s_sql_3);
for(u8_count = 0; u8_count < sd_data->u8_keywords_present; u8_count++){
		s_tmp_value = NULL;
		s_tmp_value = malloc(4 * sizeof(char));
		if(sd_data->u16_matches[u8_count]){
		printf("\nvalue of matches: %d, u8_count: %d\n", sd_data->u16_matches[u8_count], u8_count);
		snprintf(s_tmp_value, 4, "%d,", sd_data->u16_matches[u8_count]);
		}else{
		printf("\nvalue of matches: %d, u8_count: %d\n", sd_data->u16_matches[u8_count], u8_count);
		strcpy(s_tmp_value, "0,");
		}
		strcat(s_sql_cmd, s_tmp_value);
				
		printf("\n result: %s\n", s_tmp_value);

		free(s_tmp_value);
	printf("\nALLES OK\n");
	}
     	strcat(s_sql_cmd, s_sql_4);
	
	/* cast the timestamp to a string */
 
	s_tmp_value = malloc(sizeof(char) * (NUMBER_CHARS_TIMESTAMP+1));	
	snprintf(s_tmp_value, NUMBER_CHARS_TIMESTAMP+1, "%d", sd_data->u32_sys_timestamp);
	strcat(s_sql_cmd, s_tmp_value);
	free(s_tmp_value);
	/* add the end to the SQL command */
	strcat(s_sql_cmd, s_sql_5);
	
	printf("\nSPL command: %s\n\n", s_sql_cmd);
	/* execute SQL command */
	exec = PQexec(db_data->conn, s_sql_cmd);
		/* test if the SQL command was succesful */
		if ((PQresultStatus(exec)) != PGRES_COMMAND_OK) {

			printf("\nERROR: Couldnt insert data: %s\n%s\n",
					PQresStatus(PQresultStatus(exec)),
					PQresultErrorMessage(exec));
			/* log error message here */
		} else {
			if (DEBUG) {
				printf("\ndata inserted!\n");
			}
		}
	free(s_sql_cmd);
       	return 0;
}

