/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description: Feeder module to a Postgres DB for the RSSReader JSON function 
 *
 *        Version:  1.0
 *        Created:  06.08.2016 12:10:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Stephan Avenwedde 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "struct.h"
#include "decode_json.h"
#include "pgs_interface.h"
#include "log_var.h"

#define MAX_JS_LINE_LENGHT 300

extern log4c_category_t *log_tracer;
extern log4c_category_t *log_debug;
extern log4c_category_t *log_raw;

int main(int argc, const char *argv[])
{

	struct_data *sd_data;
	char *s_json;
	char s_buffer[MAX_JS_LINE_LENGHT];

	if(log4c_init()){
	printf("\nlog4c_init() failed\n");
	exit(1);


	}else{
	
	log_tracer = log4c_category_get("tracer");
	log_debug = log4c_category_get("debug");
	log_raw = log4c_category_get("raw_data");

	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,"%s: %s() -> log4c initialized",
				   argv[0],
__func__); 

	}

	s_json = calloc(1,1);

	fflush(stdin);

	while(fgets(s_buffer, MAX_JS_LINE_LENGHT, stdin)){
	s_json = realloc( s_json, ((strlen(s_json) + strlen(s_buffer) * sizeof(char)) + 1));
	if(!s_json){
	/* log error here */
	}
	strcat(s_json, s_buffer);
	}	
	printf("\n strlen: %d\n", strlen(s_json));	

	printf("\nFILE: %s\n", s_json);



	sd_data = s_data(s_json);
	run_db(sd_data);
	printf("\nprogram in struct %s\n", sd_data->s_program);
	free(s_json);
	free_struct_data(sd_data);

	return 0;

}
