#include "APP.h"


int terminado = 0;
pthread_t* conexiones_restaurantes;
int32_t id_pedido_default = 1000000;
pthread_t planificacion;

int main(void) {
	iniciar_programa("APP.config", "APP.log", "APP");
	leer_config_file();
	inicializar_repartidores();
	inicializar_listas();
	pthread_create(&planificacion, NULL, (void*)planificar_base, NULL);
	conexiones();
	return SUCCESS;
}

void inicializar_semaforos(){
	pthread_mutex_init(&mutex_lista_handshake_cliente, NULL);
	pthread_mutex_init(&mutex_lista_handshake_restaurante, NULL);
	pthread_mutex_init(&mutex_lista_restaurantes, NULL);
	pthread_mutex_init(&mutex_lista_restaurante_clientes, NULL);
	pthread_mutex_init(&sem_cantidad_ready, NULL);
	pthread_mutex_init(&mutex_PCBs_temporales, NULL);
	sem_init(&sem_repartidores_disponibles,0,0);
	sem_repartidores = calloc(cantidad_repartidores, sizeof(pthread_mutex_t));
	for(int i = 0; i < cantidad_repartidores; i++){
		pthread_mutex_init(&sem_repartidores[i], NULL);
	}
	sem_clock_entrada = calloc(grado_multiprocesamiento, sizeof(sem_t));
	sem_clock_salida = calloc(grado_multiprocesamiento, sizeof(sem_t));
	for(int i = 0; i < grado_multiprocesamiento; i++){
		sem_init(&sem_clock_entrada[i], 0, 0);
		sem_init(&sem_clock_salida[i], 0, 1);
	}
	pthread_mutex_init(&sem_elegir_repartidor, NULL);
}

void leer_config_file(){
    retardo_ciclo = py_config_get_int("RETARDO_CICLO_CPU");
    grado_multiprocesamiento = py_config_get_int("GRADO_DE_MULTIPROCESAMIENTO");
    //lista platos
    coordenadas_restaurante_default.posicionX = py_config_get_int("POSICION_REST_DEFAULT_X");
	coordenadas_restaurante_default.posicionY = py_config_get_int("POSICION_REST_DEFAULT_Y");
}

void inicializar_cola_ready(){
	colaRepartidoresReady = queue_create();
	pthread_mutex_init(&mutex_cola_repartidores, NULL);
	sem_init(&sem_repartidores_ready,0,0);
}

void inicializar_cola_PCB(){
	colaPedidos = queue_create();
	pthread_mutex_init(&mutex_cola_PCB, NULL);
	sem_init(&sem_hay_PCBs,0,0);
}

void inicializar_repartidores(){
	char* coordes_repartidores = py_config_get_string("REPARTIDORES");
	char** coordenadas_repartidores = py_config_get_array_value("REPARTIDORES");
	char** frecuencias_descanso = py_config_get_array_value("FRECUENCIA_DE_DESCANSO");
	char** tiempo_descanso = py_config_get_array_value("TIEMPO_DE_DESCANSO");

	cantidad_repartidores = cantidad_elementos_string(coordes_repartidores);
	lista_repartidores = list_create();

	inicializar_semaforos();
	inicializar_cola_PCB();

	pedido_listo = calloc(cantidad_repartidores, sizeof(bool));

	for(int i = 0; i < cantidad_repartidores; i++){
		repartidor* un_repartidor = malloc(sizeof(repartidor));
		char** coordes_x_y = string_split(coordenadas_repartidores[i], "|");
		un_repartidor->ubicacion.posicionX = atoi(coordes_x_y[0]);
		un_repartidor->ubicacion.posicionY = atoi(coordes_x_y[1]);
		un_repartidor->destino.posicionX = 10;
		un_repartidor->destino.posicionY = 10;
		un_repartidor->frecuencia_descanso = atoi(frecuencias_descanso[i]);
		un_repartidor->tiempo_descanso = atoi(tiempo_descanso[i]);
		un_repartidor->cansancio_actual = 0;
		un_repartidor->descanso_actual = 0;
		un_repartidor->estado = NEW;
		PY_LOG_INFO("El repartidor %i pasa a la cola NEW porque fue creado", i);
		un_repartidor->tipo_destino = RESTAURANTE_DESTINO;
		//printf("Repartidor %i: \nPosicion: (%i,%i)\n Frecuencia descanso: %i\nTiempo descanso: %i \n\n", i, un_repartidor->ubicacion.posicionX, un_repartidor->ubicacion.posicionY,un_repartidor->frecuencia_descanso, un_repartidor->tiempo_descanso);
		list_add(lista_repartidores, un_repartidor);
		pedido_listo[i] = false;
		sem_post(&sem_repartidores_disponibles);
		free(coordes_x_y[0]);
		free(coordes_x_y[1]);
		free(coordes_x_y);
	}
	for(int i = 0; i < cantidad_repartidores; i++){
		free(frecuencias_descanso[i]);
		free(coordenadas_repartidores[i]);
		free(tiempo_descanso[i]);
	}
	free(frecuencias_descanso);
	free(coordenadas_repartidores);
	free(tiempo_descanso);
	restaurantes_conectados = 0;
}

void mostrar_restaurantes(){
	for(int i = 0; i < lista_restaurantes->elements_count; i++){
		printf("%s \n", ((nombre*)list_get(lista_restaurantes, i))->nombre);
	}
}

void inicializar_listas(){
	lista_handshake_cliente = list_create();
	lista_restaurantes = list_create();
	lista_restaurante_clientes = list_create();
	crear_lista_default();
	PCBs_temporales = list_create();
}

void escuchar(socket_ip* si){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(si->socket, paquete);
	atender(si->socket, paquete->buffer, paquete->codigo_operacion, si->ip);
	free(paquete);
}

void conexiones(){
	sfd sfd_comanda = client_create("IP_COMANDA","PUERTO_COMANDA");
	if(sfd_comanda > 0){
		close(sfd_comanda);
	}
	else{
		exit(5);
	}
	sfd sfd_APP = server_create("PUERTO_ESCUCHA");
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);
	while(!terminado){
		sfd sfd_cliente = accept(sfd_APP,(void*) &direccionCliente, &tamanioDireccion);
		struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&direccionCliente;
		struct in_addr ipAddr = pV4Addr->sin_addr;
		char* str = malloc(INET_ADDRSTRLEN);
		inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN );
		//escuchar(sfd_cliente, str);
		socket_ip* si = malloc(sizeof(socket_ip));
		si->ip = str;
		si->socket = sfd_cliente;
		pthread_create(&conexion_escucha, NULL, (void*)escuchar, (void*)si);
	}
}

void crear_lista_default(){
	nombre_restaurante_default = malloc(sizeof(nombre));
	nombre_restaurante_default->largo_nombre = strlen("Restaurante_Default") + 1;
    //nombre_restaurante_default->nombre = malloc(nombre_restaurante_default->largo_nombre);
	nombre_restaurante_default->nombre = "RestauranteDefault";
	list_add(lista_restaurantes, nombre_restaurante_default);

	lista_handshake_restaurante = list_create();
	handshake_restaurante* handshake = malloc(sizeof(handshake_restaurante));
	handshake->nombre_restaurante = nombre_restaurante_default;
	handshake->coordenadas = coordenadas_restaurante_default;
    list_add(lista_handshake_restaurante, handshake);

    char* platos_default = py_config_get_string("PLATOS_DEFAULT");
    char** platoss_default = py_config_get_array_value("PLATOS_DEFAULT");
    int32_t cantidad_platos_default = cantidad_elementos_string(platos_default);
    lista_platos_default = list_create();
    for(int i = 0; i < cantidad_platos_default; i++){
	        nombre* un_plato = malloc(sizeof(nombre));
	        un_plato->nombre = platoss_default[i];
	        un_plato->largo_nombre = strlen(platoss_default[i]) + 1;
	        list_add(lista_platos_default, un_plato);
	}
	free(platoss_default);

}

void agregar_lista_handshake_cliente(handshake_cliente_completo* handshake){
	pthread_mutex_lock(&mutex_lista_handshake_cliente);
	list_add(lista_handshake_cliente, handshake);
	pthread_mutex_unlock(&mutex_lista_handshake_cliente);
}

void agregar_lista_handshake_restaurante(handshake_restaurante_completo* handshake){
	pthread_mutex_lock(&mutex_lista_handshake_restaurante);
	list_add(lista_handshake_restaurante, handshake);
	pthread_mutex_unlock(&mutex_lista_handshake_restaurante);
}

void agregar_lista_restaurantes(nombre* nombre_restaurante){
	pthread_mutex_lock(&mutex_lista_restaurantes);
	list_add(lista_restaurantes, nombre_restaurante);
	pthread_mutex_unlock(&mutex_lista_restaurantes);
}

void eliminar_restaurante_default(){
	pthread_mutex_lock(&mutex_lista_restaurantes);
	list_remove(lista_restaurantes,0);
	pthread_mutex_unlock(&mutex_lista_restaurantes);
}

void agregar_lista_restaurante_clientes(cliente_restaurante* elemento){
	pthread_mutex_lock(&mutex_lista_restaurante_clientes);
	list_add(lista_restaurante_clientes, elemento);
	pthread_mutex_unlock(&mutex_lista_restaurante_clientes);
}

void atender_consultar_restaurante(int socket_restaurante){
	buffer_t* buffer = serializar_lista_nombres_real(lista_restaurantes);
	enviar_respuesta(socket_restaurante, buffer, CONSULTAR_RESTAURANTE_RTA);
	free(buffer->stream);
	free(buffer);
	close(socket_restaurante);
}

void atender_seleccionar_restaurante(int socket_restaurante, nombre* nombre_restaurante, int32_t id_cliente){
	int32_t respuesta;
	if(!buscar_restaurante_por_nombre(nombre_restaurante->nombre)){
		respuesta = 1;
		free(nombre_restaurante);
	}
	else{
		if(strcmp(nombre_restaurante->nombre, "Restaurante_Default")){
			cliente_restaurante* nuevo = malloc(sizeof(cliente_restaurante));
			nuevo->restaurante = nombre_restaurante;
			nuevo->id_cliente = id_cliente;
			list_add(lista_restaurante_clientes, nuevo);
			respuesta = 0;
		}
	}
	buffer_t* buffer = serializar_int(respuesta);
	enviar_respuesta(socket_restaurante,buffer, OK_FAIL);
	free(buffer->stream);
	free(buffer);
}

buffer_t* crear_buffer_vacio() {
	buffer_t *buffer = malloc(sizeof(buffer_t));
	buffer->size = 0;
	buffer->stream = NULL;
	return buffer;
}

void atender_consultar_platos(int32_t id_cliente, int32_t socket_cliente){         //Cliente debe abrir socket de escucha
	cliente_restaurante* relacion = buscar_nombre_id_por_id(id_cliente);
	if(relacion){
		bool es_default = !strcmp(relacion->restaurante->nombre, "RestauranteDefault");
		if(es_default){
			buffer_t* buffer;
			buffer = serializar_lista_nombres_real(lista_platos_default);
			enviar_respuesta(socket_cliente, buffer, CONSULTAR_PLATOS_RTA);
			free(buffer->stream);
			free(buffer);
		}
		else{
			handshake_restaurante_completo* handshake_resto = buscar_handshake_restaurante(relacion->restaurante);
			buffer_t* buffer = crear_buffer_vacio();
			sfd socket_resto = conectar_y_enviar_manual(handshake_resto->direccion->ip, handshake_resto->direccion->puerto, buffer, CONSULTAR_PLATOS);
			free(buffer);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_enum estado = py_recv_msg(socket_resto, paquete);
			if(!estado){
				enviar_respuesta(socket_cliente, paquete->buffer, paquete->codigo_operacion);
			}
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);
		}
	}
	else{
		buffer_t* buffer_vacio = crear_buffer_vacio();
		enviar_respuesta(socket_cliente, buffer_vacio, CONSULTAR_PLATOS_RTA);
		free(buffer_vacio);
	}
}

void atender_crear_pedido(int32_t id_cliente, int32_t socket_cliente){
	cliente_restaurante* relacion = buscar_nombre_id_por_id(id_cliente);
	handshake_cliente_completo* handshake_cli = buscar_handshake_cliente(id_cliente);
	buffer_t* buffer_respuesta;
	int32_t id_pedido;
	if(relacion){
		bool es_default = !strcmp(relacion->restaurante->nombre, "RestauranteDefault");
		if(es_default){
			id_pedido = generar_id_pedido_default();
			buffer_respuesta = serializar_int(id_pedido);
		}
		else{
			handshake_restaurante_completo* handshake_resto = buscar_handshake_restaurante(relacion->restaurante);
			buffer_t* buffer_vacio = crear_buffer_vacio();
			sfd sfd_restaurante = conectar_y_enviar_manual(handshake_resto->direccion->ip, handshake_resto->direccion->puerto, buffer_vacio, CREAR_PEDIDO);
			free(buffer_vacio);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_enum estado = py_recv_msg(sfd_restaurante, paquete);
			close(sfd_restaurante);
			id_pedido = deserializar_int(paquete->buffer);
			free(paquete);
			buffer_respuesta = serializar_int(id_pedido);
			if(estado){
				perror("Restaurante desconectado");
			}
		}
		buffer_t* buffer_comanda;

		if(es_default){
			buffer_comanda = serializar_guardar_pedido(nombre_restaurante_default, id_pedido);
		}
		else{
			buffer_comanda = serializar_guardar_pedido(relacion->restaurante, id_pedido);
		}
		sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer_comanda,GUARDAR_PEDIDO);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t respuesta = deserializar_int(paquete->buffer);
		if(respuesta == 1){
			buffer_respuesta = serializar_int(-1);
			//PY_LOG_DEBUG("El pedido fue rechazado por la Comanda");
		}
		PCB* un_pcb = malloc(sizeof(PCB));
		un_pcb->id_cliente = id_cliente;
		un_pcb->id_pedido = id_pedido;
		un_pcb->ubicacion_cliente = handshake_cli->coordenadas;
		un_pcb->confirmado = false;
		if(relacion){
			handshake_restaurante_completo* handshake_resto = buscar_handshake_restaurante(relacion->restaurante);
			un_pcb->ubicacion_restaurante = handshake_resto->coordenadas;
			un_pcb->nombre_restaurante = relacion->restaurante;
		}
		else{
			un_pcb->ubicacion_restaurante = coordenadas_restaurante_default;
			un_pcb->nombre_restaurante = nombre_restaurante_default;
		}
		agregar_PCB_temporal(un_pcb);
		free(buffer_comanda->stream);
		free(buffer_comanda);
		free(paquete);
	}
	else{
		buffer_respuesta = serializar_int(-1);
	}
	enviar_respuesta(socket_cliente, buffer_respuesta, CREAR_PEDIDO_RTA);
	free(buffer_respuesta->stream);
	free(buffer_respuesta);
}

bool existe_plato_default(nombre* nombre_plato){
	for(int i = 0; i < lista_platos_default->elements_count; i++){
		nombre* nombre = list_get(lista_platos_default, i);
		if(!strcmp(nombre_plato->nombre, nombre->nombre)){
			return true;
		}
	}
	return false;
}

void atender_aniadir_plato(msg_aniadir_plato* mensaje, int32_t socket_cliente, int32_t id_cliente){
	PCB* un_pcb = buscar_pcb_temporal_por_id_pedido(mensaje->id_pedido, id_cliente);
	int32_t respuesta = 0;
	if(!un_pcb){
		//printf("NO HAY PCB \n");
		respuesta = 1;
	}
	else{
		if(un_pcb->id_cliente != id_cliente){
			//printf("EL PEDIDO NO ES DEL CLIENTE \n");
			respuesta = 1;
		}
		else{
			if(un_pcb && un_pcb->confirmado == false){
				if(strcmp(un_pcb->nombre_restaurante->nombre, "RestauranteDefault")){
					handshake_restaurante_completo* handshake_resto = buscar_handshake_restaurante(un_pcb->nombre_restaurante);
					buffer_t* buffer = serializar_aniadir_plato(mensaje->plato, mensaje->id_pedido);
					sfd sfd_restaurante = conectar_y_enviar_manual(handshake_resto->direccion->ip, handshake_resto->direccion->puerto, buffer, ANIADIR_PLATO);
					free(buffer->stream);
					free(buffer);
					py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
					py_recv_msg(sfd_restaurante, paquete);
					close(sfd_restaurante);
					respuesta = deserializar_int(paquete->buffer);
					//printf("LA RESPUESTA DE RESTAURANTE ES %i \n", respuesta);
					free(paquete);
				}
				else{
					if(!existe_plato_default(mensaje->plato)){
						respuesta = 1;
					}
				}
				if(respuesta == 0){
					buffer_t* buffer = serializar_guardar_plato(un_pcb->nombre_restaurante, mensaje->id_pedido, mensaje->plato, 1);
					sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer,GUARDAR_PLATO);
					py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
					py_recv_msg(sfd_comanda, paquete);
					close(sfd_comanda);
					respuesta = deserializar_int(paquete->buffer);
					//printf("LA RESPUESTA DE COMANDA ES %i \n", respuesta);
					free(buffer->stream);
					free(buffer);
					free(paquete);
				}
			}
			else{
				//printf("FFF \n");
				//PY_LOG_WARNING("El pedido no existe");
				respuesta = 1;
			}
		}
	}

	buffer_t* buffer_respuesta = serializar_int(respuesta);
	enviar_respuesta(socket_cliente, buffer_respuesta, ANADIR_PLATO_RTA);
	free(buffer_respuesta->stream);
	free(buffer_respuesta);
	free(mensaje->plato->nombre);
	free(mensaje->plato);
	free(mensaje);
}

bool pedido_esta_listo(Pedido* un_pedido){
	for(int i = 0; i < un_pedido->platos->elements_count; i++){
		registroComida* comida;
		comida = list_get(un_pedido->platos, i);
		if(comida->cantLista != comida->cantTotal){
			return false;
		}
	}
	return true;
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

void atender_plato_listo(buffer_t* mensaje, sfd socket){
	sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",mensaje,PLATO_LISTO);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_comanda, paquete);
	close(sfd_comanda);
	int32_t ok_fail = deserializar_int(paquete->buffer);
	if(ok_fail == 0){
		msg_plato_listo* mensaje_plato = deserializar_plato_listo(mensaje);
		buffer_t* obtener_pedido = serializar_obtener_pedido(mensaje_plato->restaurante, mensaje_plato->id_pedido);
		sfd sfd_comanda_2 = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",obtener_pedido,OBTENER_PEDIDO);
		free(obtener_pedido->stream);
		free(obtener_pedido);
		py_paquete_t* paquete_2 = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete_2);
		Pedido* un_pedido = deserializar_pedido(paquete_2->buffer);
		if(pedido_esta_listo(un_pedido)){
			int32_t id_repartidor = buscar_id_repartidor_por_id_pedido(mensaje_plato->id_pedido, mensaje_plato->restaurante);
			pedido_listo[id_repartidor] = true;
		}
		close(sfd_comanda_2);

		free(paquete_2);
		destruir_pedido(un_pedido);
		free(mensaje_plato->plato->nombre);
		free(mensaje_plato->plato);
		free(mensaje_plato->restaurante->nombre);
		free(mensaje_plato->restaurante);
		free(mensaje_plato);
	}
	else{
		PY_LOG_INFO("LA COMANDA NO QUIERE EL PLATO \n");
	}
	buffer_t* respuesta_ok_fail = serializar_int(ok_fail);
	enviar_respuesta(socket, respuesta_ok_fail, PLATO_LISTO_RTA);
	free(respuesta_ok_fail->stream);
	free(respuesta_ok_fail);
	free(paquete);
}

void atender_confirmar_pedido(buffer_t* mensaje, int32_t id_cliente, int32_t socket_cliente){
	int32_t id_pedido = deserializar_int(mensaje);
	int32_t respuesta = 0;
	PCB* pcb_pedido = buscar_pcb_temporal_por_id_pedido(id_pedido, id_cliente);
	if(!pcb_pedido){
		respuesta = 1;
	}
	else{
		if(pcb_pedido->confirmado == true || id_cliente != pcb_pedido->id_cliente){
			respuesta = 1;
		}
		else{
			buffer_t* obtener_pedido = serializar_obtener_pedido(pcb_pedido->nombre_restaurante, id_pedido);
			sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",obtener_pedido,OBTENER_PEDIDO);
			free(obtener_pedido->stream);
			free(obtener_pedido);
			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_comanda, paquete);
			free(paquete->buffer->stream);
			free(paquete->buffer);
			free(paquete);
			close(sfd_comanda);
			if(strcmp("RestauranteDefault", pcb_pedido->nombre_restaurante->nombre)){
				handshake_restaurante_completo* handshake = buscar_handshake_restaurante(pcb_pedido->nombre_restaurante);
				buffer_t* buffer_restaurante = serializar_int(id_pedido);
				sfd sfd_restaurante = conectar_y_enviar_manual(handshake->direccion->ip, handshake->direccion->puerto, buffer_restaurante, CONFIRMAR_PEDIDO);
				respuesta = recibir_respuesta_ok_fail(sfd_restaurante);
				free(buffer_restaurante->stream);
				free(buffer_restaurante);
			}
			if(respuesta == 0){
				buffer_t* buffer_comanda = serializar_confirmar_pedido_con_restaurante(id_pedido, pcb_pedido->nombre_restaurante);
				sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer_comanda,CONFIRMAR_PEDIDO);
				py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
				py_recv_msg(sfd_comanda, paquete);
				close(sfd_comanda);
				respuesta = deserializar_int(paquete->buffer);
				free(paquete);
				free(buffer_comanda->stream);
				free(buffer_comanda);
			}
			if(respuesta == 0){
				pcb_pedido->confirmado = true;
				cola_PCB_push(pcb_pedido);

			}
		}
	}
	buffer_t* respuesta_cliente = serializar_int(respuesta);
	enviar_respuesta(socket_cliente, respuesta_cliente, CONFIRMAR_PEDIDO_RTA);
	free(respuesta_cliente->stream);
	free(respuesta_cliente);
}

void atender_consultar_pedido(int32_t socket_cliente, int32_t id_pedido, int32_t id_cliente){
	PCB* pcb_pedido = buscar_pcb_temporal_por_id_pedido(id_pedido, id_cliente);
	buffer_t* obtener_pedido = serializar_obtener_pedido(pcb_pedido->nombre_restaurante, id_pedido);
	sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",obtener_pedido,OBTENER_PEDIDO);
	free(obtener_pedido);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_comanda, paquete);
	Pedido* mensaje = deserializar_pedido(paquete->buffer);
	buffer_t* respuesta = serializar_consultar_pedido_rta(mensaje->estadoPedido, mensaje->platos, pcb_pedido->nombre_restaurante);
	enviar_respuesta(socket_cliente, respuesta, CONSULTAR_PEDIDO_RTA);
}

void atender_handshake_inicial_cliente(int socket_cliente, handshake_cliente* handshake, char* ip){
	handshake_cliente_completo* handshake_real = malloc(sizeof(handshake_cliente_completo));
	handshake_real->direccion = malloc(sizeof(Direccion));
	handshake_real->coordenadas = handshake->coordenadas;
	handshake_real->direccion->puerto = handshake->puerto;
	handshake_real->direccion->ip = ip;
	handshake_real->id = handshake->id;
	agregar_lista_handshake_cliente(handshake_real);
	free(handshake);
	close(socket_cliente);
}

void atender_handshake_inicial_restaurante(int socket_restaurante, handshake_restaurante* handshake,char* ip){
	handshake_restaurante_completo* handshake_real = malloc(sizeof(handshake_restaurante_completo));
	handshake_real->coordenadas = handshake->coordenadas;
	handshake_real->direccion=malloc(sizeof(Direccion));
	handshake_real->direccion->puerto = handshake->puerto;
	handshake_real->direccion->ip = ip;
	handshake_real->nombre_restaurante = handshake->nombre_restaurante;

	PY_LOG_INFO("SE CONECTÓ EL RESTAURANTE %s \n", handshake->nombre_restaurante->nombre);
	if(restaurantes_conectados == 0){
		eliminar_restaurante_default();

	}
	restaurantes_conectados++;
	agregar_lista_restaurantes(handshake_real->nombre_restaurante);
	agregar_lista_handshake_restaurante(handshake_real);
	free(handshake);
	close(socket_restaurante);
}

void atender_consultar_modulo(sfd socket){
	buffer_t* buffer_respuesta = serializar_modulo(APP);
	enviar_respuesta(socket, buffer_respuesta, CONSULTAR_MODULO);
	free(buffer_respuesta->stream);
	free(buffer_respuesta);
}

void atender(sfd socket_cliente, buffer_t* buffer, enum_mensaje codigo_operacion, char* ip){
	void* msg;
	switch(codigo_operacion){
	case CONSULTAR_MODULO:
		free(buffer);
		atender_consultar_modulo(socket_cliente);
		break;
	case PLATO_LISTO:
		atender_plato_listo(buffer, socket_cliente);
		break;
	case HANDSHAKE_INICIAL_CLIENTE:
		msg = (void*)deserializar_handshake_cliente(buffer);
		atender_handshake_inicial_cliente(socket_cliente,(handshake_cliente*)msg, ip);
		break;
	case HANDSHAKE_INICIAL_RESTAURANTE:
		msg = (void*)deserializar_handshake_restaurante(buffer);
		atender_handshake_inicial_restaurante(socket_cliente,(handshake_restaurante*)msg, ip);
		break;
	case HANDSHAKE_NORMAL_CLIENTE:
		atender_cliente(socket_cliente, deserializar_int(buffer));
		break;
	default:
		perror("El mensaje no se puede leer");
	}
}

void atender_cliente(sfd socket_cliente, int32_t id_cliente){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_cliente, paquete);
	buffer_t* buffer = paquete->buffer;
	void* msg;

	switch(paquete->codigo_operacion){
	case CONSULTAR_RESTAURANTE:
		free(paquete->buffer);
		atender_consultar_restaurante(socket_cliente);
		break;
	case SELECCIONAR_RESTAURANTE:
		msg = (void*)deserializar_nombre(buffer);
		atender_seleccionar_restaurante(socket_cliente,(nombre*)msg, id_cliente);
		break;
	case CONSULTAR_PLATOS:
		free(paquete->buffer);
		atender_consultar_platos(id_cliente, socket_cliente);
		break;
	case CREAR_PEDIDO:
		free(paquete->buffer);
		atender_crear_pedido(id_cliente, socket_cliente);
		break;
	case ANIADIR_PLATO:
		atender_aniadir_plato(deserializar_aniadir_plato(buffer), socket_cliente, id_cliente);
		break;
	case PLATO_LISTO:
		atender_plato_listo(buffer, socket_cliente);
		break;
	case CONFIRMAR_PEDIDO:
		atender_confirmar_pedido(buffer, id_cliente, socket_cliente);
		break;
	case CONSULTAR_PEDIDO:
		atender_consultar_pedido(socket_cliente, deserializar_consultar_pedido(buffer)->id_pedido, id_cliente);
		free(deserializar_consultar_pedido(buffer));
		break;
	default:
		perror("SE ROMPIO TODO");
	}
	free(paquete);
}

void agregar_PCB_temporal(PCB* un_pcb){
	pthread_mutex_lock(&mutex_PCBs_temporales);
	list_add(PCBs_temporales, un_pcb);
	pthread_mutex_unlock(&mutex_PCBs_temporales);
}

PCB* buscar_pcb_temporal_por_id_pedido(int32_t id_pedido, int32_t id_cliente){
	for(int i = 0; i < PCBs_temporales->elements_count; i++){
		PCB* un_pcb = list_get(PCBs_temporales, i);
		if(un_pcb->id_pedido == id_pedido && un_pcb->id_cliente == id_cliente){
			return un_pcb;
		}
	}
	return NULL;
}

int32_t buscar_id_repartidor_por_id_pedido(int32_t id_pedido, nombre* nombre_restaurante){
	for(int i = 0; i < cantidad_repartidores; i++){
		if(PCBs_asignados[i]){
			int32_t pedido = PCBs_asignados[i]->id_pedido;
			if(pedido == id_pedido && !strcmp(nombre_restaurante->nombre, PCBs_asignados[i]->nombre_restaurante->nombre)){
				return i;
			}
		}
	}
	return -1;
}

nombre* buscar_restaurante_por_nombre(char* data){
	bool _comparar_nombre(void* element)
	{
		return (  strcmp(((nombre*)(element))->nombre,data)  )? false:true;
	}

	return (nombre*)(list_find(lista_restaurantes,_comparar_nombre));
}

cliente_restaurante* buscar_nombre_id_por_nombre(char* data){
	bool _comparar_nombre(void* element)
	{
		return (  strcmp(((cliente_restaurante*)(element))->restaurante->nombre,data)  )? false:true;
	}

	return (cliente_restaurante*)(list_find(lista_restaurante_clientes,_comparar_nombre));
}

cliente_restaurante* buscar_nombre_id_por_id(int32_t data){
	bool _comparar_id(void* element)
	{
		return ((cliente_restaurante*)(element))->id_cliente == data;
	}

	return (cliente_restaurante*)list_find(lista_restaurante_clientes,_comparar_id);
}

handshake_cliente_completo* buscar_handshake_cliente(int32_t data){
	for(int i = 0; i < lista_handshake_cliente->elements_count; i++){
		handshake_cliente_completo* un_handshake = list_get(lista_handshake_cliente,i);
		if(un_handshake->id == data){
			return un_handshake;
		}
	}
	return NULL;
}

handshake_restaurante_completo* buscar_handshake_restaurante(nombre* nombre_resto){
	for(int i = 0; i < lista_handshake_restaurante->elements_count; i++){
		handshake_restaurante_completo* un_handshake = list_get(lista_handshake_restaurante, i);
		if(!strcmp(nombre_resto->nombre, un_handshake->nombre_restaurante->nombre)){
			return un_handshake;
		}
	}
	return NULL;
}


void liberar_lista_nombres(t_list* lista_nombres){                           //Si hay algún duplicado, no funcona (no deberia pasar)
	for(int i = 0; i < lista_nombres->elements_count; i++){
		nombre* un_nombre = list_get(lista_nombres, i);
		free(un_nombre->nombre);
		free(un_nombre);
	}
	list_destroy(lista_nombres);
}

int32_t generar_id_pedido_default(){
	id_pedido_default--;
	return id_pedido_default;
}


char* sacar_corchetes(char* string){
	int32_t largo = strlen(string) - 1;
	char* nuevo_string = malloc(strlen(string)) - 1;

	for(int i = 1; i < largo; i++){
		nuevo_string[i-1] = string[i];
	}
	nuevo_string[largo-1] = '\0';
	return nuevo_string;
}

int32_t string_array_items_count(char** str_arr)
{
	int32_t cantidad = 0;
	if(str_arr == NULL)
	{
		return cantidad;
	} else
	{
		void _obtener_cantidad (char* element)
		{
	        cantidad++;
			return;
		}
		string_iterate_lines(str_arr,_obtener_cantidad);
		return cantidad;
	}

}

int32_t cantidad_elementos_string(char* string){
	int32_t cantidad = 1;

	for(int i = 0; i < strlen(string)+1; i++){
		if(string[i] == ','){
			cantidad++;
		}
	}
	free(string);
	return cantidad;
}

char** string_a_array(char* array_string){
	char* nuevo_string = sacar_corchetes(array_string);
	char** listin = string_split(nuevo_string,",");
	return listin;
}

void cargar_lista_string(char* array_string, t_list* lista){
	int32_t cantidad = cantidad_elementos_string(array_string);
	char** listin = string_a_array(array_string);

	for(int i = 0; i < cantidad; i++){
		nombre* nombre = malloc(sizeof(nombre));
		nombre->largo_nombre = strlen(listin[i]) + 1;
		nombre->nombre = malloc(nombre->largo_nombre);
		strcpy(nombre->nombre, listin[i]);
		list_add(lista, nombre);
	}
}

void mostrar_entrenadores(int32_t cantidad_repartidores) {
	for (int i = 0; i < cantidad_repartidores; i++) {
		repartidor *un_repartidor = list_get(lista_repartidores, i);
		printf("Entrenador %i \n", i);
		printf("Coordenadas: (%i,%i) \n", un_repartidor->ubicacion.posicionX,
				un_repartidor->ubicacion.posicionY);
		printf("Frecuencia descanso: %i \n",
				un_repartidor->frecuencia_descanso);
		printf("Tiempo decanso: %i \n", un_repartidor->tiempo_descanso);
	}
}

int32_t primer_repartidor_disponible(){
	for(int i = 0; i < cantidad_repartidores; i++){
		if(repartidor_disponible((int32_t)i)){
			return i;
		}
	}
	return -1;
}

bool repartidor_disponible(int32_t indice_repartidor){
	pthread_mutex_lock(&sem_repartidores[indice_repartidor]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice_repartidor);
	pthread_mutex_unlock(&sem_repartidores[indice_repartidor]);
	return un_repartidor->estado == NEW || un_repartidor->estado == BLOCKED_LIBRE;
}

uint32_t comparar_posiciones(CoordenadasXY p1, CoordenadasXY p2){
	return (uint32_t)(  abs( (int32_t)p1.posicionX - (int32_t)p2.posicionX )
					  + abs( (int32_t)p1.posicionY - (int32_t)p2.posicionY )  );
}

uint32_t distancia_repartidor_a_coordenada(int32_t indice_repartidor, CoordenadasXY posicion){
	pthread_mutex_lock(&sem_repartidores[indice_repartidor]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice_repartidor);
	pthread_mutex_unlock(&sem_repartidores[indice_repartidor]);
	return comparar_posiciones(un_repartidor->ubicacion, posicion);
}

int32_t buscar_repartidor_cercano(CoordenadasXY posicion)
{
	int32_t indice_elegido = primer_repartidor_disponible();
	int32_t distancia_cercana = distancia_repartidor_a_coordenada(indice_elegido, posicion);
	for(int i = indice_elegido + 1; i < cantidad_repartidores; i++){
		int32_t distancia_actual = distancia_repartidor_a_coordenada(i, posicion);
		if(distancia_actual < distancia_cercana && repartidor_disponible(i)){
			indice_elegido = i;
			distancia_cercana = distancia_actual;
		}
	}
	return indice_elegido;
}

void setear_estado_repartidor(estado_repartidor estado, int32_t indice){
	repartidor* un_repartidor = list_get(lista_repartidores, indice);
	un_repartidor->estado = estado;
}

bool esFIFO(){
	return !strcmp(algoritmo_planificacion, "FIFO");
}

bool esHRRN(){
	return !strcmp(algoritmo_planificacion, "HRRN");
}

bool esSJF(){
	return !strcmp(algoritmo_planificacion, "SJF");
}

void planificar_base(){
	algoritmo_planificacion = py_config_get_string("ALGORITMO_DE_PLANIFICACION");
	cantidad_ready = 0;
	pthread_t asignar_pedidos;
	pthread_create(&asignar_pedidos, NULL, (void*)asignar_pedidos_clientes, NULL);
	pthread_t* planificacion = calloc(grado_multiprocesamiento, sizeof(pthread_t));
	pthread_t clock;
	pthread_create(&clock, NULL, (void*)pasar_ciclo, NULL);
	if(esFIFO()){
		inicializar_cola_ready();
	}
	if(esHRRN()){
		estimacion_inicial = py_config_get_double("ESTIMACION_INICIAL");
		inicializarHRRN(cantidad_repartidores);
	}
	if(esSJF()){
		alpha = py_config_get_double("ALPHA");
		estimacion_inicial = py_config_get_double("ESTIMACION_INICIAL");
		inicializarSJF(cantidad_repartidores, estimacion_inicial);
	}
	for(int i = 0; i < grado_multiprocesamiento; i++){
		pthread_create(&planificacion[i], NULL, (void*)planificar, (void*)i);
	}
	for(int i = 0; i < grado_multiprocesamiento; i++){
		pthread_join(planificacion[i], NULL);
	}
}

void planificar(int32_t id_procesador){
	if(esFIFO()){
		planificarFIFO(id_procesador);
	}
	if(esHRRN()){
		planificarHRRN(id_procesador);
	}
	if(esSJF()){
		planificarSJF(id_procesador, alpha);
	}
}

void asignar_pedidos_clientes(){
	PCBs_asignados = calloc(cantidad_repartidores, sizeof(PCB*));
	while(1){
		PCB* un_PCB = cola_PCB_pop();
		sem_wait(&sem_repartidores_disponibles);
		int32_t repartidor_elegido = buscar_repartidor_cercano(un_PCB->ubicacion_restaurante);
		PCBs_asignados[repartidor_elegido] = un_PCB;
		pthread_mutex_lock(&sem_repartidores[repartidor_elegido]);
		repartidor* un_repartidor = list_get(lista_repartidores, repartidor_elegido);
		un_repartidor->destino = un_PCB->ubicacion_restaurante;
		un_repartidor->tipo_destino = RESTAURANTE_DESTINO;
		pthread_mutex_unlock(&sem_repartidores[repartidor_elegido]);
		PY_LOG_TRACE("El pedido de ID: %i se le asignó al repartidor %i que tiene como destino el restaurante %s ubicado en (%i,%i) y el cliente ubicado en (%i,%i)",
				un_PCB->id_pedido,
				repartidor_elegido,
				un_PCB->nombre_restaurante->nombre,
				un_PCB->ubicacion_restaurante.posicionX,
				un_PCB->ubicacion_restaurante.posicionY,
				un_PCB->ubicacion_cliente.posicionX,
				un_PCB->ubicacion_cliente.posicionY);
		if(un_repartidor->estado == NEW){
			PY_LOG_INFO("El repartidor %i pasa de la cola NEW a la cola READY porque se le asignó un pedido", repartidor_elegido);
		}
		else{
			PY_LOG_INFO("El repartidor %i pasa de la cola BLOCKED a la cola READY porque se le asignó un pedido", repartidor_elegido);
		}
		poner_en_ready(repartidor_elegido);
	}
}

void inicializarHRRN(int32_t cantidad_repartidores){
	sem_init(&sem_repartidores_ready,0,0);
	aging = calloc(cantidad_repartidores, sizeof(int32_t));
	tasa_respuesta = calloc(cantidad_repartidores, sizeof(float));
	rafaga_anterior = calloc(cantidad_repartidores, sizeof(int32_t));
	for(int i = 0; i < cantidad_repartidores; i++){
		aging[i] = 0;
		tasa_respuesta[i] = 0;
		rafaga_anterior[i] = estimacion_inicial;
	}
}

void inicializarSJF(int32_t cantidad_repartidores, double estimacion_inicial){
	sem_init(&sem_repartidores_ready,0,0);
	estimacion_actual = calloc(cantidad_repartidores, sizeof(double));
	estimacion_anterior = calloc(cantidad_repartidores, sizeof(double));
	rafaga_anterior = calloc(cantidad_repartidores, sizeof(int32_t));
	for(int i = 0; i < cantidad_repartidores; i++){
		estimacion_actual[i] = estimacion_inicial;
		estimacion_anterior[i] = -1;
		rafaga_anterior[i] = -1;
	}
}

void planificarFIFO(int32_t id_procesador){
	while(1){
		if(!cola_ready_is_empty()){
			int32_t indice_repartidor = cola_ready_pop();
	     	int32_t rafaga = calcular_rafaga(indice_repartidor);
	     	PY_LOG_INFO("El repartidor %i pasa de la cola READY a la cola EXEC porque fue elegido por el planificador", indice_repartidor);
	    	setear_estado_repartidor(EXEC, indice_repartidor);
		    mover_repartidor(indice_repartidor, rafaga, id_procesador);
		    tratar_repartidor_despues_de_moverse(indice_repartidor);
		}
		else{
			procesoIDLE(id_procesador);
		}
	}
}


void planificarHRRN(int32_t id_procesador){
	while(1){
		pthread_mutex_lock(&sem_cantidad_ready);
		if(!nadie_ready()){
			pthread_mutex_unlock(&sem_cantidad_ready);
			int32_t indice_repartidor = elegir_repartidor_HRRN();
			int32_t rafaga = calcular_rafaga(indice_repartidor);
			rafaga_anterior[indice_repartidor] = rafaga;
			mover_repartidor(indice_repartidor, rafaga, id_procesador);
			tratar_repartidor_despues_de_moverse(indice_repartidor);
		}
		else{
			pthread_mutex_unlock(&sem_cantidad_ready);
			procesoIDLE(id_procesador);
		}
	}

}

void planificarSJF(int32_t id_procesador, int32_t alpha){
	while(1){
		pthread_mutex_lock(&sem_cantidad_ready);
		if(!nadie_ready()){
			pthread_mutex_unlock(&sem_cantidad_ready);
			int32_t indice_repartidor = elegir_repartidor_SJF();
			int32_t rafaga = calcular_rafaga(indice_repartidor);
			rafaga_anterior[indice_repartidor] = rafaga;
		    mover_repartidor(indice_repartidor, rafaga, id_procesador);
		    tratar_repartidor_despues_de_moverse(indice_repartidor);
		}
		else{
			pthread_mutex_unlock(&sem_cantidad_ready);
			procesoIDLE(id_procesador);
		}
	}
}

void procesoIDLE(int32_t id_procesador){
	sem_wait(&sem_clock_salida[id_procesador]);
	sem_post(&sem_clock_entrada[id_procesador]);
}

void informar_finalizacion_pedido(int32_t indice_repartidor, int32_t id_cliente){
	int32_t respuestaa;
	PCB* PCB_correspondiente = PCBs_asignados[indice_repartidor];
	buffer_t* buffer = buffer = serializar_finalizar_pedido(PCB_correspondiente->nombre_restaurante, PCB_correspondiente->id_pedido);
	sfd socket_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA", buffer, FINALIZAR_PEDIDO);
	respuestaa = recibir_respuesta_ok_fail(socket_comanda);
	if(respuestaa!=0){
		perror("Error al finalizar pedido");
	}
	handshake_cliente_completo* handshake_cli = buscar_handshake_cliente(id_cliente);

	sfd sfd_cliente = conectar_y_enviar_manual(handshake_cli->direccion->ip, handshake_cli->direccion->puerto, buffer, FINALIZAR_PEDIDO);
	respuestaa = recibir_respuesta_ok_fail(sfd_cliente);
	free(buffer->stream);
	free(buffer);
	if(respuestaa!=0){
		perror("Error al finalizar pedido");
	}
	liberar_PCB(PCBs_asignados[indice_repartidor]);
}

void avisar_llegada_restaurante(int32_t id_repartidor){
	buffer_t* obtener_pedido = serializar_obtener_pedido(PCBs_asignados[id_repartidor]->nombre_restaurante, PCBs_asignados[id_repartidor]->id_pedido);
	sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",obtener_pedido, OBTENER_PEDIDO);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_comanda, paquete);
	Pedido* un_pedido = deserializar_pedido(paquete->buffer);
	pthread_mutex_lock(&sem_repartidores[id_repartidor]);
	repartidor* un_repartidor = list_get(lista_repartidores, id_repartidor);
	if(pedido_esta_listo(un_pedido)){
		pedido_listo[id_repartidor] = false;
		if(un_repartidor->cansancio_actual == un_repartidor->frecuencia_descanso){
			PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque está cansado", id_repartidor);
			setear_estado_repartidor(BLOCKED_DESCANSANDO_TRABAJANDO, id_repartidor);
		}
		else{
			PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola READY porque el pedido está listo para su entrega", id_repartidor);
			poner_en_ready(id_repartidor);
		}
	}
	else{
		PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque debe esperar que el pedido esté listo", id_repartidor);
		if(un_repartidor->cansancio_actual == un_repartidor->frecuencia_descanso){
			setear_estado_repartidor(BLOCKED_DESCANSANDO_ESPERANDO, id_repartidor);
		}
		else{
			setear_estado_repartidor(BLOCKED_ESPERANDO, id_repartidor);
		}
	}
	pthread_mutex_unlock(&sem_repartidores[id_repartidor]);
	destruir_pedido(un_pedido);
	free(paquete);
	free(obtener_pedido->stream);
	free(obtener_pedido);
}

/*
 * 			if(un_repartidor->frecuencia_descanso == un_repartidor->cansancio_actual){
				PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque está cansado", indice_repartidor);
			}
			else{
				PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque finalizó su pedido", indice_repartidor);
			}
 */

void tratar_repartidor_despues_de_moverse(int32_t indice_repartidor){
	pthread_mutex_lock(&sem_repartidores[indice_repartidor]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice_repartidor);
	pthread_mutex_unlock(&sem_repartidores[indice_repartidor]);
	if(llego_a_destino(indice_repartidor)){
		if(un_repartidor->tipo_destino == RESTAURANTE_DESTINO){
			PY_LOG_INFO("El repartidor %i llegó al restaurante", indice_repartidor);
			actualizar_destino(indice_repartidor);
			avisar_llegada_restaurante(indice_repartidor);
			return;
		}
		else{
			setear_estado_repartidor(BLOCKED_LIBRE, indice_repartidor);
			PY_LOG_WARNING("Se entregó el pedido %i del restaurante %s a cargo del repartidor %i", PCBs_asignados[indice_repartidor]->id_pedido, PCBs_asignados[indice_repartidor]->nombre_restaurante->nombre ,indice_repartidor);
			if(un_repartidor->frecuencia_descanso == un_repartidor->cansancio_actual){
				PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque está cansado", indice_repartidor);
			}
			else{
				PY_LOG_INFO("El repartidor %i pasa de la cola EXEC a la cola BLOCKED porque finalizó su pedido", indice_repartidor);
			}
			informar_finalizacion_pedido(indice_repartidor, PCBs_asignados[indice_repartidor]->id_cliente);
			sem_post(&sem_repartidores_disponibles);
		}
	}
	if(un_repartidor->frecuencia_descanso == un_repartidor->cansancio_actual){
		if(llego_a_destino(indice_repartidor)){
			if(un_repartidor->tipo_destino == CLIENTE){
				setear_estado_repartidor(BLOCKED_DESCANSANDO_LIBRE, indice_repartidor);
			}
			else{
				setear_estado_repartidor(BLOCKED_DESCANSANDO_ESPERANDO, indice_repartidor);
			}
		}
		else{
			PY_LOG_INFO("El repartidor %i pasa de la cola READY a la cola BLOCKED porque está cansado", indice_repartidor);
			setear_estado_repartidor(BLOCKED_DESCANSANDO_TRABAJANDO, indice_repartidor);
		}
	}
}

void actualizar_destino(int32_t indice_repartidor){
	pthread_mutex_lock(&sem_repartidores[indice_repartidor]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice_repartidor);
	pthread_mutex_unlock(&sem_repartidores[indice_repartidor]);
	un_repartidor->destino = PCBs_asignados[indice_repartidor]->ubicacion_cliente;
	un_repartidor->tipo_destino = CLIENTE;
}

void descansar_repartidores(){
	for(int i = 0; i < cantidad_repartidores; i++){
		pthread_mutex_lock(&sem_repartidores[i]);
		repartidor* un_repartidor = list_get(lista_repartidores, i);
		pthread_mutex_unlock(&sem_repartidores[i]);
		if(un_repartidor->estado == BLOCKED_DESCANSANDO_TRABAJANDO || un_repartidor->estado == BLOCKED_DESCANSANDO_LIBRE || un_repartidor->estado == BLOCKED_DESCANSANDO_ESPERANDO){
			descansar_repartidor(i);
		}
	}
}

void poner_en_ready(int32_t indice){
	setear_estado_repartidor(READY, indice);
	cantidad_ready++;
	if(esFIFO()){
		cola_ready_push(indice);
	}
}

void descansar_repartidor(int32_t indice){
	pthread_mutex_lock(&sem_repartidores[indice]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice);
	pthread_mutex_unlock(&sem_repartidores[indice]);
	un_repartidor->descanso_actual++;
	if(un_repartidor->descanso_actual == un_repartidor->tiempo_descanso){
		un_repartidor->descanso_actual = 0;
		un_repartidor->cansancio_actual = 0;
		if(un_repartidor->estado == BLOCKED_DESCANSANDO_LIBRE){
			un_repartidor->estado = BLOCKED_LIBRE;
		}
		else if(un_repartidor->estado == BLOCKED_DESCANSANDO_TRABAJANDO){
			poner_en_ready(indice);
		}
		else if(un_repartidor->estado == BLOCKED_DESCANSANDO_ESPERANDO){
			un_repartidor->estado = BLOCKED_ESPERANDO;
		}
	}
}

void envejecer(){
	for(int i = 0; i < cantidad_repartidores; i++){
		repartidor* un_repartidor = list_get(lista_repartidores, i);
		if(un_repartidor->estado == READY){
			aging[i]++;
		}
	}
}

int32_t calcular_rafaga(int32_t indice){
	int32_t ciclos;
	//pthread_mutex_lock(&sem_repartidores[indice]);
	repartidor* un_repartidor = list_get(lista_repartidores,indice);
	int32_t ciclos_hasta_descanso = un_repartidor->frecuencia_descanso - un_repartidor->cansancio_actual;
	pthread_mutex_unlock(&sem_repartidores[indice]);
	int32_t distancia_al_destino = distancia_repartidor_a_coordenada(indice, un_repartidor->destino);
	if(ciclos_hasta_descanso > distancia_al_destino){
		ciclos = distancia_al_destino;
	}
	else{
		ciclos = ciclos_hasta_descanso;
	}

	return ciclos;
}

int32_t primer_repartidor_ready(){
	repartidor* un_repartidor;
	for(int i = 0; i < cantidad_repartidores; i++){
		un_repartidor = list_get(lista_repartidores, i);
		if(un_repartidor->estado == READY){
			return i;
		}
	}
	return -1;
}

int32_t elegir_repartidor_HRRN(){
	pthread_mutex_lock(&sem_elegir_repartidor);
	calcular_tasas_respuesta();
	int32_t indice_elegido = primer_repartidor_ready();
	int32_t prioridad_ganadora = tasa_respuesta[indice_elegido];
	for(int32_t i = indice_elegido; i < cantidad_repartidores; i++){
		if(tasa_respuesta[i] > prioridad_ganadora && get_estado(i) == READY){
			indice_elegido = i;
			prioridad_ganadora = tasa_respuesta[i];
		}
	}
	PY_LOG_INFO("El repartidor %i pasa de la cola READY a la cola EXEC porque fue elegido por el planificador", indice_elegido);
	aging[indice_elegido] = 0;
	setear_estado_repartidor(EXEC, indice_elegido);
	pthread_mutex_unlock(&sem_elegir_repartidor);
	return indice_elegido;
}

int32_t elegir_repartidor_SJF(){
	pthread_mutex_lock(&sem_elegir_repartidor);
	calcular_estimaciones();
	int32_t indice_elegido = primer_repartidor_ready();
	float estimacion_ganadora = estimacion_actual[indice_elegido];
	for(int32_t i = indice_elegido; i < cantidad_repartidores; i++){
		if(estimacion_ganadora > estimacion_actual[i] && get_estado(i) == READY){
			indice_elegido = i;
			estimacion_ganadora = estimacion_actual[i];
		}
	}
	PY_LOG_INFO("El repartidor %i pasa de la cola READY a la cola EXEC porque fue elegido por el planificador", indice_elegido);

	setear_estado_repartidor(EXEC, indice_elegido);
	estimacion_anterior[indice_elegido] = estimacion_ganadora;
	pthread_mutex_unlock(&sem_elegir_repartidor);
	return indice_elegido;
}

void calcular_tasa_respuesta(int32_t indice){
	float w = (float)aging[indice];
	float s = (float)rafaga_anterior[indice];
	float R = (w+s)/s;
	tasa_respuesta[indice] = R;
}

void calcular_tasas_respuesta(){
	for(int i = 0; i < cantidad_repartidores; i++){
		calcular_tasa_respuesta(i);
	}
}

void calcular_estimacion(int32_t indice){
	if(estimacion_anterior[indice] == -1 && rafaga_anterior[indice] == -1){
		return;
	}
	double rafaga_ant = (double)rafaga_anterior[indice];
	double estimacion_ant = estimacion_anterior[indice];
	double est_actual = alpha * rafaga_ant + (1-alpha) * estimacion_ant;
	estimacion_actual[indice] = est_actual;
}

void calcular_estimaciones(){
	for(int i = 0; i < cantidad_repartidores; i++){
		calcular_estimacion(i);
	}
}

void cola_PCB_push(PCB* un_pedido){         //revisar si está bien el tipo PCB
	pthread_mutex_lock(&mutex_cola_PCB);
	queue_push(colaPedidos,(void*)un_pedido);
	pthread_mutex_unlock(&mutex_cola_PCB);

	sem_post(&sem_hay_PCBs);
}

PCB* cola_PCB_pop(){
	sem_wait(&sem_hay_PCBs);

	pthread_mutex_lock(&mutex_cola_PCB);
	PCB* un_pedido = (PCB*)queue_pop(colaPedidos);
	pthread_mutex_unlock(&mutex_cola_PCB);

	return un_pedido;
}

void cola_ready_push(int32_t indiceRepartidor){
	pthread_mutex_lock(&mutex_cola_repartidores);
	queue_push(colaRepartidoresReady,(void*)indiceRepartidor);
	pthread_mutex_unlock(&mutex_cola_repartidores);

	sem_post(&sem_repartidores_ready);
}


int32_t cola_ready_pop(){
	sem_wait(&sem_repartidores_ready);

	pthread_mutex_lock(&mutex_cola_repartidores);
	int32_t indiceRepartidor = (int32_t)queue_pop(colaRepartidoresReady);
	pthread_mutex_unlock(&mutex_cola_repartidores);

	return indiceRepartidor;
}

bool cola_ready_is_empty(){
	pthread_mutex_lock(&mutex_cola_repartidores);
	bool result = list_is_empty(colaRepartidoresReady->elements);
	pthread_mutex_unlock(&mutex_cola_repartidores);
	return result;
}

void mover_repartidor(int32_t indice, int32_t cantidad_ciclos, int32_t id_procesador){
	repartidor* un_repartidor = list_get(lista_repartidores, indice);
	for(int i = 0; i < cantidad_ciclos; i++){
		sem_wait(&sem_clock_salida[id_procesador]);
		CoordenadasXY origen = un_repartidor->ubicacion;
		CoordenadasXY destino = un_repartidor->destino;
		if(origen.posicionX > destino.posicionX){
			un_repartidor->ubicacion.posicionX--;
		}
		else if(origen.posicionX < destino.posicionX){
			un_repartidor->ubicacion.posicionX++;
		}
		else if(origen.posicionY > destino.posicionY){
			un_repartidor->ubicacion.posicionY--;
		}
		else if(origen.posicionY < destino.posicionY){
			un_repartidor->ubicacion.posicionY++;
		}
		un_repartidor->cansancio_actual++;
		PY_LOG_DEBUG("El repartidor %i se movió de la posición (%i,%i) a la posición (%i,%i)",
				indice,
				origen.posicionX,
				origen.posicionY,
				un_repartidor->ubicacion.posicionX,
				un_repartidor->ubicacion.posicionY);
		sem_post(&sem_clock_entrada[id_procesador]);
	}
}

/*
 * 	repartidor* un_repartidor;
	pthread_mutex_lock(&sem_repartidores[indice]);
	un_repartidor = list_get(lista_repartidores, indice);
	estado_repartidor estado = un_repartidor->estado;
	pthread_mutex_unlock(&sem_repartidores[indice]);
 */

void chequear_pedidos_listos(){
	for(int i = 0; i < cantidad_repartidores; i++){
		repartidor* un_repartidor;
		pthread_mutex_lock(&sem_repartidores[i]);
		un_repartidor = list_get(lista_repartidores, i);
		if(pedido_listo[i]){
			pedido_listo[i] = false;
			if(un_repartidor->estado == BLOCKED_ESPERANDO){
				PY_LOG_INFO("El repartidor %i pasa de la cola BLOCKED a la cola READY porque su pedido está listo para ser entregado", i);
				poner_en_ready(i);
			}
			if(un_repartidor->estado == BLOCKED_DESCANSANDO_ESPERANDO){
				setear_estado_repartidor(BLOCKED_DESCANSANDO_TRABAJANDO, i);
			}
		}
		pthread_mutex_unlock(&sem_repartidores[i]);

	}
}

void pasar_ciclo(){
	while(1){
		for(int i = 0; i < grado_multiprocesamiento; i++){
			sem_wait(&sem_clock_entrada[i]);
		}
		sleep(retardo_ciclo);
		chequear_pedidos_listos();
		descansar_repartidores();
		if(esHRRN()){
			envejecer();
		}
		//PY_LOG_ERROR("CICLO");
		for(int i = 0; i < grado_multiprocesamiento; i++){
			sem_post(&sem_clock_salida[i]);
		}
	}
}

bool repartidor_puede_ejecutar(int32_t indice){
	pthread_mutex_lock(&sem_repartidores[indice]);
	repartidor* un_repartidor = list_get(lista_repartidores, indice);
	bool respuesta = un_repartidor->estado == READY || un_repartidor->estado == EXEC;
	pthread_mutex_unlock(&sem_repartidores[indice]);
	return respuesta;
}

bool nadie_ready(){
	if(cantidad_ready > 0){
		cantidad_ready--;
		return false;
	}
	else{
		return true;
	}
}

int32_t recibir_respuesta_ok_fail(sfd socket_respuesta){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_respuesta, paquete);
	int32_t respuesta = deserializar_int(paquete->buffer);
	free(paquete);
	close(socket_respuesta);
	return respuesta;
}

bool llego_a_destino(int32_t indice){
	repartidor* un_repartidor = list_get(lista_repartidores, indice);
	return distancia_repartidor_a_coordenada(indice, un_repartidor->destino) == 0;
}

estado_repartidor get_estado(int32_t indice){
	repartidor* un_repartidor;
	pthread_mutex_lock(&sem_repartidores[indice]);
	un_repartidor = list_get(lista_repartidores, indice);
	estado_repartidor estado = un_repartidor->estado;
	pthread_mutex_unlock(&sem_repartidores[indice]);
	return estado;
}

void liberar_PCB(PCB* un_PCB){
	//free(un_PCB->nombre_restaurante->nombre);
	//free(un_PCB->nombre_restaurante);
	for(int i = 0; i < PCBs_temporales->elements_count; i++){
		PCB* pcb = list_get(PCBs_temporales,i);
		if(pcb->id_pedido == un_PCB->id_pedido && !strcmp(pcb->nombre_restaurante->nombre,un_PCB->nombre_restaurante->nombre)){
			list_remove(PCBs_temporales,i);
			free(un_PCB);
			return;
		}
	}
}

void terminar_programa(){
	liberar_lista_nombres(lista_restaurantes);
	free(conexiones_restaurantes);
}
