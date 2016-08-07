#ifndef _PGS_INTERFACE_H
#define _PGS_INTERFACE_H


#include <stdio.h>
#include  <libpq-fe.h>
#include <stdint.h>
#include <stdlib.h>

uint8_t connect_db();
static void exit_nicely(PGconn *conn);


#endif
