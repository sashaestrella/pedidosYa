#include "pylog.h"

static t_log*           	LOGGER_INTERNAL  = NULL;
static const t_log_level	LOGGER_LEVEL = LOG_LEVEL_TRACE;
static pthread_mutex_t  	LOGGER_MUTEX;

py_enum py_logger_init(const char* name, const char* program_name)
{
	if(pthread_mutex_init(&LOGGER_MUTEX, NULL))
		return MUTEX_INIT_ERROR;

	LOGGER_INTERNAL = log_create((char*)py_config_get_string(name), (char*)program_name, true, LOGGER_LEVEL);

	return (LOGGER_INTERNAL) ? SUCCESS : LOGGER_ERROR;
}

t_log_level	py_get_log_level(void)
{
	return LOGGER_LEVEL;
}

t_log* py_get_logger(void)
{
	return LOGGER_INTERNAL;
}

pthread_mutex_t* py_get_logger_mutex(void)
{
	return &LOGGER_MUTEX;
}

void py_delete_logger(void)
{
	log_destroy(LOGGER_INTERNAL);
	pthread_mutex_destroy(&LOGGER_MUTEX);
}

