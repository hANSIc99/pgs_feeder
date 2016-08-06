#include "decode_json.h"



struct_data *s_data(const char *json){

json_t *js_root, *js_data, *js_result, *js_pub_date, *js_source, *js_sys_time;
json_error_t error;
void *iter;


js_root = NULL;
js_data = NULL;
js_pub_date = NULL;
js_source = NULL;
js_sys_time = NULL;
js_result = NULL;


js_root = json_loads(json,0, &error);
if(!js_root){
printf("\nAn error occured while creating the JSON object\n");
} 
printf("\nNumber of elements in the object: %d\n", (int)json_object_size(js_root));



if((js_data = json_object_get(js_root, "data")) == NULL){

/* ERROR */

printf("\nAn error occured while reading the JSON object\n");
}


if((js_result = json_object_get(js_data, "result")) == NULL){
/* ERROR */
printf("\nAn error occured while reading the JSON object\n");
}

printf("\nNumber of Elements in result : %d\n",(int) json_array_size(js_result) );


if((js_source = json_object_get(js_data, "source")) != NULL){

printf("\nSource: %s\n", json_string_value(js_source));
}

if((js_pub_date = json_object_get(js_data, "pub_date")) != NULL){


printf("\nPub_Date: %s\n", json_string_value(js_pub_date));
}
if((js_sys_time = json_object_get(js_data, "sys_time")) != NULL){


printf("\nSystem time: %d\n", json_integer_value(js_sys_time));



}


return NULL;

}
