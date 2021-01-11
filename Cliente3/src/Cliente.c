#include "Cliente.h"
#define elementosCrearReceta 4


pthread_t conexion_escucha;
int32_t id_cliente;
struct_lista* lista_platos;
int32_t idPedido;

msg_consultar_pedido* parsearConsultarPedido(char* idPedido){
	msg_consultar_pedido* mensaje = malloc(sizeof(msg_consultar_pedido));
	mensaje->id_pedido = atoi(idPedido);

	return mensaje;
}

msg_plato_listo* parsearPlatoListo(char* idPedido,char* nombrePlato,char* nombreRestaurante){
	msg_plato_listo* mensaje = malloc(sizeof(msg_plato_listo));
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante) + 1;
	//mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	mensaje->id_pedido = atoi(idPedido);
	mensaje->plato=malloc(sizeof(nombre));
	mensaje->plato->largo_nombre = strlen(nombrePlato) + 1;
	//mensaje->plato->nombre=malloc(mensaje->plato->largo_nombre);
	mensaje->plato->nombre = nombrePlato;

	return mensaje;
}

msg_confirmar_pedido* parsearConfirmarPedidoConRestaurante(char* idPedido,char* nombreRestaurante){
	msg_confirmar_pedido* mensaje = malloc(sizeof(msg_confirmar_pedido));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->nombre_restaurante=malloc(sizeof(nombre));
	mensaje->nombre_restaurante->largo_nombre=strlen(nombreRestaurante)+1;
	//mensaje->nombre_restaurante->nombre=malloc(mensaje->nombre_restaurante->largo_nombre);
	mensaje->nombre_restaurante->nombre = nombreRestaurante;
	return mensaje;
}

msg_aniadir_plato* parsearAniadirPlato(char* plato,char* idPedido){
	msg_aniadir_plato* mensaje = malloc(sizeof(msg_aniadir_plato));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->plato=malloc(sizeof(nombre));
	mensaje->plato->largo_nombre = strlen(plato) + 1;
	//mensaje->plato->nombre=malloc(mensaje->plato->largo_nombre);
	mensaje->plato->nombre = plato;
	return mensaje;
}

msg_seleccionar_restaurante* parsearSeleccionarRestaurante(int32_t idCliente,char* nombreRestaurante){
	msg_seleccionar_restaurante* mensaje = malloc(sizeof(msg_seleccionar_restaurante));
	mensaje->id_cliente = idCliente;
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante)+1;
	mensaje->restaurante->nombre = nombreRestaurante;

	return mensaje;
}
msg_guardar_pedido* parsearGuardarPedido(char* idPedido,char* nombreRestaurante){
	msg_guardar_pedido* mensaje = malloc(sizeof(msg_guardar_pedido));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante)+1;
	//mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	return mensaje;
}
msg_obtener_pedido* parsearObtenerPedido(char* idPedido,char* nombreRestaurante){
	msg_obtener_pedido* mensaje = malloc(sizeof(msg_obtener_pedido));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante)+1;
	//mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	return mensaje;
}
msg_finalizar_pedido* parsearFinalizarPedido(char* idPedido,char* nombreRestaurante){
	msg_finalizar_pedido* mensaje = malloc(sizeof(msg_finalizar_pedido));
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante)+1;
	//mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	return mensaje;
}
msg_terminar_pedido* parsearTerminarPedido(char* idPedido,char* nombreRestaurante){
	msg_terminar_pedido* mensaje = malloc(sizeof(msg_terminar_pedido));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante)+1;
	mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	return mensaje;
}
msg_guardar_plato* parsearGuardarPlato(char* idPedido,char* nombrePlato,char* cantidad,char* nombreRestaurante){
	msg_guardar_plato* mensaje = malloc(sizeof(msg_guardar_plato));
	mensaje->id_pedido = atoi(idPedido);
	mensaje->plato=malloc(sizeof(nombre));
	mensaje->plato->largo_nombre = strlen(nombrePlato) + 1;
	//mensaje->plato->nombre=malloc(mensaje->plato->largo_nombre);
	mensaje->plato->nombre = nombrePlato;
	mensaje->cantidad_platos=atoi(cantidad);
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante) + 1;
	//mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;

	return mensaje;
}

msg_obtener_restaurante* parsearObtenerRestaurante(char* nombreRestaurante){
	msg_obtener_restaurante* mensaje = malloc(sizeof(msg_obtener_restaurante));
	mensaje->restaurante=malloc(sizeof(nombre));
	mensaje->restaurante->largo_nombre = strlen(nombreRestaurante) + 1;
	mensaje->restaurante->nombre=malloc(mensaje->restaurante->largo_nombre);
	mensaje->restaurante->nombre = nombreRestaurante;
	return mensaje;
}

msg_obtener_receta* parsearObtenerReceta(char* nombrePlato){
	msg_obtener_receta* mensaje=malloc(sizeof(msg_obtener_receta));
	mensaje->plato=malloc(sizeof(nombre));
	mensaje->plato->largo_nombre=strlen(nombrePlato)+1;
	mensaje->plato->nombre=malloc(mensaje->plato->largo_nombre);
	mensaje->plato->nombre=nombrePlato;
	return mensaje;

}

void atender(sfd socket, buffer_t* buffer, enum_mensaje codigo_operacion){
		int32_t okfail;
		msg_finalizar_pedido*msg;
		buffer_t*buffer2;
	switch(codigo_operacion){
		case FINALIZAR_PEDIDO:
			 msg=deserializar_finalizar_pedido(buffer);
			 okfail=0;
			//PY_LOG_INFO("Se ha finalizado el pedido %i del restaurante %s",msg->id_pedido,msg->restaurante->nombre);
			log_info(logger,"Se ha finalizado el pedido %i del restaurante %s",msg->id_pedido,msg->restaurante->nombre);
			buffer2=serializar_int(okfail);
			py_send(socket,OK_FAIL,buffer2);
			free(msg->restaurante->nombre);
			free(msg->restaurante);
			free(msg);
			free(buffer2->stream);
			free(buffer2);
			break;
		default:
			break;

    }
}
void mensajesAPP(char**array){
	if (!strcmp(array[0],"CONSULTAR_RESTAURANTE")){
		buffer_t* buffer_handshake_normal = serializar_int(py_config_get_int("ID_CLIENTE"));
		sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
		buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
		buffer_vacio->size = 0;
		py_send(sfd_app, CONSULTAR_RESTAURANTE, buffer_vacio);
		//PY_LOG_INFO("Se ha enviado un pedido de consulta de restaurantes.");
		log_info(logger,"Se envió el mensaje CONSULTAR_RESTAURANTE");
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_app, paquete);
		struct_lista* lista_nombres = deserializar_lista_nombres(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido una lista de restaurantes.");
		log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_RESTAURANTE");
		nombre* un_nombre;
		for(int i = 0; i < lista_nombres->lista->elements_count; i++){
			un_nombre=list_get(lista_nombres->lista,i);
			log_info(logger,"nombre Restaurante %s .",un_nombre->nombre);
		}
		free(buffer_handshake_normal->stream);
		free(buffer_handshake_normal);
		free(buffer_vacio);
		free(paquete);
		for(int i = 0; i < lista_nombres->lista->elements_count; i++){
			un_nombre = list_get(lista_nombres->lista, i);
			free(un_nombre->nombre);
			free(un_nombre);
		}
		list_destroy(lista_nombres->lista);
		free(lista_nombres);
		close(sfd_app);
		}
		else if(!strcmp(array[0],"SELECCIONAR_RESTAURANTE")){
			buffer_t* buffer_handshake_normal = serializar_int(py_config_get_int("ID_CLIENTE"));
			sfd sfd_app = conectar_y_enviar("IP","PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			msg_seleccionar_restaurante* mensaje;
			mensaje = parsearSeleccionarRestaurante(py_config_get_int("ID_CLIENTE"),array[1]);
			buffer_t* buffer = serializar_seleccionar_restaurante(mensaje->restaurante, mensaje->id_cliente);
			py_send(sfd_app, SELECCIONAR_RESTAURANTE, buffer);
			//PY_LOG_INFO("Se ha enviado un pedido de seleccion de restaurante.");
			log_info(logger,"Se envió el mensaje SELECCIONAR_RESTAURANTE, nombre restaurante: %s",mensaje->restaurante->nombre);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			int32_t okfail = deserializar_int(paquete->buffer);
			log_info(logger,"Se recibió la respuesta del mensaje SELECCIONAR_RESTAURANTE, respuesta: %d",okfail);
			free(buffer_handshake_normal->stream);
			free(buffer_handshake_normal);
			free(mensaje->restaurante->nombre);
			free(mensaje->restaurante);
			free(mensaje);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			close(sfd_app);
			}
		else if(!strcmp(array[0],"CONSULTAR_PLATOS")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
			buffer_vacio->size = 0;
			py_send(sfd_app, CONSULTAR_PLATOS, buffer_vacio);
			//PY_LOG_INFO("Se ha enviado un pedido de consulta de platos.");
			log_info(logger,"Se envió el mensaje CONSULTAR_PLATOS");
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			if(paquete->buffer->stream){
				lista_platos = deserializar_lista_nombres(paquete->buffer);
				PY_LOG_INFO("Se ha recibido una lista de platos.");
				log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_PLATOS");

				for(int i=0;i<lista_platos->lista->elements_count;i++){
					nombre* nombre;
			     	nombre=list_get(lista_platos->lista,i);
					PY_LOG_INFO("Plato %s",nombre->nombre);
				}
			}
			else{
				PY_LOG_INFO("No se recibió ningún plato porque el cliente no está asociado a un restaurante");
			}

			free(buffer_handshake_normal->stream);
	        free(buffer_handshake_normal);
	        free(buffer_vacio);
	        free(paquete);
			close(sfd_app);
			}
		else if(!strcmp(array[0],"CREAR_PEDIDO")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
			buffer_vacio->size = 0;
			py_send(sfd_app, CREAR_PEDIDO, buffer_vacio);
			//PY_LOG_INFO("Se ha enviado un pedido creación de pedidos.");
			log_info(logger,"Se envió el mensaje CREAR_PEDIDO");
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			idPedido = deserializar_int(paquete->buffer);
			if(idPedido==-1){
				//PY_LOG_INFO("Error al crear el pedido.");
				log_info(logger,"Se recibió la respuesta del mensaje CREAR_PEDIDO, error al crear el pedido %d",idPedido);
			}
			else{
				log_info(logger,"Se recibió la respuesta del mensaje CREAR_PEDIDO, respuesta: %d",idPedido);
			}
			free(buffer_handshake_normal->stream);
			free(buffer_handshake_normal);
			free(buffer_vacio);
			free(paquete);
			close(sfd_app);
		}
		else if(!strcmp(array[0],"AÑADIR_PLATO")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			msg_aniadir_plato* mensaje;
			mensaje = parsearAniadirPlato(array[1],array[2]);
			buffer_t* buffer = serializar_aniadir_plato(mensaje->plato, mensaje->id_pedido);
			py_send(sfd_app, ANIADIR_PLATO, buffer);
			//PY_LOG_INFO("Se ha enviado un pedido para añadir un plato.");
			log_info(logger,"Se envió el mensaje AÑADIR_PLATO con id de pedido %d y nombre plato %s",mensaje->id_pedido,mensaje->plato->nombre);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			int32_t okfail;
			okfail = deserializar_int(paquete->buffer);
			log_info(logger,"Se recibió la respuesta del mensaje AÑADIR_PLATO, respuesta: %d",okfail);
			free(buffer_handshake_normal->stream);
			free(buffer_handshake_normal);
			free(mensaje->plato->nombre);
			free(mensaje->plato);
			free(mensaje);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			close(sfd_app);
		}
		else if(!strcmp(array[0],"PLATO_LISTO")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			msg_plato_listo* mensaje;
			mensaje = parsearPlatoListo(array[1],array[2],array[3]);
			buffer_t* buffer = serializar_plato_listo(mensaje->plato,mensaje->restaurante,mensaje->id_pedido);
			py_send(sfd_app, PLATO_LISTO, buffer);
			//PY_LOG_INFO("Se ha enviado un aviso de plato listo.");
			log_info(logger,"Se envió el mensaje PLATO_LISTO para el restaurante %s,id de pedido %d y nombre plato %s",mensaje->restaurante->nombre,mensaje->id_pedido,mensaje->plato->nombre);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			//PY_LOG_INFO("Ha llegado la notificacion de un plato listo.");
			int32_t okfail;
			okfail = deserializar_int(paquete->buffer);
			log_info(logger,"Se recibió la respuesta del mensaje PLATO_LISTO, respuesta: %d",okfail);
			free(buffer_handshake_normal);
			free(buffer_handshake_normal->stream);
			free(mensaje->plato->nombre);
			free(mensaje->plato);
			free(mensaje->restaurante->nombre);
			free(mensaje->restaurante);
			free(mensaje);
			free(buffer);
			free(paquete);
			close(sfd_app);
		}
		else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP", "PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			int32_t id_pedido = atoi(array[1]);
			buffer_t* buffer = serializar_int(id_pedido);
			py_send(sfd_app,CONFIRMAR_PEDIDO, buffer);
			//PY_LOG_INFO("Se ha enviado un pedido de confirmacion de pedido.");
			log_info(logger,"Se envió el mensaje CONFIRMAR_PEDIDO con id de pedido %d",id_pedido);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			//PY_LOG_INFO("Ha llegado la confirmacion de pedido.");
			int32_t okfail;
			okfail = deserializar_int(paquete->buffer);
			log_info(logger,"Se recibió la respuesta del mensaje CONFIRMAR_PEDIDO, respuesta: %d",okfail);
			free(buffer_handshake_normal->stream);
			free(buffer_handshake_normal);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			close(sfd_app);
		}
		else if(!strcmp(array[0],"CONSULTAR_PEDIDO")){
			buffer_t* buffer_handshake_normal = serializar_int(id_cliente);
			sfd sfd_app = conectar_y_enviar("IP","PUERTO",buffer_handshake_normal, HANDSHAKE_NORMAL_CLIENTE);
			msg_consultar_pedido* mensaje;
			mensaje = parsearConsultarPedido(array[1]);
			buffer_t* buffer = serializar_consultar_pedido(mensaje->id_pedido);
			py_send(sfd_app,CONSULTAR_PEDIDO, buffer);
			//PY_LOG_INFO("Se ha enviado una consulta de pedido");
			log_info(logger,"Se envió el mensaje CONSULTAR_PEDIDO con id de pedido %d",mensaje->id_pedido);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete);
			msg_consultar_pedido_rta*pedido=malloc(sizeof(msg_consultar_pedido_rta));
			pedido = deserializar_consultar_pedido_rta(buffer);
			log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_PEDIDO,nombre del restaurante: %s y estado del pedido: %d",pedido->nombre_restaurante->nombre,pedido->pedido.estadoPedido);;
			//printf("Nombre del restaurante del pedido: %s\n",pedido->nombre_restaurante->nombre);
			/*
			for(int i=0;i<pedido->pedido.platos->elements_count;i++){
				registroComida* registro;
				registro = list_get(pedido->pedido.platos, i);
				printf("Nombre plato: %s \n",registro->plato->nombre);
				printf("Cantidad total: %i \n",registro->cantTotal);
				printf("Cantidad lista: %i \n",registro->cantLista);
			}*/

			free(buffer_handshake_normal->stream);
			free(buffer_handshake_normal);
			free(mensaje);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			free(pedido);
			close(sfd_app);
		}
}

void destruir_pedido(Pedido* un_pedido){
	for(int i = 0 ; i < un_pedido->platos->elements_count; i++){
		registroComida* un_plato = list_get(un_pedido->platos,i);
		free(un_plato->plato->nombre);
		free(un_plato->plato);
		free(un_plato);
	}
	list_destroy(un_pedido->platos);
	free(un_pedido);
}

void mensajesComanda(char**array){
	if(!strcmp(array[0],"GUARDAR_PEDIDO")){
		msg_guardar_pedido* mensaje;
		mensaje = parsearGuardarPedido(array[1], array[2]);
		buffer_t* buffer = serializar_guardar_pedido(mensaje->restaurante, mensaje->id_pedido);
		sfd sfd_comanda= conectar_y_enviar("IP", "PUERTO",buffer, GUARDAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado un pedido para guardar un pedido");
		log_info(logger,"Se envió el mensaje GUARDAR_PEDIDO para el restaurante %s y id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido un pedido para guardar un pedido");
		log_info(logger,"Se recibió la respuesta del mensaje GUARDAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_comanda);
	}
	else if(!strcmp(array[0],"GUARDAR_PLATO")){
		msg_guardar_plato* mensaje;
		mensaje = parsearGuardarPlato(array[1],array[2],array[3],array[4]);
		buffer_t* buffer = serializar_guardar_plato(mensaje->restaurante,mensaje->id_pedido,mensaje->plato,mensaje->cantidad_platos);
		sfd sfd_comanda = conectar_y_enviar("IP","PUERTO",buffer,GUARDAR_PLATO);
		//PY_LOG_INFO("Se ha enviado un pedido para guardar plato")
		log_info(logger,"Se envió el mensaje GUARDAR_PLATO para el restaurante %s,id de pedido %d y nombre plato %s",mensaje->restaurante->nombre,mensaje->id_pedido,mensaje->plato->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido una confirmacion para guardar plato");
		log_info(logger,"Se recibió la respuesta del mensaje GUARDAR_PLATO, respuesta: %d",okfail);
		free(mensaje->plato->nombre);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje->plato);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_comanda);
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		msg_confirmar_pedido* mensaje;
		mensaje = parsearConfirmarPedidoConRestaurante(array[1],array[2]);
		buffer_t* buffer = serializar_confirmar_pedido_con_restaurante(mensaje->id_pedido,mensaje->nombre_restaurante);
		sfd sfd_comanda = conectar_y_enviar("IP","PUERTO", buffer, CONFIRMAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado un pedido de confirmacion de pedido");
		log_info(logger,"Se envió el mensaje CONFIRMAR_PEDIDO para el restaurante %s y id de peiddo %d",mensaje->nombre_restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha confirmado el pedido");
		log_info(logger,"Se recibió la respuesta del mensaje CONFIRMAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->nombre_restaurante->nombre);
		free(mensaje->nombre_restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_comanda);
	}
	else if(!strcmp(array[0],"PLATO_LISTO")){
	msg_plato_listo* mensaje;
	mensaje = parsearPlatoListo(array[1],array[2],array[3]);
	buffer_t* buffer = serializar_plato_listo(mensaje->plato,mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_comanda = conectar_y_enviar("IP","PUERTO",buffer,PLATO_LISTO);
	//PY_LOG_INFO("Se ha enviado un aviso de plato listo");
	log_info(logger,"Se envió el mensaje PLATO_LISTO para el restaurante %s,id de pedido %d y nombre plato %s",mensaje->restaurante->nombre,mensaje->id_pedido,mensaje->plato->nombre);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_comanda, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	//PY_LOG_INFO("Se ha recibido la confirmacion de plato listo");
	log_info(logger,"Se recibió la respuesta del mensaje PLATO_LISTO, respuesta: %d",okfail);
	free(mensaje->plato->nombre);
	free(mensaje->restaurante->nombre);
	free(mensaje->plato);
	free(mensaje->restaurante);
	free(mensaje);
	free(buffer->stream);
	free(buffer);
	free(paquete);
	close(sfd_comanda);
	}
	else if(!strcmp(array[0],"OBTENER_PEDIDO")){
	msg_obtener_pedido* mensaje;
	mensaje = parsearObtenerPedido(array[1],array[2]);
    buffer_t* buffer = serializar_obtener_pedido(mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_comanda = conectar_y_enviar("IP","PUERTO", buffer, OBTENER_PEDIDO);
	//PY_LOG_INFO("Se ha enviado pedido para obtener pedido");
	log_info(logger,"Se envió el mensaje OBTENER_PEDIDO para el restaurante %s y id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_comanda, paquete);
	Pedido* unPedido = deserializar_pedido(paquete->buffer);
	//PY_LOG_INFO("Se ha recibido un pedido");
	log_info(logger,"Se recibió la respuesta del mensaje OBTENER_PEDIDO, estado del pedido solicitado: %d",unPedido->estadoPedido);
	/*
	for(int i=0;i<unPedido->platos->elements_count;i++){
		registroComida* registro;
		registro = list_get(unPedido->platos, i);
		printf("Platos del pedido: \n");
		printf("Nombre plato: %s \n",registro->plato->nombre);
		printf("Cantidad total: %i \n",registro->cantTotal);
		printf("Cantidad lista: %i \n",registro->cantLista);
	}*/
	free(mensaje->restaurante->nombre);
	free(mensaje->restaurante);
	free(mensaje);
	free(buffer->stream);
	free(buffer);
	free(paquete);
	destruir_pedido(unPedido);
	close(sfd_comanda);
	}
	else if(!strcmp(array[0],"FINALIZAR_PEDIDO")){
		msg_finalizar_pedido* mensaje;
		mensaje = parsearFinalizarPedido(array[1],array[2]);
	    buffer_t* buffer = serializar_finalizar_pedido(mensaje->restaurante,mensaje->id_pedido);
		sfd sfd_comanda = conectar_y_enviar("IP","PUERTO",buffer,FINALIZAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado una finalizacion de pedido");
		log_info(logger,"Se envió el mensaje FINALIZAR_PEDIDO para el restaurante %s y el id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido la finalización de pedido");
		log_info(logger,"Se recibió la respuesta del mensaje FINALIZAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_comanda);
	}
}

void destruirInfoRestaurante(InfoRestaurante* info){
	for(int i = 0; i < info->Platos->elements_count; i++){
		Plato* un_plato = list_get(info->Platos, i);
		free(un_plato->nombre->nombre);
		free(un_plato->nombre);
		free(un_plato);
	}
	list_destroy(info->Platos);
	for(int i = 0; i < info->afinidades->elements_count; i++){
		nombre* afinidad = list_get(info->afinidades, i);
		free(afinidad->nombre);
		free(afinidad);
	}
	list_destroy(info->afinidades);
	free(info);
}

void mensajesSindicato(char**array){
	if(!strcmp(array[0],"OBTENER_RESTAURANTE")){
		msg_obtener_restaurante* mensaje;
		mensaje = parsearObtenerRestaurante(array[1]);
		buffer_t* buffer = serializar_obtener_restaurante(mensaje->restaurante);
		sfd sfd_sindicato = conectar_y_enviar("IP","PUERTO", buffer, OBTENER_RESTAURANTE);
		//PY_LOG_INFO("Se ha enviado pedido para obtener restaurante");
		log_info(logger,"Se envió el mensaje OBTENER_RESTAURANTE para el restaurante %s",mensaje->restaurante->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		InfoRestaurante* restaurante = deserializar_restaurante_sin_nombre(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido el pedido para obtener restaurante");
		log_info(logger,"Se recibió la respuesta del mensaje OBTENER_RESTAURANTE");
		/*
		for(int i=0;i<restaurante->afinidades->elements_count;i++){
			nombre*afinidad;
			afinidad= list_get(restaurante->afinidades, i);
			printf("Afinidad %s \n",afinidad->nombre);
		}
		printf("Posicion X:%i \n",restaurante->Posicion.posicionX);
		printf("Posicion Y:%i \n",restaurante->Posicion.posicionY);
		for(int i=0;i<restaurante->Platos->elements_count;i++){
			Plato* plato;
			plato=list_get(restaurante->Platos, i);
     		printf("Plato: %s \n",plato->nombre->nombre);
			printf("Precio: %i \n",plato->precio);
		}
		printf("Cantidad de hornos:%d \n",restaurante->cantidadDeHornos);
		printf("Cantidad de pedidos:%d \n",restaurante->cantidadDePedidos);*/
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		destruirInfoRestaurante(restaurante);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"CONSULTAR_PLATOS")){
	buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
	buffer_vacio->size = 0;
	sfd sfd_sindicato = conectar_y_enviar("IP", "PUERTO",buffer_vacio, CONSULTAR_PLATOS);
	//PY_LOG_INFO("Se ha enviado un pedido de consulta de platos");
	log_info(logger,"Se envió el mensaje CONSULTAR_PLATOS");
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_sindicato, paquete);
	lista_platos = deserializar_lista_registros_de_comida(paquete->buffer);
	//PY_LOG_INFO("Se ha recibido una lista de platos");
	log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_PLATOS");
	/*
	for(int i=0;i<lista_platos->lista->elements_count;i++){
		Plato* plato;
		plato= list_get(lista_platos->lista, i);
		printf("Plato %s \n",plato->nombre->nombre);
	}*/
	free(buffer_vacio);
	free(paquete);
	for(int i = 0; i < lista_platos->lista->elements_count; i++){
		Plato* un_plato = list_get(lista_platos->lista, i);
		free(un_plato->nombre->nombre);
		free(un_plato->nombre);
		free(un_plato);
	}
	list_destroy(lista_platos->lista);
	free(lista_platos);
	close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"GUARDAR_PEDIDO")){
		msg_guardar_pedido* mensaje = parsearGuardarPedido(array[1], array[2]);
	    buffer_t* buffer = serializar_guardar_pedido(mensaje->restaurante, mensaje->id_pedido);
		sfd sfd_sindicato =conectar_y_enviar("IP", "PUERTO", buffer, GUARDAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado un pedido para guardar un pedido");
		log_info(logger,"Se envió el mensaje GUARDAR_PEDIDO para el restaurante %s y el id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido un pedido para guardar un pedido");
		log_info(logger,"Se recibió la respuesta del mensaje GUARDAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"GUARDAR_PLATO")){
		msg_guardar_plato* mensaje;
		mensaje = parsearGuardarPlato(array[1],array[2],array[3],array[4]);
		buffer_t* buffer = serializar_guardar_plato(mensaje->restaurante,mensaje->id_pedido,mensaje->plato,mensaje->cantidad_platos);
		sfd sfd_sindicato= conectar_y_enviar("IP","PUERTO", buffer, GUARDAR_PLATO);
		//PY_LOG_INFO("Se ha enviado un pedido para guardar plato")
		log_info(logger,"Se envió el mensaje GUARDAR_PLATO para el restaurante %s, id de pedido %d y nombre plato %s",mensaje->restaurante->nombre,mensaje->id_pedido,mensaje->plato->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido una confirmacion para guardar plato");
		log_info(logger,"Se recibió la respuesta del mensaje GUARDAR_PLATO, respuesta: %d",okfail);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		msg_confirmar_pedido* mensaje;
		mensaje = parsearConfirmarPedidoConRestaurante(array[1],array[2]);
		buffer_t* buffer = serializar_confirmar_pedido_con_restaurante(mensaje->id_pedido, mensaje->nombre_restaurante);
		sfd sfd_sindicato = conectar_y_enviar("IP", "PUERTO", buffer, CONFIRMAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado un pedido de confirmacion de pedido");
		log_info(logger,"Se envió el mensaje CONFIRMAR_PEDIDO para el restaurante %s y id de pedido %d",mensaje->nombre_restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha confirmado el pedido");
		log_info(logger,"Se recibió la respuesta del mensaje CONFIRMAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->nombre_restaurante->nombre);
		free(mensaje->nombre_restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"PLATO_LISTO")){
		msg_plato_listo* mensaje;
	    mensaje = parsearPlatoListo(array[1],array[2],array[3]);
		buffer_t* buffer = serializar_plato_listo(mensaje->plato,mensaje->restaurante,mensaje->id_pedido);
		sfd sfd_sindicato = conectar_y_enviar("IP","PUERTO", buffer, PLATO_LISTO);
		//PY_LOG_INFO("Se ha enviado un aviso de plato listo");
		log_info(logger,"Se envió el mensaje PLATO_LISTO para el restaurante %s,id de pedido %d y nombre plato %s",mensaje->restaurante->nombre,mensaje->id_pedido,mensaje->restaurante->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
    	py_recv_msg(sfd_sindicato, paquete);
		int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido la confirmacion de plato listo");
		log_info(logger,"Se recibió la respuesta del mensaje PLATO_LISTO, respuesta: %d",okfail);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"OBTENER_PEDIDO")){
		msg_obtener_pedido* mensaje;
		mensaje = parsearObtenerPedido(array[1],array[2]);
		buffer_t* buffer = serializar_obtener_pedido(mensaje->restaurante,mensaje->id_pedido);
		sfd sfd_sindicato = conectar_y_enviar("IP", "PUERTO", buffer, OBTENER_PEDIDO);
		//PY_LOG_INFO("Se ha enviado pedido para obtener pedido");
		log_info(logger,"Se envió el mensaje OBTENER_PEDIDO para el restaurante %s y id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		Pedido* pedido = deserializar_pedido(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido un pedido");
		log_info(logger,"Se recibió la respuesta del mensaje OBTENER_PEDIDO, estado del pedido solicitado: %d",pedido->estadoPedido);
			//PY_LOG_INFO("Se ha recibido un pedido");

		/*
		for(int i=0;i<pedido->platos->elements_count;i++){
			registroComida* registro;
    		registro = list_get(pedido->platos, i);
			printf("Nombre plato: %s \n",registro->plato->nombre);
			printf("Cantidad total: %i \n",registro->cantTotal);
			printf("Cantidad lista: %i \n",registro->cantLista);
		}*/
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer);
		free(paquete);
		destruir_pedido(pedido);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"TERMINAR_PEDIDO")){
		msg_terminar_pedido* mensaje;
		mensaje = parsearTerminarPedido(array[1],array[2]);
		buffer_t* buffer = serializar_terminar_pedido(mensaje->restaurante,mensaje->id_pedido);
		sfd sfd_sindicato = conectar_y_enviar("IP", "PUERTO", buffer, TERMINAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado pedido para terminar pedido");
		log_info(logger,"Se envió el mensaje TERMINAR_PEDIDO para el restaurante %s y id de pedido %d",mensaje->restaurante->nombre,mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
    	int32_t okfail = deserializar_int(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido la confirmacion para terminar pedido");
		log_info(logger,"Se recibió la respuesta del mensaje TERMINAR_PEDIDO, respuesta: %d",okfail);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_sindicato);
	}
	else if(!strcmp(array[0],"OBTENER_RECETA")){
		msg_obtener_receta* mensaje;
		mensaje = parsearObtenerReceta(array[1]);
		buffer_t* buffer = serializar_obtener_receta(mensaje->plato);
		sfd sfd_sindicato = conectar_y_enviar("IP", "PUERTO", buffer, TERMINAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado pedido para obtener receta");
		log_info(logger,"Se envió el mensaje OBTENER_RECETA, nombre plato: %s",mensaje->plato->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_sindicato, paquete);
		Receta* receta = deserializar_receta(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido el pedido con la receta");
		log_info(logger,"Se recibió la respuesta del mensaje OBTENER_RECETA");
		/*
		printf("Se mostrarán los pasos del la receta \n");
		for(int i=0;i>receta->Pasos->elements_count;i++){
			Paso* un_paso;
			un_paso=list_get(receta->Pasos,i);
			printf("Paso %i: %s",i,un_paso->nombre->nombre);
		}*/
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		for(int i = 0; i < receta->Pasos->elements_count; i++){
			nombre* paso = list_get(receta->Pasos, i);
			free(paso->nombre);
			free(paso);
		}
		list_destroy(receta->Pasos);
		free(receta);
		close(sfd_sindicato);
	}
}

void mensajesRestaurante(char**array){
	if(!strcmp(array[0],"CONSULTAR_PLATOS")){
		buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
	    buffer_vacio->size = 0;
		sfd sfd_restaurante = conectar_y_enviar("IP", "PUERTO",buffer_vacio, CONSULTAR_PLATOS);
		//PY_LOG_INFO("Se ha enviado un pedido de consulta de platos");
		log_info(logger,"Se envió el mensaje CONSULTAR_PLATOS");
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_restaurante, paquete);
		lista_platos = deserializar_lista_nombres(paquete->buffer);
		//PY_LOG_INFO("Se ha recibido una lista de platos");
		log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_PLATOS");
		/*
		for(int i=0;i<lista_platos->lista->elements_count;i++){
			nombre* nombre;
			nombre=list_get(lista_platos->lista, i);
			printf("%s \n",nombre->nombre);
		}*/
		free(buffer_vacio);
		free(paquete);
		for(int i = 0; i < lista_platos->lista->elements_count; i++){
			nombre* plato = list_get(lista_platos->lista, i);
			free(plato->nombre);
			free(plato);
		}
		list_destroy(lista_platos->lista);
		free(lista_platos);
		close(sfd_restaurante);
	}
	else if(!strcmp(array[0],"CREAR_PEDIDO")){
		buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
		buffer_vacio->size = 0;
		sfd sfd_restaurante = conectar_y_enviar("IP", "PUERTO",buffer_vacio,CREAR_PEDIDO);
		//PY_LOG_INFO("Se ha enviado un pedido de creación de pedidos");
		log_info(logger,"Se envió el mensaje CREAR_PEDIDO");
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_restaurante, paquete);
		idPedido = deserializar_int(paquete->buffer);
		if(idPedido==-1){
			//PY_LOG_INFO("Error al crear el pedido")
			log_info(logger,"Se recibió la respuesta del mensaje CREAR_PEDIDO, error no se pudo crear");
		}
		else{
			//PY_LOG_INFO("Se creado un pedido");
		    printf("ID pedido: %i\n ",idPedido);
			log_info(logger,"Se recibió la respuesta del mensaje CREAR_PEDIDO, id de pedido: %d",idPedido);
		}
		free(buffer_vacio);
		free(paquete);
		close(sfd_restaurante);
	}
	else if(!strcmp(array[0],"AÑADIR_PLATO")){
		msg_aniadir_plato* mensaje;
		mensaje = parsearAniadirPlato(array[1],array[2]);
		buffer_t* buffer = serializar_aniadir_plato(mensaje->plato, mensaje->id_pedido);
		sfd sfd_restaurante = conectar_y_enviar("IP", "PUERTO", buffer, ANIADIR_PLATO);
		//PY_LOG_INFO("Se ha enviado un pedido de creación de plato");
		log_info(logger,"Se envió el mensaje ANIADIR_PLATO, id de pedido %d y nombre plato: %s",mensaje->id_pedido,mensaje->plato->nombre);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_restaurante, paquete);
		//PY_LOG_INFO("Se ha recibido de añadicion un plato");
		int32_t okfail;
		okfail = deserializar_int(paquete->buffer);
		log_info(logger,"Se recibió la respuesta del mensaje ANIADIR_PLATO, respuesta: %d",okfail);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		close(sfd_restaurante);
	}
    else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
			int32_t id_pedido = atoi(array[1]);
			buffer_t* buffer = serializar_int(id_pedido);
			sfd sfd_restaurante = conectar_y_enviar("IP", "PUERTO", buffer, CONFIRMAR_PEDIDO);
			//PY_LOG_INFO("Se ha enviado un pedido de confirmacion de pedido");
			log_info(logger,"Se envió el mensaje CONFIRMAR_PEDIDO, id de pedido: %d",id_pedido);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_restaurante, paquete);
			//PY_LOG_INFO("Ha llegado la confirmacion de pedido");
			int32_t okfail;
			okfail = deserializar_int(paquete->buffer);
			log_info(logger,"Se recibió la respuesta del mensaje CONFIRMAR_PEDIDO, respuesta: %d",okfail);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			close(sfd_restaurante);
	}
    else if(!strcmp(array[0],"CONSULTAR_PEDIDO")){
		msg_consultar_pedido* mensaje;
		mensaje = parsearConsultarPedido(array[1]);
		buffer_t* buffer = serializar_consultar_pedido(mensaje->id_pedido);
		sfd sfd_restaurante = conectar_y_enviar("IP", "PUERTO", buffer, CONSULTAR_PEDIDO);
		log_info(logger, "Se envió el mensaje CONSULTAR_PEDIDO, id de pedido %d",mensaje->id_pedido);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_restaurante, paquete);
		msg_consultar_pedido_rta*pedido;
		pedido = deserializar_consultar_pedido_rta(paquete->buffer);
		log_info(logger,"Se recibió la respuesta del mensaje CONSULTAR_PEDIDO,nombre del restaurante: %s y estado del pedido: %d",pedido->nombre_restaurante->nombre,pedido->pedido.estadoPedido);
		/*
		printf("Nombre del restaurante del pedido: %s\n",pedido->nombre_restaurante->nombre);
		for(int i=0;i<pedido->pedido.platos->elements_count;i++){
			registroComida* registro;
			registro = list_get(pedido->pedido.platos, i);
			printf("Nombre plato: %s \n",registro->plato->nombre);
			printf("Cantidad total: %i \n",registro->cantTotal);
			printf("Cantidad lista: %i \n",registro->cantLista);
		}*/
		free(mensaje);
		free(pedido->nombre_restaurante->nombre);
		free(pedido->nombre_restaurante);
		free(pedido);
		free(buffer->stream);
		free(buffer);
		free(paquete);
		free(pedido);
		close(sfd_restaurante);
    }
}
void leerMensaje(char**array,enum_modulo modulo){
	switch(modulo){
	case APP:
		mensajesAPP(array);
		break;
	case COMANDA:
		mensajesComanda(array);
		break;
	case SINDICATO:
		mensajesSindicato(array);
		break;
	case RESTAURANTE:
		mensajesRestaurante(array);
		break;
	}
}

int revisarApp(char**array){
	if (!strcmp(array[0],"CONSULTAR_RESTAURANTE")){
			return 0;
	}
	else if(!strcmp(array[0],"SELECCIONAR_RESTAURANTE")){
		return 0;
	}
	else if(!strcmp(array[0],"CONSULTAR_PLATOS")){
		return 0;
	}
	else if(!strcmp(array[0],"CREAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"AÑADIR_PLATO")){
		return 0;
	}
	else if(!strcmp(array[0],"PLATO_LISTO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONSULTAR_PEDIDO")){
		return 0;
	}
	else{
		return 1;
	}
}
int revisarRestaurante(char**array){
	if (!strcmp(array[0],"CONSULTAR_PLATOS")){
			return 0;
	}
	else if(!strcmp(array[0],"CREAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"AÑADIR_PLATO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONSULTAR_PEDIDO")){
		return 0;
	}
	else{
		return 1;
	}
}
int revisarComanda(char**array){
	if (!strcmp(array[0],"GUARDAR_PEDIDO")){
			return 0;
	}
	else if(!strcmp(array[0],"GUARDAR_PLATO")){
		return 0;
	}
	else if(!strcmp(array[0],"OBTENER_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"PLATO_LISTO")){
		return 0;
	}
	else if(!strcmp(array[0],"FINALIZAR_PEDIDO")){
		return 0;
	}
	else{
		return 1;
	}
}
int revisarSindicato(char**array){
	if (!strcmp(array[0],"CONSULTAR_PLATOS")){
			return 0;
	}
	else if(!strcmp(array[0],"GUARDAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"GUARDAR_PLATO")){
		return 0;
	}
	else if(!strcmp(array[0],"CONFIRMAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"OBTENER_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"OBTENER_RESTAURANTE")){
		return 0;
	}
	else if(!strcmp(array[0],"PLATO_LISTO")){
		return 0;
	}
	else if(!strcmp(array[0],"TERMINAR_PEDIDO")){
		return 0;
	}
	else if(!strcmp(array[0],"OBTENER_RECETA")){
		return 0;
	}
	else{
		return 1;
	}
}

int32_t cantidad_elementos_string(char* string){
	int32_t cantidad = 1;

	for(int i = 0; i < strlen(string)+1; i++){
		if(string[i] == ' '){
			cantidad++;
		}
	}
	return cantidad;
}

void consola(enum_modulo modulo){
	  char * linea;
	  int revision;
	  char** array;
	  while(1){
        linea=readline("");
        int32_t cant_lineas = cantidad_elementos_string(linea);
        array = string_split(linea, " ");

        if (modulo==APP){
        	revision=revisarApp(array);
        }
        else if (modulo==RESTAURANTE){
        	revision=revisarRestaurante(array);
        }
        else if(modulo==SINDICATO){
        	revision=revisarSindicato(array);
        }
        else if(modulo==COMANDA){
        	revision=revisarComanda(array);
        }

        if(revision==0){
        	leerMensaje(array,modulo);
        	free(array[0]);
        	//printf("mensaje recibido. \n");
        }
        else{
        	printf("No se paso un mensaje correcto. \n");
            for(int i = 0 ; i < cant_lineas; i++){
            	free(array[i]);
            }
        }

       	free(array);
       	free(linea);
	  }

}

void escuchar(sfd socket){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket, paquete);
	atender(socket, paquete->buffer, paquete->codigo_operacion);
	free(paquete);
}

void conexiones(enum_modulo modulo){
	pthread_t escuchar_consola;
	pthread_create(&escuchar_consola, NULL, (void*)consola,(void*)modulo);
	sfd sfd_servidor = server_create("PUERTO_ESCUCHA");
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);

	while(1){
			sfd sfd_cliente= accept(sfd_servidor,(void*) &direccionCliente, &tamanioDireccion);
			if(sfd_cliente< 0){
			       // PY_LOG_TRACE("Error");
				log_info(logger,"Error");
			}
			pthread_create(&conexion_escucha, NULL, (void*)escuchar, (void*)sfd_cliente);

    }
}

handshake_cliente* inicializarHandshake(){
	handshake_cliente* handshake = malloc(sizeof(handshake_cliente));
	handshake->puerto=py_config_get_string("PUERTO_ESCUCHA");
	handshake->id=py_config_get_int("ID_CLIENTE");
	handshake->coordenadas.posicionX=py_config_get_int("POSICION_X");
	handshake->coordenadas.posicionY=py_config_get_int("POSICION_Y");
    id_cliente=handshake->id;
	return handshake;
}

int main() {
		//iniciar_programa("cliente.config", "cliente.log", "cliente");                //COMENTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
		py_config_init("cliente.config");
		char* pathDelLogger = py_config_get_string("ARCHIVO_LOG");
		logger = log_create(pathDelLogger,"ClienteLog" ,1, LOG_LEVEL_INFO);
		//logger = py_get_logger();                                                         //COMENTAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
		if(logger == NULL){
			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_error(logger,"Path del logger inexistente. Intente de nuevo.");
		}
		handshake_cliente* handshake;
		handshake=inicializarHandshake();
		buffer_t*buffer=malloc(sizeof(buffer_t));
		buffer->size=0;
		enum_modulo modulo;
		sfd socket=conectar_y_enviar("IP","PUERTO",buffer,CONSULTAR_MODULO);
		free(buffer);
		py_paquete_t*paquete=malloc(sizeof(py_paquete_t));
		py_recv_msg(socket,paquete);
		modulo=deserealizar_modulo(paquete->buffer);

		logger->is_active_console = false;
		log_info(logger,"Me conecté al módulo: %d [APP=0;RESTAURANTE=1;SINDICATO=2;COMANDA=3]",modulo);


	if(modulo==APP){
		printf("%i \n",handshake->id);
		buffer_t* buffer_handshake = serializar_handshake_cliente(handshake);
		sfd sfd_APP = conectar_y_enviar("IP","PUERTO",buffer_handshake, HANDSHAKE_INICIAL_CLIENTE);
		free(buffer_handshake->stream);
		free(buffer_handshake);
		close(sfd_APP);
		conexiones(APP);
	}
	else {
		conexiones(modulo);
	}

	//py_delete_logger();
	//py_config_delete();
}

