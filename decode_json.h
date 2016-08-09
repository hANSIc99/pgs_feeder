#ifndef _DECODE_JSON_H
#define _DECODE_JSON_H

#include <stdio.h>
#include "struct.h"
#include <jansson.h>
#include <string.h>

struct_data *s_data(const char *json);
void free_struct_data(struct_data *sd_data);


#endif
