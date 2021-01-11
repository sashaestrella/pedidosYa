#include "Swap.h"
int contadorPagina = 0;

int inicializarSwap(){
	tamanioSwap = atoi(py_config_get_string("TAMANIO_SWAP"));
	//swap = malloc(tamanioSwap); //Esto ya no se si va
	cantPaginasEnSwap = tamanioSwap / tamanioPagina;
	printf("La cantidad de paginas que entran en memoria SWAP son: %d\n",cantPaginasEnSwap);
	direccionSwap = py_config_get_string("DIRECCION_SWAP");

	paginasEnSwap = list_create();
	pedidosSwap = list_create();

	framesEnSwap = malloc(sizeof(EstadoFrame) * cantPaginasEnSwap);
	for(int i=0;i<cantPaginasEnSwap;i++){
		framesEnSwap[i] = LIBRE;
	}

	mapeo = inicializarMapeo();

	return 0;
}

void* inicializarMapeo(){
	char* direccion = malloc(strlen(direccionSwap) + 9);
	strcpy(direccion,direccionSwap);
	string_append(&direccion,"swap.bin");
	archSwap = open(direccion,O_RDWR | O_CREAT,S_IRUSR | S_IWUSR);
	ftruncate(archSwap,cantPaginasEnSwap*tamanioPagina);
	printf("Direccion del archivo Swap: %s\n",direccion);

	free(direccion);

	if(archSwap == -1){

		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_error(logger,"Error al abrir el archivo Swap");
		free(contenidoSwap);
		free(direccion);
		close(archSwap);
	}else{
		contenidoSwap = mmap(NULL,cantPaginasEnSwap*tamanioPagina+1,PROT_READ | PROT_WRITE,MAP_SHARED,archSwap,0);

		if(contenidoSwap == MAP_FAILED){

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_error(logger,"Error mapeando memoria Swap");
		}

		return contenidoSwap;
	}

}

int buscarMarcoLibre(){
	for(int marco_disponible=0; marco_disponible<cantPaginasEnSwap; marco_disponible++){
		if(framesEnSwap[marco_disponible] == LIBRE){

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_info(logger,"El marco a asignar en SWAP es el: %d", marco_disponible);
			return marco_disponible;
		}
	}
}

int guardarPaginaEnSwapNueva(nombre* nombreRestaurante,uint32_t idPedido,nombre* nombrePlato,int cantidad){
	PedidoSwap* pedido = verificarPedidoSwap(idPedido,nombreRestaurante);
	ContenidoPaginaSwap* contenido = buscarPaginaEnPedidoSwap(pedido,nombrePlato->nombre);
	if(contenido){

		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_error(logger,"La pagina %s ya se encuentra cargada en memoria",contenido->nombrePlato);
	}else{
		int marco = buscarMarcoLibre();
		Pagina* pagina = malloc(sizeof(Pagina));
		pagina->frame = marco;
		pagina->P = 0;
		pagina->U = 1;
		pagina->M = 0;
		pagina->nroPagina = contadorPagina;

		ContenidoPaginaSwap* contenidoPag = malloc(sizeof(ContenidoPaginaSwap));
		contenidoPag->cantTotal = cantidad;
		contenidoPag->cantLista = 0;
		contenidoPag->nroPagina = contadorPagina;
		char* nombre = string_duplicate(nombrePlato->nombre);
		strcpy(contenidoPag->nombrePlato,nombrePlato->nombre);

		char* cantTotal = string_itoa(cantidad);
		char* cantLista = string_itoa(0);
		char* paginaACopiar = string_new();
		string_append(&paginaACopiar,cantTotal);
		string_append(&paginaACopiar,",");
		string_append(&paginaACopiar,cantLista);
		string_append(&paginaACopiar,",");
		string_append(&paginaACopiar,nombre);

		pthread_mutex_lock(&swapPagina);
		void* stream = mapeo + (tamanioPagina*marco);
		memcpy(stream,paginaACopiar,strlen(paginaACopiar));
		stream += strlen(paginaACopiar);
		memset(stream, '\0', tamanioPagina - strlen(paginaACopiar));

		framesEnSwap[marco] = OCUPADO;
		list_add(paginasEnSwap,pagina);
		pthread_mutex_unlock(&swapPagina);

		pthread_mutex_lock(&agregarPedidoSwap);
		list_add(pedido->platos,contenidoPag);
		pthread_mutex_unlock(&agregarPedidoSwap);


		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		uint32_t posicionRelativa = (stream - mapeo) - strlen(paginaACopiar);
		log_info(logger,"Se inserto en SWAP el plato: %s en el frame: %d, donde comienza en: %u",nombrePlato->nombre,marco,posicionRelativa);

		if(msync(mapeo,strlen(paginaACopiar), MS_SYNC) < 0) {

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_error(logger,"[msync]Error al volcar los cambios a SWAP");
			return -1;
		}else{

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_info(logger,"[msync]Se agregó la pagina en SWAP exitosamente");
		}

		free(cantTotal);
		free(cantLista);
		free(nombre);
		free(paginaACopiar);
	}

	pthread_mutex_lock(&modifPagina);
	contadorPagina++;
	pthread_mutex_unlock(&modifPagina);

	return 0;
}


int actualizarPaginaEnSwapVieja(nombre* nombreRestaurante,uint32_t idPedido,uint32_t cantTotal,uint32_t cantLista,char* nombrePlato){
	//buscar esta pagina en el frame guardado y actualizar la informacion si la pag
	//tiene el bit de modificado en 1, si no queda como estaba

	PedidoSwap* pedido = buscarPedidoSwap(idPedido,nombreRestaurante);
	ContenidoPaginaSwap* pagina = buscarPaginaEnPedidoSwap(pedido,nombrePlato);
	Pagina* paginaPosible;
	for(int i=0;i<paginasEnSwap->elements_count;i++){
		paginaPosible = list_get(paginasEnSwap,i);
		if(paginaPosible->nroPagina == pagina->nroPagina){
			paginaPosible->P = 0;
			paginaPosible->U = 0;
			char** paginaALeer = leerPaginaGuardadaEnSwap(paginaPosible->frame);
			char* total = string_itoa(cantTotal);
			char* lista = string_itoa(cantLista);
			strcpy(paginaALeer[0],total);
			strcpy(paginaALeer[1],lista);

			char* paginaActualizada = string_new();
			string_append(&paginaActualizada,paginaALeer[0]);
			string_append(&paginaActualizada,",");
			string_append(&paginaActualizada,paginaALeer[1]);
			string_append(&paginaActualizada,",");
			string_append(&paginaActualizada,nombrePlato);

			pthread_mutex_lock(&swapPagina);
			void* stream = mapeo + (tamanioPagina*paginaPosible->frame);
			memcpy(stream,paginaActualizada,strlen(paginaActualizada));
			stream += strlen(paginaActualizada);
			memset(stream, '\0', tamanioPagina - strlen(paginaActualizada));
			pthread_mutex_unlock(&swapPagina);

			//char** paginaALeerDespues = leerPaginaGuardadaEnSwap(paginaPosible->frame);

			if(msync(mapeo,strlen(paginaActualizada), MS_SYNC) < 0) {

				if(logger->is_active_console == false){
					logger->is_active_console = true;
				}
				log_error(logger,"[msync]Error al volcar los cambios a SWAP");
				return -1;
			}else{

				if(logger->is_active_console == false){
					logger->is_active_console = true;
				}
				log_info(logger,"[msync]Se guardaron los cambios de la página %s en SWAP exitosamente",nombrePlato);
			}

			liberadorDeArray(paginaALeer);
			free(total);
			free(lista);
			free(paginaActualizada);
			//free(paginaALeer);
		}
	}

	return 0;
}

void liberadorDeArray(char** array){
	int contador = 0;
	while(array[contador] != NULL){
		free(array[contador]);
		contador++;
	}
	free(array);
}

char** leerPaginaGuardadaEnSwap(int frame){
	pthread_mutex_lock(&leerPagina);
	char* pagina = malloc(tamanioPagina + 1);
	memset(pagina, '\0', tamanioPagina + 1);

	void* stream = mapeo + (tamanioPagina*frame);
	memcpy(pagina,stream,tamanioPagina);
	pthread_mutex_unlock(&leerPagina);

	char** separarPagina = string_split(pagina,",");

	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_info(logger,"Se procede a leer en SWAP la página en el frame %d",frame);
	//printf("Cantidad total: %s\n Cantidad lista: %s\n Nombre plato: %s\n\n",separarPagina[0],separarPagina[1],separarPagina[2]);

	free(pagina);
	return separarPagina;
}

int obtenerPosicionDePedidoSwap(PedidoSwap* pedido){
	int posicion;
	PedidoSwap* otroPedido;

	for(int i=0; i<pedidosSwap->elements_count; i++){
		otroPedido = list_get(pedidosSwap,i);
		if(otroPedido->idPedido == pedido->idPedido){
			posicion = i;
			break;
		}
	}
	return posicion;
}

PedidoSwap* buscarPedidoSwap(uint32_t idPedido,nombre* nombreRestaurante){
	bool seEncuentraId(void* pedido){
		PedidoSwap* unPedido = (PedidoSwap*)pedido;
		return (unPedido->idPedido == idPedido) && !strcmp(unPedido->nombreRestaurante->nombre,nombreRestaurante->nombre);
	}

	PedidoSwap* pedido_buscado = list_find(pedidosSwap,(void*)seEncuentraId);
	return pedido_buscado;
}

ContenidoPaginaSwap* buscarPaginaEnPedidoSwap(PedidoSwap* pedido,char* nombrePlato){
	bool seEncuentraNombre(void* pagina){
		ContenidoPaginaSwap* contenido = (ContenidoPaginaSwap*)pagina;
		return (strcmp(contenido->nombrePlato,nombrePlato)==0);
	}

	ContenidoPaginaSwap* pagina_buscada = list_find(pedido->platos,(void*)seEncuentraNombre);
	return pagina_buscada;
}

Pagina* buscarPaginaEnPaginasSwap(int nroDePagina){
	bool seEncuentra(void* pagina){
		Pagina* contenido = (Pagina*)pagina;
		return (contenido->nroPagina == nroDePagina);
	}

	Pagina* pagina_buscada = list_find(paginasEnSwap,(void*)seEncuentra);
	return pagina_buscada;
}

PedidoSwap* verificarPedidoSwap(uint32_t idPedido,nombre*nombreRestaurante){
	PedidoSwap* pedido = buscarPedidoSwap(idPedido,nombreRestaurante);
	if(pedido){
		return pedido;
	}else{
		PedidoSwap* pedidoSwap = malloc(sizeof(PedidoSwap));
		pedidoSwap->idPedido = idPedido;
		pedidoSwap->nombreRestaurante=nombreRestaurante;
		pedidoSwap->platos = list_create();
		pthread_mutex_lock(&agregarPedidoSwap);
		list_add(pedidosSwap,pedidoSwap);
		pthread_mutex_unlock(&agregarPedidoSwap);

		return pedidoSwap;
	}
}

Pedido* armarPedidoAEnviar(nombre* nombreRestaurante,uint32_t idPedido){
	PedidoComanda* pedidoComanda = obtenerPedidoDePedidosComanda(idPedido,nombreRestaurante);
	PedidoSwap* pedidoSwap = buscarPedidoSwap(idPedido,nombreRestaurante);

	t_list* platosAEnviar = list_create();
	ContenidoPaginaSwap* contenidoPag;
	for(int i=0;i<pedidoSwap->platos->elements_count;i++){
		contenidoPag = list_get(pedidoSwap->platos,i);

		registroComida* registro = malloc(sizeof(registroComida));
		registro->cantTotal = contenidoPag->cantTotal;
		registro->cantLista = contenidoPag->cantLista;
		registro->plato = malloc(sizeof(nombre));
		registro->plato->largo_nombre = strlen(contenidoPag->nombrePlato)+1;
		registro->plato->nombre = malloc(registro->plato->largo_nombre);
		//registro->plato->nombre = contenidoPag->nombrePlato;
		strcpy(registro->plato->nombre,contenidoPag->nombrePlato);

		list_add(platosAEnviar,registro);
	}

	Pedido* pedidoAEnviar = malloc(sizeof(Pedido));
	pedidoAEnviar->estadoPedido = pedidoComanda->estadoPedido;
	pedidoAEnviar->platos = platosAEnviar;
	return pedidoAEnviar;
}

int buscarPosicionPaginaEnLista(Pagina* pagina){
	int posicion;

	for(int i=0; i<paginasEnSwap->elements_count; i++){
		Pagina* pagina_a_chequear = list_get(paginasEnSwap,i);
		if(pagina->nroPagina == pagina_a_chequear->nroPagina){
			posicion = i;
			break;
		}
	}
	return posicion;
}

void eliminarPaginasDeSwap(nombre* nombreRestaurante,uint32_t idPedido){
	PedidoSwap* pedido = buscarPedidoSwap(idPedido,nombreRestaurante);
	for(int i=0;i<pedido->platos->elements_count;i++){
		ContenidoPaginaSwap* contenido = list_get(pedido->platos,i);
		Pagina* paginaBuscada = buscarPaginaEnPaginasSwap(contenido->nroPagina);
		int posicion = buscarPosicionPaginaEnLista(paginaBuscada);
		Pagina* paginaABorrar = list_remove(paginasEnSwap,posicion);
		framesEnSwap[paginaABorrar->frame] = LIBRE;
		free(paginaABorrar);
		free(contenido);
	}

	//free(pedido->nombreRestaurante->nombre);
	//free(pedido->nombreRestaurante);
	free(pedido);
}

int todosLosFramesEstanOcupados(){
	t_list* frames = list_create();
	for(int i=0;i<cantPaginasEnSwap;i++){
		if(framesEnSwap[i] == OCUPADO){
			list_add(frames,&i);
		}
	}

	if(list_size(frames) == cantPaginasEnSwap){
		list_destroy(frames);
		return 0;
	}

	list_destroy(frames);
	return 1;
}
