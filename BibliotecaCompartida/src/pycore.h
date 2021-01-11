#ifndef PY_CORE_H
#define PY_CORE_H

#define DB_NON_NULL(...)	__attribute__((nonnull(__VA_ARGS__)))

#define DB_ARGS(...) __VA_ARGS__

#define DB_LAMBDA(return_type, args, code)\
	({ return_type __f__(args) code __f__; })

//C Standard libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

//Unix libs
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

//Readline
//#include <readline/readline.h>

//Sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>

//UTN Commons
#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/bitarray.h>
#include <commons/config.h>
#include <commons/temporal.h>

#define DB_CAST_TO_UINT32(buffer)      *((uint32_t*)(buffer))

typedef int32_t sfd;

typedef enum
{
	SUCCESS = 0,
	LOOK_UP_ERROR,
	BIND_ERROR,
	LISTEN_ERROR,
	CONN_ERROR,
	CONN_LOST,
	SERIALIZE_ERROR,
	DESERIALIZE_ERROR,
	SEND_ERROR,
	RECV_ERROR,
	PARTIAL_SEND,
	PARTIAL_RECV,
	LOGGER_ERROR,
	MUTEX_INIT_ERROR,
	MEM_ERROR,
	CONFIG_ERROR
} py_enum;

typedef enum {
	CONSULTAR_RESTAURANTE = 0,
	SELECCIONAR_RESTAURANTE = 1,
	OBTENER_RESTAURANTE = 2,
	CONSULTAR_PLATOS = 3,
	CREAR_PEDIDO = 4,
	GUARDAR_PEDIDO = 5,
	ANIADIR_PLATO = 6,
	GUARDAR_PLATO = 7,
	CONFIRMAR_PEDIDO = 8,
	PLATO_LISTO = 9,
	CONSULTAR_PEDIDO = 10,
	OBTENER_PEDIDO = 11,
	FINALIZAR_PEDIDO = 12,
	TERMINAR_PEDIDO = 13,
	OBTENER_RECETA = 14,
	CONSULTAR_RESTAURANTE_RTA = 15,
	OBTENER_RESTAURANTE_RTA = 16,
	CONSULTAR_PLATOS_RTA = 17,
	CREAR_PEDIDO_RTA = 18,
	CONSULTAR_PEDIDO_RTA = 19,
	OBTENER_PEDIDO_RTA = 20,
	OBTENER_RECETA_RTA = 21,
	OK_FAIL = 22,
	HANDSHAKE_INICIAL_CLIENTE = 23,
	HANDSHAKE_INICIAL_RESTAURANTE = 24,
	HANDSHAKE_NORMAL_CLIENTE = 25,
	ANADIR_PLATO_RTA=26,
	CONFIRMAR_PEDIDO_RTA=27,
	PLATO_LISTO_RTA=28,
	CONSULTAR_MODULO=29
}enum_mensaje;

typedef enum {
	APP=0,
	RESTAURANTE=1,
	SINDICATO=2,
	COMANDA=3
}enum_modulo;

typedef struct{
	uint32_t size;
	void*    stream;
}buffer_t;

typedef struct{
	enum_mensaje codigo_operacion;
	buffer_t*    buffer;
}py_paquete_t;

typedef struct{
	uint32_t largo_nombre;
	char*    nombre;
}nombre;

typedef struct{
	uint32_t posicionX;
	uint32_t posicionY;
}CoordenadasXY;

typedef struct{
	t_list*       afinidades;
	CoordenadasXY Posicion;
	t_list*       Platos;
	uint32_t      cantidadDeHornos;
	int32_t      cantidadDePedidos;
	uint32_t 	  cantidadDeCocineros;
}InfoRestaurante;

typedef struct{
	nombre* nombre_restaurante;
	InfoRestaurante info_Restaurante;
}Restaurante;

typedef struct{
	nombre*   nombre;
	uint32_t tiempo;
}Paso;

typedef struct{
	t_list* Pasos;
}Receta;

typedef struct{
	nombre* nombreReceta;
	Receta receta;
}RecetaConNombre; //para poder identificar la receta
typedef struct{
    Receta receta;
    int32_t precio;
}RecetaConPrecio;

typedef struct{
	CoordenadasXY Posicion;
	uint32_t      frecuencia_descanso;
	uint32_t      tiempo_descanso;
}Repartidor;

typedef struct{
	nombre* nombre;
	uint32_t precio;
}Plato;

typedef enum{
	PENDIENTE = 0,
	CONFIRMADO = 1,
	TERMINADO = 2,
	SINPEDIDO = 3
}Estado_pedido;

typedef struct{
	Estado_pedido estadoPedido;
	t_list*       platos; //lista de registroComida
	uint32_t      precioTotal;
}Pedido;

typedef struct{
	nombre* nombre_restaurante;
	Pedido pedido;
}msg_consultar_pedido_rta;

typedef struct{
	nombre*  restaurante;
	int32_t id_cliente;
}msg_seleccionar_restaurante;

typedef struct{
	nombre* restaurante;
}msg_obtener_restaurante;

//lista sacada de Sindicato
t_list* registros_de_platos;  //lista de registroComida
////

typedef struct{//estructura para deserializar
	t_list* lista;
}struct_lista;

typedef struct{
	nombre* plato;
	uint32_t cantTotal;
	uint32_t cantLista;
}registroComida;

typedef struct{
	nombre* restaurante;
}msg_consultar_platos;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
}msg_guardar_pedido;

typedef struct{
	nombre*   plato;
	uint32_t id_pedido;
}msg_aniadir_plato;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
	nombre*   plato;
	uint32_t cantidad_platos;
}msg_guardar_plato;

typedef struct{
	uint32_t id_pedido;
	nombre* nombre_restaurante;
}msg_confirmar_pedido;

typedef struct{
	uint32_t id_pedido;
	nombre* nombre_restaurante;
}msg_confirmar_pedido_con_restaurante;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
	nombre*   plato;
}msg_plato_listo;

typedef struct{
	uint32_t id_pedido;
}msg_consultar_pedido;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
}msg_obtener_pedido;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
}msg_finalizar_pedido;

typedef struct{
	nombre*   restaurante;
	uint32_t id_pedido;
}msg_terminar_pedido;

typedef struct{
	nombre* plato;
}msg_obtener_receta;

#endif




