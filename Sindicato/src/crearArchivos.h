

#ifndef CREARARCHIVOS_H_
#define CREARARCHIVOS_H_

#include <src/pycore.h>
#include <src/pyconfig.h>
#include <src/pylog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "inicializarAfip.h"
#include "lecturaDeArchivos.h"



int calcularTamanioRestauranteSin(char**array);
void asignarBloques(int bloques[],int cant);
void crearUnicaSentenciaRestaurante(char*sentencias,char** array);
void escribirEnBloques(char*sentencias,int bloques[],int cantBloques,int tamanio);
int calcularTamanioSentencia(int tamanio,int bloque);
void crearSentenciaInfo(char*sentencia,int tamanio,int bloque);
void crearInfo(int tamanio,int bloque,char*ruta);
int calcularTamanioRecetaSin(char**array);
void crearUnicaSentenciaReceta(char*sentencias,char**array);
void crearYasginarBloques(char**array,char*info,int tipo);
void crearRestaurante(char**array);
void crearReceta(char**array);
int tamanioPedidoVacio();
void crearSentenciaPedidoVacio(char*sentencia);
void crearYAsignarPedidos(char*rutaPedido);
int32_t crearPedido(void*msg);
int tamanioPedido(Pedido*pedido);
void crearSentenciaPedido(char* sentencia,Pedido* pedido);
void modificarBitarray(int bloques[],int cantBloques) ;
void borrarContenidoBloques(int bloques[],int cantBloques);
void reescribirPedido(Pedido*pedido,archivo_Info archivo,char*rutaPedido,int bloques[],int tamBloques);
int32_t modificarPedido(msg_guardar_plato* mensaje,char*rutaPedido);
int32_t guardarPlato(void*msg);
int32_t modificarEstadoPedido(msg_confirmar_pedido*mensaje,char*rutaPedido,Estado_pedido estadoActual,Estado_pedido estadoNew);
int32_t confirmarPedido(void*msg);
int32_t terminarPedido(void*msg) ;
int32_t modificarPlato (msg_plato_listo*mensaje,char*rutaPedido);
int32_t platoListo(void*msg);
#endif /* CREARARCHIVOS_H_ */
