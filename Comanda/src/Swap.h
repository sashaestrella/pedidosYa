#ifndef SWAP_H
#define SWAP_H

#include "Comanda.h"

int tamanioSwap;
int archSwap;
char* direccionSwap;
char* contenidoSwap;

int inicializarSwap();
void* inicializarMapeo();
int buscarMarcoLibre();
int guardarPaginaEnSwapNueva(nombre* nombreRestaurante,uint32_t idPedido,nombre* nombrePlato,int cantidad);
int actualizarPaginaEnSwapVieja(nombre* nombreRestaurante,uint32_t idPedido,uint32_t cantTotal,uint32_t cantLista,char* nombrePlato);
char** leerPaginaGuardadaEnSwap(int frame);
int obtenerPosicionDePedidoSwap(PedidoSwap* pedido);
PedidoSwap* buscarPedidoSwap(uint32_t idPedido,nombre*nombreRestaurante);
ContenidoPaginaSwap* buscarPaginaEnPedidoSwap(PedidoSwap* pedido,char* nombrePlato);
PedidoSwap* verificarPedidoSwap(uint32_t idPedido,nombre*nombreRestaurante);
void eliminarPaginasDeSwap(nombre* nombreRestaurante,uint32_t idPedido);
int todosLosFramesEstanOcupados();

#endif
