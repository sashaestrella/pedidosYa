#include "MemoriaPrincipal.h"

TablaDeSegmentos* crearTablaDeSegmentos(nombre* nombreRestaurante){
	pthread_mutex_lock(&mutexTablasDeSegmentos);
	char* nombreTabla = strdup(nombreRestaurante->nombre);
	TablaDeSegmentos* tabla = malloc(sizeof(TablaDeSegmentos));
	tabla->nombreRestaurante = malloc(sizeof(nombre));
	tabla->nombreRestaurante->nombre = nombreTabla;
	tabla->segmentos = list_create();

	list_add(tablasDeSegmentos,tabla);
	pthread_mutex_unlock(&mutexTablasDeSegmentos);

	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_debug(logger,"Se creó la tabla de segmentos del restaurante -%s-", nombreRestaurante->nombre);

	return tabla;
}

TablaDeSegmentos* buscarTablaDeSegmentos(nombre* nombreRestaurante){
	bool seEncuentraNombre(void* tabla){
		TablaDeSegmentos* contenido = (TablaDeSegmentos*)tabla;
		return (strcmp(contenido->nombreRestaurante->nombre,nombreRestaurante->nombre) == 0);
	}

	TablaDeSegmentos* tabla_buscada = (TablaDeSegmentos*)list_find(tablasDeSegmentos,(void*)seEncuentraNombre);

	return tabla_buscada;
}

//buscar el segmento en la tabla de segmentos y lo retorna
Segmento* buscarSegmento(uint32_t idBuscado,TablaDeSegmentos* tabla){
	bool  seEncuentraNombre(void* segmento){
		Segmento* segmento_a_buscar = (Segmento*) segmento;
		return  segmento_a_buscar->idPedido == idBuscado;
	}

	Segmento* segmento_buscado = (Segmento*)list_find(tabla->segmentos,(void*)seEncuentraNombre);
	return segmento_buscado;
}

//busca la pagina en la tabla de paginas del segmento y la retorna
Pagina* buscarPagina(char* nombrePlato, Segmento* segmento_buscado){
	bool retorno = false;
	bool seEncuentraNombrePlato(void* pagina){
		Pagina* unaPagina = (Pagina*)pagina;
		char* nombrePlatoSacadoDeMemoria = malloc(24);
		memcpy(nombrePlatoSacadoDeMemoria,memoria + (unaPagina->frame * tamanioPagina) + sizeof(uint32_t)*2,24);
		if(strcmp(nombrePlatoSacadoDeMemoria,nombrePlato)==0){
			retorno = true;
		}
		free(nombrePlatoSacadoDeMemoria);
		return retorno;
	}

	Pagina* pagina_buscada = (Pagina*)list_find(segmento_buscado->tablaDePaginas.paginas,seEncuentraNombrePlato);
	return pagina_buscada;
}


uint32_t get_timestamp(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	uint32_t  timestamp = (uint32_t)((tv.tv_sec)*1000 + (tv.tv_usec)/1000);
	return timestamp;
}

int aplicarLRU(){
	uint32_t tiempoMasViejo = get_timestamp();
	int marco_libre;
	TablaDeSegmentos* tabla;
	Segmento* segmentoConPagMasVieja;
	Segmento* segmentoAEvaluar;
	Pagina* paginaAEvaluar;
	TablaDeSegmentos* tablaADevolver;
	int posicion;

	for(int i=0;i<tablasDeSegmentos->elements_count;i++){
		tabla = list_get(tablasDeSegmentos,i);
		for(int j=0;j<tabla->segmentos->elements_count;j++){
			segmentoAEvaluar = list_get(tabla->segmentos,j);
			for(int z=0;z<segmentoAEvaluar->tablaDePaginas.paginas->elements_count;z++){
				paginaAEvaluar = list_get(segmentoAEvaluar->tablaDePaginas.paginas,z);
				if(paginaAEvaluar->timestamp < tiempoMasViejo){
					tiempoMasViejo = paginaAEvaluar->timestamp;
					segmentoConPagMasVieja = segmentoAEvaluar;
					tablaADevolver = tabla;
					posicion = z;
				}
			}
		}
	}


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_info(logger,"Se eliminará una página por LRU");
	//encontre la pag mas vieja, procedo a liberar el frame
	Pagina* paginaARemover = (Pagina*)list_remove(segmentoConPagMasVieja->tablaDePaginas.paginas,posicion);

	pthread_mutex_lock(&mutexMemoria);
	void* base = memoria + (paginaARemover->frame * tamanioPagina);
	uint32_t cantTotal;
	memcpy(&cantTotal,base, sizeof(uint32_t));
	base += sizeof(uint32_t);
	uint32_t cantLista;
	memcpy(&cantLista,base,sizeof(uint32_t));
	base += sizeof(uint32_t);
	char* nombrePlato = malloc(24);
	memcpy(nombrePlato,base,24);
	base += 24;
	pthread_mutex_unlock(&mutexMemoria);


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	uint32_t posicionRelativa = (base - memoria) - tamanioPagina;
	log_debug(logger,"Se eliminó por LRU el frame donde comenzaba en: %u",posicionRelativa);

	//mandar a swap paginaARemover
	if(paginaARemover->M == 1){
		actualizarPaginaEnSwapVieja(tablaADevolver->nombreRestaurante,segmentoConPagMasVieja->idPedido,cantTotal,cantLista,nombrePlato);
	}

	estadosFrames[paginaARemover->frame] = LIBRE;
	marco_libre = paginaARemover->frame;

	free(nombrePlato);
	free(paginaARemover);
	return marco_libre;
}


int aplicarClockMejorado(){
	int marco_libre;
	TablaDeSegmentos* tablaAEvaluar;
	TablaDeSegmentos* tabla;
	Segmento* segmentoAEvaluar;
	Segmento* segmento;
	Pagina* paginaAEvaluar;
	Pagina* pagina;

	t_list* listaAIterar = list_create();

	for(int i=0;i<tablasDeSegmentos->elements_count;i++){
		tablaAEvaluar = list_get(tablasDeSegmentos,i);
			for(int j=0;j<tablaAEvaluar->segmentos->elements_count;j++){
				segmentoAEvaluar = list_get(tablaAEvaluar->segmentos,j);
					for(int z=0;z<segmentoAEvaluar->tablaDePaginas.paginas->elements_count;z++){
						paginaAEvaluar = list_get(segmentoAEvaluar->tablaDePaginas.paginas,z);
						estructuraClock* estructura = malloc(sizeof(estructuraClock));
						estructura->pagina = paginaAEvaluar;
						estructura->segmento = segmentoAEvaluar;
						estructura->tabla = tablaAEvaluar;
						list_add(listaAIterar,estructura);
					}
			}
	}

	//printf("LA LISTA TIENE %d frames\n",list_size(listaAIterar));

	bool esMayorQue(void* estructura1,void* estructura2){
		estructuraClock* estructura = (estructuraClock*)estructura1;
		estructuraClock* otraEstructura = (estructuraClock*)estructura2;
		return estructura->pagina->frame < otraEstructura->pagina->frame;
	}
	list_sort(listaAIterar,esMayorQue);

	estructuraClock* unaEstructura;
	int salir = 1;
	while(salir == 1){
		for(int x=0;x<listaAIterar->elements_count;x++){
			unaEstructura = list_get(listaAIterar,x);
			//printf("FRAME %d, BIT DE USO: %d y BIT DE MODIF %d\n",unaEstructura->pagina->frame,unaEstructura->pagina->U,unaEstructura->pagina->M);
			if((unaEstructura->pagina->U == 0) && (unaEstructura->pagina->M == 0)){
				tabla = unaEstructura->tabla;
				segmento = unaEstructura->segmento;
				pagina = unaEstructura->pagina;
				salir = 0;
				break;
			}
		}

		if(salir == 0){
			break;
		}

		for(int y=0;y<listaAIterar->elements_count;y++){
			unaEstructura = list_get(listaAIterar,y);
			if((unaEstructura->pagina->U == 0) && (unaEstructura->pagina->M == 1)){
				tabla = unaEstructura->tabla;
				segmento = unaEstructura->segmento;
				pagina = unaEstructura->pagina;
				salir = 0;
				break;
			}else if(unaEstructura->pagina->U == 1){
				unaEstructura->pagina->U = 0;
				//printf("FRAME %d, BIT DE USO: %d y BIT DE MODIF %d\n",unaEstructura->pagina->frame,unaEstructura->pagina->U,unaEstructura->pagina->M);
			}
		}

		if(salir == 0){
			break;
		}
	}

	Segmento* segmentoBuscado = buscarSegmento(segmento->idPedido,tabla);
	//printf("ID SEGMENTO BUSCADO %d\n",segmentoBuscado->idPedido);
	int posicion = buscarPosicionPaginaEnTabla(segmentoBuscado,pagina);

	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_info(logger,"Se eliminará una página por CLOCK MEJORADO");
	Pagina* paginaARemover = (Pagina*)list_remove(segmentoBuscado->tablaDePaginas.paginas,posicion);
	//printf("FRAME DE PAGINA REMOVIDA %d\n",paginaARemover->frame);

	pthread_mutex_lock(&mutexMemoria);
	void* base = memoria + (paginaARemover->frame * tamanioPagina);
	uint32_t cantTotal;
	memcpy(&cantTotal,base, sizeof(uint32_t));
	base += sizeof(uint32_t);
	uint32_t cantLista;
	memcpy(&cantLista,base,sizeof(uint32_t));
	base += sizeof(uint32_t);
	char* nombrePlato = malloc(24);
	memcpy(nombrePlato,base,24);
	base += 24;
	pthread_mutex_unlock(&mutexMemoria);


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	uint32_t posicionRelativa = (base - memoria) - tamanioPagina;
	log_debug(logger,"Se eliminó por CLOCK-MEJ el frame donde comenzaba en: %u",posicionRelativa);

	//mandar a swap paginaARemover
	if(paginaARemover->M == 1){
		actualizarPaginaEnSwapVieja(tabla->nombreRestaurante,segmento->idPedido,cantTotal,cantLista,nombrePlato);
	}

	estadosFrames[paginaARemover->frame] = LIBRE;
	marco_libre = paginaARemover->frame;

	free(nombrePlato);
	free(paginaARemover);
	list_destroy_and_destroy_elements(listaAIterar,&free);
	return marco_libre;
}

int buscarPaginaLibre(){
	for(int marco_disponible=0; marco_disponible<cantDePagSegunTamMemoria; marco_disponible++){
		if(estadosFrames[marco_disponible] == LIBRE){

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_info(logger,"El marco a asignar en memoria es el: %d", marco_disponible);
			return marco_disponible;
		}

	}


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_info(logger,"Todos los marcos se encuentran ocupados por lo que se deberá aplicar el algoritmo dado por configuracion");

	int marco;
	//elimino la pagina de memoria y la tengo que mandar a swap
	if(!strcmp(algoritmoReemplazo,"LRU")){
		marco = aplicarLRU();

		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"El marco a asignar en memoria es el: %d", marco);
		return marco;
	}else if(!strcmp(algoritmoReemplazo,"CLOCK_MEJ")){
		marco = aplicarClockMejorado();

		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"El marco a asignar en memoria es el: %d", marco);
		return marco;
	}
	return -1;
}

//agrego el segmento si no se encuentra en memoria
int administrarGuardarPedido(TablaDeSegmentos* tabla,uint32_t idPedido){
	Segmento* segmento = buscarSegmento(idPedido,tabla);

	if(segmento){
		logger->is_active_console = false;
		log_error(logger,"El segmento del restaurante -%s- con id %d ya existe",tabla->nombreRestaurante->nombre,idPedido);

		return 1;
	}else{
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		pthread_mutex_lock(&mutexSegmentos);
		list_add(tabla->segmentos,otroSegmento);
		pthread_mutex_unlock(&mutexSegmentos);

		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = PENDIENTE;
		pedido->nombreRestaurante = tabla->nombreRestaurante;
		pthread_mutex_lock(&agregarPedidoComanda);
		list_add(pedidosComanda,pedido);
		pthread_mutex_unlock(&agregarPedidoComanda);
		verificarPedidoSwap(idPedido,tabla->nombreRestaurante);

		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_debug(logger,"Se agregó el pedido %d del restaurante %s correctamente",idPedido,tabla->nombreRestaurante->nombre);

		return 0;
	}

}


void copiarPaginaEnFrame(uint32_t cantTotal, uint32_t cantLista, char* nombrePlato, uint32_t nroFrame){
	void* base = memoria + nroFrame * tamanioPagina;
	memcpy(base, &cantTotal, sizeof(uint32_t));
	base += sizeof(uint32_t);
	memcpy(base, &cantLista, sizeof(uint32_t));
	base += sizeof(uint32_t);
	memcpy(base, nombrePlato,24);
	base += 24;


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	uint32_t posicionRelativa = (base - memoria) - tamanioPagina;
	log_debug(logger,"Se insertó en memoria el plato: %s en el frame: %d, donde comienza en: %u",nombrePlato,nroFrame,posicionRelativa);

}

//agrego el segmento si no se encuentra en memoria y agrego la pagina que quiero agregar
int administrarGuardarPlato(TablaDeSegmentos* tabla,uint32_t idPedido,nombre* nombrePlato,uint32_t cantPlatos){
	int marco_disponible;
	Pagina* pagina_buscada;
	Segmento* segmento = buscarSegmento(idPedido,tabla);
	if(segmento){
		PedidoComanda* pedido = obtenerPedidoDePedidosComanda(idPedido,tabla->nombreRestaurante);
		if(pedido->estadoPedido == PENDIENTE){
			pagina_buscada = buscarPagina(nombrePlato->nombre,segmento);
			if(pagina_buscada){
				pthread_mutex_lock(&cantTotalDePlatos);
				uint32_t cantidadTotal;
				memcpy(&cantidadTotal,memoria + pagina_buscada->frame*tamanioPagina, sizeof(uint32_t));
				cantidadTotal += cantPlatos;
				memcpy(memoria + pagina_buscada->frame*tamanioPagina,&cantidadTotal,sizeof(uint32_t));
				pagina_buscada->M=1;
				pthread_mutex_unlock(&cantTotalDePlatos);

				pthread_mutex_lock(&contPaginaCantTotal);
				PedidoSwap* pedido = buscarPedidoSwap(idPedido,tabla->nombreRestaurante);
				ContenidoPaginaSwap* paginaEnSwap = buscarPaginaEnPedidoSwap(pedido,nombrePlato->nombre);
				paginaEnSwap->cantTotal += cantPlatos;
				pthread_mutex_unlock(&contPaginaCantTotal);

				//actualizarTimestamp(segmento,nombrePlato->nombre);
				pthread_mutex_lock(&timestamp);
				uint32_t timestampNuevo = get_timestamp();
				pagina_buscada->timestamp = timestampNuevo;
				//printf("Se actualizó el timestamp a %d de la página\n",pagina_buscada->timestamp);
				pthread_mutex_unlock(&timestamp);


				if(logger->is_active_console == false){
					logger->is_active_console = true;
				}
				log_info(logger,"Se actualizó la cantidad total a %d de la página %s del pedido %d, restaurante: %s",paginaEnSwap->cantTotal,nombrePlato->nombre,idPedido,tabla->nombreRestaurante->nombre);

				return 0;
			} else {
				//como la pag no esta en memoria la busco en swap
				PedidoSwap* pedido = buscarPedidoSwap(idPedido,tabla->nombreRestaurante);
				if(pedido){
					ContenidoPaginaSwap* paginaEnSwap = buscarPaginaEnPedidoSwap(pedido,nombrePlato->nombre);
					if(paginaEnSwap){
						//si esta en swap la traigo

						if(logger->is_active_console == false){
							logger->is_active_console = true;
						}
						log_info(logger,"La página %s del pedido %d está cargada en SWAP, procedo a traerla a memoria principal",nombrePlato->nombre,idPedido);

						marco_disponible = buscarPaginaLibre();
						/*
						pthread_mutex_lock(&timestamp);
						contadorTimestamp++;
						pthread_mutex_unlock(&timestamp);*/
						Pagina* paginaEnMemoria = malloc(sizeof(Pagina));
						paginaEnMemoria->frame = marco_disponible;
						paginaEnMemoria->M = 0;
						paginaEnMemoria->U = 1;
						paginaEnMemoria->P = 1;

						pthread_mutex_lock(&timestamp);
						uint32_t timestampNuevo = get_timestamp();
						paginaEnMemoria->timestamp = timestampNuevo;
						//printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria->timestamp);
						pthread_mutex_unlock(&timestamp);

						pthread_mutex_lock(&mutexMemoria);
						ContenidoPagina* contPagina = malloc(sizeof(ContenidoPagina));
						contPagina->cantTotal = (paginaEnSwap->cantTotal) + cantPlatos;
						contPagina->cantLista = paginaEnSwap->cantLista;
						char* nombre = string_duplicate(paginaEnSwap->nombrePlato);
						strcpy(contPagina->nombrePlato,nombre);

						copiarPaginaEnFrame(contPagina->cantTotal,contPagina->cantLista,contPagina->nombrePlato,marco_disponible);
						estadosFrames[marco_disponible] = OCUPADO;
						list_add(segmento->tablaDePaginas.paginas, paginaEnMemoria);
						pthread_mutex_unlock(&mutexMemoria);


						if(logger->is_active_console == false){
							logger->is_active_console = true;
						}
						log_info(logger,"Se añadió el plato %s en la tabla de páginas del pedido con id %d",nombrePlato->nombre,idPedido);

						free(nombre);
						free(contPagina);
						return 0;
					}else{
						// si no esta la cargo en ambos lados

						if(logger->is_active_console == false){
							logger->is_active_console = true;
						}
						log_info(logger,"La página %s del pedido %d no está cargada en SWAP, procedo a cargarla en SWAP y en memoria principal",nombrePlato->nombre,idPedido);

						guardarPaginaEnSwapNueva(tabla->nombreRestaurante,idPedido,nombrePlato,cantPlatos);

						marco_disponible = buscarPaginaLibre();
						/*
						pthread_mutex_lock(&timestamp);
						contadorTimestamp++;
						pthread_mutex_unlock(&timestamp);*/
						Pagina* paginaEnMemoria = malloc(sizeof(Pagina));
						paginaEnMemoria->frame = marco_disponible;
						paginaEnMemoria->M = 0;
						paginaEnMemoria->U = 1;
						paginaEnMemoria->P = 1;

						pthread_mutex_lock(&timestamp);
						uint32_t timestampNuevo = get_timestamp();
						paginaEnMemoria->timestamp = timestampNuevo;
						//printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria->timestamp);
						pthread_mutex_unlock(&timestamp);

						pthread_mutex_lock(&mutexMemoria);
						ContenidoPagina* contPagina = malloc(sizeof(ContenidoPagina));
						contPagina->cantTotal = cantPlatos;
						contPagina->cantLista = 0;
						strcpy(contPagina->nombrePlato,nombrePlato->nombre);

						copiarPaginaEnFrame(contPagina->cantTotal,contPagina->cantLista,contPagina->nombrePlato,marco_disponible);
						estadosFrames[marco_disponible] = OCUPADO;
						list_add(segmento->tablaDePaginas.paginas, paginaEnMemoria);
						pthread_mutex_unlock(&mutexMemoria);


						if(logger->is_active_console == false){
							logger->is_active_console = true;
						}
						log_info(logger,"Se añadió el plato %s en la tabla de páginas del pedido %d, restaurante: %s",nombrePlato->nombre,idPedido,tabla->nombreRestaurante->nombre);

						free(contPagina);
						return 0;
					}
				}
			}
		}else{

			logger->is_active_console = false;
			log_error(logger,"El pedido con id %d del restaurante -%s- no se encuentra en estado PENDIENTE",idPedido,tabla->nombreRestaurante->nombre);
			return 1;
		}
	}else{

		logger->is_active_console = false;
		log_error(logger,"El segmento del restaurante -%s- con id %d no existe",tabla->nombreRestaurante->nombre,idPedido);
		return 1;
	}
return -1;
}

PedidoComanda* obtenerPedidoDePedidosComanda(uint32_t idPedido,nombre* nombreRestaurante){
	PedidoComanda* pedido;
	for(int i=0;i<pedidosComanda->elements_count;i++){
		pedido = list_get(pedidosComanda,i);
		if((pedido->idPedido == idPedido) && !strcmp(pedido->nombreRestaurante->nombre,nombreRestaurante->nombre)){
			return pedido;
		}
	}
	return pedido;
}


void cargarEnFramesLosPlatosDelPedido(TablaDeSegmentos* tabla,Segmento* segmento){
	int marco_disponible;
	Pagina* paginaACargar;
	Pagina* paginaBuscada;
	ContenidoPaginaSwap* contenidoPaginaEnSwap;

	PedidoSwap* pedido = buscarPedidoSwap(segmento->idPedido,tabla->nombreRestaurante);

	for(int i=0;i<pedido->platos->elements_count;i++){
		contenidoPaginaEnSwap = list_get(pedido->platos,i);
		paginaBuscada = buscarPagina(contenidoPaginaEnSwap->nombrePlato,segmento);
		if(paginaBuscada){

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_warning(logger,"La página %s del pedido %d ya se encuentra cargada en memoria",contenidoPaginaEnSwap->nombrePlato,segmento->idPedido);
		}else{
			marco_disponible = buscarPaginaLibre();

			paginaACargar = malloc(sizeof(Pagina));
			paginaACargar->frame = marco_disponible;
			paginaACargar->M = 0;
			paginaACargar->P = 1;
			paginaACargar->U = 1;
			/*
			pthread_mutex_lock(&timestamp);
			contadorTimestamp++;
			pthread_mutex_unlock(&timestamp);*/
			pthread_mutex_lock(&timestamp);
			uint32_t timestampNuevo = get_timestamp();
			paginaACargar->timestamp = timestampNuevo;
			//printf("Se actualizó el timestamp a %d de la página\n",paginaACargar->timestamp);
			pthread_mutex_unlock(&timestamp);
			//paginaACargar->timestamp = contadorTimestamp;

			ContenidoPagina* contenidoAGrabar = malloc(sizeof(ContenidoPagina));
			contenidoAGrabar->cantTotal = contenidoPaginaEnSwap->cantTotal;
			contenidoAGrabar->cantLista = contenidoPaginaEnSwap->cantLista;
			char* nombre = string_duplicate(contenidoPaginaEnSwap->nombrePlato);
			strcpy(contenidoAGrabar->nombrePlato,nombre);

			pthread_mutex_lock(&mutexMemoria);
			copiarPaginaEnFrame(contenidoAGrabar->cantTotal,contenidoAGrabar->cantLista,contenidoAGrabar->nombrePlato,marco_disponible);

			estadosFrames[marco_disponible] = OCUPADO;
			list_add(segmento->tablaDePaginas.paginas, paginaACargar);
			pthread_mutex_unlock(&mutexMemoria);

			free(nombre);
			free(contenidoAGrabar);
		}
	}


	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_debug(logger,"Se encuentran cargados en memoria todos los platos del pedido %d, restaurante: %s",segmento->idPedido,tabla->nombreRestaurante->nombre);
}

void pasarLosPlatosAListos(PedidoSwap* pedido,Segmento* unSegmento){
	ContenidoPaginaSwap* contenidoPag;
	for(int i=0;i<pedido->platos->elements_count;i++){
		contenidoPag = list_get(pedido->platos,i);
		Pagina* unaPagina = buscarPagina(contenidoPag->nombrePlato,unSegmento);
		pthread_mutex_lock(&cantTotalDePlatos);
		uint32_t cantidadTotal;
		memcpy(&cantidadTotal,memoria + unaPagina->frame*tamanioPagina, sizeof(uint32_t));
		uint32_t cantidadLista;
		memcpy(&cantidadLista,memoria + unaPagina->frame*tamanioPagina + sizeof(uint32_t),sizeof(uint32_t));
		cantidadLista = cantidadTotal;
		memcpy(memoria + unaPagina->frame*tamanioPagina + sizeof(uint32_t),&cantidadLista,sizeof(uint32_t));
		unaPagina->M=1;
		pthread_mutex_unlock(&cantTotalDePlatos);

		pthread_mutex_lock(&contPaginaCantTotal);
		contenidoPag->cantLista = contenidoPag->cantTotal;
		pthread_mutex_unlock(&contPaginaCantTotal);
	}
}

int administrarConfirmarPedido(TablaDeSegmentos* tabla,uint32_t idPedido){
	Segmento* unSegmento = buscarSegmento(idPedido,tabla);

	if(unSegmento){
		PedidoComanda* unPedido = obtenerPedidoDePedidosComanda(idPedido,tabla->nombreRestaurante);

		if(!strcmp(tabla->nombreRestaurante->nombre,"RestauranteDefault")){
			PedidoSwap* pedidoSwap = buscarPedidoSwap(idPedido,tabla->nombreRestaurante);
			pasarLosPlatosAListos(pedidoSwap,unSegmento);

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_info(logger,"Los platos se encuentran listos");
		}

		if(unPedido->estadoPedido != PENDIENTE){

			logger->is_active_console = false;
			log_error(logger,"El pedido %d no se encuentra en estado pendiente",idPedido);
			return 1;
		}else{
			pthread_mutex_lock(&estadoPedido);
			unPedido->estadoPedido = CONFIRMADO;
			pthread_mutex_unlock(&estadoPedido);

			if(logger->is_active_console == false){
				logger->is_active_console = true;
			}
			log_debug(logger,"Se cambió el estado del pedido %d a CONFIRMADO",unSegmento->idPedido);
			return 0;
		}
	}else{

		logger->is_active_console = false;
		log_error(logger,"El segmento del restaurante -%s- con id %d no existe",tabla->nombreRestaurante->nombre,idPedido);
		return 1;
	}
}
		

int administrarPlatoListo(TablaDeSegmentos* tabla,uint32_t idPedido,nombre* unNombre){
	Segmento* unSegmento = buscarSegmento(idPedido,tabla);

	if(unSegmento){
		cargarEnFramesLosPlatosDelPedido(tabla,unSegmento);
		Pagina* unaPagina = buscarPagina(unNombre->nombre,unSegmento);
		if(unaPagina){
			PedidoComanda* pedidoComanda = obtenerPedidoDePedidosComanda(idPedido,tabla->nombreRestaurante);
			if(pedidoComanda->estadoPedido != CONFIRMADO){
				printf("EL ESTADO ES %i \n", pedidoComanda->estadoPedido);
				logger->is_active_console = false;
				log_error(logger,"El pedido %d no se encuentra en estado confirmado",idPedido);
				return 1;
			}else{
				//si el pedido esta en Confirmado aumentar en uno la cantidadlista del plato
				pthread_mutex_lock(&cantListaDePlatos);
				uint32_t cantidadLista;
				memcpy(&cantidadLista, memoria + (tamanioPagina * unaPagina->frame)+ sizeof(uint32_t), sizeof(uint32_t));
				cantidadLista++;
				memcpy(memoria + (tamanioPagina * unaPagina->frame)+ sizeof(uint32_t),&cantidadLista,sizeof(uint32_t));
				unaPagina->M = 1;
				pthread_mutex_unlock(&cantListaDePlatos);

				pthread_mutex_lock(&timestamp);
				uint32_t timestampNuevo = get_timestamp();
				unaPagina->timestamp = timestampNuevo;
				//printf("Se actualizó el timestamp de la página %s a %d\n",unNombre->nombre,unaPagina->timestamp);
				pthread_mutex_unlock(&timestamp);

				//actualizarTimestamp(unSegmento,unNombre->nombre);

				pthread_mutex_lock(&contPaginaCantLista);
				PedidoSwap* pedido = buscarPedidoSwap(idPedido,tabla->nombreRestaurante);
				ContenidoPaginaSwap* paginaEnSwap = buscarPaginaEnPedidoSwap(pedido,unNombre->nombre);
				paginaEnSwap->cantLista++;
				pthread_mutex_unlock(&contPaginaCantLista);


				if(logger->is_active_console == false){
					logger->is_active_console = true;
				}
				log_info(logger,"Se actualizó la cantidad lista a %d de la página %s del pedido %d, restaurante: %s",paginaEnSwap->cantLista,unNombre->nombre,idPedido,tabla->nombreRestaurante->nombre);

				//y en caso de que todos los platos esten listos cambio el estado a Terminado
				int loQueDevuelve = platosListosDelPedido(unSegmento);
				if(loQueDevuelve == 0){
					pthread_mutex_lock(&estadoPedido);
					//pedidoComanda->estadoPedido = TERMINADO;
					pthread_mutex_unlock(&estadoPedido);

					logger->is_active_console = true;
					log_trace(logger,"Se actualizo el estado del pedido %d a TERMINADO",idPedido);
				}
				return 0;
			}
		}else{

			logger->is_active_console = false;
			log_error(logger,"La página -%s- no existe en el pedido con id %d",unNombre->nombre,idPedido);
			return 1;
		}
	}else{

		logger->is_active_console = false;
		log_error(logger,"El segmento del restaurante -%s- con id %d no existe",tabla->nombreRestaurante->nombre,idPedido);
		return 1;
	}
}

int platosListosDelPedido(Segmento* segmento){
	Pagina* unaPagina;
	t_list* platoListo = list_create();
	for(int i=0;i<segmento->tablaDePaginas.paginas->elements_count;i++){
		unaPagina = list_get(segmento->tablaDePaginas.paginas,i);
		uint32_t cantidadTotal;
		memcpy(&cantidadTotal, memoria + (tamanioPagina * unaPagina->frame), sizeof(uint32_t));
		uint32_t cantidadLista;
		memcpy(&cantidadLista, memoria + (tamanioPagina * unaPagina->frame)+ sizeof(uint32_t), sizeof(uint32_t));
		if(cantidadLista == cantidadTotal){
			list_add(platoListo,unaPagina);
		}
	}

	if(segmento->tablaDePaginas.paginas->elements_count == list_size(platoListo)){
		list_destroy(platoListo);
		return 0;
	} else {
		list_destroy(platoListo);
		return 1;
	}
}

void eliminarPaginasDeComanda(Segmento* segmento,TablaDeSegmentos* tabla){
	int posicion = buscarPosicionSegmentoEnTabla(segmento->idPedido,tabla);
	Segmento* segmentoABorrar = list_remove(tabla->segmentos,posicion);
	for(int i=0; i<segmento->tablaDePaginas.paginas->elements_count; i++){
		Pagina* pagina = list_get(segmentoABorrar->tablaDePaginas.paginas,i);
		estadosFrames[pagina->frame] = LIBRE;
	}

	list_destroy_and_destroy_elements(segmentoABorrar->tablaDePaginas.paginas,&free);
	free(segmentoABorrar);
}

int administrarFinalizarPedido(TablaDeSegmentos* tabla,uint32_t idPedido){
	Segmento* unSegmento = buscarSegmento(idPedido,tabla);
	if(unSegmento == NULL){
		logger->is_active_console = false;
		log_error(logger,"El segmento del restaurante -%s- con id %d no existe",tabla->nombreRestaurante->nombre,idPedido);
		return 1;
	}

	pthread_mutex_lock(&agregarPedidoComanda);
	eliminarPaginasDeComanda(unSegmento,tabla);
	PedidoComanda* pedido = obtenerPedidoDePedidosComanda(idPedido,tabla->nombreRestaurante);
	int posicion = buscarPosicionPedidoComanda(pedido);
	PedidoComanda* pedidoABorrar = list_remove(pedidosComanda,posicion);
	//free(pedidoABorrar->nombreRestaurante->nombre);
	//free(pedidoABorrar->nombreRestaurante);
	free(pedidoABorrar);
	pthread_mutex_unlock(&agregarPedidoComanda);

	pthread_mutex_lock(&agregarPedidoSwap);
	eliminarPaginasDeSwap(tabla->nombreRestaurante,idPedido);
	pthread_mutex_unlock(&agregarPedidoSwap);

	if(logger->is_active_console == false){
		logger->is_active_console = true;
	}
	log_trace(logger,"Se eliminaron todas las páginas respecto del pedido con ID: %d, del restaurante %s",idPedido,tabla->nombreRestaurante->nombre);

	return 0;
}

int buscarPosicionSegmentoEnTabla(uint32_t idPedido,TablaDeSegmentos* tabla){
	int cant_segmentos = list_size(tabla->segmentos);
	int posicion;

	for(int i=0; i<cant_segmentos; i++){
		Segmento* segmento_a_chequear = list_get(tabla->segmentos, i);
		if(idPedido == segmento_a_chequear->idPedido){
			posicion = i;
			break;
		}
	}
	return posicion;
}

int buscarPosicionPaginaEnTabla(Segmento* segmento,Pagina* pagina){
	int posicion;

	for(int i=0; i<segmento->tablaDePaginas.paginas->elements_count; i++){
		Pagina* pagina_a_chequear = list_get(segmento->tablaDePaginas.paginas,i);
		if(pagina->frame == pagina_a_chequear->frame){
			posicion = i;
			break;
		}
	}
	return posicion;
}


int buscarPosicionPedidoComanda(PedidoComanda* pedido){
	int posicion;
	PedidoComanda* otroPedido;

	for(int i=0; i<pedidosComanda->elements_count; i++){
		otroPedido = list_get(pedidosComanda,i);
		if(otroPedido->idPedido == pedido->idPedido){
			posicion = i;
			break;
		}
	}
	return posicion;
}
