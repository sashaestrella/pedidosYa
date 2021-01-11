#include "conexiones.h"
#define elementosCrearRestaurante 8
#define elementosCrearReceta 4


void crearArchivo(char**array){
	if(!strcmp(array[0],"crearRestaurante")){
		crearRestaurante(array);
	}
	else if(!strcmp(array[0],"crearReceta")){
		crearReceta(array);
	}
}
void consola(){
	  char * linea;
	  int revision;
	  char** array;
	  while(1){
        linea=readline("");

        array = string_split(linea, " ");
        revision=revisar(array);
        if(revision==0){
        	crearArchivo(array);
        	PY_LOG_INFO("se agrego perfectamente. \n");
        }
        free(linea);
       	liberar_lista(array);
	    }

}
void liberar_lista(char** lista){
int contador = 0;
while(lista[contador] != NULL){
        free(lista[contador]);
        contador++;
}

free(lista);
}
int revisar(char**array){
	int elementos;
	elementos=cantidadElementos(array);
	if (!strcmp(array[0],"crearRestaurante")){
		if(elementos!=elementosCrearRestaurante){
			PY_LOG_INFO("no posee los parametros necesarios, se esperan %d \n",elementosCrearRestaurante);
			return 1;
		}
		else{
			return 0;
		}

	}
	else {
		if(!strcmp(array[0],"crearReceta")) {
		if(elementos!=elementosCrearReceta){
			PY_LOG_INFO("no posee los parametros necesarios, se esperan %d \n",elementosCrearReceta);
			return 1;
		}
		else{
			return 0;
		}

	}
	else{
		PY_LOG_INFO("no se ingreso ni crearReceta ni crearRestaurante \n");
		return 1;
	}
}
}
int cantidadElementos(char**array){
	int i=0;
	while(array[i]!=NULL){
		i++;
	}
	return i;
}


void conexiones(){
	pthread_t escuchar_consola;
	pthread_create(&escuchar_consola, NULL, (void*)consola,NULL);
	sfd sfd_sindicato = server_create("PUERTO_ESCUCHA");
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);
	while(1){
		int sfd_restaurante = accept(sfd_sindicato,(void*) &direccionCliente, &tamanioDireccion);
		if(sfd_restaurante < 0){
	        PY_LOG_TRACE("Error al escuchar el restaurante");
		}
		pthread_t* escuchar_restaurante;
		pthread_create(&escuchar_restaurante, NULL, (void*)escuchar, (void*)sfd_restaurante);
	}
}

void escuchar(sfd socket_restaurante){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_enum status = py_recv_msg(socket_restaurante, paquete);
	if(!status){
		atender(socket_restaurante ,paquete->buffer, paquete->codigo_operacion);
		PY_LOG_TRACE("Llegó un mensaje");
	}
	free(paquete);
}


void atender(sfd socket_restaurante, buffer_t* buffer, enum_mensaje codigo_operacion){
	void* msg;
	int32_t respuesta = 0;
	Pedido* pedido;
	Restaurante* restaurante;
	buffer_t* buffer2;

	switch(codigo_operacion){
	case CONSULTAR_PLATOS:
		msg = (void*)deserializar_consultar_platos(buffer);
		listas=consultarPlatos(msg);
		buffer2=serializar_lista_nombres(listas);
		py_send(socket_restaurante,CONSULTAR_PLATOS_RTA,buffer2);
		liberarPlatosSin(listas);
		free(buffer2->stream);
		free(buffer2);

		break;
	case GUARDAR_PEDIDO:
		msg = (void*)deserializar_guardar_pedido(buffer);
		respuesta=crearPedido(msg);
		buffer2=serializar_int(respuesta);
		py_send(socket_restaurante, OK_FAIL, buffer2);
		free(buffer2->stream);
		free(buffer2);
		break;
	case GUARDAR_PLATO:
		msg = (void*)deserializar_guardar_plato(buffer);
		respuesta=guardarPlato(msg);
		buffer2=serializar_int(respuesta);
		py_send(socket_restaurante, OK_FAIL, buffer2);
		free(buffer2->stream);
		free(buffer2);
		break;
	case CONFIRMAR_PEDIDO:
		msg = (void*)deserializar_confirmar_pedido(buffer);
		respuesta=confirmarPedido(msg);
		py_send(socket_restaurante, OK_FAIL, serializar_int(respuesta));
		break;
	case OBTENER_RESTAURANTE:
		msg = (void*)deserializar_obtener_restaurante(buffer);
		restaurante=obtenerRestaurante(msg);
		buffer2=serializar_restaurante(restaurante);
		py_send(socket_restaurante, OBTENER_RESTAURANTE_RTA,buffer2);
		free(restaurante->nombre_restaurante->nombre);
		free(restaurante->nombre_restaurante);
		liberarListasRestauranteSin(restaurante->info_Restaurante.Platos,restaurante->info_Restaurante.afinidades);
		free(restaurante);
		free(buffer2->stream);
		free(buffer2);

		break;
	case OBTENER_PEDIDO:
		msg = (void*)deserializar_obtener_pedido(buffer);
		pedido=obtenerPedido(msg);
		buffer2=serializar_pedido(pedido->estadoPedido, pedido->platos);
		py_send(socket_restaurante,CONSULTAR_PLATOS_RTA,buffer2);
		liberarPlatosSin(pedido->platos);
		free(pedido);
		free(buffer2->stream);
		free(buffer2);
		break;
	case PLATO_LISTO:
		msg = (void*)deserializar_plato_listo(buffer);
		respuesta=platoListo(msg);
		py_send(socket_restaurante, OK_FAIL, serializar_int(respuesta));
		break;
	case OBTENER_RECETA:
		msg = (void*)deserializar_obtener_receta(buffer);
		listas=consultarRecetas(msg);
		buffer2=serializar_receta(listas);
		py_send(socket_restaurante, OBTENER_RECETA_RTA, buffer2);
		liberarPasosSin(listas);
		free(buffer2->stream);
		free(buffer2);

		break;
	case TERMINAR_PEDIDO:
		msg = (void*)deserializar_terminar_pedido(buffer);
		respuesta=terminarPedido(msg);
		py_send(socket_restaurante, OK_FAIL, serializar_int(respuesta));
		break;
	default:
		perror("El mensaje no se puede leer");
	}


	close(socket_restaurante);
}

void liberarPasosSin(t_list*pasos){
	for(int i=0;i<pasos->elements_count;i++){
	Paso*paso=list_get(pasos,i);
				free(paso->nombre->nombre);
				free(paso->nombre);
				free(paso);
			}
			list_destroy(pasos);

}
void liberarPlatosSin(t_list*platos){
	for(int i=0;i<platos->elements_count;i++){
			Plato*plato=list_get(platos,i);
			free(plato->nombre->nombre);
			free(plato->nombre);
			free(plato);
		}
		list_destroy(platos);
}
void liberarListasRestauranteSin(t_list* platos,t_list*afinidades){
	for(int i=0;i<platos->elements_count;i++){
		Plato*plato=list_get(platos,i);
		free(plato->nombre->nombre);
		free(plato->nombre);
		free(plato);
	}
	list_destroy(platos);
	for(int i=0;i<afinidades->elements_count;i++){
		nombre*afinidad=list_get(afinidades,i);
		free(afinidad->nombre);
		free(afinidad);
	}
	list_destroy(afinidades);
}
