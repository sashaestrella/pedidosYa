#ifndef PYHANDSHAKE_H_
#define PYHANDSHAKE_H_

#include "pycore.h"
#include "pyserialize.h"

typedef struct{
	char* ip;
	char* puerto;
}Direccion;

typedef struct{
	char* puerto;
	nombre* nombre_restaurante;
	CoordenadasXY coordenadas;
}handshake_restaurante;

typedef struct{
	char* puerto;
	int32_t id;
	CoordenadasXY coordenadas;
}handshake_cliente;

buffer_t* serializar_handshake_restaurante(handshake_restaurante handshake);
buffer_t* serializar_handshake_cliente(handshake_cliente* handshake);
handshake_restaurante* deserializar_handshake_restaurante(buffer_t* buffer);
handshake_cliente* deserializar_handshake_cliente(buffer_t* buffer);

#endif
