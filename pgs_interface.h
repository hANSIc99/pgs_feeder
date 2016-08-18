#ifndef _PGS_INTERFACE_H
#define _PGS_INTERFACE_H

#include <stdio.h>
#include  <libpq-fe.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "struct.h"
#include "debug.h"

#define MAX_SQL_LENGHT 5000
#define MAX_SQL_COLUMN 500


uint8_t run_db(struct_data * sd_data);
PGconn *connect_db();
struct_db_info *check_create_table(struct_data * sd_data, struct_db_info * db_data);
void free_dbinfo(struct_db_info * db_info);
uint8_t write_data(struct_db_info *db_data, struct_data *sd_data);

#endif
