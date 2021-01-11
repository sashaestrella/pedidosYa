#include "tests.h"

void prueba_multiprocesamiento(){
	Receta* receta2 = malloc(sizeof(Receta));
	Receta* receta3 = malloc(sizeof(Receta));

	Paso* paso4 = malloc(sizeof(Paso));
	paso4->nombre = malloc(sizeof(nombre));
	paso4->nombre->largo_nombre = 8;
	paso4->nombre->nombre = malloc(8);
	paso4->nombre->nombre = "REBOZAR";
	paso4->tiempo = 3;

	Paso* paso5 = malloc(sizeof(Paso));
	paso5->nombre = malloc(sizeof(nombre));
	paso5->nombre->largo_nombre = 9;
	paso5->nombre->nombre = malloc(9);
	paso5->nombre->nombre = "APLASTAR";
	paso5->tiempo = 2;

	Paso* paso6 = malloc(sizeof(Paso));
	paso6->nombre = malloc(sizeof(nombre));
	paso6->nombre->largo_nombre = 8;
	paso6->nombre->nombre = malloc(8);
	paso6->nombre->nombre = "REPOSAR";
	paso6->tiempo = 4;

	Paso* paso7 = malloc(sizeof(Paso));
	paso7->nombre = malloc(sizeof(nombre));
	paso7->nombre->largo_nombre = 7;
	paso7->nombre->nombre = malloc(7);
	paso7->nombre->nombre = "AMASAR";
	paso7->tiempo = 3;

	Paso* paso8 = malloc(sizeof(Paso));
	paso8->nombre = malloc(sizeof(nombre));
	paso8->nombre->largo_nombre = 8;
	paso8->nombre->nombre = malloc(8);
	paso8->nombre->nombre = "ESTIRAR";
	paso8->tiempo = 2;

	Paso* paso9 = malloc(sizeof(Paso));
	paso9->nombre = malloc(sizeof(nombre));
	paso9->nombre->largo_nombre = 7;
	paso9->nombre->nombre = malloc(7);
	paso9->nombre->nombre = "HERVIR";
	paso9->tiempo = 4;

	receta2->Pasos = list_create();
	list_add(receta2->Pasos, (void*)paso4);
	list_add(receta2->Pasos, (void*)paso5);
	list_add(receta2->Pasos, (void*)paso6);

	receta3->Pasos = list_create();
	list_add(receta3->Pasos, (void*)paso7);
	list_add(receta3->Pasos, (void*)paso8);
	list_add(receta3->Pasos, (void*)paso9);
}

/*
void inicializar_restaurante(){
	restaurante = malloc(sizeof(InfoRestaurante));

	restaurante->afinidades = list_create();

	nombre* afinidad1 = malloc(sizeof(nombre));
	afinidad1->largo_nombre = strlen("Pizza") + 1;
	afinidad1->nombre = malloc(afinidad1->largo_nombre);
	afinidad1->nombre = "Pizza";

	nombre* afinidad2 = malloc(sizeof(nombre));
	afinidad2->largo_nombre = strlen("Milanesa") + 1;
	afinidad2->nombre = malloc(afinidad2->largo_nombre);
	afinidad2->nombre = "Milanesa";

	nombre* afinidad3 = malloc(sizeof(nombre));
	afinidad3->largo_nombre = strlen("Pizza") + 1;
	afinidad3->nombre = malloc(afinidad3->largo_nombre);
	afinidad3->nombre = "Pizza";

	nombre* afinidad4 = malloc(sizeof(nombre));
	afinidad4->largo_nombre = strlen("Milanesa") + 1;
	afinidad4->nombre = malloc(afinidad4->largo_nombre);
	afinidad4->nombre = "Milanesa";

	nombre* afinidad5 = malloc(sizeof(nombre));
	afinidad5->largo_nombre = strlen("Pasta") + 1;
	afinidad5->nombre = malloc(afinidad5->largo_nombre);
	afinidad5->nombre = "Pasta";

	nombre* afinidad6 = malloc(sizeof(nombre));
	afinidad6->largo_nombre = strlen("Empanada") + 1;
	afinidad6->nombre = malloc(afinidad6->largo_nombre);
	afinidad6->nombre = "Empanada";

	nombre* afinidad7 = malloc(sizeof(nombre));
	afinidad7->largo_nombre = strlen("Asado") + 1;
	afinidad7->nombre = malloc(afinidad7->largo_nombre);
	afinidad7->nombre = "Asado";

	nombre* afinidad8 = malloc(sizeof(nombre));
	afinidad8->largo_nombre = strlen("Pizza") + 1;
	afinidad8->nombre = malloc(afinidad8->largo_nombre);
	afinidad8->nombre = "Pizza";

	list_add(restaurante->afinidades, (void*)afinidad1);
	list_add(restaurante->afinidades, (void*)afinidad2);
	list_add(restaurante->afinidades, (void*)afinidad3);
	list_add(restaurante->afinidades, (void*)afinidad4);
	list_add(restaurante->afinidades, (void*)afinidad5);
	list_add(restaurante->afinidades, (void*)afinidad6);
	list_add(restaurante->afinidades, (void*)afinidad7);
	list_add(restaurante->afinidades, (void*)afinidad8);

	restaurante->cantidadDeHornos = 5;
	restaurante->cantidadDeCocineros = 9;

	cant_cocineros = restaurante->cantidadDeCocineros;

	procesadores = queue_create();

	for(int i = 1; i < cant_cocineros + 1; i++){
		queue_push(procesadores, (void*)i);
	}
}
*/

void crear_pedidos(){
	Pedido* pedido1 = malloc(sizeof(Pedido));

	registroComida* plato1 = malloc(sizeof(registroComida));
	plato1->plato = malloc(sizeof(nombre));
	plato1->plato->largo_nombre = 6;
	plato1->plato->nombre = malloc(6);
	plato1->plato->nombre = "Pizza";

	registroComida* plato2 = malloc(sizeof(registroComida));
	plato2->plato = malloc(sizeof(nombre));
	plato2->plato->largo_nombre = 6;
	plato2->plato->nombre = malloc(6);
	plato2->plato->nombre = "Pasta";

	pedido1->platos = list_create();
	list_add(pedido1->platos, (void*)plato1);
	list_add(pedido1->platos, (void*)plato2);

	Pedido* pedido2 = malloc(sizeof(Pedido));

	registroComida* plato3 = malloc(sizeof(registroComida));
	plato3->plato = malloc(sizeof(nombre));
	plato3->plato->largo_nombre = 9;
	plato3->plato->nombre = malloc(9);
	plato3->plato->nombre = "Milanesa";

	registroComida* plato4 = malloc(sizeof(registroComida));
	plato4->plato = malloc(sizeof(nombre));
	plato4->plato->largo_nombre = 10;
	plato4->plato->nombre = malloc(10);
	plato4->plato->nombre = "Empanadas";

	pedido2->platos = list_create();
	list_add(pedido2->platos, (void*)plato3);
	list_add(pedido2->platos, (void*)plato4);

	Pedido* pedido3 = malloc(sizeof(Pedido));

	registroComida* plato5 = malloc(sizeof(registroComida));
	plato5->plato = malloc(sizeof(nombre));
	plato5->plato->largo_nombre = 6;
	plato5->plato->nombre = malloc(6);
	plato5->plato->nombre = "Locro";

	registroComida* plato6 = malloc(sizeof(registroComida));
	plato6->plato = malloc(sizeof(nombre));
	plato6->plato->largo_nombre = 9;
	plato6->plato->nombre = malloc(9);
	plato6->plato->nombre = "Estofado";

	pedido3->platos = list_create();
	list_add(pedido3->platos, (void*)plato5);
	list_add(pedido3->platos, (void*)plato6);

	Pedido* pedido4 = malloc(sizeof(Pedido));

	registroComida* plato7 = malloc(sizeof(registroComida));
	plato7->plato = malloc(sizeof(nombre));
	plato7->plato->largo_nombre = 10;
	plato7->plato->nombre = malloc(10);
	plato7->plato->nombre = "Spaghetti";

	registroComida* plato8 = malloc(sizeof(registroComida));
	plato8->plato = malloc(sizeof(nombre));
	plato8->plato->largo_nombre = 6;
	plato8->plato->nombre = malloc(6);
	plato8->plato->nombre = "Pizza";

	pedido4->platos = list_create();
	list_add(pedido4->platos, (void*)plato7);
	list_add(pedido4->platos, (void*)plato8);

	encolar_platos(pedido1, 1, 0);
	encolar_platos(pedido2, 2, 2);
	encolar_platos(pedido3, 3, 4);
	encolar_platos(pedido4, 4, 6);
}

void crear_recetas(){
	Receta* receta1 = malloc(sizeof(Receta));
	Receta* receta2 = malloc(sizeof(Receta));
	Receta* receta3 = malloc(sizeof(Receta));
	Receta* receta4 = malloc(sizeof(Receta));
	Receta* receta5 = malloc(sizeof(Receta));
	Receta* receta6 = malloc(sizeof(Receta));
	Receta* receta7 = malloc(sizeof(Receta));
	Receta* receta8 = malloc(sizeof(Receta));

	Receta** recetas = calloc(8, sizeof(Receta));

	Paso* paso1 = malloc(sizeof(Paso));
	paso1->nombre = malloc(sizeof(nombre));
	paso1->nombre->largo_nombre = 7;
	paso1->nombre->nombre = malloc(7);
	paso1->nombre->nombre = "AMASAR";
	paso1->tiempo = 3;

	Paso* paso2 = malloc(sizeof(Paso));
	paso2->nombre = malloc(sizeof(nombre));
	paso2->nombre->largo_nombre = 8;
	paso2->nombre->nombre = malloc(8);
	paso2->nombre->nombre = "REPOSAR";
	paso2->tiempo = 2;

	Paso* paso3 = malloc(sizeof(Paso));
	paso3->nombre = malloc(sizeof(nombre));
	paso3->nombre->largo_nombre = 8;
	paso3->nombre->nombre = malloc(8);
	paso3->nombre->nombre = "HORNEAR";
	paso3->tiempo = 4;

	receta1->Pasos = list_create();
	list_add(receta1->Pasos, (void*)paso1);
	list_add(receta1->Pasos, (void*)paso2);
	list_add(receta1->Pasos, (void*)paso3);

	Paso* paso4 = malloc(sizeof(Paso));
	paso4->nombre = malloc(sizeof(nombre));
	paso4->nombre->largo_nombre = 7;
	paso4 ->nombre->nombre = malloc(7);
	paso4->nombre->nombre = "AMASAR";
	paso4->tiempo = 3;

	Paso* paso5 = malloc(sizeof(Paso));
	paso5->nombre = malloc(sizeof(nombre));
	paso5->nombre->largo_nombre = 8;
	paso5->nombre->nombre = malloc(8);
	paso5->nombre->nombre = "REPOSAR";
	paso5->tiempo = 1;

	Paso* paso6 = malloc(sizeof(Paso));
	paso6->nombre = malloc(sizeof(nombre));
	paso6->nombre->largo_nombre = 8;
	paso6->nombre->nombre = malloc(8);
	paso6->nombre->nombre = "COCINAR";
	paso6->tiempo = 3;

	receta2->Pasos = list_create();
	list_add(receta2->Pasos, (void*)paso4);
	list_add(receta2->Pasos, (void*)paso5);
	list_add(receta2->Pasos, (void*)paso6);

	Paso* paso7 = malloc(sizeof(Paso));
	paso7->nombre = malloc(sizeof(nombre));
	paso7->nombre->largo_nombre = 9;
	paso7->nombre->nombre = malloc(9);
	paso7->nombre->nombre = "APLASTAR";
	paso7->tiempo = 2;

	Paso* paso8 = malloc(sizeof(Paso));
	paso8->nombre = malloc(sizeof(nombre));
	paso8->nombre->largo_nombre = 8;
	paso8->nombre->nombre = malloc(8);
	paso8->nombre->nombre = "REBOSAR";
	paso8->tiempo = 5;

	Paso* paso9 = malloc(sizeof(Paso));
	paso9->nombre = malloc(sizeof(nombre));
	paso9->nombre->largo_nombre = 8;
	paso9->nombre->nombre = malloc(8);
	paso9->nombre->nombre = "HORNEAR";
	paso9->tiempo = 4;

	receta3->Pasos = list_create();
	list_add(receta3->Pasos, (void*)paso7);
	list_add(receta3->Pasos, (void*)paso8);
	list_add(receta3->Pasos, (void*)paso9);

	Paso* paso10 = malloc(sizeof(Paso));
	paso10->nombre = malloc(sizeof(nombre));
	paso10->nombre->largo_nombre = 12;
	paso10->nombre->nombre = malloc(12);
	paso10->nombre->nombre = "HACER TAPAS";
	paso10->tiempo = 3;

	Paso* paso11 = malloc(sizeof(Paso));
	paso11->nombre = malloc(sizeof(nombre));
	paso11->nombre->largo_nombre = 9;
	paso11->nombre->nombre = malloc(9);
	paso11->nombre->nombre = "RELLENAR";
	paso11->tiempo = 2;

	Paso* paso12 = malloc(sizeof(Paso));
	paso12->nombre = malloc(sizeof(nombre));
	paso12->nombre->largo_nombre = 8;
	paso12->nombre->nombre = malloc(8);
	paso12->nombre->nombre = "HORNEAR";
	paso12->tiempo = 4;

	receta4->Pasos = list_create();
	list_add(receta4->Pasos, (void*)paso10);
	list_add(receta4->Pasos, (void*)paso11);
	list_add(receta4->Pasos, (void*)paso12);

	Paso* paso13 = malloc(sizeof(Paso));
	paso13->nombre = malloc(sizeof(nombre));
	paso13->nombre->largo_nombre = 14;
	paso13->nombre->nombre = malloc(14);
	paso13->nombre->nombre = "PONER EL AGUA";
	paso13->tiempo = 2;

	Paso* paso14 = malloc(sizeof(Paso));
	paso14->nombre = malloc(sizeof(nombre));
	paso14->nombre->largo_nombre = 8;
	paso14->nombre->nombre = malloc(8);
	paso14->nombre->nombre = "COCINAR";
	paso14->tiempo = 5;

	receta5->Pasos = list_create();
	list_add(receta5->Pasos, (void*)paso13);
	list_add(receta5->Pasos, (void*)paso14);

	Paso* paso15 = malloc(sizeof(Paso));
	paso15->nombre = malloc(sizeof(nombre));
	paso15->nombre->largo_nombre = 8;
	paso15->nombre->nombre = malloc(8);
	paso15->nombre->nombre = "REPOSAR";
	paso15->tiempo = 3;

	Paso* paso16 = malloc(sizeof(Paso));
	paso16->nombre = malloc(sizeof(nombre));
	paso16->nombre->largo_nombre = 8;
	paso16->nombre->nombre = malloc(8);
	paso16->nombre->nombre = "HORNEAR";
	paso16->tiempo = 6;

	receta6->Pasos = list_create();
	list_add(receta6->Pasos, (void*)paso15);
	list_add(receta6->Pasos, (void*)paso16);

	Paso* paso17 = malloc(sizeof(Paso));
	paso17->nombre = malloc(sizeof(nombre));
	paso17->nombre->largo_nombre = 8;
	paso17->nombre->nombre = malloc(8);
	paso17->nombre->nombre = "REPOSAR";
	paso17->tiempo = 4;

	receta7->Pasos = list_create();
	list_add(receta7->Pasos, (void*)paso17);

	Paso* paso18 = malloc(sizeof(Paso));
	paso18->nombre = malloc(sizeof(nombre));
	paso18->nombre->largo_nombre = 8;
	paso18->nombre->nombre = malloc(8);
	paso18->nombre->nombre = "SAZONAR";
	paso18->tiempo = 5;

	Paso* paso19 = malloc(sizeof(Paso));
	paso19->nombre = malloc(sizeof(nombre));
	paso19->nombre->largo_nombre = 12;
	paso19->nombre->nombre = malloc(12);
	paso19->nombre->nombre = "CONDIMENTAR";
	paso19->tiempo = 2;

	Paso* paso20 = malloc(sizeof(Paso));
	paso20->nombre = malloc(sizeof(nombre));
	paso20->nombre->largo_nombre = 8;
	paso20->nombre->nombre = malloc(8);
	paso20->nombre->nombre = "HORNEAR";
	paso20->tiempo = 2;

	receta8->Pasos = list_create();
	list_add(receta8->Pasos, (void*)paso18);
	list_add(receta8->Pasos, (void*)paso19);
	list_add(receta8->Pasos, (void*)paso20);

	recetas[0] = receta1;
	recetas[1] = receta2;
	recetas[2] = receta3;
	recetas[3] = receta4;
	recetas[4] = receta5;
	recetas[5] = receta6;
	recetas[6] = receta7;
	recetas[7] = receta8;
}

void cargar_pcb(){
	/*
	PCB* pcb = crearPCB(plato, id_pedido, recetas[indice]);
	encolar_segun_afinidad(pcb);
	indice++;
	*/
}

/*
void prueba_individual(){
	crearReceta AsadoJugoso [Cortar,Hornear,Servir] [4,5,2]
	crearReceta AsadoSeco [Cortar,Hornear,Servir] [4,10,2]
	crearReceta Choripan [Cortar,Hornear,Servir] [1,2,1]
	crearReceta TortillaDePapa [Cortar,Hervir,Hornear,Servir] [2,2,3,1]
	crearRestaurante ElParrillon 2 [5,5] [AsadoJugoso] [AsadoJugoso,AsadoSeco,Choripan,TortillaDePapa] [180,200,100,150] 1

}
*/
