#include "Sindicato.h"

int terminado = 0;
t_config* configuracion;


int main(void) {
	iniciar_programa("sindicato.config", "sindicato.log", "sindicato");
	inicializarAFIP();
	conexiones();
	return 0;
}

