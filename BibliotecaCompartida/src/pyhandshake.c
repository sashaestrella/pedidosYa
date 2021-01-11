#include "pyhandshake.h"

buffer_t* serializar_handshake_restaurante(handshake_restaurante handshake){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	int32_t largo_puerto = strlen(handshake.puerto) + 1;

	buffer->size = 5 * sizeof(uint32_t) + handshake.nombre_restaurante->largo_nombre + largo_puerto;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, largo_puerto ,handshake.puerto, &offset);
	db_copiar_string(buffer->stream, handshake.nombre_restaurante->largo_nombre ,handshake.nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, handshake.coordenadas.posicionX, &offset);
	db_copiar_uint32(buffer->stream, handshake.coordenadas.posicionY, &offset);

	return buffer;
}

buffer_t* serializar_handshake_cliente(handshake_cliente* handshake){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	int32_t largo_puerto = strlen(handshake->puerto) + 1;

	buffer->size = 5 * sizeof(uint32_t) + largo_puerto;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, largo_puerto ,handshake->puerto, &offset);
	db_copiar_uint32(buffer->stream, handshake->id, &offset);
	db_copiar_uint32(buffer->stream, handshake->coordenadas.posicionX, &offset);
	db_copiar_uint32(buffer->stream, handshake->coordenadas.posicionY, &offset);

	return buffer;
}

handshake_restaurante* deserializar_handshake_restaurante(buffer_t* buffer){
	handshake_restaurante* msg = malloc(sizeof(handshake_restaurante));
	void* stream = buffer->stream;

	int32_t largo_puerto;

	msg->nombre_restaurante = malloc(sizeof(nombre));

	memcpy(&largo_puerto, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->puerto = malloc(largo_puerto);
	memcpy(msg->puerto, stream, largo_puerto);
	stream += largo_puerto;

	memcpy(&msg->nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->nombre_restaurante->nombre = malloc(msg->nombre_restaurante->largo_nombre);
	memcpy(msg->nombre_restaurante->nombre, stream, msg->nombre_restaurante->largo_nombre);
	stream += msg->nombre_restaurante->largo_nombre;

	memcpy(&msg->coordenadas.posicionX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&msg->coordenadas.posicionY, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}


handshake_cliente* deserializar_handshake_cliente(buffer_t* buffer){
	handshake_cliente* msg = malloc(sizeof(handshake_cliente));
	void* stream = buffer->stream;

	int32_t largo_puerto;

	memcpy(&largo_puerto, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->puerto = malloc(largo_puerto);
	memcpy(msg->puerto, stream, largo_puerto);
	stream += largo_puerto;

	memcpy(&msg->id, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&msg->coordenadas.posicionX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&msg->coordenadas.posicionY, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}
