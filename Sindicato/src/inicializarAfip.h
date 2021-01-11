

#ifndef INICIALIZARAFIP_H_
#define INICIALIZARAFIP_H_
#include <src/pycore.h>
#include <src/pyconfig.h>
#include <src/pylog.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>


char* afip;
int tamanioBitarray;

typedef enum{
	CrearRESTAURANTE = 0,
	CrearRECETA = 1
}tipos;

typedef struct{
	size_t block_size;
	int blocks;
	char* magic_number;
}t_metadata_archivo;

void borrarAFIP();
void borrarBloques();
void borrarMetadata();
void inicializarAFIP();
void crearDirectorios();
void crearMetadata();
void crearBitarray();
void crearBloques();
void crearCadaBloque(int cantidadBloques);
char* rutaDirectorio(char*rutaInicial,char* nombreDirectorio);
DIR* abrirDirectorio(char*rutaInicial,char* rutaDirectorio);
void verificarDirectorio(DIR* directorio);
#endif /* INICIALIZARAFIP_H_ */
