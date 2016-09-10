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
extern char s_prgrm_name[50];

int main(int argc, const char *argv[])
{

	struct_data *sd_data;
	char *s_json;
	char s_buffer[MAX_JS_LINE_LENGHT];

	strcpy(s_prgrm_name, argv[0]);	

	if(log4c_init()){
	printf("\nlog4c_init() failed\n");
	exit(1);


	}else{
	
	log_tracer = log4c_category_get("tracer");
	log_debug = log4c_category_get("debug");
	log_raw = log4c_category_get("raw_data");

	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE,"%s: %s() -> log4c initialized", argv[0], __func__); 

	}

	s_json = calloc(1,1);

	fflush(stdin);

	/* getting the inpug */

	while(fgets(s_buffer, MAX_JS_LINE_LENGHT, stdin)){
	s_json = realloc( s_json, ((strlen(s_json) + strlen(s_buffer) * sizeof(char)) + 1));
	if(!s_json){
	log4c_category_log(log_debug, LOG4C_PRIORITY_ERROR,"%s: %s() -> realloc failed",   argv[0], __func__); 
	exit(1);
	}
	strcat(s_json, s_buffer);
	}	
	log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> lenght of string: %lu",   argv[0], __func__, strlen(s_json)); 
	log4c_category_log(log_raw, LOG4C_PRIORITY_TRACE, "%s: %s() -> stdin string: %s",   argv[0], __func__, s_json); 

	
	sd_data = s_data(s_json);
	run_db(sd_data);
	free(s_json);
	free_struct_data(sd_data);

	return 0;

}
