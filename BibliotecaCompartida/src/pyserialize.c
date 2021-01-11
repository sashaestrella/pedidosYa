#include "pyserialize.h"
#include "pymsg.h"

py_paquete_t* py_armar_paquete(enum_mensaje operacion, buffer_t* buffer);

void* py_serializar_paquete(py_paquete_t* paquete, uint32_t* bytes);


py_paquete_t* py_armar_paquete(enum_mensaje operacion, buffer_t* buffer)
{
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));

    paquete->codigo_operacion = operacion;
    if(buffer)
    {
        paquete->buffer           = malloc(sizeof(buffer_t));
        paquete->buffer->stream   = malloc(buffer->size);
        memcpy(paquete->buffer->stream, buffer->stream, buffer->size);
        paquete->buffer->size     = buffer->size;
    } else
    {
        paquete->buffer = NULL;
    }
    return paquete;
}



/*
 * @Layout Paquete Serializado:
 * ----------------------------
 *
 *      | codigo_mensaje   | tamaño_buffer |     buffer    |
 *      ----------------------------------------------------------------------------------------------------
 *      |     4 bytes      |    4 bytes    | tamaño_buffer |
 */


void* py_serializar_paquete(py_paquete_t* paquete, uint32_t* bytes)
{
   uint32_t sizeS = sizeof(int32_t);

   if(paquete->buffer)
	   sizeS += sizeof(int32_t) + paquete->buffer->size;

   void* serializado = malloc(sizeS);
   *bytes = 0;

   memcpy(serializado + *bytes, &paquete->codigo_operacion, sizeof(int32_t));
   *bytes += sizeof(int32_t);

   if(paquete->buffer)
   {
	   memcpy(serializado + *bytes, &paquete->buffer->size,   sizeof(uint32_t));
	   *bytes += sizeof(uint32_t);
	   memcpy(serializado + *bytes,  paquete->buffer->stream, paquete->buffer->size);
	   *bytes += paquete->buffer->size;
   }

   return serializado;
}


py_enum 	py_send( sfd             sfd_servidor,
					 enum_mensaje    operacion,
					 buffer_t*    buffer          )
{
	py_enum status = SUCCESS;

	py_paquete_t* paquete        = py_armar_paquete(operacion,buffer);

	uint32_t      size;
	void*         serializado    = py_serializar_paquete(paquete,&size);
	int           bytes_enviados = 0;

	bytes_enviados = send(sfd_servidor, serializado, size, MSG_NOSIGNAL);

	if(bytes_enviados == -1)
	{
		if(errno == EPIPE)
			status = CONN_LOST;
		else
			status = SEND_ERROR;
	}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
	free(serializado);


	return status;

}

py_enum   py_recv_msg(sfd sfd_conexion, py_paquete_t* paquete)
{
	py_enum status;
	status = py_recv_headers(sfd_conexion, paquete);

	if(status != SUCCESS)
		return status;

	paquete->buffer = malloc(sizeof(buffer_t));

	status = py_recv_buffer(sfd_conexion, paquete->buffer);
	if(status != SUCCESS)
	{
		return PARTIAL_RECV;
	}

	return SUCCESS;
}


py_enum py_recv_headers(sfd sfd_cliente, py_paquete_t* paquete)
{
	if( recv(sfd_cliente, &(paquete->codigo_operacion), sizeof(int32_t),0) <= 0 )
	{
		close(sfd_cliente);
		return RECV_ERROR;
	}
	return SUCCESS;
}


py_enum py_recv_buffer(sfd sfd_cliente, buffer_t* buffer)
{
	if( recv(sfd_cliente,&buffer->size,sizeof(uint32_t),0) < 0 )
	{
		free(buffer);
		close(sfd_cliente);
		return PARTIAL_RECV;
	}
	if(buffer->size == 0){
		return SUCCESS;
	}
	buffer->stream = malloc(buffer->size);

	if(recv(sfd_cliente,buffer->stream,buffer->size,0) < 0 )
	{
		free(buffer->stream);
		free(buffer);
		close(sfd_cliente);
		return PARTIAL_RECV;
	}

	return SUCCESS;
}

//------------------------------SERIALIZACION--------------------------------------

buffer_t* serializar_seleccionar_restaurante(nombre* nombre_restaurante, uint32_t id_cliente){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = 2*sizeof(uint32_t) + nombre_restaurante->largo_nombre;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_cliente, &offset);

	return buffer;
}

buffer_t* serializar_obtener_restaurante(nombre* nombre_restaurante){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t) + nombre_restaurante->largo_nombre;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);

	return buffer;
}

buffer_t* serializar_consultar_platos(nombre* nombre_restaurante){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t) + nombre_restaurante->largo_nombre;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);

	return buffer;
}

buffer_t* serializar_guardar_pedido(nombre* nombre_restaurante, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = 	nombre_restaurante->largo_nombre + 2 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_aniadir_plato(nombre* nombre_plato, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_plato->largo_nombre + 2 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_plato->largo_nombre, nombre_plato->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_guardar_plato(nombre* nombre_restaurante, uint32_t id_pedido, nombre* nombre_plato, uint32_t cantidad_platos){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_restaurante->largo_nombre + nombre_plato->largo_nombre + 4 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);
	db_copiar_string(buffer->stream, nombre_plato->largo_nombre, nombre_plato->nombre, &offset);
	db_copiar_uint32(buffer->stream, cantidad_platos, &offset);

	return buffer;
}

buffer_t* serializar_confirmar_pedido(uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_confirmar_pedido_con_restaurante(uint32_t id_pedido, nombre* nombre_restaurante){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t) + calcularTamanioNombre(nombre_restaurante);
	buffer->stream = malloc(buffer->size);

	db_copiar_uint32(buffer->stream, id_pedido, &offset);
	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);

	return buffer;
}

buffer_t* serializar_plato_listo(nombre* nombre_plato, nombre* nombre_restaurante, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_plato->largo_nombre + nombre_restaurante->largo_nombre + 3 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_plato->largo_nombre, nombre_plato->nombre, &offset);
	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_consultar_pedido(uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_obtener_pedido(nombre* nombre_restaurante, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_restaurante->largo_nombre + 2 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_finalizar_pedido(nombre* nombre_restaurante, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_restaurante->largo_nombre + 2 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_terminar_pedido(nombre* nombre_restaurante, uint32_t id_pedido){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_restaurante->largo_nombre + 2 * sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, id_pedido, &offset);

	return buffer;
}

buffer_t* serializar_obtener_receta(nombre* nombre_plato){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = nombre_plato->largo_nombre + sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_plato->largo_nombre, nombre_plato->nombre, &offset);

	return buffer;
}

buffer_t* serializar_int(int32_t numero){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(int32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_uint32(buffer->stream, numero, &offset);

	return buffer;
}

buffer_t* serializar_string(nombre* string){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = string->largo_nombre + sizeof(uint32_t);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, string->largo_nombre, string->nombre, &offset);

	return buffer;
}

buffer_t* serializar_lista_nombres(t_list* lista_nombres){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioListaNombres(lista_nombres);
	buffer->stream = malloc(buffer->size);

	serializarNombres(buffer->stream, lista_nombres, &offset);

	return buffer;
}

buffer_t* serializar_lista_nombres_real(t_list* lista_nombres){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioListaNombresReal(lista_nombres);
	buffer->stream = malloc(buffer->size);

	serializarNombresReal(buffer->stream, lista_nombres, &offset);

	return buffer;
}


buffer_t* serializar_registro_de_comida(registroComida* registro){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = registro->plato->largo_nombre + sizeof(uint32_t)*3;
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, registro->plato->largo_nombre, registro->plato->nombre, &offset);
	db_copiar_uint32(buffer->stream, registro->cantTotal, &offset);
	db_copiar_uint32(buffer->stream, registro->cantLista, &offset);

	return buffer;
}

buffer_t* serializar_lista_registros_comida(t_list* registros_de_comidas){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioListaRegistrosComida(registros_de_comidas);
	buffer->stream = malloc(buffer->size);

	serializarListaDeRegistrosComidas(buffer->stream, registros_de_platos, &offset);

	return buffer;
}

buffer_t* serializar_restaurante(Restaurante* restaurante){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioRestaurante(restaurante);
	buffer->stream = malloc(buffer->size);

	serializarRestaurante(buffer->stream, restaurante, &offset);

	return buffer;
}

buffer_t* serializar_receta(t_list* pasos){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioReceta(pasos);
	buffer->stream = malloc(buffer->size);

	serializarReceta(buffer->stream,pasos, &offset);

	return buffer;
}

buffer_t* serializar_pedido(Estado_pedido estadoPedido,t_list* platos){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = sizeof(uint32_t) + calcularTamanioListaRegistrosComida(platos);
	buffer->stream = malloc(buffer->size);

	db_copiar_uint32(buffer->stream, estadoPedido, &offset);
	serializarListaDeRegistrosComidas(buffer->stream,platos,&offset);

	return buffer;
}

buffer_t* serializar_consultar_pedido_rta(Estado_pedido estadoPedido,t_list* platos, nombre* nombre_restaurante){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = 2 * sizeof(uint32_t) + calcularTamanioListaRegistrosComida(platos) + calcularTamanioNombre(nombre_restaurante);
	buffer->stream = malloc(buffer->size);

	db_copiar_string(buffer->stream, nombre_restaurante->largo_nombre, nombre_restaurante->nombre, &offset);
	db_copiar_uint32(buffer->stream, estadoPedido, &offset);
	serializarListaDeRegistrosComidas(buffer->stream,platos,&offset);

	return buffer;
}

buffer_t* serializar_obtener_restaurante_rta(t_list* afinidades,CoordenadasXY coordenadas,t_list* platos,uint32_t cantDeHornos,uint32_t cantidadDePedidos, uint32_t cantidadDeCocineros){
	buffer_t* buffer = malloc(sizeof(buffer_t));
	int offset = 0;

	buffer->size = calcularTamanioListaNombres(afinidades) + sizeof(CoordenadasXY) + calcularTamanioPlatos(platos) + sizeof(uint32_t) * 3;
	buffer->stream = malloc(buffer->size);

	serializarAfinidades(buffer->stream, afinidades, &offset);
	db_copiar_uint32(buffer->stream, coordenadas.posicionX, &offset);
	db_copiar_uint32(buffer->stream, coordenadas.posicionY, &offset);
	serializarPlatos(buffer->stream,platos, &offset);
	db_copiar_uint32(buffer->stream, cantDeHornos, &offset);
	db_copiar_uint32(buffer->stream, cantidadDePedidos, &offset);
	db_copiar_uint32(buffer->stream, cantidadDeCocineros, &offset);

	return buffer;
}
buffer_t* serializar_modulo(enum_modulo modulo){
	buffer_t* buffer=malloc(sizeof(buffer_t));
	buffer->size=sizeof(uint32_t);
	buffer->stream=malloc(buffer->size);
	int offset=0;
	db_copiar_uint32(buffer->stream, modulo, &offset);
	return buffer;
}
//----------------------------------DESERIALIZACION--------------------------------

msg_seleccionar_restaurante* deserializar_seleccionar_restaurante (buffer_t* buffer){
	msg_seleccionar_restaurante* msg = malloc(sizeof(msg_seleccionar_restaurante));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_cliente, stream, sizeof(uint32_t));


	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_obtener_restaurante* deserializar_obtener_restaurante(buffer_t* buffer){
	msg_obtener_restaurante* msg = malloc(sizeof(msg_obtener_restaurante));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_consultar_platos* deserializar_consultar_platos(buffer_t* buffer){
	msg_consultar_platos* msg = malloc(sizeof(msg_consultar_platos));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_guardar_pedido* deserializar_guardar_pedido(buffer_t* buffer){
	msg_guardar_pedido* msg = malloc(sizeof(msg_guardar_pedido));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_aniadir_plato* deserializar_aniadir_plato(buffer_t* buffer){
	msg_aniadir_plato* msg = malloc(sizeof(msg_aniadir_plato));
	msg->plato = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->plato->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->plato->nombre = malloc(msg->plato->largo_nombre);
	memcpy(msg->plato->nombre, stream, msg->plato->largo_nombre);
	stream += msg->plato->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_guardar_plato* deserializar_guardar_plato(buffer_t* buffer){
	msg_guardar_plato* msg = malloc(sizeof(msg_guardar_plato));
	msg->restaurante = malloc(sizeof(nombre));
	msg->plato = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&msg->plato->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->plato->nombre = malloc(msg->plato->largo_nombre);
	memcpy(msg->plato->nombre, stream, msg->plato->largo_nombre);
	stream += msg->plato->largo_nombre;

	memcpy(&msg->cantidad_platos, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_confirmar_pedido_con_restaurante* deserializar_confirmar_pedido_con_restaurante(buffer_t* buffer){
	msg_confirmar_pedido_con_restaurante* msg = malloc(sizeof(msg_confirmar_pedido_con_restaurante));
	msg->nombre_restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(int32_t);
	memcpy(&msg->nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(int32_t);
	memcpy(msg->nombre_restaurante->nombre, stream, msg->nombre_restaurante->largo_nombre);
	stream += msg->nombre_restaurante->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_confirmar_pedido* deserializar_confirmar_pedido(buffer_t* buffer){
	msg_confirmar_pedido* msg = malloc(sizeof(msg_confirmar_pedido));
	msg->nombre_restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	memcpy(&msg->nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->nombre_restaurante->nombre = malloc(msg->nombre_restaurante->largo_nombre);
	memcpy(msg->nombre_restaurante->nombre, stream, msg->nombre_restaurante->largo_nombre);
	stream += msg->nombre_restaurante->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_plato_listo* deserializar_plato_listo(buffer_t* buffer){
	msg_plato_listo* msg = malloc(sizeof(msg_plato_listo));
	msg->restaurante = malloc(sizeof(nombre));
	msg->plato = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->plato->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->plato->nombre = malloc(msg->plato->largo_nombre);
	memcpy(msg->plato->nombre, stream, msg->plato->largo_nombre);
	stream += msg->plato->largo_nombre;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_consultar_pedido* deserializar_consultar_pedido(buffer_t* buffer){
	msg_consultar_pedido* msg = malloc(sizeof(msg_consultar_pedido));
	void* stream = buffer->stream;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));

	free(stream);
	free(buffer);

	return msg;
}

msg_consultar_pedido_rta* deserializar_consultar_pedido_rta(buffer_t* buffer){
	msg_consultar_pedido_rta* msg = malloc(sizeof(msg_consultar_pedido_rta));
	msg->nombre_restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(int32_t);
	memcpy(msg->nombre_restaurante->nombre, stream, msg->nombre_restaurante->largo_nombre);
	stream +=  msg->nombre_restaurante->largo_nombre;
	memcpy(&msg->pedido.estadoPedido, stream, sizeof(uint32_t));
	stream += sizeof(int32_t);

	t_list* registros = list_create();
	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int i=0;i < cantidad_elementos;i++){
		registroComida* registro = malloc(sizeof(registroComida));
		memcpy(&registro->plato->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		registro->plato->nombre = malloc(registro->plato->largo_nombre);
		memcpy(registro->plato->nombre, stream, registro->plato->largo_nombre);
		stream += registro->plato->largo_nombre;

		memcpy(&registro->cantTotal, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		memcpy(&registro->cantLista, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(registros,registro);
	}
	msg->pedido.platos = registros;

	free(buffer->stream);
	free(buffer);

	return msg;
}


msg_obtener_pedido* deserializar_obtener_pedido(buffer_t* buffer){
	msg_obtener_pedido* msg = malloc(sizeof(msg_obtener_pedido));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_finalizar_pedido* deserializar_finalizar_pedido(buffer_t* buffer){
	msg_finalizar_pedido* msg = malloc(sizeof(msg_finalizar_pedido));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_terminar_pedido* deserializar_terminar_pedido(buffer_t* buffer){
	msg_terminar_pedido* msg = malloc(sizeof(msg_terminar_pedido));
	msg->restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->restaurante->nombre = malloc(msg->restaurante->largo_nombre);
	memcpy(msg->restaurante->nombre, stream, msg->restaurante->largo_nombre);
	stream += msg->restaurante->largo_nombre;

	memcpy(&msg->id_pedido, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return msg;
}

msg_obtener_receta* deserializar_obtener_receta(buffer_t* buffer){
	msg_obtener_receta* msg = malloc(sizeof(msg_obtener_receta));
	msg->plato = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&msg->plato->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	msg->plato->nombre = malloc(msg->plato->largo_nombre);
	memcpy(msg->plato->nombre, stream, msg->plato->largo_nombre);
	stream += msg->plato->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return msg;
}

//-----------------------SERIALIZACION ESTRUCTURAS---------------------------------


void serializarPlato(void* stream, Plato* unPlato, int* offset_ptr){
	db_copiar_string(stream, unPlato->nombre->largo_nombre, unPlato->nombre->nombre, offset_ptr);
	db_copiar_uint32(stream, unPlato->precio, offset_ptr);
}

void serializarPlatos(void* stream, t_list* platos, int* offset_ptr){
	Plato* unPlato;
	db_copiar_uint32(stream, platos->elements_count, offset_ptr);
	for(int i = 0;i< platos->elements_count; i++){
		unPlato = list_get(platos, i);
		serializarPlato(stream, unPlato, offset_ptr);
	}
}

void serializarRepartidor(void* stream, Repartidor repartidor, int* offset_ptr){
	serializarCoordenadas(stream, repartidor.Posicion, offset_ptr);
	db_copiar_uint32(stream, repartidor.frecuencia_descanso, offset_ptr);
	db_copiar_uint32(stream, repartidor.tiempo_descanso, offset_ptr);
}

void serializarCoordenadas(void* stream, CoordenadasXY coordenadas, int* offset_ptr) {
	db_copiar_uint32(stream, coordenadas.posicionX, offset_ptr);
	db_copiar_uint32(stream, coordenadas.posicionY, offset_ptr);
}

void serializarRestauranteSinNombre(void* stream, InfoRestaurante* restaurante, int* offset_ptr){
	serializarAfinidades(stream, restaurante->afinidades, offset_ptr);
	serializarCoordenadas(stream, restaurante->Posicion, offset_ptr);
	serializarPlatos(stream, restaurante->Platos, offset_ptr);
	db_copiar_int32(stream, restaurante->cantidadDePedidos, offset_ptr);
	db_copiar_uint32(stream, restaurante->cantidadDeHornos, offset_ptr);
	db_copiar_uint32(stream, restaurante->cantidadDeCocineros, offset_ptr);
}

void serializarRestaurante(void* stream, Restaurante* restaurante, int* offset_ptr){
	db_copiar_string(stream, restaurante->nombre_restaurante->largo_nombre, restaurante->nombre_restaurante->nombre, offset_ptr);        //no se si offset va sin &
	serializarRestauranteSinNombre(stream, &restaurante->info_Restaurante, offset_ptr);
}

void serializarReceta(void* stream,t_list* pasos, int* offset_ptr){
	Paso* unPaso;
	db_copiar_uint32(stream,pasos->elements_count, offset_ptr);
    for(int i=0;i<pasos->elements_count;i++){
    	unPaso = list_get(pasos,i);
    	db_copiar_string(stream, unPaso->nombre->largo_nombre, unPaso->nombre->nombre, offset_ptr);      //capaz stream necesita un & y capaz offset no...
    	db_copiar_uint32(stream, unPaso->tiempo, offset_ptr);
    }
}

void serializarRecetas(void* stream, t_list* recetas, int* offset_ptr){
	Receta* unaReceta;
	db_copiar_uint32(stream,recetas->elements_count, offset_ptr);
	for(int i = 0; recetas->elements_count; i++){
		unaReceta = list_get(recetas, i);
		serializarReceta(stream, unaReceta->Pasos, offset_ptr);
	}
}

void serializarRecetaConPrecio(void* stream, Receta receta, int32_t precio, int* offset_ptr){
	serializarReceta(stream, receta.Pasos, offset_ptr);
	db_copiar_uint32(stream, precio, offset_ptr);
}



void serializarAfinidades(void* stream, t_list* afinidades, int* offset_ptr){
	nombre* nombre_afinidad;
	db_copiar_uint32(stream, afinidades->elements_count, offset_ptr);
	for(int i = 0; i < afinidades->elements_count; i++){
		nombre_afinidad = list_get(afinidades, i);
		db_copiar_string(stream, nombre_afinidad->largo_nombre, nombre_afinidad->nombre, offset_ptr);
	}
}

void serializarNombres(void* stream, t_list* nombres, int* offset_ptr){
	Plato* nombre;
	db_copiar_uint32(stream, nombres->elements_count, offset_ptr);
	for(int i = 0; i < nombres->elements_count; i++){
		nombre = list_get(nombres, i);
		db_copiar_string(stream, nombre->nombre->largo_nombre, nombre->nombre->nombre, offset_ptr);
	}
}

void serializarNombresReal(void* stream, t_list* nombres, int* offset_ptr){
	nombre* nombre;
	db_copiar_uint32(stream, nombres->elements_count, offset_ptr);
	for(int i = 0; i < nombres->elements_count; i++){
		nombre = list_get(nombres, i);
		db_copiar_string(stream, nombre->largo_nombre, nombre->nombre, offset_ptr);
	}
}

void serializarRegistroComida(void* stream,registroComida* registro,int* offset_ptr){
	db_copiar_string(stream, registro->plato->largo_nombre, registro->plato->nombre, offset_ptr);
	db_copiar_uint32(stream, registro->cantTotal, offset_ptr);
	db_copiar_uint32(stream, registro->cantLista, offset_ptr);
}

void serializarListaDeRegistrosComidas(void* stream,t_list* registros_de_platos,int* offset_ptr){
	registroComida* registro;
	db_copiar_uint32(stream,registros_de_platos->elements_count,offset_ptr);
	for(int i = 0; i < registros_de_platos->elements_count; i++){
		registro = list_get(registros_de_platos,i);
		db_copiar_string(stream, registro->plato->largo_nombre, registro->plato->nombre, offset_ptr);
		db_copiar_uint32(stream, registro->cantTotal, offset_ptr);
		db_copiar_uint32(stream, registro->cantLista, offset_ptr);
	}
}

void serializarPedido(void* stream, Pedido pedido, int* offset_ptr){
	db_copiar_uint32(stream, pedido.estadoPedido, offset_ptr);
	serializarListaDeRegistrosComidas(stream, pedido.platos, offset_ptr);
}

void db_copiar_uint32(void* stream, uint32_t entero, int* offset_ptr){
	memcpy(stream + *offset_ptr, &entero, sizeof(uint32_t));
	*offset_ptr += sizeof(uint32_t);
}
void db_copiar_int32(void* stream, int32_t entero, int* offset_ptr){
	memcpy(stream + *offset_ptr, &entero, sizeof(int32_t));
	*offset_ptr += sizeof(int32_t);
}

void db_copiar_string(void* stream, uint32_t size, const char* cadena, int* offset_ptr){
	db_copiar_uint32(stream, size, offset_ptr);

	memcpy(stream + *offset_ptr,cadena,size);
	*offset_ptr += size;
}

//------------------------------DESERIALIZACION ESTRUCTURAS--------------------------------

int32_t deserializar_int(buffer_t* buffer){
	//int32_t unNumero = (int32_t)buffer->stream;
	void* stream = buffer->stream;

	int32_t unNumero;
	memcpy(&unNumero, stream, sizeof(uint32_t));
	stream += sizeof(int32_t);

	free(buffer->stream);
	free(buffer);

	return unNumero;
}

struct_lista* deserializar_lista_registros_de_comida(buffer_t* buffer){
	struct_lista* registrosDeComida = malloc(sizeof(struct_lista));
	void* stream = buffer->stream;
	t_list* registros = list_create();
	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int i=0;i < cantidad_elementos;i++){
		registroComida* registro = malloc(sizeof(registroComida));
		memcpy(&registro->plato->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		registro->plato->nombre = malloc(registro->plato->largo_nombre);
		memcpy(registro->plato->nombre, stream, registro->plato->largo_nombre);
		stream += registro->plato->largo_nombre;

		memcpy(&registro->cantTotal, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		memcpy(&registro->cantLista, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(registros,registro);
	}
	registrosDeComida->lista = registros;

	free(buffer->stream);
	free(buffer);

	return registrosDeComida;
}

Pedido* deserializar_pedido(buffer_t* buffer){
	Pedido* pedido = malloc(sizeof(Pedido));
	void* stream = buffer->stream;
	t_list* registros = list_create();

	memcpy(&pedido->estadoPedido,stream,sizeof(int));
	stream += sizeof(int);

	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int i=0;i < cantidad_elementos;i++){
		registroComida* registro = malloc(sizeof(registroComida));
		registro->plato = malloc(sizeof(nombre));
		memcpy(&registro->plato->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		registro->plato->nombre = malloc(registro->plato->largo_nombre);
		memcpy(registro->plato->nombre, stream, registro->plato->largo_nombre);
		stream += registro->plato->largo_nombre;

		memcpy(&registro->cantTotal, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		memcpy(&registro->cantLista, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(registros,registro);
	}
	pedido->platos = registros;

	free(buffer->stream);
	free(buffer);

	return pedido;
}

Receta* deserializar_receta(buffer_t* buffer){
	Receta* receta = malloc(sizeof(Receta));
	void* stream = buffer->stream;
	t_list* unosPasos = list_create();

	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	for(int i=0;i < cantidad_elementos;i++){
		Paso* paso = malloc(sizeof(Paso));
		paso->nombre=malloc(sizeof(nombre));
		memcpy(&paso->nombre->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		paso->nombre->nombre = malloc(paso->nombre->largo_nombre);
		memcpy(paso->nombre->nombre, stream, paso->nombre->largo_nombre);
		stream += paso->nombre->largo_nombre;

		memcpy(&paso->tiempo, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(unosPasos,paso);
	}
	receta->Pasos = unosPasos;

	free(buffer->stream);
	free(buffer);

	return receta;

}

struct_lista* deserializar_lista_nombres(buffer_t* buffer){
	struct_lista* lista_nombres = malloc(sizeof(struct_lista));
	void* stream = buffer->stream;
	t_list* registros = list_create();

	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int i=0;i < cantidad_elementos ;i++){
		nombre* nombre_restaurante = malloc(sizeof(nombre));
		memcpy(&nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		nombre_restaurante->nombre = malloc(nombre_restaurante->largo_nombre);
		memcpy(nombre_restaurante->nombre, stream, nombre_restaurante->largo_nombre);
		stream += nombre_restaurante->largo_nombre;
		list_add(registros,nombre_restaurante);
	}
	lista_nombres->lista = registros;

	free(buffer->stream);
	free(buffer);

	return lista_nombres;
}

nombre* deserializar_nombre(buffer_t* buffer){
	nombre* nombre_restaurante = malloc(sizeof(nombre));
	void* stream = buffer->stream;

	memcpy(&nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);


	nombre_restaurante->nombre = malloc(nombre_restaurante->largo_nombre);
	memcpy(nombre_restaurante->nombre, stream, nombre_restaurante->largo_nombre);
	stream += nombre_restaurante->largo_nombre;

	free(buffer->stream);
	free(buffer);

	return nombre_restaurante;
}

Restaurante* deserializar_restaurante(buffer_t* buffer){
	Restaurante* restaurante = malloc(sizeof(Restaurante));
	void* stream = buffer->stream;
	restaurante->nombre_restaurante=malloc(sizeof(nombre));

	memcpy(&restaurante->nombre_restaurante->largo_nombre, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	restaurante->nombre_restaurante->nombre = malloc(restaurante->nombre_restaurante->largo_nombre);
	memcpy(restaurante->nombre_restaurante->nombre, stream, restaurante->nombre_restaurante->largo_nombre);
	stream += restaurante->nombre_restaurante->largo_nombre;

	t_list* lista_afinidades = list_create();
	t_list* platos = list_create();

	int32_t cantidad_afinidades = 0;
	memcpy(&cantidad_afinidades, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	for(int i=0;i <  cantidad_afinidades;i++){
		nombre* afinidad = malloc(sizeof(nombre));
		memcpy(&afinidad->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		afinidad->nombre = malloc(afinidad->largo_nombre);
		memcpy(afinidad->nombre, stream, afinidad->largo_nombre);
		stream += afinidad->largo_nombre;

		list_add(lista_afinidades, afinidad);
	}

	restaurante->info_Restaurante.afinidades = lista_afinidades;

	memcpy(&restaurante->info_Restaurante.Posicion.posicionX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&restaurante->info_Restaurante.Posicion.posicionY, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	for(int j=0;j < cantidad_elementos;j++){
		Plato* plato = malloc(sizeof(Plato));
		plato->nombre=malloc(sizeof(nombre));
		memcpy(&plato->nombre->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);
		plato->nombre->nombre = malloc(plato->nombre->largo_nombre+1);
		memcpy(plato->nombre->nombre, stream, plato->nombre->largo_nombre);
		stream += plato->nombre->largo_nombre;

		memcpy(&plato->precio, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(platos,plato);
	}

	restaurante->info_Restaurante.Platos = platos;
	memcpy(&restaurante->info_Restaurante.cantidadDePedidos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	memcpy(&restaurante->info_Restaurante.cantidadDeHornos, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);


	memcpy(&restaurante->info_Restaurante.cantidadDeCocineros, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	free(buffer->stream);
	free(buffer);

	return restaurante;
}

InfoRestaurante* deserializar_restaurante_sin_nombre(buffer_t* buffer){
	InfoRestaurante* restaurante = malloc(sizeof(InfoRestaurante));
	void* stream = buffer->stream;

	t_list* lista_afinidades = list_create();
	t_list* platos = list_create();

	int32_t cantidad_afinidades = 0;
	memcpy(&cantidad_afinidades, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int i=0;i <  cantidad_afinidades;i++){
		nombre* afinidad = malloc(sizeof(nombre));
		memcpy(&afinidad->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		afinidad->nombre = malloc(afinidad->largo_nombre);
		memcpy(afinidad->nombre, stream, afinidad->largo_nombre);
		stream += afinidad->largo_nombre;

		list_add(lista_afinidades, afinidad);
	}

	restaurante->afinidades = lista_afinidades;

	memcpy(&restaurante->Posicion.posicionX, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);
	memcpy(&restaurante->Posicion.posicionY, stream, sizeof(uint32_t));
	stream += sizeof(uint32_t);

	int32_t cantidad_elementos = 0;
	memcpy(&cantidad_elementos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	for(int j=0;j < cantidad_elementos;j++){
		Plato* plato = malloc(sizeof(Plato));
		memcpy(&plato->nombre->largo_nombre, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		plato->nombre->nombre = malloc(plato->nombre->largo_nombre);
		memcpy(plato->nombre->nombre, stream, plato->nombre->largo_nombre);
		stream += plato->nombre->largo_nombre;

		memcpy(&plato->precio, stream, sizeof(uint32_t));
		stream += sizeof(uint32_t);

		list_add(platos,plato);
	}

	restaurante->Platos = platos;

	memcpy(&restaurante->cantidadDeHornos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	memcpy(&restaurante->cantidadDePedidos, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	memcpy(&restaurante->cantidadDeCocineros, stream, sizeof(int32_t));
	stream += sizeof(int32_t);

	free(buffer->stream);
	free(buffer);

	return restaurante;
}
enum_modulo deserealizar_modulo(buffer_t* buffer){
	enum_modulo modulo;
	void*stream=buffer->stream;
	memcpy(&modulo,stream,sizeof(int));
	stream+=sizeof(int);
	free(buffer->stream);
	free(buffer);
	return modulo;
}

