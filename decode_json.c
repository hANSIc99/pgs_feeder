#include "decode_json.h"

#define _POSIX_VERSION = 200112L
#define DEBUG 1

struct_data *s_data(const char *json)
{
	log4c_category_log(log_tracer, LOG4C_PRIORITY_TRACE, "%s: %s() -> entering s_data()",  s_prgrm_name , __func__); 


	json_t *js_root, *js_data, *js_program, *js_tmp, *js_result,
	    *js_result_obj, *js_array_obj;
	json_error_t error;
	uint8_t u8_array_pos = 0;
	struct_data *sd_data;
	sd_data = malloc(sizeof(struct_data));

	js_root = NULL;
	js_data = NULL;
	js_result = NULL;
	js_result_obj = NULL;
	js_array_obj = NULL;

	sd_data = s_data_init(sd_data);

	u8_array_pos = 0;
	js_root = json_loads(json, 0, &error);
	if (!js_root) {

	log4c_category_log(log_debug, LOG4C_PRIORITY_ERROR, "%s: %s() -> error while creating json object: %s",  s_prgrm_name , __func__, error.text); 

	}

	if (!(js_data = json_object_get(js_root, "data"))) {

		log4c_category_log(log_debug, LOG4C_PRIORITY_ERROR, "%s: %s() -> error while reading the JSON-object: \"data\"",  s_prgrm_name , __func__); 
	}

	if (!(js_result = json_object_get(js_data, "result"))) {

		log4c_category_log(log_debug, LOG4C_PRIORITY_ERROR, "%s: %s() -> error while reading the JSON-object: \"result\"",  s_prgrm_name , __func__); 
	}

	while ((js_result_obj = json_array_get(js_result, u8_array_pos))) {
		/* writing the keyword in struct */

		js_array_obj = json_object_get(js_result_obj, "s_keyword");

		sd_data->s_search_keyword[u8_array_pos] =
		    strdup(json_string_value(js_array_obj));

		js_array_obj = json_object_get(js_result_obj, "u16_result");

		sd_data->u16_matches[u8_array_pos] =
		    json_integer_value(js_array_obj);

		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> keyword: %s, value: %d",  s_prgrm_name , __func__, sd_data->s_search_keyword[u8_array_pos], sd_data->u16_matches[u8_array_pos]); 

		++u8_array_pos;

	}

	/* indicates how many keywords are available, necessary to build the table */
	log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG,  "%s: %s() -> keywords available: %d",  s_prgrm_name , __func__, u8_array_pos); 

	sd_data->u8_keywords_present = u8_array_pos;

	if ((js_tmp = json_object_get(js_data, "source"))) {

		sd_data->s_source = strdup(json_string_value(js_tmp));

		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> source: %s",  s_prgrm_name , __func__, sd_data->s_source); 

	}
	if ((js_tmp = json_object_get(js_data, "customer"))) {

		sd_data->s_customer = strdup(json_string_value(js_tmp));

		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> customer: %s",  s_prgrm_name , __func__, sd_data->s_customer); 
	}
	if ((js_tmp = json_object_get(js_data, "pub_date"))) {
		/* pub date is not processed actually */
		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> pub_date: %s",  s_prgrm_name , __func__, json_string_value(js_tmp)); 
	}

	if ((js_tmp = json_object_get(js_data, "sys_time"))) {

		sd_data->u32_sys_timestamp = json_integer_value(js_tmp);

		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> system time: %d",  s_prgrm_name , __func__, sd_data->u32_sys_timestamp); 

	}

	if ((js_program = json_object_get(js_root, "PRGRM"))) {

		sd_data->s_program = strdup(json_string_value(js_program));

		log4c_category_log(log_tracer, LOG4C_PRIORITY_DEBUG, "%s: %s() -> program name: %s",  s_prgrm_name , __func__, sd_data->s_program); 
	}

	json_decref(js_root);
	return sd_data;

}

void free_struct_data(struct_data * sd_data)
{
	uint8_t u8_counter = 0;

	while ((sd_data->s_search_keyword[u8_counter])) {

		free(sd_data->s_search_keyword[u8_counter]);
		++u8_counter;

	}
	free(sd_data->s_source);
	free(sd_data->s_program);
	free(sd_data->s_customer);

#if 0
	free(sd_data->s_link);
#endif
	free(sd_data);

}



struct_data *s_data_init(struct_data *sd_data){

uint8_t u8_count;

sd_data->u16_position = 0;
sd_data->s_source = NULL;
sd_data->s_customer = NULL;
sd_data->s_program = NULL;
sd_data->s_link = NULL;
sd_data->u32_sys_timestamp = 0;
sd_data->u8_keywords_present = 0;


for(u8_count = 0; u8_count < MAX_SEARCHKEYWORDS; u8_count++){
sd_data->s_search_keyword[u8_count] = NULL;
sd_data->u16_matches[u8_count] = 0;
}
return sd_data;

}
