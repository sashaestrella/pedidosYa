#include "tests.h"


void prueba_planificar_en_fifo_multiprocesamiento_1(){

	PCB* pcb1 = malloc(sizeof(PCB));
	PCB* pcb2 = malloc(sizeof(PCB));
	PCB* pcb3 = malloc(sizeof(PCB));
	PCB* pcb4 = malloc(sizeof(PCB));
	PCB* pcb5 = malloc(sizeof(PCB));

	CoordenadasXY direccion1;
	CoordenadasXY direccion2;
	CoordenadasXY direccion3;
	CoordenadasXY direccion4;
	CoordenadasXY direccion5;

	nombre* resto_uno    = malloc(sizeof(nombre));
	nombre* resto_dos    = malloc(sizeof(nombre));
	nombre* resto_tres   = malloc(sizeof(nombre));
	nombre* resto_cuatro = malloc(sizeof(nombre));
	nombre* resto_cinco  = malloc(sizeof(nombre));

	direccion1.posicionX = 20;
	direccion1.posicionY = 20;
	direccion2.posicionX = 18;
	direccion2.posicionY = 18;
	direccion3.posicionX = 17;
	direccion3.posicionY = 17;
	direccion4.posicionX = 11;
	direccion4.posicionY = 13;
	direccion5.posicionX = 13;
	direccion5.posicionY = 12;

	resto_uno->largo_nombre = strlen("Restaurante Italiano") + 1;
	//resto_uno->nombre = malloc(resto_uno->largo_nombre);
	resto_uno->nombre = "Restaurante Italiano";

	resto_dos->largo_nombre = strlen("Restaurante Inalambrico") + 1;
	//resto_dos->nombre = malloc(resto_dos->largo_nombre);
	resto_dos->nombre = "Restaurante Inalambrico";

	resto_tres->largo_nombre = strlen("Restaurante Mexicano") + 1;
	//resto_tres->nombre = malloc(resto_uno->largo_nombre);
	resto_tres->nombre = "Restaurante Mexicano";

	resto_cuatro->largo_nombre = strlen("Restaurante Japones") + 1;
	//resto_cuatro->nombre = malloc(resto_dos->largo_nombre);
	resto_cuatro->nombre = "Restaurante Japones";

	resto_cinco->largo_nombre = strlen("Restaurante Barato") + 1;
	//resto_cinco->nombre = malloc(resto_uno->largo_nombre);
	resto_cinco->nombre = "Restaurante Barato";

	pcb1->ubicacion_restaurante = direccion1;
	pcb2->ubicacion_restaurante = direccion2;
	pcb3->ubicacion_restaurante = direccion3;
	pcb4->ubicacion_restaurante = direccion4;
	pcb5->ubicacion_restaurante = direccion5;

	pcb1->ubicacion_cliente = direccion5;
	pcb3->ubicacion_cliente = direccion4;
	pcb2->ubicacion_cliente = direccion3;
	pcb4->ubicacion_cliente = direccion2;
	pcb5->ubicacion_cliente = direccion1;

    pcb1->nombre_restaurante = resto_uno;
    pcb2->nombre_restaurante = resto_dos;
    pcb3->nombre_restaurante = resto_tres;
    pcb4->nombre_restaurante = resto_cuatro;
    pcb5->nombre_restaurante = resto_cinco;

    pcb1->id_pedido = 1;
    pcb2->id_pedido = 2;
    pcb3->id_pedido = 3;
    pcb4->id_pedido = 4;
    pcb5->id_pedido = 5;

	cola_PCB_push(pcb1);
	cola_PCB_push(pcb2);
	cola_PCB_push(pcb3);
	cola_PCB_push(pcb4);
	cola_PCB_push(pcb5);

	planificar_base();
}

void prueba_mover_repartidor(){
	mover_repartidor(0, 1, 0);
	mover_repartidor(0, 9, 0);
	mover_repartidor(0, 6, 0);
	mover_repartidor(0, 2, 0);
}

void prueba_primer_repartidor_disponible(){
	setear_estado_repartidor(READY, 1);
	setear_estado_repartidor(READY, 2);
	printf("%i \n", primer_repartidor_disponible());
}

void prueba_buscar_repartidor_cercano(){
	CoordenadasXY una_direccion;
	una_direccion.posicionX = 10;
	una_direccion.posicionY = 10;
	setear_estado_repartidor(BLOCKED_DESCANSANDO_TRABAJANDO, 0);
	setear_estado_repartidor(READY, 2);

	int32_t indice = buscar_repartidor_cercano(una_direccion);

	printf("El entrenador más cercano a la ubicación es: %i", indice);
}

void prueba_distancia_repartidor_a_coordenada(){
	CoordenadasXY una_direccion;
	una_direccion.posicionX = 10;
	una_direccion.posicionY = 10;
	int32_t distancia = distancia_repartidor_a_coordenada(1, una_direccion);

	printf("La distancia del entrenador 1 a la posición marcada es: %i \n", distancia);
}

void prueba_comparar_posiciones(){
	CoordenadasXY una_direccion;
	una_direccion.posicionX = 10;
	una_direccion.posicionY = 10;

	CoordenadasXY otra_direccion;
	otra_direccion.posicionX = 0;
	otra_direccion.posicionY = 5;

	int32_t distancia = comparar_posiciones(una_direccion, otra_direccion);

	printf("La distancia entre las direcciones es: %i \n", distancia);
}

void prueba_entrenador_disponible(){
	if(repartidor_disponible(0)){
		printf("El entrenador 0 está disponible \n");
	}
}

void prueba_buscar_restaurante_por_nombre(){
	nombre* resto_uno = malloc(sizeof(nombre));
	nombre* resto_dos = malloc(sizeof(nombre));

	resto_uno->largo_nombre = strlen("Restaurante Italiano") + 1;
	resto_uno->nombre = malloc(resto_uno->largo_nombre);
	resto_uno->nombre = "Restaurante Italiano";

	resto_dos->largo_nombre = strlen("Restaurante Inalambrico") + 1;
	resto_dos->nombre = malloc(resto_dos->largo_nombre);
	resto_dos->nombre = "Restaurante Inalambrico";

	agregar_lista_restaurantes(resto_uno);
	agregar_lista_restaurantes(resto_dos);

	nombre* nombre_prueba = buscar_restaurante_por_nombre("Restaurante Italiano");
	if(nombre_prueba){
		printf("Nombre: %s", nombre_prueba->nombre);
	}
	else{
		printf("El restaurante no existe :c");
	}
}

void prueba_buscar_restaurante_cliente(){
	lista_restaurante_clientes = list_create();

	cliente_restaurante* uno = malloc(sizeof(cliente_restaurante));
	cliente_restaurante* dos = malloc(sizeof(cliente_restaurante));

	uno->id_cliente = 15;
	uno->restaurante = malloc(sizeof(nombre));
	uno->restaurante->largo_nombre = strlen("Restaurante Italano") + 1;
	uno->restaurante->nombre = malloc(uno->restaurante->largo_nombre);
	uno->restaurante->nombre = "Restaurante Italano";

	dos->id_cliente = 42;
	dos->restaurante = malloc(sizeof(nombre));
	dos->restaurante->largo_nombre = strlen("Restaurante Inalambrico") + 1;
	dos->restaurante->nombre = malloc(uno->restaurante->largo_nombre);
	dos->restaurante->nombre = "Restaurante Inalambrico";

	agregar_lista_restaurante_clientes(uno);
	agregar_lista_restaurante_clientes(dos);

	cliente_restaurante* prueba_uno = buscar_nombre_id_por_nombre("Restaurante Inalambrico");
	cliente_restaurante* prueba_dos = buscar_nombre_id_por_id(15);
	cliente_restaurante* prueba_tres = buscar_nombre_id_por_id(35);

	printf("Nombre restaurante 1: %s \nId:%i \n\n", prueba_dos->restaurante->nombre, prueba_dos->id_cliente);
    printf("Nombre restaurante 2: %s \nId:%i \n\n", prueba_uno->restaurante->nombre, prueba_uno->id_cliente);
    if(!prueba_tres){
    	printf("El restaurante con id 35 no existe");
    }
}
/*
void prueba_handshake_restaurante(){
	handshake_restaurante handshake;
	handshake.coordenadas.posicionX = 4;
	handshake.coordenadas.posicionY = 5;
	handshake.direccion = malloc(sizeof(Direccion));
	handshake.nombre_restaurante = malloc(sizeof(nombre));
	handshake.direccion->ip = "127.0.0.1";
	handshake.direccion->puerto = "4040";
	handshake.nombre_restaurante->largo_nombre = strlen("Restaurante Nuevo") + 1;
	handshake.nombre_restaurante->nombre = "Restaurante Nuevo";
	buffer_t* buffer = serializar_handshake_restaurante(handshake);

	handshake_restaurante* handshake_2;
	handshake_2 = deserializar_handshake_restaurante(buffer);

	printf("Posicion: (%i,%i) \n", handshake_2->coordenadas.posicionX, handshake_2->coordenadas.posicionY);
	printf("Ip: %s \nPuerto: %s \n", handshake_2->direccion->ip, handshake_2->direccion->puerto);
	printf("Nombre: %s\n", handshake_2->nombre_restaurante->nombre);
}

void prueba_handshake_cliente(){
	handshake_cliente handshake;
	handshake.coordenadas.posicionX = 2;
	handshake.coordenadas.posicionY = 8;
	handshake.direccion = malloc(sizeof(Direccion));
	handshake.direccion->ip = "127.37.45.11";
	handshake.direccion->puerto = "8083";
	handshake.id = 5;
	buffer_t* buffer = serializar_handshake_cliente(handshake);

	handshake_cliente* handshake_2;
	handshake_2 = deserializar_handshake_cliente(buffer);

	printf("Posicion: (%i,%i) \n", handshake_2->coordenadas.posicionX, handshake_2->coordenadas.posicionY);
	printf("Ip: %s \nPuerto: %s \n", handshake_2->direccion->ip, handshake_2->direccion->puerto);
	printf("ID: %i\n", handshake_2->id);
}
*/
