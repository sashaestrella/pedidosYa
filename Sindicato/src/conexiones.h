/*
 * conexiones.h
 *
 *  Created on: 21 oct. 2020
 *      Author: utnso
 */

#ifndef CONEXIONES_H_
#define CONEXIONES_H_
#include <src/pycore.h>
#include <src/pyconfig.h>
#include <src/pylog.h>
#include <src/pysockets.h>
#include <src/pyserialize.h>
#include <dirent.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "inicializarAfip.h"
#include "lecturaDeArchivos.h"
#include "crearArchivos.h"
t_list*listas;
void crearArchivo(char**array);
void consola();
int revisar(char**array);
int cantidadElementos(char**array);
void conexiones();
void escuchar(sfd socket_restaurante);
void atender(sfd socket_restaurante, buffer_t* buffer, enum_mensaje codigo_operacion);
void liberarListasRestauranteSin(t_list* platos,t_list*afinidades);
void liberar_lista(char** lista);
void liberarPlatosSin(t_list*platos);
void liberarPasosSin(t_list*pasos);
#endif /* CONEXIONES_H_ */
