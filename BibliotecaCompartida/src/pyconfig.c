#include "pyconfig.h"

t_config* DB_CONFIG_INTERNAL = NULL;

void py_config_init(char* nombre){
	DB_CONFIG_INTERNAL = config_create(nombre);
}

void py_config_delete(void)
{
    config_destroy(DB_CONFIG_INTERNAL);
}

char* py_config_get_string(const char* key)
{
    return config_get_string_value(DB_CONFIG_INTERNAL, (char*)key);
}

int py_config_get_int(const char* key)
{
    return config_get_int_value(DB_CONFIG_INTERNAL, (char*)key);
}

long py_config_get_long(const char* key)
{
    return config_get_long_value(DB_CONFIG_INTERNAL, (char*)key);
}

double  py_config_get_double(const char* key)
{
    return config_get_double_value(DB_CONFIG_INTERNAL, (char*)key);
}

char** py_config_get_array_value(const char* key)
{
    return config_get_array_value(DB_CONFIG_INTERNAL, (char*)key);
}

int  py_config_keys_amount(void)
{
    return config_keys_amount(DB_CONFIG_INTERNAL);
}

void py_config_set_value(const char* key, const char* value)
{
    config_set_value(DB_CONFIG_INTERNAL, (char*)key, (char*)value);
}

void py_config_remove_key(const char* key)
{
    config_remove_key(DB_CONFIG_INTERNAL, (char*)key);
}

void py_config_save(void)
{
    config_save(DB_CONFIG_INTERNAL);
}

py_enum py_config_save_in_file(const char* path)
{
    return (config_save_in_file(DB_CONFIG_INTERNAL, (char*)path) != -1)
        ? CONFIG_ERROR
        : SUCCESS;
}

