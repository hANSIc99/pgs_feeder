#include "decode_json.h"

#define _POSIX_VERSION = 200112L
#define DEBUG 1

struct_data *s_data(const char *json)
{

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

	for (; u8_array_pos < MAX_SEARCHKEYWORDS; ++u8_array_pos) {
		sd_data->s_search_keyword[u8_array_pos] = NULL;
	}
	u8_array_pos = 0;
	js_root = json_loads(json, 0, &error);
	if (!js_root) {

		/* ERROR */
		printf("\nAn error occured while creating the JSON object\n");
	}

	if (!(js_data = json_object_get(js_root, "data"))) {

		/* ERROR */

		printf("\nAn error occured while reading the JSON object\n");
	}

	if (!(js_result = json_object_get(js_data, "result"))) {
		/* ERROR */
		printf("\nAn error occured while reading the JSON object\n");
	}

	while ((js_result_obj = json_array_get(js_result, u8_array_pos))) {
		/* writing the keyword in struct */

		js_array_obj = json_object_get(js_result_obj, "s_keyword");

		sd_data->s_search_keyword[u8_array_pos] =
		    strdup(json_string_value(js_array_obj));

		js_array_obj = json_object_get(js_result_obj, "u16_result");

		sd_data->u16_matches[u8_array_pos] =
		    json_integer_value(js_array_obj);
		if (1) {
			printf("\nobject keyword %s value: %d\n",
			       sd_data->s_search_keyword[u8_array_pos],
			       sd_data->u16_matches[u8_array_pos]);
		}
		++u8_array_pos;

	}

	/* indicates how many keywords are available, necessary to build the table */

	sd_data->u8_keywords_present = u8_array_pos;

	if ((js_tmp = json_object_get(js_data, "source"))) {

		sd_data->s_source = strdup(json_string_value(js_tmp));
		if (DEBUG) {
			printf("\nSource: %s\n", sd_data->s_source);
		}
	}
	if ((js_tmp = json_object_get(js_data, "customer"))) {

		sd_data->s_customer = strdup(json_string_value(js_tmp));
		if (DEBUG) {
			printf("\nCustomer: %s\n", sd_data->s_customer);
		}
	}
	if ((js_tmp = json_object_get(js_data, "pub_date"))) {

		if (DEBUG) {
			printf("\nPub_Date: %s\n", json_string_value(js_tmp));
		}
	}

	if ((js_tmp = json_object_get(js_data, "sys_time"))) {

		sd_data->u32_sys_timestamp = json_integer_value(js_tmp);
		if (DEBUG) {
			printf("\nSystem time: %d\n",
			       sd_data->u32_sys_timestamp);
		}

	}

	if ((js_program = json_object_get(js_root, "PRGRM"))) {

		sd_data->s_program = strdup(json_string_value(js_program));

		if (DEBUG) {
			printf("\nProgram: %s\n", sd_data->s_program);
		}
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
