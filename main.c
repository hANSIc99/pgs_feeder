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

int main(int argc, const char *argv[]) {

struct_data *sd_data;
sd_data = malloc(sizeof(struct_data));

run_db();

if(argc < 2){
	printf("\nError, expected JSON Object as argument.\n");
	exit(1);
}



printf("\n%s\n", *argv);

++argv;
printf("\n%s\n", *argv);

printf("\nString lenght: %d\n", (int)strlen(*argv));

s_data(*argv);
free(sd_data);
return 0;

}




#if 0
iter = json_object_iter(js_root);
while(iter){

printf("\nRoot Object = %s\n", json_object_iter_key(iter));

iter = json_object_iter_next(js_root, iter);

}



}
#endif
