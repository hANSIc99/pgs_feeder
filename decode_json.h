#ifndef _DECODE_JSON_H
#define _DECODE_JSON_H

#include <stdio.h>
#include "struct.h"
#include <jansson.h>
#include <string.h>
#include "log_var.h"

struct_data *s_data(const char *json);
void free_struct_data(struct_data * sd_data);
struct_data *s_data_init(struct_data *sd_data);


extern log4c_category_t *log_tracer;
extern log4c_category_t *log_debug;
extern log4c_category_t *log_raw;
extern char s_prgrm_name[50] ;

#endif
