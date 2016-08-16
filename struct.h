/*
 * struct.h
 * 
 * Copyright 2016 Stephan Avenwedde <stephanpc@stepohan-ubuntu64>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#ifndef _STRUCT_H
#define _STRUCT_H

#include <stdint.h>
#include <time.h>
#define MAX_SEARCHKEYWORDS 10
#define NUMBER_CHARS_TIMESTAMP 10
#include  <libpq-fe.h>

#define true 1
#define false 0

typedef uint8_t bool;
/* s_news = Name des Struct */
typedef struct s_data {

	uint16_t u16_position;

	char *s_source;
	char *s_customer;
	char *s_program;
	char *s_link;

	uint32_t u32_sys_timestamp;

	uint8_t u8_keywords_present;

	char *s_search_keyword[MAX_SEARCHKEYWORDS];
	uint16_t u16_matches[MAX_SEARCHKEYWORDS];

	struct s_data *next;
	struct s_data *previous;

} struct_data;
/* struct_news = Name für Variablendeklaration: struct_news VarName */

typedef struct s_dbinfo {

	PGconn *conn;

	char *s_tablename_data;
	char *s_column_names[MAX_SEARCHKEYWORDS];
	

} struct_db_info;

#endif
