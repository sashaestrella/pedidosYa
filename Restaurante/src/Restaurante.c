#include "Restaurante.h"

pthread_t planificacion;

int main(void) {
	iniciar_programa("restaurante.config", "restaurante.log", "restaurante");

	char* nombre_r = py_config_get_string("NOMBRE_RESTAURANTE");

	nombre_restaurante = malloc(sizeof(nombre));
	nombre_restaurante->largo_nombre = strlen(nombre_r) + 1;
	//nombre_restaurante->nombre = malloc(nombre_restaurante->largo_nombre);
	nombre_restaurante->nombre = nombre_r;

	obtener_metadata_restaurante();
	crear_colas_planificacion();
	inicializar_semaforos();
	pthread_create(&planificacion, NULL, (void*)inicializar_planificacion, NULL);
	conexiones();

	return SUCCESS;
}

void obtener_metadata_restaurante(){
	buffer_t* buffer = serializar_obtener_restaurante(nombre_restaurante);
	sfd sfd_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer, OBTENER_RESTAURANTE);

	free(buffer->stream);
	free(buffer);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_sindicato, paquete);

	restaurante = deserializar_restaurante(paquete->buffer);
	PY_LOG_INFO("Cantidad de hornos: %i", restaurante->info_Restaurante.cantidadDeHornos);

	free(paquete);

	if(restaurante->info_Restaurante.cantidadDePedidos==-1){
	 PY_LOG_INFO("No existe el restaurante");
	}
	else{
		PY_LOG_INFO("%s \n",restaurante->nombre_restaurante->nombre);
	cantidad_procesadores = cant_cocineros = restaurante->info_Restaurante.cantidadDeCocineros;
	procesadores = queue_create();

	for(uint32_t i = 0; i < cantidad_procesadores; i++){
		queue_push(procesadores, (void*)(i + 1));
	}
	}
	close(sfd_sindicato);
}

void crear_colas_planificacion(){
	crear_colas_afinidades();
	crear_cola_hornos();

	retardo_ciclo = py_config_get_int("RETARDO_CICLO_CPU");
}

void crear_colas_afinidades(){
	cant_afinidades = calcular_cantidad_afinidades(restaurante->info_Restaurante.afinidades);

	nombre* afinidad;

	cola_afinidades = calloc(cant_afinidades, sizeof(cola_afinidad));

	t_list* afinidades_sin_repetir = lista_sin_repetidos(restaurante->info_Restaurante.afinidades);

	uint32_t procesadores_acumulados = 0;

	for(int i = 0; i < cant_afinidades - 1; i++){
		cola_afinidades[i].cola = queue_create();

		afinidad = list_get(afinidades_sin_repetir, i);

		cola_afinidades[i].nombre = malloc(sizeof(nombre));
		cola_afinidades[i].nombre->largo_nombre = strlen(afinidad->nombre) + 1;
		cola_afinidades[i].nombre->nombre = malloc(cola_afinidades[i].nombre->largo_nombre);
		cola_afinidades[i].nombre->nombre = afinidad->nombre;

		uint32_t cant_repetidos = cantidad_repetidos(afinidad->nombre);
		cola_afinidades[i].cantidad_procesadores = cant_repetidos;

		cola_afinidades[i].procesador = calloc(cant_repetidos, sizeof(int32_t));

		for(int j = 0; j < cant_repetidos; j++){
			int valor = (int32_t)queue_pop(procesadores);
			cola_afinidades[i].procesador[j] = valor;
			procesadores_acumulados++;
		}
	}
	list_destroy(afinidades_sin_repetir);

	cola_afinidades[cant_afinidades - 1].cola = queue_create();

	cola_afinidades[cant_afinidades - 1].nombre = malloc(sizeof(nombre));
	cola_afinidades[cant_afinidades - 1].nombre->largo_nombre = 5;
	cola_afinidades[cant_afinidades - 1].nombre->nombre = malloc(5);
	cola_afinidades[cant_afinidades - 1].nombre->nombre = "Otro";

	procesadores_restantes = cant_cocineros - procesadores_acumulados;
	cola_afinidades[cant_afinidades - 1].cantidad_procesadores = procesadores_restantes;

	cola_afinidades[cant_afinidades - 1].procesador = calloc(procesadores_restantes, sizeof(int32_t));

	for(int i = 0; i < procesadores_restantes; i++){
		cola_afinidades[cant_afinidades - 1].procesador[i] = (int32_t)queue_pop(procesadores);
	}
}

void crear_cola_hornos(){
	cant_hornos = restaurante->info_Restaurante.cantidadDeHornos;

	cola_hornos = queue_create();
}

void inicializar_semaforos(){
	pthread_mutex_init(&mutex_lista_handshake_cliente, NULL);

	pthread_mutex_init(&mutex_cola_afinidades, NULL);
	pthread_mutex_init(&mutex_cola_hornos, NULL);

	pthread_mutex_init(&mutex_algoritmo, NULL);

	sem_cocinero_entrada = calloc(cant_cocineros, sizeof(sem_t));
	sem_cocinero_salida = calloc(cant_cocineros, sizeof(sem_t));

	sem_horno_entrada = calloc(cant_hornos, sizeof(sem_t));
	sem_horno_salida = calloc(cant_hornos, sizeof(sem_t));

	for(int i = 0; i < cant_cocineros; i++){
		sem_init(&sem_cocinero_entrada[i], 0, 0);
		sem_init(&sem_cocinero_salida[i], 0, 1);
	}

	for(int i = 0; i < cant_hornos; i++){
		sem_init(&sem_horno_entrada[i], 0, 0);
		sem_init(&sem_horno_salida[i], 0, 1);
	}
}

void conexiones(){
	enviar_handshake_APP();

	sfd sfd_restaurante = server_create("PUERTO_ESCUCHA");
	struct sockaddr_in direccionCliente;
	unsigned int tamanioDireccion = sizeof(struct sockaddr_in);

	while(!terminado){
		sfd sfd_cliente = accept(sfd_restaurante,(void*) &direccionCliente, &tamanioDireccion);
	    escuchar(sfd_cliente);
	}

	close(sfd_restaurante);
}

void enviar_handshake_APP(){
	handshake_restaurante handshake;

	handshake.puerto = py_config_get_string("PUERTO_ESCUCHA");

	handshake.nombre_restaurante = malloc(sizeof(nombre));
	handshake.nombre_restaurante->largo_nombre = nombre_restaurante->largo_nombre;
	handshake.nombre_restaurante->nombre = malloc(handshake.nombre_restaurante->largo_nombre);
	handshake.nombre_restaurante->nombre = nombre_restaurante->nombre;

	handshake.coordenadas.posicionX = restaurante->info_Restaurante.Posicion.posicionX;
	handshake.coordenadas.posicionY = restaurante->info_Restaurante.Posicion.posicionY;

	buffer_t* buffer = serializar_handshake_restaurante(handshake);
	sfd sfd_APP = conectar_y_enviar("IP_APP", "PUERTO_APP", buffer, HANDSHAKE_INICIAL_RESTAURANTE);
	free(buffer->stream);
	free(buffer);
	close(sfd_APP);
}

void escuchar(sfd socket){
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket, paquete);

	atender(socket, paquete->codigo_operacion,paquete->buffer);

	free(paquete);
}

void atender(sfd socket, enum_mensaje codigo_operacion,buffer_t*buffer){
	switch(codigo_operacion){
	case CONSULTAR_MODULO:
		free(buffer);
		atender_consultar_modulo(socket);
		break;
	case CONSULTAR_PLATOS:
		free(buffer->stream);
		free(buffer);
		atender_consultar_platos(socket);
		break;
	case CREAR_PEDIDO:
		free(buffer);
		atender_crear_pedido(socket);
		break;
	case ANIADIR_PLATO:
		atender_aniadir_plato(socket,buffer);
		break;
	case CONFIRMAR_PEDIDO:
		atender_confirmar_pedido(socket,buffer);
		break;
	case CONSULTAR_PEDIDO:
		free(buffer->stream);
		free(buffer);
		atender_consultar_pedido(socket);
		break;
	default:
		perror("El mensaje no se puede leer");
	}
}

void atender_consultar_modulo(sfd socket){
	buffer_t* buffer = serializar_modulo(RESTAURANTE);
	enviar_respuesta(socket, buffer, CONSULTAR_MODULO);
	free(buffer->stream);
	free(buffer);
}

void atender_consultar_platos(sfd socket){
	buffer_t* buffer_cons_platos = serializar_consultar_platos(nombre_restaurante);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer_cons_platos, CONSULTAR_PLATOS); //a sindicato

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);
	enviar_respuesta(socket, paquete->buffer , CONSULTAR_PLATOS_RTA);
}

void atender_crear_pedido(sfd socket){
	id_autogenerado++;

	buffer_t* buffer_guardar_pedido = serializar_guardar_pedido(nombre_restaurante, id_autogenerado);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer_guardar_pedido, GUARDAR_PEDIDO);

	free(buffer_guardar_pedido->stream);
	free(buffer_guardar_pedido);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);

	int32_t respuesta = deserializar_int(paquete->buffer);
	free(paquete);
	buffer_t* buffer_crear_pedido_rta;
	if(respuesta == 0){
		PY_LOG_INFO("El pedido %i fue creado por el Sindicato", id_autogenerado);

		buffer_crear_pedido_rta = serializar_int(id_autogenerado);
		enviar_respuesta(socket, buffer_crear_pedido_rta, CREAR_PEDIDO_RTA);
	}else{
		PY_LOG_INFO("El pedido %i no fue creado por el Sindicato", id_autogenerado);

		buffer_crear_pedido_rta = serializar_int(-1);
		enviar_respuesta(socket, buffer_crear_pedido_rta, CREAR_PEDIDO_RTA);
	}
	free(buffer_crear_pedido_rta->stream);
	free(buffer_crear_pedido_rta);
}

void atender_aniadir_plato(sfd socket,buffer_t*buffer){


	msg_aniadir_plato* msg = deserializar_aniadir_plato(buffer);

	buffer_t* buffer_guardar_plato = serializar_guardar_plato(nombre_restaurante, msg->id_pedido,msg->plato, 1);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer_guardar_plato, GUARDAR_PLATO);

	free(msg->plato->nombre);
	free(msg->plato);
	free(msg);

	free(buffer_guardar_plato->stream);
	free(buffer_guardar_plato);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);


	int32_t respuesta = deserializar_int(paquete->buffer);
	free(paquete);

	if(respuesta == 0){
		PY_LOG_INFO("El plato fue añadido por el Sindicato");
	}else{
		PY_LOG_INFO("El plato no fue añadido por el Sindicato");
	}

	buffer_t* buffer_aniadir_plato_rta = serializar_int(respuesta);
	enviar_respuesta(socket, buffer_aniadir_plato_rta, OK_FAIL);

	free(buffer_aniadir_plato_rta->stream);
	free(buffer_aniadir_plato_rta);
}

void atender_confirmar_pedido(sfd socket,buffer_t* buffer){
	int32_t id_pedido = deserializar_int(buffer);
	buffer_t*buffer2;

	buffer_t* buffer_obt_pedido = serializar_confirmar_pedido_con_restaurante(id_pedido, nombre_restaurante);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer_obt_pedido,CONFIRMAR_PEDIDO);

	free(buffer_obt_pedido->stream);
	free(buffer_obt_pedido);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);

	Pedido* pedido;
    int32_t valor=deserializar_int(paquete->buffer);
    free(paquete);
	buffer_t* buffer_conf_pedido_rta;

    if (valor==1){
	    PY_LOG_INFO("El pedido %i no fue confirmado por el Sindicato", id_pedido);

		buffer_conf_pedido_rta = serializar_int(1);
		enviar_respuesta(socket, buffer_conf_pedido_rta, OK_FAIL);
    }
    else if(valor==0){
		PY_LOG_INFO("El pedido %i fue confirmado por el Sindicato", id_pedido);
		msg_obtener_pedido*msg=malloc(sizeof(msg_obtener_pedido));
		msg->id_pedido=id_pedido;
		msg->restaurante=malloc(sizeof(nombre));
		msg->restaurante->largo_nombre=strlen(nombre_restaurante->nombre)+1;
		msg->restaurante->nombre=malloc(msg->restaurante->largo_nombre);
		strcpy(msg->restaurante->nombre,nombre_restaurante->nombre);
        buffer2=serializar_obtener_pedido(msg->restaurante,msg->id_pedido);
		sfd socket_sindicato2 = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer2,OBTENER_PEDIDO);
		free(msg->restaurante->nombre);
		free(msg->restaurante);
		free(msg);
		free(buffer2->stream);
		free(buffer2);
		py_paquete_t* paquete2 = malloc(sizeof(py_paquete_t));
		py_recv_msg(socket_sindicato2, paquete2);
		pedido= deserializar_pedido(paquete2->buffer);
		encolar_platos(pedido, id_pedido);
		buffer_conf_pedido_rta = serializar_int(0);
		enviar_respuesta(socket, buffer_conf_pedido_rta, OK_FAIL);
		free(paquete2);
	}
    free(buffer_conf_pedido_rta->stream);
    free(buffer_conf_pedido_rta);

}

void atender_consultar_pedido(sfd socket){

	//msg_consultar_pedido* id_pedido = deserializar_consultar_pedido(buffer);
/*
	buffer_t* buffer_obt_pedido = serializar_obtener_pedido(nombre_restaurante, id_pedido);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer_obt_pedido, OBTENER_PEDIDO);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);
	Pedido* pedido = deserializar_pedido(paquete->buffer);
*/

	Pedido* pedido = malloc(sizeof(Pedido));
	pedido->estadoPedido = CONFIRMADO;
	pedido->precioTotal = 10;
	pedido->platos = list_create();

	registroComida* comida1 = malloc(sizeof(registroComida));
	comida1->plato = malloc(sizeof(nombre));
	comida1->plato->largo_nombre = strlen("Papas") + 1;
	comida1->plato->nombre = malloc(comida1->plato->largo_nombre);
	comida1->plato->nombre = "Papas";
	comida1->cantTotal = 2;
	comida1->cantLista = 1;

	registroComida* comida2 = malloc(sizeof(registroComida));
	comida2->plato = malloc(sizeof(nombre));
	comida2->plato->largo_nombre = strlen("Pizza") + 1;
	comida2->plato->nombre = malloc(comida2->plato->largo_nombre);
	comida2->plato->nombre = "Pizza";
	comida2->cantTotal = 4;
	comida2->cantLista = 4;

	list_add(pedido->platos, (void*)comida1);
	list_add(pedido->platos, (void*)comida2);


	buffer_t* buffer_cons_pedido_rta = serializar_consultar_pedido_rta(pedido->estadoPedido, pedido->platos, nombre_restaurante);
	enviar_respuesta(socket, buffer_cons_pedido_rta, CONSULTAR_PEDIDO_RTA);
	free(buffer_cons_pedido_rta->stream);
	free(buffer_cons_pedido_rta);
}

void encolar_platos(Pedido* pedido, int32_t id_pedido){
	for(int i = 0; i < pedido->platos->elements_count; i++){
		registroComida* plato = list_get(pedido->platos, i);

		for(int j = 0; j < plato->cantTotal; j++){
			buffer_t* buffer = serializar_obtener_receta(plato->plato);
			sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer, OBTENER_RECETA); //a sindicato

			free(buffer->stream);
			free(buffer);

			py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
			py_recv_msg(socket_sindicato, paquete);
			close(socket_sindicato);

			Receta* receta = deserializar_receta(paquete->buffer);

			free(paquete);

			PCB* pcb = crearPCB(plato, id_pedido, receta);
			encolar_segun_afinidad(pcb);
		}
	}
	list_destroy(pedido->platos);
	free(pedido);
}

PCB* crearPCB(registroComida* plato, int32_t id_pedido, Receta* receta){
	PCB* pcb = malloc(sizeof(PCB));

	pcb->plato = malloc(sizeof(PlatoConReceta));
	pcb->plato->nombre = malloc(sizeof(nombre));
	pcb->plato->nombre->nombre = string_duplicate(plato->plato->nombre);
	pcb->plato->nombre->largo_nombre = plato->plato->largo_nombre;

	//pcb->plato->receta = malloc(sizeof(Receta));
	pcb->plato->receta = receta;

	pcb->estado = READY;
	pcb->id_pedido = id_pedido;
	pcb->indice_rafaga = 0;
	pcb->indice_ciclo = 0;

	PY_LOG_DEBUG("Se creo el PCB de el/la %s del pedido %i", pcb->plato->nombre->nombre, pcb->id_pedido);
	sleep(retardo_ciclo);

	//free(plato->plato->nombre);
	//free(plato->plato);
	//free(plato);

	return pcb;
}

void encolar_segun_afinidad(PCB* pcb){
	bool encolado = false;
	for(int i = 0; i < cant_afinidades - 1; i++){
		if(!strcmp(pcb->plato->nombre->nombre, cola_afinidades[i].nombre->nombre)){
			encolar_cola_ready(pcb, i);
			encolado = true;
		}
	}

	if(!encolado){
		encolar_cola_ready(pcb, cant_afinidades -1);
	}
}

void pasar_ciclo(){
	while(1){

		for(int i = 0; i < cant_cocineros; i++){
			sem_wait(&sem_cocinero_entrada[i]);
		}

		for(int i = 0; i < cant_hornos; i++){
			sem_wait(&sem_horno_entrada[i]);
		}

		sleep(retardo_ciclo);
		//PY_LOG_ERROR("CICLO");

		for(int i = 0; i < cant_cocineros; i++){
			sem_post(&sem_cocinero_salida[i]);
		}

		for(int i = 0; i < cant_hornos; i++){
			sem_post(&sem_horno_salida[i]);
		}
	}
}

void ciclo_cocinero(int indice_cocinero){
	sem_wait(&sem_cocinero_salida[indice_cocinero]);
	sem_post(&sem_cocinero_entrada[indice_cocinero]);
}

void ciclo_horno(int indice_horno){
	sem_wait(&sem_horno_salida[indice_horno]);
	sem_post(&sem_horno_entrada[indice_horno]);
}

void inicializar_planificacion(){
	pthread_t* planificacion = calloc(cant_cocineros, sizeof(pthread_t));
	pthread_t* entrada_salida = calloc(cant_hornos, sizeof(pthread_t));

	algoritmo_de_planificacion = py_config_get_string("ALGORITMO_PLANIFICACION");

	pthread_t clock;
	pthread_create(&clock, NULL, (void*)pasar_ciclo, NULL);

	for(int i = 0; i < cant_cocineros; i++){
		pthread_create(&planificacion[i], NULL, (void*)planificar, (void*)i);
	}

	for(int i = 0; i < cant_hornos; i++){
		pthread_create(&entrada_salida[i], NULL, (void*)planificar_horno, (void*)i);
	}

	for(int i = 0; i < cant_cocineros; i++){
		pthread_join(planificacion[i], NULL);
	}

	for(int i = 0; i < cant_hornos; i++){
		pthread_join(entrada_salida[i], NULL);
	}
}

void planificar(int indice_cocinero){
	int indice_cola = obtener_indice_cola(indice_cocinero);

	while(1){
		if(!cola_ready_is_empty(indice_cola)){
			PCB* plato = extraer_cola_ready(indice_cola);
			Paso* paso = list_get(plato->plato->receta->Pasos, plato->indice_rafaga);
			plato->estado = EXEC;
			realizar_paso(plato, paso, indice_cocinero);
		}else{
			ciclo_cocinero(indice_cocinero);
		}
	}
}

int obtener_indice_cola(int indice_cocinero){
	int procesadores = 0;

	for(int i = 0; i < cant_afinidades; i++){
		pthread_mutex_lock(&mutex_cola_afinidades);
		procesadores += cola_afinidades[i].cantidad_procesadores;
		pthread_mutex_unlock(&mutex_cola_afinidades);

		if(indice_cocinero + 1 <= procesadores){
			return i;
		}
	}

	return cant_afinidades - 1;
}

void planificar_horno(int indice_horno){
	while(1){
		if(!cola_blocked_is_empty()){
			PCB* plato = extraer_cola_blocked();
			Paso* paso = list_get(plato->plato->receta->Pasos, plato->indice_rafaga);
			hornear(plato, paso, indice_horno);
			paso_siguiente(plato, paso);
		}else{
			ciclo_horno(indice_horno);
		}
	}
}

void realizar_paso(PCB* plato, Paso* paso, int indice_cocinero){
	if((!strcmp(paso->nombre->nombre, "REPOSAR")) || (!strcmp(paso->nombre->nombre, "Reposar")) || (!strcmp(paso->nombre->nombre, "reposar"))){
		reposar(plato, paso, indice_cocinero);
		paso_siguiente(plato, paso);
	}else if((!strcmp(paso->nombre->nombre, "HORNEAR")) || (!strcmp(paso->nombre->nombre, "Hornear")) || (!strcmp(paso->nombre->nombre, "hornear"))){
		encolar_cola_blocked(plato);
	}else{
		otro_paso(plato, paso, indice_cocinero);
		paso_siguiente(plato, paso);
	}
}

void reposar(PCB* plato, Paso* paso, int indice_cocinero){
	sem_wait(&sem_cocinero_salida[indice_cocinero]);
	PY_LOG_TRACE("Se empieza a Reposar un/a %s del pedido %i\n", plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_cocinero_entrada[indice_cocinero]);

	plato->estado = BLOCKED;

	for(int i = 0; i < paso->tiempo; i++){
		ciclo_cocinero(indice_cocinero);
	}

	sem_wait(&sem_cocinero_salida[indice_cocinero]);
	PY_LOG_TRACE("Se termino de Reposar el/la %s del pedido %i\n", plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_cocinero_entrada[indice_cocinero]);

	plato->estado = READY;
}

void hornear(PCB* plato, Paso* paso, int indice_horno){
	sem_wait(&sem_horno_salida[indice_horno]);
	PY_LOG_TRACE("Se empieza a Hornear un/a %s del pedido %i\n", plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_horno_entrada[indice_horno]);

	plato->estado = BLOCKED;

	for(int i = 0; i < paso->tiempo; i++){
		ciclo_horno(indice_horno);
	}

	sem_wait(&sem_horno_salida[indice_horno]);
	PY_LOG_TRACE("Se termino de Hornear el/la %s del pedido %i\n", plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_horno_entrada[indice_horno]);

	plato->estado = READY;
}

void otro_paso(PCB* plato, Paso* paso, int indice_cocinero){
	sem_wait(&sem_cocinero_salida[indice_cocinero]);
	PY_LOG_TRACE("Se empieza a %s el/la %s del pedido %i\n", paso->nombre->nombre, plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_cocinero_entrada[indice_cocinero]);

	for(int i = 0; i < paso->tiempo; i++){
		ciclo_cocinero(indice_cocinero);
	}

	sem_wait(&sem_cocinero_salida[indice_cocinero]);
	PY_LOG_TRACE("Se termino de %s el/la %s del pedido %i\n", paso->nombre->nombre, plato->plato->nombre->nombre, plato->id_pedido);
	sem_post(&sem_cocinero_entrada[indice_cocinero]);

	plato->estado = READY;
}

void paso_siguiente(PCB* plato, Paso* paso){
	if(plato->indice_rafaga == plato->plato->receta->Pasos->elements_count - 1){
		plato->estado = EXIT;
		PY_LOG_WARNING("Se termino de elaborar el/la %s del pedido %i", plato->plato->nombre->nombre, plato->id_pedido);
		plato_listo(plato);
	}else{
		plato->indice_rafaga++;
		plato->indice_ciclo = 0;
		encolar_segun_afinidad(plato);
	}
}

void liberar_pcb(PCB* plato){
	free(plato->plato->nombre->nombre);
	free(plato->plato->nombre);
	for(int i = 0; i < plato->plato->receta->Pasos->elements_count; i++){
		Paso* un_paso = list_get(plato->plato->receta->Pasos, i);
		free(un_paso->nombre->nombre);
		free(un_paso->nombre);
		free(un_paso);
	}
	list_destroy(plato->plato->receta->Pasos);
	free(plato->plato->receta);
	free(plato->plato);
	free(plato);
}

void plato_listo(PCB* plato){
	buffer_t* buffer_app = serializar_plato_listo(plato->plato->nombre, nombre_restaurante, plato->id_pedido);
	sfd socket_app = conectar_y_enviar("IP_APP", "PUERTO_APP", buffer_app , PLATO_LISTO);
	//py_paquete_t* paqueteXD = malloc(sizeof(py_paquete_t));
	//py_recv_msg(socket_app, paqueteXD);
	//int32_t respuesta_app = deserializar_int(paqueteXD->buffer);
    close(socket_app);
    //if(respuesta_app == 0){
    //	printf("A LA APP LE LLEGÓ BIEN \n");
    //}
    //else{
    //	printf("A LA APP LE LLEGÓ MAL \n");
    //}

    free(buffer_app->stream);
    free(buffer_app);

	//printf("Nombre plato: %s\nNombre restaurante: %s\nId plato: %i \n", plato->plato->nombre->nombre, nombre_restaurante->nombre, plato->id_pedido);
	buffer_t* buffer = serializar_plato_listo(plato->plato->nombre, nombre_restaurante, plato->id_pedido);
	sfd socket_sindicato = conectar_y_enviar("IP_SINDICATO", "PUERTO_SINDICATO", buffer, PLATO_LISTO);

	free(buffer->stream);
	free(buffer);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(socket_sindicato, paquete);
	close(socket_sindicato);

	int32_t respuesta = deserializar_int(paquete->buffer);
	free(paquete);
	if(respuesta){
		PY_LOG_INFO("El plato listo fue rechazado por el Sindicato");
	}else{
		PY_LOG_INFO("El plato listo fue aceptado por el Sindicato");
	}
	liberar_pcb(plato);

}

bool cola_ready_is_empty(int indice_cocinero){
	pthread_mutex_lock(&mutex_cola_afinidades);
	bool result = list_is_empty(cola_afinidades[indice_cocinero].cola->elements);
	pthread_mutex_unlock(&mutex_cola_afinidades);

	return result;
}

bool cola_blocked_is_empty(){
	pthread_mutex_lock(&mutex_cola_hornos);
	bool result = list_is_empty(cola_hornos->elements);
	pthread_mutex_unlock(&mutex_cola_hornos);

	return result;
}

PCB* extraer_cola_ready(int indice_cocinero){
	pthread_mutex_lock(&mutex_cola_afinidades);
	PCB* plato = queue_pop(cola_afinidades[indice_cocinero].cola);
	pthread_mutex_unlock(&mutex_cola_afinidades);

	return plato;
}

PCB* extraer_cola_blocked(){
	pthread_mutex_lock(&mutex_cola_hornos);
	PCB* plato = (PCB*)queue_pop(cola_hornos);
	pthread_mutex_unlock(&mutex_cola_hornos);

	return plato;
}

void encolar_cola_ready(PCB* plato, int indice_afinidad){
	pthread_mutex_lock(&mutex_cola_afinidades);
	queue_push(cola_afinidades[indice_afinidad].cola, (void*)plato);
	pthread_mutex_unlock(&mutex_cola_afinidades);
}

void encolar_cola_blocked(PCB* plato){
	pthread_mutex_lock(&mutex_cola_hornos);
	queue_push(cola_hornos, (void*)plato);
	pthread_mutex_unlock(&mutex_cola_hornos);
}

uint32_t cantidad_repetidos(char* nombre){
	t_list* lista = lista_repetidos(nombre);
	return lista->elements_count;
}

t_list* lista_repetidos(char* nombre_afinidad){
	repetidos = list_create();
	for(int i = 0; i < restaurante->info_Restaurante.afinidades->elements_count; i++){
		void* element = list_get(restaurante->info_Restaurante.afinidades, i);
		if((((nombre*)element)->nombre == nombre_afinidad)){
			list_add(repetidos, element);
		}
	}
	return repetidos;
}

uint32_t calcular_cantidad_afinidades(){
	t_list* lista2 = lista_sin_repetidos();
	uint32_t cantidad = lista2->elements_count + 1;
	list_destroy(lista2);
	return cantidad;
}

t_list* lista_sin_repetidos(){
	sin_repetidos = list_create();
	for(int i = 0; i < restaurante->info_Restaurante.afinidades->elements_count; i++){
		void* element = list_get(restaurante->info_Restaurante.afinidades, i);
		if(no_se_repite((nombre*)element)){
			list_add(sin_repetidos, element);
		}
	}
	return sin_repetidos;
}

bool no_se_repite(nombre* afinidad){
	bool resultado = true;
	if(!list_is_empty(sin_repetidos)){
		for(int i = 0; i < sin_repetidos->elements_count; i++){
			nombre* otra_afinidad = list_get(sin_repetidos, i);
			if(!strcmp(afinidad->nombre, otra_afinidad->nombre)){
				resultado = false;
			}
		}
	}
	return resultado;
}
