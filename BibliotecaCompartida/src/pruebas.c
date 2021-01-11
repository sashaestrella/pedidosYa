#include "pycore.h"
/*
static nombre* nombre_restaurante1;
static nombre* nombre_restaurante2;
static t_list* lista_restaurantes;
static Cocinero* cocinero1;
static nombre nombre_cocinero1;
static nombre cocinero1.afinidad;
static Cocinero* cocinero2;
static nombre nombre_cocinero2;
static nombre cocinero2.afinidad;
static t_list* lista_cocineros;
static Plato* plato1;
static Plato* plato2;
static Receta* receta1;
static Receta* receta2;
static nombre nombre_paso1;
static nombre nombre_paso2;
static nombre nombre_paso3;
static Paso* paso1;
static Paso* paso2;
static Paso* paso3;
static t_list* lista_pasos1;
static t_list* lista_pasos2;
static CoordenadasXY coordenadas1;
static CoordenadasXY coordenadas2;
static int32_t numero;
static uint32_t ok;
*/
/*
void inicializarVariables(){
	nombre_restaurante1 = malloc(sizeof(nombre));
	nombre_restaurante1->largo_nombre = strlen("Restaurante Nuevo") + 1;
	nombre_restaurante1->nombre = malloc(nombre_restaurante1->largo_nombre);
	nombre_restaurante1->nombre = "Restaurante Nuevo";

	nombre_restaurante2 = malloc(sizeof(nombre));
	nombre_restaurante2->largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2->nombre = malloc(nombre_restaurante2->largo_nombre);
	nombre_restaurante2->nombre = "Restaurante Viejo";

	lista_restaurantes = list_create();
	list_add(lista_restaurantes, nombre_restaurante1);
	list_add(lista_restaurantes, nombre_restaurante2);

	cocinero1 = malloc(sizeof(Cocinero));
	cocinero2 = malloc(sizeof(Cocinero));

	nombre_cocinero1.largo_nombre = strlen("Reinaldo") + 1;
	nombre_cocinero1.nombre = malloc(nombre_cocinero1.largo_nombre);
	nombre_cocinero1.nombre = "Reinaldo";

	nombre_cocinero2.largo_nombre = strlen("Franchesca") + 1;
	nombre_cocinero2.nombre = malloc(nombre_cocinero1.largo_nombre);
	nombre_cocinero2.nombre = "Franchesca";

	afinidad1.largo_nombre = strlen("Fideos con Tuco") + 1;
	afinidad1.nombre = malloc(nombre_cocinero1.largo_nombre);
	afinidad1.nombre = "Fideos con Tuco";

	afinidad2.largo_nombre = strlen("Pizza") + 1;
	afinidad2.nombre = malloc(nombre_cocinero1.largo_nombre);
	afinidad2.nombre = "Pizza";

	cocinero1->nombreCocinero = nombre_cocinero1;
	cocinero2->nombreCocinero = nombre_cocinero2;

	cocinero1->afinidad = afinidad1;
	cocinero2->afinidad = afinidad2;

	plato1 = malloc(sizeof(Plato));
	plato2 = malloc(sizeof(Plato));

	plato1->nombre = afinidad1;
	plato2->nombre = afinidad2;

	nombre_paso1.largo_nombre = strlen("Hacer la masa") + 1;
	nombre_paso1.nombre = malloc(nombre_cocinero1.largo_nombre);
	nombre_paso1.nombre = "Hacer la masa";

	nombre_paso2.largo_nombre = strlen("Poner la salsa") + 1;
	nombre_paso2.nombre = malloc(nombre_cocinero1.largo_nombre);
	nombre_paso2.nombre = "Poner la salsa";

	nombre_paso3.largo_nombre = strlen("Cocinar") + 1;
	nombre_paso3.nombre = malloc(nombre_cocinero1.largo_nombre);
	nombre_paso3.nombre = "Cocinar";

	paso1->nombre = nombre_paso1;
	paso2->nombre = nombre_paso2;
	paso3->nombre = nombre_paso3;

	paso1->tiempo = 10;
	paso2->tiempo = 10;
	paso3->tiempo = 10;

	lista_pasos1 = list_create();
	lista_pasos2 = list_create();

	receta1->Pasos = list_create();
	receta2->Pasos = list_create();

	list_add(lista_pasos1, paso1);
	list_add(lista_pasos1, paso2);

	list_add(lista_pasos2, paso2);
	list_add(lista_pasos2, paso3);

	list_add(receta1->Pasos, lista_pasos1);
	list_add(receta1->Pasos, lista_pasos2);

	list_add(receta2->Pasos, lista_pasos2);
	list_add(receta2->Pasos, lista_pasos1);

	lista_cocineros = list_create();
	list_add(lista_cocineros, cocinero1);
	list_add(lista_cocineros, cocinero2);

	coordenadas1.posicionX = 3;
	coordenadas1.posicionY = 4;

	coordenadas2.posicionX = 5;
	coordenadas2.posicionY = 6;

	numero = 149;

	ok = 1;
}*/

//instrucciones: copiar el contenido en el main() del módulo correspondiente

/*           CONSULTAR_RESTAURANTE_COMANDA
 	buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
	buffer_vacio->size = 0;
	buffer_vacio->stream = NULL;
	sfd sfd_app = conectar_y_enviar("IP_APP", "PUERTO_APP",buffer_vacio, CONSULTAR_RESTAURANTE);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	printf("%i \n", paquete->codigo_operacion);
	struct_lista* lista_nombres = deserializar_lista_nombres(paquete->buffer);
	nombre* nombre1 = list_get(lista_nombres->lista, 0);
	nombre* nombre2 = list_get(lista_nombres->lista, 1);
	nombre* nombre3 = list_get(lista_nombres->lista, 2);
	printf("%s \n", nombre3->nombre);
 */

/*        CONSULTAR_RESTAURANTE_APP
   	static nombre* nombre_restaurante1;
	static nombre* nombre_restaurante2;
	static nombre* nombre_restaurante3;

	nombre_restaurante1 = malloc(sizeof(nombre));
	nombre_restaurante1->largo_nombre = strlen("Restaurante Nuevo");
	nombre_restaurante1->nombre = malloc(nombre_restaurante1->largo_nombre);
	nombre_restaurante1->nombre = "Restaurante Nuevo";

	nombre_restaurante2 = malloc(sizeof(nombre));
	nombre_restaurante2->largo_nombre = strlen("Restaurante Viejo");
	nombre_restaurante2->nombre = malloc(nombre_restaurante2->largo_nombre);
	nombre_restaurante2->nombre = "Restaurante Viejo";

	nombre_restaurante3 = malloc(sizeof(nombre));
	nombre_restaurante3->largo_nombre = strlen("Restaurante Italiano");
	nombre_restaurante3->nombre = malloc(nombre_restaurante3->largo_nombre);
	nombre_restaurante3->nombre = "Restaurante Italiano";

	lista_restaurantes = list_create();
	list_add(lista_restaurantes, nombre_restaurante1);
	list_add(lista_restaurantes, nombre_restaurante2);
	list_add(lista_restaurantes, nombre_restaurante3);

	buffer_t* buffer;
	nombre* nombre_Rest = list_get(lista_restaurantes, 0);
	printf("%s \n", nombre_Rest->nombre);
 */


/*                         SELECCIONAR_RESTAURANTE_COMANDA
   	msg_seleccionar_restaurante* mensaje = malloc(sizeof(msg_seleccionar_restaurante));
	mensaje->id_cliente = 14;
	nombre nombre_restaurante2;
	//nombre_restaurante2 = malloc(sizeof(nombre));
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";
	mensaje->restaurante = nombre_restaurante2;
	buffer_t* buffer = serializar_seleccionar_restaurante(mensaje->restaurante, mensaje->id_cliente);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, SELECCIONAR_RESTAURANTE);
	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_ok_fail(buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 */


/* OBTENER RESTAURANTE APP
	t_list* afinidades = list_create();
		t_list* lista_pasos1 = list_create();
		t_list* lista_pasos2 = list_create();
		t_list* recetas = list_create();

		nombre afinidad1;
		afinidad1.largo_nombre = strlen("Pizza") + 1;
		afinidad1.nombre = malloc(afinidad1.largo_nombre);
		afinidad1.nombre = "Pizza";
		nombre afinidad2;
		afinidad2.largo_nombre = strlen("Fideos con Tuco") + 1;
		afinidad2.nombre = malloc(afinidad2.largo_nombre);
		afinidad2.nombre = "Fideos con Tuco";

		restauranteSinNombre = malloc(sizeof(RestauranteSinNombre));
		list_add(afinidades,&afinidad1);
		list_add(afinidades,&afinidad2);

		Paso paso1;
		nombre nombre_paso1;
		nombre_paso1.largo_nombre = strlen("Hacer la masa") + 1;
		nombre_paso1.nombre = malloc(nombre_paso1.largo_nombre);
		nombre_paso1.nombre = "Hacer la masa";
		paso1.nombre = nombre_paso1;
		paso1.tiempo = 4;
		Paso paso2;
		nombre nombre_paso2;
		nombre_paso2.largo_nombre = strlen("Poner la salsa") + 1;
		nombre_paso2.nombre = malloc(nombre_paso2.largo_nombre);
		nombre_paso2.nombre = "Poner la salsa";
		paso2.nombre = nombre_paso2;
		paso2.tiempo = 2;
		Paso paso3;
		nombre nombre_paso3;
		nombre_paso3.largo_nombre = strlen("Cocinar") + 1;
		nombre_paso3.nombre = malloc(nombre_paso3.largo_nombre);
		nombre_paso3.nombre = "Cocinar";
		paso3.nombre = nombre_paso3;
		paso3.tiempo = 5;

		list_add(lista_pasos1,&paso1);
		list_add(lista_pasos1,&paso2);
		list_add(lista_pasos1,&paso3);

		list_add(lista_pasos2,&paso1);
		list_add(lista_pasos2,&paso2);
		list_add(lista_pasos2,&paso3);

		RecetaConPrecio receta1;
		receta1.receta.Pasos = lista_pasos1;
		receta1.precio = 10;
		RecetaConPrecio receta2;
		receta2.receta.Pasos = lista_pasos2;
		receta2.precio = 20;

		list_add(recetas,&receta1);
		list_add(recetas,&receta2);

		CoordenadasXY coordenadas;
		coordenadas.posicionX = 5;
		coordenadas.posicionY = 5;

		restauranteSinNombre->afinidades = afinidades;
		restauranteSinNombre->coordenadas = coordenadas;
		restauranteSinNombre->recetas = recetas;
		restauranteSinNombre->cantidadDeHornos = 5;
		restauranteSinNombre->cantidadDePedidos = 2;

		int tamanio = list_size(restauranteSinNombre->afinidades);
		printf("Tamanio lista cocineros: %d \n",tamanio);*/

/*   OBTENER RESTAURANTE COMANDA
	nombre nombre_restaurante2;
		nombre_restaurante2.largo_nombre = strlen("Restaurante Italiano") + 1;
		nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
		nombre_restaurante2.nombre = "Restaurante Italiano";

		msg_obtener_restaurante* mensaje = malloc(sizeof(msg_obtener_restaurante));
		mensaje->restaurante = nombre_restaurante2;
		buffer_t* buffer = serializar_obtener_restaurante(mensaje->restaurante);
		sfd sfd_app = conectar_y_enviar("IP_APP", "PUERTO_APP", buffer, OBTENER_RESTAURANTE);

		py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
		py_recv_msg(sfd_app, paquete);
		RestauranteSinNombre* restauranteSinNombre;
		restauranteSinNombre = deserealizar_restaurante_sin_nombre(paquete->buffer);
		printf("Cantidad de hornos: %i \n",restauranteSinNombre->cantidadDeHornos);*/

/* CONSULTAR PLATOS COMANDA
	msg_consultar_platos* mensaje = malloc(sizeof(msg_consultar_platos));
	nombre nombre_restaurante1;
	nombre_restaurante1.largo_nombre = strlen("Restaurante Nuevo");
	nombre_restaurante1.nombre = malloc(nombre_restaurante1.largo_nombre);
	nombre_restaurante1.nombre = "Restaurante Nuevo";

	mensaje->restaurante = nombre_restaurante1;
	buffer_t* buffer = serializar_consultar_platos(mensaje->restaurante);
	sfd sfd_app = conectar_y_enviar("IP_APP", "PUERTO_APP", buffer,CONSULTAR_PLATOS);


	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	struct_lista* lista = deserializar_lista_nombres(paquete->buffer);
	nombre* unPlato;
	for(int i=0;i<lista->lista->elements_count;i++){
		unPlato=list_get(lista->lista, i);
		printf("Plato: %s \n",unPlato->nombre);
	}
	*/

/* CONSULTAR PLATOS APP
 * listaNombresComida=list_create();
		nombre nombre1;
		nombre1.largo_nombre=strlen("plato1")+1;
		nombre1.nombre=malloc(nombre1.largo_nombre);
		nombre1.nombre="plato1";

		nombre nombre2;
		nombre2.largo_nombre=strlen("plato2")+1;
		nombre2.nombre=malloc(nombre2.largo_nombre);
		nombre2.nombre="plato2";

		list_add(listaNombresComida,&nombre1);
		list_add(listaNombresComida,&nombre2);
 */
/* CREAR PEDIDO COMANDA
 * buffer_t* buffer_vacio = malloc(sizeof(buffer_t));
	buffer_vacio->size = 0;
	buffer_vacio->stream = NULL;
	sfd sfd_app = conectar_y_enviar("IP_APP", "PUERTO_APP",buffer_vacio, CREAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t idPedido = deserializar_int(paquete->buffer);
	printf("ID recibido: %i\n ",idPedido);
 */

/* GUARDAR PEDIDO COMANDA
 * msg_guardar_pedido* mensaje = malloc(sizeof(msg_guardar_pedido));
	nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 7;

	buffer_t* buffer = serializar_guardar_pedido(mensaje->restaurante, mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, GUARDAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 */

/* ANIADIR PLATO COMANDA
 * msg_aniadir_plato* mensaje = malloc(sizeof(msg_aniadir_plato));
	nombre nombre_plato;
	nombre_plato.largo_nombre = strlen("PIZZA") + 1;
	nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
	nombre_plato.nombre = "PIZZA";
	mensaje->plato = nombre_plato;
	mensaje->id_pedido = 7;

	buffer_t* buffer = serializar_aniadir_plato(mensaje->plato, mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, ANIADIR_PLATO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 *
 */

/* GUARDAR PLATO COMANDA
 nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";

	nombre nombre_plato;
	nombre_plato.largo_nombre = strlen("PIZZA") + 1;
	nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
	nombre_plato.nombre = "PIZZA";

	msg_guardar_plato* mensaje = malloc(sizeof(msg_guardar_plato));
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 7;
	mensaje->plato = nombre_plato;
	mensaje->cantidad_platos = 5;

	buffer_t* buffer = serializar_guardar_plato(mensaje->restaurante,mensaje->id_pedido,mensaje->plato, mensaje->cantidad_platos);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, GUARDAR_PLATO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 *
 */

/* CONFIRMAR PEDIDO COMANDA
 * msg_confirmar_pedido* mensaje = malloc(sizeof(msg_confirmar_pedido));
	mensaje->id_pedido = 7;

	buffer_t* buffer = serializar_confirmar_pedido(mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, CONFIRMAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 */

/*  PLATO LISTO COMANDA
 * nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";

	nombre nombre_plato;
	nombre_plato.largo_nombre = strlen("PIZZA") + 1;
	nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
	nombre_plato.nombre = "PIZZA";

	msg_plato_listo* mensaje = malloc(sizeof(msg_plato_listo));
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 7;
	mensaje->plato = nombre_plato;

	buffer_t* buffer = serializar_plato_listo(mensaje->plato,mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, PLATO_LISTO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 *
 */

/* FINALIZAR PEDIDO COMANDA
 * nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";

	msg_finalizar_pedido* mensaje = malloc(sizeof(msg_finalizar_pedido));
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 7;


	buffer_t* buffer = serializar_finalizar_pedido(mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, FINALIZAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 *
 */

/*  TERMINAR PEDIDO COMANDA
 * nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Viejo") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Viejo";

	msg_terminar_pedido* mensaje = malloc(sizeof(msg_terminar_pedido));
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 7;

	buffer_t* buffer = serializar_terminar_pedido(mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, TERMINAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	int32_t okfail = deserializar_int(paquete->buffer);
	if(okfail){
		printf("OK \n");
	}
	else{
		printf(" \n");
	}
 */

/* OBTENER RECETA APP
 * Declaro como variable global Receta*receta;
 * t_list* nuevosPasos = list_create();
		Paso paso1;
		nombre nombre_paso1;
		nombre_paso1.largo_nombre = strlen("Hacer la masa") + 1;
		nombre_paso1.nombre = malloc(nombre_paso1.largo_nombre);
		nombre_paso1.nombre = "Hacer la masa";
		paso1.nombre = nombre_paso1;
		paso1.tiempo = 4;
		Paso paso2;
		nombre nombre_paso2;
		nombre_paso2.largo_nombre = strlen("Poner la salsa") + 1;
		nombre_paso2.nombre = malloc(nombre_paso2.largo_nombre);
		nombre_paso2.nombre = "Poner la salsa";
		paso2.nombre = nombre_paso2;
		paso2.tiempo = 2;
		Paso paso3;
		nombre nombre_paso3;
		nombre_paso3.largo_nombre = strlen("Cocinar") + 1;
		nombre_paso3.nombre = malloc(nombre_paso3.largo_nombre);
		nombre_paso3.nombre = "Cocinar";
		paso3.nombre = nombre_paso3;
		paso3.tiempo = 5;
        receta= malloc(sizeof(Receta));
		list_add(nuevosPasos,&paso1);
		list_add(nuevosPasos,&paso2);
		list_add(nuevosPasos,&paso3);

		receta->Pasos = nuevosPasos;

 *
 */

/* OBTENER RECETA COMANDA
 * 	nombre nombre_plato;
	nombre_plato.largo_nombre = strlen("PIZZA") + 1;
	nombre_plato.nombre = malloc(nombre_plato.largo_nombre);
	nombre_plato.nombre = "PIZZA";

	msg_obtener_receta* mensaje = malloc(sizeof(msg_obtener_receta));
	mensaje->plato = nombre_plato;

	buffer_t* buffer = serializar_obtener_receta(mensaje->plato);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, OBTENER_RECETA);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	Receta* receta;
	receta = deserializar_receta(paquete->buffer);
	Paso* unPaso;
	for(int i=0;i<receta->Pasos->elements_count;i++){
		unPaso=list_get(receta->Pasos, i);
		printf("Paso: %s \n",unPaso->nombre.nombre);
	}
	free(receta);
 */

/* CONSULTAR PEDIDO COMANDA
 * msg_consultar_pedido* mensaje = malloc(sizeof(msg_consultar_pedido));
	mensaje->id_pedido = 7;

	buffer_t* buffer = serializar_consultar_pedido(mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, CONSULTAR_PEDIDO);

	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);
	Pedido* pedido;
	pedido = deserializarPedido(paquete->buffer);
	printf("Nombre del restaurante del pedido: %s\n",pedido->nombre_restaurante.nombre);
	registroComida* registro;
	for(int i=0;i<pedido->platos->elements_count;i++){
		registro = list_get(pedido->platos, i);
		printf("Nombre plato: %s \n",registro->plato.nombre);
	}

	free(pedido);
 */

/*  CONSULTAR PEDIDO APP
 * nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Italiano") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Italiano";

	nombre registro1;
	registro1.largo_nombre = strlen("Registro1") + 1;
	registro1.nombre = malloc(registro1.largo_nombre);
	registro1.nombre = "Registro1";

	nombre registro2;
	registro2.largo_nombre = strlen("Registro2") + 1;
	registro2.nombre = malloc(registro2.largo_nombre);
	registro2.nombre = "Registro2";

	registros_de_platos=list_create();
	registroComida registroComida1;
	registroComida1.cantLista=5;
	registroComida1.cantTotal=25;
	registroComida1.plato = registro1;

	registroComida registroComida2;
	registroComida2.cantLista=5;
	registroComida2.cantTotal=25;
	registroComida2.plato = registro2;

	pedido = malloc(sizeof(Pedido));
	list_add(registros_de_platos,&registroComida1);
	list_add(registros_de_platos,&registroComida2);

	pedido->nombre_restaurante = nombre_restaurante2;
	pedido->estadoPedido = CONFIRMADO;
	pedido->platos = registros_de_platos;
 */

/*  OBTENER_PEDIDO APP
 * 	nombre nombre_restaurante2;
		nombre_restaurante2.largo_nombre = strlen("Restaurante Italiano") + 1;
		nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
		nombre_restaurante2.nombre = "Restaurante Italiano";

		nombre registro1;
		registro1.largo_nombre = strlen("Registro1") + 1;
		registro1.nombre = malloc(registro1.largo_nombre);
		registro1.nombre = "Registro1";

		nombre registro2;
		registro2.largo_nombre = strlen("Registro2") + 1;
		registro2.nombre = malloc(registro2.largo_nombre);
		registro2.nombre = "Registro2";

		registros_de_platos=list_create();
		registroComida registroComida1;
		registroComida1.cantLista=5;
		registroComida1.cantTotal=25;
		registroComida1.plato = registro1;

		registroComida registroComida2;
		registroComida2.cantLista=5;
		registroComida2.cantTotal=25;
		registroComida2.plato = registro2;

		pedido = malloc(sizeof(Pedido));
		list_add(registros_de_platos,&registroComida1);
		list_add(registros_de_platos,&registroComida2);

		pedido->nombre_restaurante = nombre_restaurante2;
		pedido->estadoPedido = CONFIRMADO;
		pedido->platos = registros_de_platos;
 */

/* OBTENER_PEDIDO COMANDA
 * nombre nombre_restaurante2;
	nombre_restaurante2.largo_nombre = strlen("Restaurante Italiano") + 1;
	nombre_restaurante2.nombre = malloc(nombre_restaurante2.largo_nombre);
	nombre_restaurante2.nombre = "Restaurante Italiano";

	msg_obtener_pedido* mensaje = malloc(sizeof(msg_obtener_pedido));
	mensaje->restaurante = nombre_restaurante2;
	mensaje->id_pedido = 2;
	buffer_t* buffer = serializar_obtener_pedido(mensaje->restaurante,mensaje->id_pedido);
	sfd sfd_app = conectar_y_enviar("IP_COMANDA", "PUERTO_APP", buffer, OBTENER_PEDIDO);


	py_paquete_t* paquete = malloc(sizeof(py_paquete_t));
	py_recv_msg(sfd_app, paquete);

	rta_obtener_pedido* pedido = deserializarPedido(paquete->buffer);
	printf("EL estado del pedido recibido es: %d",pedido->estadoPedido);
 */

