#ifndef COMANDA_H
#define COMANDA_H

#include <src/pycore.h>
#include <src/pyhandshake.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <sys/time.h>

void* memoria;
int tamanioMemoria;
int tamanioPagina;
int cantDePagSegunTamMemoria;
char* algoritmoReemplazo;
int cantPaginasEnSwap;
void* mapeo;

t_list* tablasDeSegmentos;
t_list* pedidosComanda;

t_list* pedidosSwap; //lista de PedidoSwap
t_list* paginasEnSwap;

pthread_t hiloConexiones;

t_log* logger;

pthread_mutex_t cantTotalDePlatos;
pthread_mutex_t cantListaDePlatos;
pthread_mutex_t contPaginaCantTotal;
pthread_mutex_t contPaginaCantLista;
pthread_mutex_t estadoPedido;
pthread_mutex_t timestamp;
pthread_mutex_t actualizarBitModificado;
pthread_mutex_t mutexMemoria;

pthread_mutex_t agregarPedidoComanda;
pthread_mutex_t agregarPedidoSwap;
pthread_mutex_t mutexTablasDeSegmentos;
pthread_mutex_t mutexSegmentos;
pthread_mutex_t modifPagina;
pthread_mutex_t swapPagina;
pthread_mutex_t leerPagina;

typedef struct{
	Estado_pedido estadoPedido;
	uint32_t idPedido;
	nombre* nombreRestaurante;
}PedidoComanda;

typedef struct{
	uint32_t idPedido;
	t_list* platos; //lista de ContenidoPaginaSwap
	nombre* nombreRestaurante;
}PedidoSwap;

typedef struct{
	uint32_t cantTotal;
	uint32_t cantLista;
	char nombrePlato[24];
}ContenidoPagina; //ṕlato

typedef struct{
	uint32_t cantTotal;
	uint32_t cantLista;
	char nombrePlato[24];
	int nroPagina;
}ContenidoPaginaSwap; //ṕlato

typedef struct{
	int nroPagina;
	int M; //bit de modificado
	int U; //bit de uso
	int P; //bit de presencia
	uint32_t frame;
	int timestamp;
}Pagina; //Pagina en memoria

typedef struct{
	t_list* paginas; //lista de Pagina
}TablaDePaginas;

typedef struct{
	int idPedido; //Identificador de proceso
	TablaDePaginas tablaDePaginas;//la cant de pag va a ser la misma que la cant de platos del pedido
}Segmento; //pedido

typedef struct{
	nombre* nombreRestaurante;
	t_list* segmentos;
}TablaDeSegmentos; //una por restaurante

typedef struct{
	Pagina* pagina;
	Segmento* segmento;
	TablaDeSegmentos* tabla;
}estructuraClock;

typedef enum{
	LIBRE,
	OCUPADO
}EstadoFrame;

EstadoFrame* framesEnSwap;
EstadoFrame* estadosFrames;
ContenidoPagina* contenidoPagina;

void conexiones();
void escuchar(sfd socket_restaurante);
void atender(sfd socket_restaurante, py_paquete_t* paquete, enum_mensaje codigo_operacion);
py_enum enviar_respuesta(sfd sfd_servidor, buffer_t* buffer, enum_mensaje tipo_mensaje);
buffer_t* serializar_mensaje_de_error(nombre* mensajeDeError);

msg_guardar_pedido* deserializar_guardar_pedido(buffer_t* buffer);
msg_guardar_plato* deserializar_guardar_plato(buffer_t* buffer);
msg_obtener_pedido* deserializar_obtener_pedido(buffer_t* buffer);
msg_confirmar_pedido_con_restaurante* deserializar_confirmar_pedido_con_restaurante(buffer_t* buffer);
msg_plato_listo* deserializar_plato_listo(buffer_t* buffer);
msg_finalizar_pedido* deserializar_finalizar_pedido(buffer_t* buffer);
buffer_t* serializar_int(int32_t numero);

TablaDeSegmentos* buscarTablaDeSegmentos(nombre* nombreRestaurante);
TablaDeSegmentos* crearTablaDeSegmentos(nombre* nombreRestaurante);
Segmento* buscarSegmentoYDevolverlo(int idBuscado,TablaDeSegmentos* tabla);
int buscarPaginaLibre();
Pagina* buscarPagina(char* nombrePlato, Segmento* segmento_buscado);
PedidoComanda* obtenerPedidoDePedidosComanda(uint32_t idPedido,nombre*nombreRestaurante);
void cargarEnFramesTodosLosPlatos(TablaDePaginas tabla,t_list* platos);
void cargarEnFramesLosPlatosFaltantes(TablaDePaginas tabla,t_list* platos);
int buscarPosicionSegmentoEnTabla(uint32_t idPedido,TablaDeSegmentos* tabla);
void eliminarPaginasDeComanda(Segmento* segmento,TablaDeSegmentos* tabla);
void cargarEnFramesLosPlatosDelPedido(TablaDeSegmentos* tabla,Segmento* segmento);
ContenidoPagina* buscarPlatoEnPedidosComanda(int idPedido,char* nombrePlato);
int platosListosDelPedido(Segmento* segmento);
int buscarPosicionPedidoComanda(PedidoComanda* pedido);
void liberarMarcosDelSegmento(Segmento* segmento);

int administrarGuardarPedido(TablaDeSegmentos* tabla,uint32_t idPedido);
int administrarGuardarPlato(TablaDeSegmentos* tabla,uint32_t idPedido,nombre* nombrePlato,uint32_t cantPlatos);
int administrarConfirmarPedido(TablaDeSegmentos* tabla,uint32_t idPedido);
int administrarPlatoListo(TablaDeSegmentos* tabla,uint32_t idPedido,nombre* unNombre);
int administrarObtenerPedido(TablaDeSegmentos* tabla,uint32_t idPedido);
int administrarFinalizarPedido(TablaDeSegmentos* tabla,uint32_t idPedido);

#endif
