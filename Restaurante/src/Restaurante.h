#ifndef RESTAURANTE_H_
#define RESTAURANTE_H_

#include <src/pylog.h>
#include <src/pyserialize.h>
#include <src/pymsg.h>
#include <src/pyconexiones.h>
#include <src/pysockets.h>
#include <src/pyhandshake.h>

//Variables
int terminado = 0;
t_config* configuracion;
int32_t id_autogenerado = 0;


//Info Restaurante
nombre* nombre_restaurante;
Restaurante* restaurante;
uint32_t cant_afinidades;
uint32_t procesadores_restantes;
uint32_t cant_hornos;
uint32_t cant_cocineros;


//Planifiacion
char* algoritmo_de_planificacion;
int32_t retardo_ciclo;
uint32_t cantidad_procesadores;

//Estructuras

typedef enum{
	NEW,
	READY,
	EXEC,
	BLOCKED,
	EXIT
}enum_estado;

typedef struct{
	nombre* nombre;
	Receta* receta;
}PlatoConReceta;

typedef struct{
	PlatoConReceta* plato;
	enum_estado estado;
	int32_t id_pedido;
	uint32_t indice_rafaga;
	int32_t indice_ciclo;
	uint32_t ultimo_plato;
}PCB;

typedef struct{
	nombre* nombre;
	t_queue* cola;
	int32_t* procesador;
	int32_t cantidad_procesadores;
}cola_afinidad;

typedef enum{
	REPOSAR = 0,
	HORNEAR
}enum_identificador;

typedef enum{
	FIFO = 0,
	RR
}enum_planificacion;


//Colecciones
t_list* lista_handshake_cliente;
t_list* sin_repetidos;
t_list* repetidos;
t_queue* cola_hornos;
t_queue* procesadores;
cola_afinidad* cola_afinidades;


//Hilos
pthread_t conexion_escucha;
sem_t* sem_cocinero_entrada;
sem_t* sem_cocinero_salida;
sem_t* sem_horno_entrada;
sem_t* sem_horno_salida;


//Semaforos
pthread_mutex_t mutex_cola_hornos;
pthread_mutex_t mutex_cola_afinidades;
pthread_mutex_t mutex_algoritmo;
pthread_mutex_t mutex_lista_handshake_cliente;


//Funciones

void obtener_metadata_restaurante();

void crear_colas_planificacion();
void crear_colas_afinidades();
void crear_cola_hornos();

void inicializar_semaforos();

void conexiones();
void enviar_handshake_APP();
void escuchar(sfd socket_restaurante);

void atender(sfd socket, enum_mensaje codigo_operacion,buffer_t*buffer);
void atender_consultar_modulo(sfd socket);
void atender_consultar_platos(sfd socket);
void atender_crear_pedido(sfd socket);
void atender_aniadir_plato(sfd socket,buffer_t*buffer);
void atender_confirmar_pedido(sfd socket,buffer_t* buffer);
void atender_consultar_pedido(sfd socket);

void crear_lista_platos(t_list* msg, t_list* platos);

void encolar_platos(Pedido* pedido, int32_t id_pedido);
PCB* crearPCB(registroComida* plato, int32_t id_pedido, Receta* receta);
void encolar_segun_afinidad(PCB* pcb);

void pasar_ciclo();
void ciclo_cocinero(int indice_cocinero);
void ciclo_horno(int indice_horno);

void inicializar_planificacion();
void planificar(int indice_cocinero);
void planificar_horno(int indice_horno);
int obtener_indice_cola(int indice_cocinero);

void realizar_paso(PCB* plato, Paso* paso, int indice_cocinero);
void reposar(PCB* plato, Paso* paso, int indice_cocinero);
void hornear(PCB* plato, Paso* paso, int indice_horno);
void otro_paso(PCB* plato, Paso* paso, int indice_cocinero);
void paso_siguiente(PCB* plato, Paso* paso);
void liberar_pcb(PCB* plato);
void plato_listo(PCB* plato);

bool cola_ready_is_empty(int indice_cocinero);
bool cola_blocked_is_empty();
PCB* extraer_cola_ready(int indice_cocinero);
PCB* extraer_cola_blocked();
void encolar_cola_ready(PCB* plato, int indice_afinidad);
void encolar_cola_blocked(PCB* plato);

uint32_t cantidad_repetidos(char* nombre);
t_list* lista_repetidos(char* nombre_afinidad);
uint32_t calcular_cantidad_afinidades();
t_list* lista_sin_repetidos();
bool no_se_repite(nombre* afinidad);

void atender_handshake_inicial_cliente(int socket_cliente, handshake_cliente* handshake);
void agregar_lista_handshake_cliente(handshake_cliente* handshake);
handshake_cliente* buscar_handshake_cliente(int32_t data);

#endif
