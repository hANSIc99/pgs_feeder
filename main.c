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

int main(int argc, const char *argv[])
{

	struct_data *sd_data;

	if (argc < 2) {
		printf("\nError, expected JSON Object as argument.\n");
		exit(1);
	}

	printf("\n%s\n", *argv);

	++argv;
	printf("\n%s\n", *argv);

	sd_data = s_data(*argv);

	run_db(sd_data);
	printf("\nprogram in struct %s\n", sd_data->s_program);

	free_struct_data(sd_data);

	return 0;

}
