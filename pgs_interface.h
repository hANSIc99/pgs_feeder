#ifndef _PGS_INTERFACE_H
#define _PGS_INTERFACE_H


#include <stdio.h>
#include  <libpq-fe.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "struct.h"

uint8_t run_db(struct_data *sd_data);
PGconn *connect_db();


#endif
