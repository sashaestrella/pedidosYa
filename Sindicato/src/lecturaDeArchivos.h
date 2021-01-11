

#ifndef LECTURADEARCHIVOS_H_
#define LECTURADEARCHIVOS_H_
#include <src/pycore.h>
#include <src/pyconfig.h>
#include <src/pylog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "inicializarAfip.h"


typedef struct{
	uint32_t tamanio;
	uint32_t  bloqueInicial;
}archivo_Info;

int calcularTamanioArchivo(char*ruta);
int cantidadBloques(size_t tamanioBloque,int tamanio);
archivo_Info infoArchivo(char*rutaInfo,char*sentencias,int tamanioArchivo);
void leerBloques(int bloques[],int cantidadBloques,int tamanio);
void leerSentencias(int bloques[],int cantBloques,char*sentencias,int tamanioSentencias,int tamanioBloques);
int leerLista(char*sentencias,int i,t_list* lista);
void asignarRestaurante(Restaurante* restaurante,char* sentencias,int tamanioSentencia);
void leerRestaurante(archivo_Info archivo,Restaurante* restaurante,char*sentencias);
Restaurante* buscarRestaurante(char*rutaInfoRestaurante,char*nombreRestaurante);
t_list* consultarPlatos(void*msg);
void asignarReceta (Receta*receta,char*sentencias,int tamanioSentencia);
void leerReceta(archivo_Info archivo,Receta*receta,char*sentencias);
t_list* buscarReceta(char*rutaReceta,char*nombrePlato);
t_list* consultarRecetas(void*msg);
void asignarPedido(Pedido*pedido,char*sentencias,int tamanioSentencia);
void leerPedido(archivo_Info archivo,Pedido*pedido,char*sentencias);
Pedido* obtenerPedido(void*msg);
Restaurante* obtenerRestaurante (void*msg);
#endif /* LECTURADEARCHIVOS_H_ */
