/*
 * pyconfig.h
 *
 *  Created on: 30 ago. 2020
 *      Author: utnso
 */

#ifndef PYCONFIG_H_
#define PYCONFIG_H_

#include "pycore.h"

void    py_config_init(char* nombre);
void    config_delete(void);

char*   py_config_get_string(const char* key)      DB_NON_NULL(1);
int     py_config_get_int(const char* key)         DB_NON_NULL(1);
long    py_config_get_long(const char* key)        DB_NON_NULL(1);
double  py_config_get_double(const char* key)      DB_NON_NULL(1);
char**  py_config_get_array_value(const char* key);

int  py_config_keys_amount(void);
void py_config_set_value(const char* key, const char* value) DB_NON_NULL(1, 2);
void py_config_remove_key(const char* key) DB_NON_NULL(1);

void    py_config_save(void);
py_enum py_config_save_in_file(const char* path) DB_NON_NULL(1);

#endif
