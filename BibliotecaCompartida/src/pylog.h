#ifndef DB_LOG_H
#define DB_LOG_H

#include "pycore.h"
#include "pyconfig.h"

py_enum logger_init(const char* file, const char* program_name);

t_log_level		 py_get_log_level(void);
pthread_mutex_t* py_get_logger_mutex(void);
t_log*           py_get_logger(void);

void             py_delete_logger(void);

#define PY_LOG_TRACE(...)\
	log_trace(py_get_logger(), __VA_ARGS__);\

#define PY_LOG_DEBUG(...)\
	log_debug(py_get_logger(), __VA_ARGS__);\

#define PY_LOG_INFO(...)\
	log_info(py_get_logger(), __VA_ARGS__);\

#define PY_LOG_WARNING(...)\
	log_warning(py_get_logger(), __VA_ARGS__);\

#define PY_LOG_ERROR(...)\
	log_error(py_get_logger(), __VA_ARGS__);\


#endif
