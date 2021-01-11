#include "Comanda.h"
#include "MemoriaPrincipal.h"

void* pruebas(){
	/*PRUEBA CON APP
	 * nombre* nombre_restaurante = malloc(sizeof(nombre));
		nombre_restaurante->largo_nombre = strlen("Restaurante Nuevo")+1;
		nombre_restaurante->nombre = malloc(nombre_restaurante->largo_nombre);
		nombre_restaurante->nombre = "Restaurante Nuevo";
		nombre* nombre_plato = malloc(sizeof(nombre));
		nombre_plato->largo_nombre = strlen("PIZZA") + 1;
		nombre_plato->nombre = malloc(nombre_plato->largo_nombre);
		nombre_plato->nombre = "PIZZA";
		nombre* nombre_plato2 = malloc(sizeof(nombre));
		nombre_plato2->largo_nombre = strlen("TARTA") + 1;
		nombre_plato2->nombre = malloc(nombre_plato2->largo_nombre);
		nombre_plato2->nombre = "TARTA";
		int id_pedido = 7;
		buffer_t* buffer_comanda = serializar_guardar_pedido(nombre_restaurante, id_pedido);
		sfd sfd_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer_comanda,GUARDAR_PEDIDO);
		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_comanda, paquete);
		int32_t respuesta = deserializar_int(paquete->buffer);
		if(respuesta == 0){
			printf("OK \n");
		}
		else{
			printf("FAIL\n");
		}
		nombre* nombre_restaurante2 = malloc(sizeof(nombre));
			nombre_restaurante2->largo_nombre = strlen("Restaurante Piola")+1;
			nombre_restaurante2->nombre = malloc(nombre_restaurante2->largo_nombre);
			nombre_restaurante2->nombre = "Restaurante Piola";
			buffer_t* buffer= serializar_guardar_pedido(nombre_restaurante2, 4);
			sfd comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer,GUARDAR_PEDIDO);
			py_paquete_t* paquete2 = malloc(sizeof(py_paquete_t));
			py_recv_msg(comanda, paquete2);
			int32_t respuesta2 = deserializar_int(paquete2->buffer);
			if(respuesta2 == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
			buffer_t* bufferC = serializar_guardar_plato(nombre_restaurante, 7,nombre_plato, 1);
			sfd sfdA = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",bufferC,GUARDAR_PLATO);
			py_paquete_t* paquete3 = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfdA, paquete3);
			close(sfdA);
			int respuesta3 = deserializar_int(paquete3->buffer);
			if(respuesta3 == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
			buffer_t* buffer4 = serializar_guardar_plato(nombre_restaurante, 7,nombre_plato2, 1);
			sfd sfd4 = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA",buffer4,GUARDAR_PLATO);
			py_paquete_t* paquete4 = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd4, paquete4);
			close(sfd4);
			int respuesta4 = deserializar_int(paquete4->buffer);
			if(respuesta4 == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
			msg_confirmar_pedido* mensaje = malloc(sizeof(msg_confirmar_pedido));
			mensaje->id_pedido = 7;
			buffer_t* buffer6 = serializar_confirmar_pedido_con_restaurante(mensaje->id_pedido,nombre_restaurante);
			sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_COMANDA", buffer6, CONFIRMAR_PEDIDO);
			py_paquete_t* paquete6 = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_app, paquete6);
			int32_t okfail = deserializar_int(paquete6->buffer);
			if(okfail == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
			buffer_t* buffer2 = serializar_plato_listo(nombre_plato,nombre_restaurante,7);
			sfd sfd_APP = conectar_y_enviar("IP_COMANDA", "PUERTO_COMANDA", buffer2, PLATO_LISTO);
			py_paquete_t* paquete7 = malloc(sizeof(py_paquete_t));
			py_recv_msg(sfd_APP, paquete7);
			int32_t okfail2 = deserializar_int(paquete7->buffer);
			if(okfail2 == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
			buffer_t* obtenerPed = serializar_obtener_pedido(nombre_restaurante,7);
			sfd obtPed = conectar_y_enviar("IP_COMANDA", "PUERTO_COMANDA",obtenerPed,OBTENER_PEDIDO);
			py_paquete_t* paquete9 = malloc(sizeof(py_paquete_t));
			py_recv_msg(obtPed, paquete9);
			Pedido* unPedido = deserializar_pedido(paquete9->buffer);
			printf("ESTADO PEDIDO RECIBIDO: %d\n",unPedido->estadoPedido);
			buffer_t* buffer3 = serializar_finalizar_pedido(nombre_restaurante,7);
			sfd socket_comanda = conectar_y_enviar("IP_COMANDA","PUERTO_COMANDA", buffer3, FINALIZAR_PEDIDO);
			py_paquete_t* paquete8 = malloc(sizeof(py_paquete_t));
			py_recv_msg(socket_comanda, paquete8);
			int32_t respuesta6 = deserializar_int(paquete8->buffer);
			if(respuesta6 == 0){
				printf("OK \n");
			}
			else{
				printf("FAIL\n");
			}
	 */
		/* COPIADO DE PAGINAS EN MEMORIA CON LRU Y CLOCK MEJORADO
		 * nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		int marco_disponible = buscarPaginaLibre();
		int cantPlatos = 2;
		nombre nombre_plato;
		nombre_plato.largo_nombre = strlen("PIZZA") + 1;
		nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
		nombre_plato.nombre = "PIZZA";
		PedidoSwap* pedido = buscarPedidoSwap(idPedido);
		ContenidoPaginaSwap* paginaEnSwap = buscarPaginaEnPedidoSwap(pedido,nombre_plato.nombre);
		printf("El numero de pagina es %d\n",paginaEnSwap->nroPagina);
		contenidoPagina = malloc(sizeof(ContenidoPagina));
		contenidoPagina->cantTotal = (paginaEnSwap->cantTotal) + cantPlatos;
		contenidoPagina->cantLista = paginaEnSwap->cantLista;
		strcpy(contenidoPagina->nombrePlato,paginaEnSwap->nombrePlato);
		int contadorTimestamp = 0;
		pthread_mutex_lock(&timestamp);
		contadorTimestamp++;
		pthread_mutex_unlock(&timestamp);
		Pagina* paginaEnMemoria = malloc(sizeof(Pagina));
		paginaEnMemoria->frame = marco_disponible;
		paginaEnMemoria->M = 1;
		paginaEnMemoria->P = 1;
		paginaEnMemoria->timestamp = contadorTimestamp;
		printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria->timestamp);
		pthread_mutex_lock(&mutexMemoria);
		copiarPaginaEnFrame(contenidoPagina->cantTotal,contenidoPagina->cantLista,contenidoPagina->nombrePlato,marco_disponible);
		estadosFrames[marco_disponible] = OCUPADO;
		list_add(otroSegmento->tablaDePaginas.paginas, paginaEnMemoria);
		pthread_mutex_unlock(&mutexMemoria);
		logger = py_get_logger();
		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"Se añadió el plato en la tabla de páginas del pedido");
		//////////////////////////////////
		int marco_disponible2 = buscarPaginaLibre();
		//int cantPlatos = 2;
		nombre nombre_plato2;
		nombre_plato2.largo_nombre = strlen("MILANESA") + 1;
		nombre_plato2.nombre = malloc(nombre_plato2.largo_nombre);
		nombre_plato2.nombre = "MILANESA";
		PedidoSwap* pedido2 = buscarPedidoSwap(idPedido);
		ContenidoPaginaSwap* paginaEnSwap2 = buscarPaginaEnPedidoSwap(pedido2,nombre_plato2.nombre);
		ContenidoPagina* contenido = malloc(sizeof(ContenidoPagina));
		contenido->cantTotal = (paginaEnSwap2->cantTotal) + cantPlatos;
		contenido->cantLista = paginaEnSwap2->cantLista;
		strcpy(contenido->nombrePlato,paginaEnSwap2->nombrePlato);
		pthread_mutex_lock(&timestamp);
		contadorTimestamp++;
		pthread_mutex_unlock(&timestamp);
		Pagina* paginaEnMemoria2 = malloc(sizeof(Pagina));
		paginaEnMemoria2->frame = marco_disponible2;
		paginaEnMemoria2->M = 1;
		paginaEnMemoria2->P = 1;
		paginaEnMemoria2->timestamp = contadorTimestamp;
		printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria2->timestamp);
		pthread_mutex_lock(&mutexMemoria);
		copiarPaginaEnFrame(contenido->cantTotal,contenido->cantLista,contenido->nombrePlato,marco_disponible2);
		estadosFrames[marco_disponible2] = OCUPADO;
		list_add(otroSegmento->tablaDePaginas.paginas, paginaEnMemoria2);
		pthread_mutex_unlock(&mutexMemoria);
		logger = py_get_logger();
		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"Se añadió el plato en la tabla de páginas del pedido");
		//////////////////////////////////
		int marco_disponible3 = buscarPaginaLibre();
		//int cantPlatos = 2;
		nombre nombre_plato3;
		nombre_plato3.largo_nombre = strlen("EMPANADA") + 1;
		nombre_plato3.nombre = malloc(nombre_plato3.largo_nombre);
		nombre_plato3.nombre = "EMPANADA";
		PedidoSwap* pedido3 = buscarPedidoSwap(idPedido);
		ContenidoPaginaSwap* paginaEnSwap3 = buscarPaginaEnPedidoSwap(pedido3,nombre_plato3.nombre);
		ContenidoPagina* contenido2 = malloc(sizeof(ContenidoPagina));
		contenido2->cantTotal = (paginaEnSwap3->cantTotal) + cantPlatos;
		contenido2->cantLista = paginaEnSwap3->cantLista;
		strcpy(contenido2->nombrePlato,paginaEnSwap3->nombrePlato);
		pthread_mutex_lock(&timestamp);
		contadorTimestamp++;
		pthread_mutex_unlock(&timestamp);
		Pagina* paginaEnMemoria3 = malloc(sizeof(Pagina));
		paginaEnMemoria3->frame = marco_disponible3;
		paginaEnMemoria3->M = 1;
		paginaEnMemoria3->P = 1;
		paginaEnMemoria3->timestamp = contadorTimestamp;
		printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria3->timestamp);
		pthread_mutex_lock(&mutexMemoria);
		copiarPaginaEnFrame(contenido2->cantTotal,contenido2->cantLista,contenido2->nombrePlato,marco_disponible3);
		estadosFrames[marco_disponible3] = OCUPADO;
		list_add(otroSegmento->tablaDePaginas.paginas, paginaEnMemoria3);
		pthread_mutex_unlock(&mutexMemoria);
		logger = py_get_logger();
		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"Se añadió el plato en la tabla de páginas del pedido");
		//////////////////////////////////
		int marco_disponible4 = buscarPaginaLibre();
		//int cantPlatos = 2;
		nombre nombre_plato4;
		nombre_plato4.largo_nombre = strlen("FIDEOS") + 1;
		nombre_plato4.nombre = malloc(nombre_plato4.largo_nombre);
		nombre_plato4.nombre = "FIDEOS";
		PedidoSwap* pedido4 = buscarPedidoSwap(idPedido);
		ContenidoPaginaSwap* paginaEnSwap4 = buscarPaginaEnPedidoSwap(pedido4,nombre_plato4.nombre);
		ContenidoPagina* contenido3 = malloc(sizeof(ContenidoPagina));
		contenido3->cantTotal = (paginaEnSwap4->cantTotal) + cantPlatos;
		contenido3->cantLista = paginaEnSwap4->cantLista;
		strcpy(contenido3->nombrePlato,paginaEnSwap4->nombrePlato);
		pthread_mutex_lock(&timestamp);
		contadorTimestamp++;
		pthread_mutex_unlock(&timestamp);
		Pagina* paginaEnMemoria4 = malloc(sizeof(Pagina));
		paginaEnMemoria4->frame = marco_disponible4;
		paginaEnMemoria4->M = 1;
		paginaEnMemoria4->P = 1;
		paginaEnMemoria4->timestamp = contadorTimestamp;
		printf("Se actualizó el timestamp a %d de la página\n",paginaEnMemoria4->timestamp);
		pthread_mutex_lock(&mutexMemoria);
		copiarPaginaEnFrame(contenido3->cantTotal,contenido3->cantLista,contenido3->nombrePlato,marco_disponible4);
		estadosFrames[marco_disponible4] = OCUPADO;
		list_add(otroSegmento->tablaDePaginas.paginas, paginaEnMemoria4);
		pthread_mutex_unlock(&mutexMemoria);
		logger = py_get_logger();
		if(logger->is_active_console == false){
			logger->is_active_console = true;
		}
		log_info(logger,"Se añadió el plato en la tabla de páginas del pedido");
		 */

		/* GUARDAR PEDIDO
		nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = CONFIRMADO;
		list_add(pedidosComanda,pedido);
		int id = 5;
		int32_t respuesta = administrarGuardarPedido(tabla,id);
		printf("RESPUESTA: %d",respuesta);*/

		/* GUARDAR PLATO
		 * nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = PENDIENTE;
		list_add(pedidosComanda,pedido);
		PedidoSwap* pedidoSwap = malloc(sizeof(PedidoSwap));
		pedidoSwap->idPedido = idPedido;
		pedidoSwap->platos = list_create();
		list_add(pedidosSwap,pedidoSwap);
		nombre nombre_plato2;
		nombre_plato2.largo_nombre = strlen("PIZZA") + 1;
		nombre_plato2.nombre = malloc(nombre_plato2.largo_nombre);
		nombre_plato2.nombre = "PIZZA";
		int cantPlatos = 2;
		int id = 2;
		int respuesta = administrarGuardarPlato(tabla,id,nombre_plato2,cantPlatos);
		printf("RESPUESTA %d\n",respuesta);
		 */

		/* CONFIRMAR PEDIDO
		 * nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = PENDIENTE;
		list_add(pedidosComanda,pedido);
		int id = 2;
		int respuesta = administrarConfirmarPedido(tabla,id);
		 */

		/* PLATO LISTO
		 * 	nombre nombre_restaurante1;
			nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
			nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
			nombre_restaurante1.nombre = "Restaurante Nuevo";
			TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
			int idPedido = 2;
			Segmento* otroSegmento = malloc(sizeof(Segmento));
			otroSegmento->idPedido = idPedido;
			otroSegmento->tablaDePaginas.paginas = list_create();
			list_add(tabla->segmentos,otroSegmento);
			PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
			pedido->idPedido = idPedido;
			pedido->estadoPedido = CONFIRMADO;
			pedido->platos = list_create();
			list_add(pedidosComanda,pedido);
			nombre nombre_plato2;
			nombre_plato2.largo_nombre = strlen("PIZZA") + 1;
			nombre_plato2.nombre = malloc(nombre_plato2.largo_nombre);
			nombre_plato2.nombre = "PIZZA";
			int contadorTimestamp = 0;
			puts("Comienza la busqueda de un frame libre");
			int marco_disponible = buscarPaginaLibre();
			//Creo nueva pagina
			Pagina* nueva_pagina = malloc(sizeof(Pagina));
			nueva_pagina->frame = marco_disponible;
			nueva_pagina->M = 0;
			nueva_pagina->P = 1;
			nueva_pagina->U = 1;
			pthread_mutex_lock(&timestamp);
			contadorTimestamp++;
			pthread_mutex_unlock(&timestamp);
			nueva_pagina->timestamp = contadorTimestamp;
			PedidoComanda* pedido1 = obtenerPedidoDePedidosComanda(idPedido);
			contenidoPagina = malloc(sizeof(ContenidoPagina));
			contenidoPagina->cantTotal = 1;
			contenidoPagina->cantLista = 0;
			char* nombreP = string_duplicate(nombre_plato2.nombre);
			strcpy(contenidoPagina->nombrePlato,nombre_plato2.nombre);
			copiarPaginaEnFrame(contenidoPagina->cantTotal,contenidoPagina->cantLista,contenidoPagina->nombrePlato,marco_disponible);
			//marco frame como ocupado y agrego la pagina a la tabla de paginas del segmento
			estadosFrames[marco_disponible] = OCUPADO;
			list_add(otroSegmento->tablaDePaginas.paginas, nueva_pagina);
			ContenidoPagina* contenido = malloc(sizeof(ContenidoPagina));
			contenido->cantTotal = 1;
			contenido->cantLista = 0;
			strcpy(contenido->nombrePlato,nombreP);
			list_add(pedido1->platos,contenido);
			nombre nombre_plato;
				nombre_plato.largo_nombre = strlen("PIZZA") + 1;
				nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
				nombre_plato.nombre = "PIZZA";
			int id = 2;
			int respuesta = administrarPlatoListo(tabla,id,nombre_plato);
		 */

		/*  FINALIZAR PEDIDO
		 * nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = CONFIRMADO;
		list_add(pedidosComanda,pedido);
		int id = 2;
		int respuesta = administrarFinalizarPedido(tabla,id);
		 */

		/* CARGAR EN FRAMES TODOS LOS PLATOS DEL PEDIDO (SACANDOLOS DE SWAP)
		 * nombre nombre_restaurante1;
		nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
		nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
		nombre_restaurante1.nombre = "Restaurante Nuevo";
		TablaDeSegmentos* tabla = crearTablaDeSegmentos(nombre_restaurante1);
		int idPedido = 2;
		Segmento* otroSegmento = malloc(sizeof(Segmento));
		otroSegmento->idPedido = idPedido;
		otroSegmento->tablaDePaginas.paginas = list_create();
		list_add(tabla->segmentos,otroSegmento);
		PedidoComanda* pedido = malloc(sizeof(PedidoComanda));
		pedido->idPedido = idPedido;
		pedido->estadoPedido = PENDIENTE;
		list_add(pedidosComanda,pedido);
		ContenidoPagina* contenido = malloc(sizeof(ContenidoPagina));
		contenido->cantTotal = 6;
		contenido->cantLista = 4;
		strcpy(contenido->nombrePlato,"PIZZA");
		ContenidoPagina* contenido2 = malloc(sizeof(ContenidoPagina));
		contenido2->cantTotal = 6;
		contenido2->cantLista = 3;
		strcpy(contenido2->nombrePlato,"FIDEOS");
		list_add(pedido->platos,contenido);
		list_add(pedido->platos,contenido2);
		cargarEnFramesLosPlatosDelPedido(otroSegmento);
		 */

	return 0;
}
