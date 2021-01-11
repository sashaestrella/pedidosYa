#ifndef APP_H_
#define APP_H_

#include <stddef.h>
#include <src/pycore.h>
#include <src/pylog.h>
#include <src/pyconfig.h>
#include <src/pyserialize.h>
#include <src/pymsg.h>
#include <src/pysockets.h>
#include <src/pyconexiones.h>

#include "tests.h"

int32_t restaurantes_conectados;

typedef enum{
	RESTAURANTE_DESTINO = 0,
	CLIENTE = 1
}py_cliente;

typedef struct{
	char* ip;
	char* puerto;
}Direccion;

typedef struct{
	char* puerto;
	nombre* nombre_restaurante;
	CoordenadasXY coordenadas;
}handshake_restaurante;

typedef struct{
	char* puerto;
	int32_t id;
	CoordenadasXY coordenadas;
}handshake_cliente;

typedef struct{
	sfd socket;
	char* ip;
}socket_ip;

typedef struct{
	Direccion* direccion;
	nombre* nombre_restaurante;
	CoordenadasXY coordenadas;
}handshake_restaurante_completo;

typedef struct{
	Direccion* direccion;
	int32_t id;
	CoordenadasXY coordenadas;
}handshake_cliente_completo;

typedef struct{
	int32_t id_cliente;
	nombre* restaurante;
}cliente_restaurante;

typedef enum{
	NEW = 0,
	READY = 1,
	BLOCKED_ESPERANDO = 2,                             //Está esperando en el restaurante por la finalización de los platos
	BLOCKED_DESCANSANDO_ESPERANDO = 3,                 //Está esperando en el restaurante por la finalización de los platos y está descansando
	BLOCKED_DESCANSANDO_TRABAJANDO = 4,                //Está descansando y trabajando
	BLOCKED_DESCANSANDO_LIBRE = 5,                     //Está descansando y además no tiene ningún pedido asignado
	BLOCKED_LIBRE = 6,                                 //Está esperando que se le asigne un pedido para repartir
	EXEC = 7,
	EXIT = 8
}estado_repartidor;

typedef struct{
	CoordenadasXY ubicacion;
	int32_t frecuencia_descanso;
	int32_t tiempo_descanso;
	estado_repartidor estado;
	CoordenadasXY destino;
	int32_t descanso_actual;
	int32_t cansancio_actual;
	py_cliente tipo_destino;
}repartidor;

typedef struct{
	nombre* nombre_restaurante;
	int32_t id_pedido;
}restaurante_pedido;

typedef struct{
	int32_t id_pedido;
	int32_t id_cliente;
	CoordenadasXY ubicacion_restaurante;
	CoordenadasXY ubicacion_cliente;
	nombre* nombre_restaurante;
	bool confirmado;
}PCB;

#define CAST_TO_NOMBRE_PTR(ptr)          ((nombre*)(ptr))

pthread_t conexion_escucha;

char* algoritmo_planificacion;
int32_t       retardo_ciclo;
int32_t       grado_multiprocesamiento;
char*         platos;
CoordenadasXY coordenadas_restaurante_default;

int32_t cantidad_repartidores;
int32_t cantidad_ready;

pthread_mutex_t sem_elegir_repartidor;
bool* pedido_listo;

//Particulares de HRRN
int32_t* aging;
float* tasa_respuesta;
pthread_mutex_t* mutex_tasa_respuesta;
pthread_mutex_t* mutex_aging;

//Particulares de SJF
double* estimacion_actual;
double* estimacion_anterior;
int32_t* rafaga_anterior;
int32_t* largo_rafaga;
double alpha;
double estimacion_inicial;

//Listas
t_list* lista_restaurantes;
t_list* lista_restaurante_clientes;
t_list* lista_restaurantes_default;
t_list* lista_handshake_cliente;
t_list* lista_handshake_restaurante;
t_list* lista_platos_default;
t_list* lista_repartidores;
t_list* lista_restaurante_pedido;
t_list* PCBs_temporales;

//Semaforos
pthread_mutex_t   mutex_lista_handshake_cliente;
pthread_mutex_t   mutex_lista_handshake_restaurante;
pthread_mutex_t   mutex_lista_restaurantes;
pthread_mutex_t   mutex_lista_restaurante_clientes;
pthread_mutex_t   mutex_lista_restaurante_clientes;
sem_t             sem_repartidores_disponibles;
pthread_mutex_t*  sem_repartidores;
sem_t             sem_habilitar_descanso;
sem_t*  sem_clock_entrada;
sem_t*  sem_clock_salida;
pthread_mutex_t   sem_cantidad_ready;
pthread_mutex_t   mutex_PCBs_temporales;

//Colas
t_queue* 		    colaRepartidoresReady;
pthread_mutex_t 	mutex_cola_repartidores;
sem_t           	sem_repartidores_ready;
t_queue* 		    colaPedidos;
pthread_mutex_t 	mutex_cola_PCB;
sem_t           	sem_hay_PCBs;

PCB**  PCBs_asignados;

nombre* nombre_restaurante_default;

void                   inicializar_semaforos();
void                   leer_config_file();
void                   inicializar_repartidores();
void                   mostrar_restaurantes();
void                   inicializar_listas();
void                   escuchar(socket_ip* si);
void                   conexiones();
void                   crear_lista_default();
void                   agregar_lista_handshake_cliente(handshake_cliente_completo* handshake);
void                   agregar_lista_handshake_restaurante(handshake_restaurante_completo* handshake);
void                   agregar_lista_restaurantes(nombre* nombre_restaurante);
void                   eliminar_restaurante_default();
void                   agregar_lista_restaurante_clientes(cliente_restaurante* elemento);
void                   atender_consultar_restaurante(int socket_restaurante);
void                   atender_seleccionar_restaurante(int socket_restaurante, nombre* nombre_restaurante, int32_t id_cliente);
buffer_t*              crear_buffer_vacio();
void                   atender_consultar_platos(int32_t id_cliente, int32_t socket_cliente);
void                   atender_crear_pedido(int32_t id_cliente, int32_t socket_cliente);
bool                   existe_plato_default(nombre* nombre_plato);
void                   atender_aniadir_plato(msg_aniadir_plato* mensaje, int32_t socket_cliente, int32_t id_cliente);
bool                   pedido_esta_listo(Pedido* un_pedido);
void                   destruir_pedido(Pedido* un_pedido);
void                   atender_plato_listo(buffer_t* mensaje, sfd socket);
void                   atender_confirmar_pedido(buffer_t* mensaje, int32_t id_cliente, int32_t socket_cliente);
void                   atender_consultar_pedido(int32_t socket_cliente, int32_t id_pedido, int32_t id_cliente);
void                   atender_handshake_inicial_cliente(int socket_cliente, handshake_cliente* handshake, char* ip);
void                   atender_handshake_inicial_restaurante(int socket_restaurante, handshake_restaurante* handshake, char* ip);
void                   atender_consultar_modulo(sfd socket);
void                   atender(sfd socket_restaurante, buffer_t* buffer, enum_mensaje codigo_operacion, char* ip);
void                   atender_cliente(sfd socket_cliente, int32_t id_cliente);
void                   agregar_PCB_temporal(PCB* un_pcb);
PCB*                   buscar_pcb_temporal_por_id_pedido(int32_t id_pedido, int32_t id_cliente);
int32_t                buscar_id_repartidor_por_id_pedido(int32_t id_pedido, nombre* nombre_restaurante);
nombre*                buscar_restaurante_por_nombre(char* data);
cliente_restaurante*   buscar_nombre_id_por_nombre(char* data);
cliente_restaurante*   buscar_nombre_id_por_id(int32_t data);
handshake_cliente_completo*     buscar_handshake_cliente(int32_t id);
handshake_restaurante_completo* buscar_handshake_restaurante(nombre* nombre_resto);
void                   liberar_lista_nombres(t_list* lista_nombres);
int32_t                generar_id_pedido_default();
char*                  sacar_corchetes(char* string);
int32_t                cantidad_elementos_string(char* string);
char**                 string_a_array(char* array_string);
void                   cargar_lista_string(char* array_string, t_list* lista);
void                   mostrar_entrenadores(int32_t cantidad_repartidores);
int32_t                primer_repartidor_disponible();
bool                   repartidor_disponible(int32_t indice_entrenador);
uint32_t               comparar_posiciones(CoordenadasXY p1, CoordenadasXY p2);
uint32_t               distancia_repartidor_a_coordenada(int32_t indice_repartidor, CoordenadasXY posicion);
int32_t                buscar_repartidor_cercano(CoordenadasXY posicion);
void                   setear_estado_repartidor(estado_repartidor estado, int32_t indice);
bool                   esFIFO();
bool                   esHRRN();
bool                   esSJF();
void                   planificar_base();
void                   planificar(int32_t id_procesador);
void                   asignar_pedidos_clientes();
void                   inicializarHRRN(int32_t cantidad_repartidores);
void                   inicializarSJF(int32_t cantidad_repartidores, double estimacion_inicial);
void                   planificarFIFO(int32_t id_procesador);
void                   planificarHRRN(int32_t id_procesador);
void                   planificarSJF(int32_t id_procesador, int32_t alpha);
void                   procesoIDLE(int32_t id_procesador);
void                   avisar_llegada_restaurante(int32_t id_repartidor);
void                   tratar_repartidor_despues_de_moverse(int32_t indice_repartidor);
void                   actualizar_destino(int32_t indice_repartidor);
void                   descansar_repartidores();
void                   poner_en_ready(int32_t indice);
void                   descansar_repartidor(int32_t indice);
void                   envejecer();
int32_t                calcular_rafaga(int32_t indice);
int32_t                primer_repartidor_ready();
int32_t                elegir_repartidor_HRRN();
int32_t                elegir_repartidor_SJF();
void                   calcular_tasa_respuesta(int32_t indice);
void                   calcular_tasas_respuesta();
void                   calcular_estimacion(int32_t indice);
void                   calcular_estimaciones();
void                   inicializar_cola_ready();
void                   inicializar_cola_PCB();
void                   cola_PCB_push(PCB* un_pedido) ;
PCB*                   cola_PCB_pop();
void                   cola_ready_push(int32_t indiceEntrenador);
int32_t                cola_ready_pop();
bool                   cola_ready_is_empty();
void                   mover_repartidor(int32_t indice, int32_t cantidad_ciclos, int32_t id_procesador);
void                   chequear_pedidos_listos();
void                   pasar_ciclo();
void                   habilitar_descanso();
bool                   nadie_ready();
int32_t                recibir_respuesta_ok_fail(sfd socket_respuesta);
bool                   llego_a_destino(int32_t indice);
estado_repartidor      get_estado(int32_t indice);
void                   liberar_PCB(PCB* un_PCB);
void                   terminar_programa();





Pedido* pedido; //pedido obtenido de comanda para la respuesta de CONSULTAR_PEDIDO


#endif
