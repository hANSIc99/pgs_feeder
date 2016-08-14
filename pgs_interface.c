#include "pgs_interface.h"

#define CHECK_TABLE_1 "SELECT relname FROM pg_class WHERE relname = '"
#define CHECK_TABLE_2 "';"
#define CREATE_TABLE_1 


#define DEBUG 1

static void exit_nicely(PGconn *conn);

uint8_t run_db(struct_data *sd_data){


	struct_db_info *s_db_info;



	
	s_db_info = malloc(sizeof(struct_db_info));
	
	/* establish a connection to a database */

	s_db_info->conn = connect_db();
	/* error checking has to be implemented */
	s_db_info = check_create_table(sd_data, s_db_info);


		return 0;
}




PGconn *connect_db(){

	char *conninfo = "host=localhost port=5432 dbname=mydb user=stephan password=rakete connect_timeout=10";
	PGconn *conn;

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



struct_db_info *check_create_table(struct_data *sd_data, struct_db_info *db_data){

	const char *s_create_table_1 = "CREATE TABLE ";
	const char *s_create_table_2 = "(";
	const char *s_create_table_3 = ");";
	const char *s_create_table_8 = "(	"
		"ID INT PRIMARY KEY NOT NULL,"
		"NAME TEXT NOT NULL,"	
		"AGE INT NOT NULL,"	
		"ADDRESS CHAR(50),"		
		"SALARY REAL"			
		");";
	/* the keyword stands before CHAR(50) */
	const char *s_create_table_key_col = " CHAR(50)" ;

#if  0     /* ----- #if 0 : If0Label_1 ----- */
	const char *s_create_k_table_1 = "CREATE TABLE ";
	/* table name */
	const char *s_create_k_table_2 = "(";
	/* data types */
	const char *s_create_k_table_3 ="," ;
	/* last data type */	
	const char *s_create_k_table_4 = ");";
	const char *s_create_k_datatype = " ";
#endif     /* ----- #if 0 : If0Label_1 ----- */

	char *s_ptr, *s_tmp_ptr, *s_sql_cmd, *s_table_column;
	uint16_t u16_sql_lenght, u16_column_lenght;
	uint8_t u8_count, u8_testflag;
	PGresult *exec;

/* remove dots from source string */
	/* +3 byte for the leading and the middel '_' & for the '\0' */
	/* +2 byte for the later keyword version += "_k" */
	
	db_data->s_tablename_data = malloc(((strlen(sd_data->s_source) + strlen(sd_data->s_customer)) * sizeof(char)) +3);


	#if 1
	db_data->s_tablename_data[0] = '_' ;  
	strcpy((db_data->s_tablename_data)+1, sd_data->s_customer);
	strcat(db_data->s_tablename_data, "_");
	strcat(db_data->s_tablename_data, sd_data->s_source);


	/* table name created */
	s_tmp_ptr = db_data->s_tablename_data;

	while((s_ptr=strpbrk(s_tmp_ptr, "."))){


		*s_ptr = '_' ;
		s_tmp_ptr = s_ptr + 1;		
		printf("\n dot found\n");

	}
	#endif
	printf("\nNew table name: %s\n", db_data->s_tablename_data);	

	/* DATA TABLE */
	/* check is the data table excist */

	u16_sql_lenght = strlen(CHECK_TABLE_1) + strlen(CHECK_TABLE_2) + strlen(db_data->s_tablename_data) +1;
	s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

	strcpy(s_sql_cmd, CHECK_TABLE_1);
	strcat(s_sql_cmd, db_data->s_tablename_data);
	strcat(s_sql_cmd, CHECK_TABLE_2);
	printf("\nSQL command before execution: \n%s\n\n", s_sql_cmd);
	exec = PQexec(db_data->conn, s_sql_cmd );

	free(s_sql_cmd);

	if(DEBUG){
	printf("\ntest source = %s\n", sd_data->s_source);
	printf("\npg_status as string: %s\n", PQresStatus(PQresultStatus(exec)));
	}



	/* if PQntubles returns a 1 (for 1 table), the table doenst have to be created */
	if(PQntuples(exec)){
		printf("\nTable excist: %d\n", PQntuples(exec));
		PQclear(exec);
	} else {
		/* create table for the data*/

		u16_sql_lenght = strlen(s_create_table_1) + 1;
		u16_sql_lenght += strlen(db_data->s_tablename_data);
		u16_sql_lenght += strlen(s_create_table_2);
		
	
		
		/* begin of the lenght of the columns */	

	
		for(u8_count = 0 ; u8_count < sd_data->u8_keywords_present; ++u8_count){
			/* add for every keyword stringlenght + SQL extension to the lenght */
			u16_sql_lenght += strlen(sd_data->s_search_keyword[u8_count]) + strlen(s_create_table_key_col);
			
			printf("\nKeyword available! \n");	
		} 
		/* add necessary amount of ',' to the lenght */	
		printf("\nAmount of commas: %d\n", u8_count);
		u16_sql_lenght += (--u8_count);
		
		/* add the lenght of ending to the lenght */
		u16_sql_lenght += strlen(s_create_table_3);
		
		printf("\nOverall lenght: %d\n", u16_sql_lenght);


		s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

#if 1
		strcpy(s_sql_cmd, s_create_table_1);
		strcat(s_sql_cmd, db_data->s_tablename_data);
		strcat(s_sql_cmd, s_create_table_2); 
		/* begin to add the columns to the table */
		if(1){
		printf("\nSQL COMMAND: \n%s\n", s_sql_cmd );
		}
		for(u8_count = 0 ; u8_count < sd_data->u8_keywords_present; ++u8_count){
			/* add add keyword columns to the table */
		u16_column_lenght = 0;
		u16_column_lenght = strlen(sd_data->s_search_keyword[u8_count]);
		u16_column_lenght += strlen(s_create_table_key_col);



		(u8_count < sd_data->u8_keywords_present-1) ? u16_column_lenght += 1 : u16_column_lenght  ; 	

		/* construction site */
		
		s_table_column = malloc(sizeof(char) * u16_column_lenght );	

						
			printf("\nKeyword Number %d! \n", u8_count);	
		} 

		exec = PQexec(db_data->conn, s_sql_cmd);

		free(s_sql_cmd);

		/* test if the SQL command was succesful */
		if((PQresultStatus(exec)) != PGRES_COMMAND_OK){

			printf("\nERROR: Couldnt create table: %s\n%s\n", PQresStatus(PQresultStatus(exec)),PQresultErrorMessage(exec));
			/* log error message here */
		} else {
			if(DEBUG){
			printf("\nTable created!\n");
			}
		}

		PQclear(exec);
#endif
	}

#if 0
	/* KEYWORD TABLE */
	/* +3 byte for the leading and the middel '_' & for the '\0' */
	/* +2 byte for the later keyword version += "_k" */
	
	db_data->s_tablename_keywords = malloc(((strlen(sd_data->s_customer)) * sizeof(char)) +1);
	strcpy(db_data->s_tablename_keywords, sd_data->s_customer);
	
	
	printf("\nNew keyword table name: %s\n", db_data->s_tablename_keywords);	







	u16_sql_lenght = strlen(CHECK_TABLE_1) + strlen(CHECK_TABLE_2) + strlen(db_data->s_tablename_keywords) +1;
	s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

	strcpy(s_sql_cmd, CHECK_TABLE_1);
	strcat(s_sql_cmd, db_data->s_tablename_keywords);
	strcat(s_sql_cmd, CHECK_TABLE_2);
	printf("\nSQL command before execution: \n%s\n\n", s_sql_cmd);
	exec = PQexec(db_data->conn, s_sql_cmd );

	free(s_sql_cmd);
	if(PQntuples(exec)){
		printf("\nTable excist: %d\n", PQntuples(exec));
		PQclear(exec);
	} else {
		/* create table for the keywords */
		/* construction site */


		for(u8_count = 0 ; u8_count < sd_data->u8_keywords_present; ++u8_count){

			printf("\nKeyword available! \n");	
		} 
			u16_sql_lenght = strlen(s_create_table_1)+strlen(s_create_table_2)+strlen(db_data->s_tablename_keywords) +1;


		s_sql_cmd = malloc((sizeof(char)) * u16_sql_lenght);

		strcpy(s_sql_cmd, s_create_table_1);
		strcat(s_sql_cmd, db_data->s_tablename_keywords);
		strcat(s_sql_cmd, s_create_table_2); 
		if(DEBUG){
		printf("\nSQL COMMAND KEYWORD: \n%s\n", s_sql_cmd );
		}

		exec = PQexec(db_data->conn, s_sql_cmd);

		free(s_sql_cmd);

		/* test if the SQL command was succesful */
		if((PQresultStatus(exec)) != PGRES_COMMAND_OK){

			printf("\nERROR: Couldnt create keyword table: %s\n%s\n", PQresStatus(PQresultStatus(exec)),PQresultErrorMessage(exec));
			/* log error message here */
		} else {
			if(DEBUG){
			printf("\nTable created!\n");
			}
		}

		PQclear(exec);
	}
#endif


return db_data;
}
