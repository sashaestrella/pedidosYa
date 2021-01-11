#include "pyconexiones.h"

void iniciar_programa(char* archivo_config, char* archivo_log, char* modulo){
	py_config_init(archivo_config);
	py_logger_init(archivo_log, modulo);
}

