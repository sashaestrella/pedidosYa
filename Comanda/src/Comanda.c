#include "Comanda.h"

int main(void) {
	py_config_init("comanda.config");
	char* pathDelLogger = py_config_get_string("ARCHIVO_LOG");
	//printf("ARCHIVO_LOG = %s\n",pathDelLogger);
	//py_logger_init(pathDelLogger, "comanda");
	logger = log_create(pathDelLogger, "ComandaLog" , 1, LOG_LEVEL_INFO);
	logger->detail = LOG_LEVEL_TRACE;
	if(logger == NULL){
		//logger = py_get_logger();
		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_error(logger,"Path del logger inexistente. Intente de nuevo.");
	}

	pthread_mutex_init(&cantTotalDePlatos,NULL);
	pthread_mutex_init(&cantListaDePlatos,NULL);
	pthread_mutex_init(&estadoPedido,NULL);
	pthread_mutex_init(&contPaginaCantTotal,NULL);
	pthread_mutex_init(&contPaginaCantLista,NULL);
	pthread_mutex_init(&timestamp,NULL);
	pthread_mutex_init(&actualizarBitModificado,NULL);
	pthread_mutex_init(&mutexMemoria,NULL);
	pthread_mutex_init(&agregarPedidoComanda,NULL);
	pthread_mutex_init(&agregarPedidoSwap,NULL);
	pthread_mutex_init(&mutexTablasDeSegmentos,NULL);
	pthread_mutex_init(&mutexSegmentos,NULL);
	pthread_mutex_init(&modifPagina,NULL);
	pthread_mutex_init(&swapPagina,NULL);
	pthread_mutex_init(&leerPagina,NULL);

	tablasDeSegmentos = list_create();
	pedidosComanda = list_create();

	tamanioMemoria = atoi(py_config_get_string("TAMANIO_MEMORIA"));
	memoria = malloc(tamanioMemoria);
	tamanioPagina = sizeof(uint32_t)*2 + 24;
	printf("El tamanio de cada pagina es de %d bytes\n",tamanioPagina);

	cantDePagSegunTamMemoria = tamanioMemoria / tamanioPagina;
	printf("La cantidad de paginas que entran en memoria principal son: %d\n",cantDePagSegunTamMemoria);

	algoritmoReemplazo = py_config_get_string("ALGORITMO_REEMPLAZO");
	//levanto a los frames como libres
	//en cada frame se guarda una pagina
	estadosFrames = malloc(sizeof(EstadoFrame) * cantDePagSegunTamMemoria);
	for(int i=0;i<cantDePagSegunTamMemoria;i++){
		estadosFrames[i] = LIBRE;
	}

	inicializarSwap();
	conexiones();


	//esta bien que lo ponga acá?
	//munmap(mapeo,cantPaginasEnSwap*tamanioPagina);

	return 0;
}

void escuchar(sfd socket_restaurante){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_restaurante, paquete);
	atender(socket_restaurante, paquete, paquete->codigo_operacion);
}

void conexiones(){
	sfd sfd_APP = server_create("PUERTO_ESCUCHA");
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);
	sfd sfd_cliente;
	while((sfd_cliente = accept(sfd_APP,(void*) &direccionCliente, &tamanioDireccion)) > 0){
		pthread_t hiloEscuchar;



		pthread_create(&hiloEscuchar,NULL,(void*)escuchar,(void*)sfd_cliente);
		pthread_detach(hiloEscuchar);
	}
}

void atender(sfd socket_restaurante, py_paquete_t* paquete, enum_mensaje codigo_operacion){
	int32_t respuesta;
	msg_guardar_pedido* guardarPedido;
	msg_guardar_plato* guardarPlato;
	msg_obtener_pedido* obtenerPedido;
	msg_confirmar_pedido* confirmarPedido;
	msg_plato_listo* platoListo;
	msg_finalizar_pedido* finalizarPedido;

	TablaDeSegmentos* tabla;
	TablaDeSegmentos* nuevaTabla;
	Segmento* segmento;

	buffer_t* buffer;
	void* msg;

	switch(codigo_operacion){
		case 0:
			//logger = py_get_logger();
			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_info(logger,"Se conectó la APP");
			break;
		case GUARDAR_PEDIDO:
			guardarPedido = deserializar_guardar_pedido(paquete->buffer);
			tabla = buscarTablaDeSegmentos(guardarPedido->restaurante);
			if(tabla){
				respuesta = administrarGuardarPedido(tabla,guardarPedido->id_pedido);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			} else if(todosLosFramesEstanOcupados() == 0){
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"No se puede crear la tabla de segmentos del restaurante -%s- porque no hay espacio disponible en memoria",guardarPedido->restaurante->nombre);
				respuesta = 1;
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			} else {
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_info(logger,"La tabla de segmentos del restaurante -%s- no existe,procedo a crearla",guardarPedido->restaurante->nombre);
				nuevaTabla = crearTablaDeSegmentos(guardarPedido->restaurante);
				respuesta = administrarGuardarPedido(nuevaTabla,guardarPedido->id_pedido);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}
			free(guardarPedido->restaurante->nombre);
			free(guardarPedido->restaurante);
			free(guardarPedido);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		case GUARDAR_PLATO:
			guardarPlato = deserializar_guardar_plato(paquete->buffer);
			tabla = buscarTablaDeSegmentos(guardarPlato->restaurante);
			if(tabla){
				respuesta = administrarGuardarPlato(tabla,guardarPlato->id_pedido,guardarPlato->plato,guardarPlato->cantidad_platos);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}else{
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"La tabla de segmentos del restaurante -%s- no existe",guardarPlato->restaurante->nombre);
				respuesta = 1;
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}
			free(guardarPlato->plato->nombre);
			free(guardarPlato->restaurante->nombre);
			free(guardarPlato->restaurante);
			free(guardarPlato->plato);
			free(guardarPlato);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		case OBTENER_PEDIDO:
			obtenerPedido = deserializar_obtener_pedido(paquete->buffer);
			tabla = buscarTablaDeSegmentos(obtenerPedido->restaurante);
			if(tabla){
				segmento = buscarSegmento(obtenerPedido->id_pedido,tabla);
				if(segmento){
					cargarEnFramesLosPlatosDelPedido(tabla,segmento);
					Pedido* pedidoAEnviar = armarPedidoAEnviar(tabla->nombreRestaurante,segmento->idPedido);
					buffer = serializar_pedido(pedidoAEnviar->estadoPedido,pedidoAEnviar->platos);
					enviar_respuesta(socket_restaurante,buffer,OBTENER_PEDIDO_RTA);
					for(int i=0;i<pedidoAEnviar->platos->elements_count;i++){
						registroComida* registro = list_get(pedidoAEnviar->platos,i);
						free(registro->plato->nombre);
						free(registro->plato);
						free(registro);
					}
					list_destroy(pedidoAEnviar->platos);
					free(pedidoAEnviar);
				}else{
					//logger = py_get_logger();
					logger->is_active_console = false;
					log_error(logger,"El segmento del restaurante -%s- con id %d no existe",obtenerPedido->restaurante->nombre,obtenerPedido->id_pedido);
					//enviar pedido vacio
					t_list* lista_vacia = list_create();
					buffer = serializar_pedido(SINPEDIDO,lista_vacia);
					enviar_respuesta(socket_restaurante,buffer,OBTENER_PEDIDO_RTA);
					list_destroy(lista_vacia);
				}
			}else{
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"La tabla de segmentos del restaurante -%s- no existe",obtenerPedido->restaurante->nombre);
				//enviar pedido vacio
				t_list* lista_vacia = list_create();
				buffer = serializar_pedido(SINPEDIDO,lista_vacia);
				enviar_respuesta(socket_restaurante,buffer,OBTENER_PEDIDO_RTA);
				list_destroy(lista_vacia);
			}
			free(obtenerPedido->restaurante->nombre);
			free(obtenerPedido->restaurante);
			free(obtenerPedido);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		case CONFIRMAR_PEDIDO:
			confirmarPedido = deserializar_confirmar_pedido(paquete->buffer);
			tabla = buscarTablaDeSegmentos(confirmarPedido->nombre_restaurante);
			if(tabla){
				respuesta = administrarConfirmarPedido(tabla,confirmarPedido->id_pedido);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}else{
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"La tabla de segmentos del restaurante -%s- no existe",confirmarPedido->nombre_restaurante->nombre);
				respuesta = 1;
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}
			free(confirmarPedido->nombre_restaurante->nombre);
			free(confirmarPedido->nombre_restaurante);
			free(confirmarPedido);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		case PLATO_LISTO:
			platoListo = deserializar_plato_listo(paquete->buffer);
			tabla = buscarTablaDeSegmentos(platoListo->restaurante);
			if(tabla){
				respuesta = administrarPlatoListo(tabla,platoListo->id_pedido,platoListo->plato);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}else{
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"La tabla de segmentos del restaurante -%s- no existe",platoListo->restaurante->nombre);
				respuesta = 1;
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}
			free(platoListo->restaurante->nombre);
			free(platoListo->restaurante);
			free(platoListo->plato->nombre);
			free(platoListo->plato);
			free(platoListo);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		case FINALIZAR_PEDIDO:
			finalizarPedido = deserializar_finalizar_pedido(paquete->buffer);
			tabla = buscarTablaDeSegmentos(finalizarPedido->restaurante);
			if(tabla){
				respuesta = administrarFinalizarPedido(tabla,finalizarPedido->id_pedido);
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}else{
				//logger = py_get_logger();
				logger->is_active_console = false;
				log_error(logger,"La tabla de segmentos del restaurante -%s- no existe",finalizarPedido->restaurante->nombre);
				respuesta = 1;
				buffer = serializar_int(respuesta);
				enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
			}
			free(finalizarPedido->restaurante->nombre);
			free(finalizarPedido->restaurante);
			free(finalizarPedido);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
			/*
		case HANDSHAKE_INICIAL_CLIENTE:
			handshake = deserializar_handshake_cliente(paquete->buffer);
			logger = py_get_logger();
			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_debug(logger,"Recibí handshake de un cliente, su id es: %d",handshake->id);
			buffer_t* buffer_respuesta = serializar_modulo(COMANDA);
			enviar_respuesta(socket_restaurante, buffer_respuesta, CONSULTAR_MODULO);
			free(handshake->direccion);
			free(handshake);
			break;*/
		case CONSULTAR_MODULO:
			msg = malloc(sizeof(void*));
			buffer = serializar_modulo(COMANDA);
			enviar_respuesta(socket_restaurante, buffer,CONSULTAR_MODULO);
			free(msg);
			free(buffer->stream);
			free(buffer);
			free(paquete);
			break;
		default:
				perror("El mensaje no se puede leer");
	}
	close(socket_restaurante);
}
