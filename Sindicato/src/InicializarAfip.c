#include "inicializarAfip.h"

const char* MAGIC_NUMBER = "AFIP";
char* bitarrayGlobal;
char* rutaDirectorio(char*rutaInicial,char* nombreDirectorio){
	char* rutaDirectorio = malloc(strlen(rutaInicial)+ strlen(nombreDirectorio) + 2);
    strcpy(rutaDirectorio, rutaInicial);
    strcat(rutaDirectorio, "/");
    strcat(rutaDirectorio, nombreDirectorio);
    return rutaDirectorio;
}
DIR* abrirDirectorio(char*rutaInicial,char* rutaDirectorio){
	char* ruta = malloc(strlen(rutaInicial) + strlen(rutaDirectorio) + 2);
	strcpy(ruta, rutaInicial);
	strcat(ruta, "/");
	strcat(ruta, rutaDirectorio);
	DIR* directorio = opendir(ruta);
	free(ruta);
	return directorio;
}


void verificarDirectorio(DIR* directorio){
	if(directorio == NULL){
		perror("Error al abrir el directorio");
		exit(1);
	}
}
void borrarFiles(){
	char*rutaDirec=rutaDirectorio(afip,"Files");
	int fd2=open(rutaDirec,O_RDONLY);
	if(fd2>=0){
	char*rutaRestaurantes=rutaDirectorio(rutaDirec,"Restaurantes");
	int fd3=open(rutaRestaurantes,O_RDONLY);
	char*rutaRecetas=rutaDirectorio(rutaDirec,"Recetas");
	int fd4=open(rutaRecetas,O_RDONLY);
	if(fd3>=0){
	DIR*directorioRes=abrirDirectorio(rutaDirec,"Restaurantes");
	struct dirent*ent;
	while((ent=readdir(directorioRes))!=NULL){
		char*rutaResta=rutaDirectorio(rutaRestaurantes,(char*)ent->d_name);
		struct dirent*ent2;
		DIR*directorioResta=abrirDirectorio(rutaRestaurantes,(char*)ent->d_name);
		while((ent2=readdir(directorioResta))!=NULL){
			char*archivo=rutaDirectorio(rutaResta,(char*)ent2->d_name);
			remove(archivo);
			free(archivo);
		}
     rmdir(rutaResta);
     free(rutaResta);
     closedir(directorioResta);
	}
	closedir(directorioRes);
	}
	if(fd4>=0){
		DIR*directorioRecetas=abrirDirectorio(rutaDirec,"Recetas");
		struct dirent*ent;
		while((ent=readdir(directorioRecetas))!=NULL){
			char*archivo=rutaDirectorio(rutaRecetas,(char*)ent->d_name);
			remove(archivo);
			free(archivo);
		}

		closedir(directorioRecetas);
	}
	rmdir(rutaRecetas);
	rmdir(rutaRestaurantes);
	free(rutaRestaurantes);
	free(rutaRecetas);
	}
	rmdir(rutaDirec);
	free(rutaDirec);
}
void borrarMetadata(){
	char*rutaDirec=rutaDirectorio(afip,"Metadata");
		int fd2=open(rutaDirec,O_RDONLY);
		if(fd2>=0){
			struct dirent*ent2;
			DIR*directorioMetadata=abrirDirectorio(afip,"Metadata");
			while((ent2=readdir(directorioMetadata))!=NULL){
				char*archivo=rutaDirectorio(rutaDirec,(char*)ent2->d_name);
				remove(archivo);
				free(archivo);
			}
	     closedir(directorioMetadata);
		}
    rmdir(rutaDirec);
	free(rutaDirec);
}
void borrarBloques(){
	char*rutaDirec=rutaDirectorio(afip,"Blocks");
		int fd2=open(rutaDirec,O_RDONLY);
		if(fd2>=0){
			struct dirent*ent2;
			DIR*directorioMetadata=abrirDirectorio(afip,"Blocks");
			while((ent2=readdir(directorioMetadata))!=NULL){
				char*archivo=rutaDirectorio(rutaDirec,(char*)ent2->d_name);
				remove(archivo);
				free(archivo);
			}
	     closedir(directorioMetadata);
		}
	rmdir(rutaDirec);
	free(rutaDirec);
}
void borrarAFIP(){
	int fd=open(afip,O_RDONLY);
	if(fd>=0){
	borrarFiles();
	borrarMetadata();
	borrarBloques();
	rmdir(afip);
	}
}
void inicializarAFIP(){
	afip=py_config_get_string("PUNTO_MONTAJE");
	borrarAFIP();
	mkdir(afip,0777);
	crearDirectorios();
	crearMetadata();
	crearBitarray();
	crearBloques();
}
void crearDirectorios(){
	char* files = rutaDirectorio(afip,"Files");
	mkdir(files, 0777);
	char* metadata = rutaDirectorio(afip,"Metadata");
	mkdir(metadata, 0777);
	char* blocks = rutaDirectorio(afip,"Blocks");
	mkdir(blocks, 0777);
	DIR*directorioFiles=abrirDirectorio(afip,"Files");
	verificarDirectorio(directorioFiles);
	char*restaurantes=rutaDirectorio(files,"Restaurantes");
	mkdir(restaurantes,0777);
	char*recetas=rutaDirectorio(files,"Recetas");
	mkdir(recetas,0777);
	free(files);
	free(metadata);
	free(blocks);
	closedir(directorioFiles);
	free(restaurantes);
	free(recetas);
}
void crearMetadata(){
	DIR* directorioMetadata = abrirDirectorio(afip,"Metadata");
	verificarDirectorio(directorioMetadata);
	char* rutaDirectorioMetadata = rutaDirectorio(afip,"Metadata");
	char* rutaArchivoMetadata = rutaDirectorio(rutaDirectorioMetadata, "Metadata.AFIP");
	FILE* archivoMetadata = fopen(rutaArchivoMetadata, "w");

	t_metadata_archivo* estructura_metadata = malloc(sizeof(t_metadata_archivo));
	estructura_metadata->block_size = 64;
	estructura_metadata->blocks = 5192;
	estructura_metadata->magic_number = malloc(strlen(MAGIC_NUMBER)+1);
	strcpy(estructura_metadata->magic_number, MAGIC_NUMBER);

	fwrite(estructura_metadata, sizeof(t_metadata_archivo), 1, archivoMetadata);

	free(estructura_metadata->magic_number);
	free(estructura_metadata);
	fclose(archivoMetadata);
	free(rutaArchivoMetadata);
	free(rutaDirectorioMetadata);
	closedir(directorioMetadata);
}

void crearBitarray(){
	DIR* directorioMetadata = abrirDirectorio(afip,"Metadata");
	verificarDirectorio(directorioMetadata);
	char* rutaDirectorioMetadata = rutaDirectorio(afip,"Metadata");
	char* rutaBitmap = rutaDirectorio(rutaDirectorioMetadata, "Bitmap.bin");
	char* rutaMetadata=rutaDirectorio(rutaDirectorioMetadata,"Metadata.AFIP");
	FILE* archivoMetadata = fopen(rutaMetadata,"r");
	t_metadata_archivo* estructura_metadata = malloc(sizeof(t_metadata_archivo));
	FILE* bitmap = fopen(rutaBitmap, "w");
	t_bitarray* bitarray = malloc(sizeof(t_bitarray));

	fread(estructura_metadata, sizeof(t_metadata_archivo), 1, archivoMetadata);
	bitarray->size = estructura_metadata->blocks;
	tamanioBitarray=bitarray->size;
	bitarrayGlobal=malloc(bitarray->size);
	bitarray->bitarray =bitarrayGlobal;
	for(int i = 0; i < bitarray->size; i++){
		bitarray->bitarray[i] = '0';
	}

	bitarray->mode = LSB_FIRST;
	fwrite(bitarray, sizeof(t_bitarray), 1, bitmap);
	//free(bitarray->bitarray);
    free(bitarray);
	fclose(bitmap);
	free(estructura_metadata);
	fclose(archivoMetadata);
	closedir(directorioMetadata);
	free(rutaMetadata);
	free(rutaBitmap);
	free(rutaDirectorioMetadata);
}
void crearBloques(){
	DIR* directorioMetadata = abrirDirectorio(afip,"Metadata");
	verificarDirectorio(directorioMetadata);
	char* rutaDirectorioMetadata = rutaDirectorio(afip,"Metadata");
	char* rutaArchivoMetadata=rutaDirectorio(rutaDirectorioMetadata,"Metadata.AFIP");
	FILE* archivoMetadata = fopen(rutaArchivoMetadata,"r");
	t_metadata_archivo* estructura_metadata = malloc(sizeof(t_metadata_archivo));

	fread(estructura_metadata, sizeof(t_metadata_archivo), 1, archivoMetadata);
	crearCadaBloque(estructura_metadata->blocks);

	free(estructura_metadata);
	fclose(archivoMetadata);
	free(rutaArchivoMetadata);
	closedir(directorioMetadata);
	free(rutaDirectorioMetadata);
}

void crearCadaBloque(int cantidadBloques){
	DIR* directorioBlocks = abrirDirectorio(afip,"Blocks");
	verificarDirectorio(directorioBlocks);
	char* rutaDirectorioBlocks = rutaDirectorio(afip,"Blocks");

	for(int i = 1; i <= cantidadBloques; i++){
		char numeroBloques[4];
		sprintf(numeroBloques, "%d", i);
		char* rutaArchivoBloque = rutaDirectorio(rutaDirectorioBlocks, numeroBloques);
		FILE* bloque = fopen(rutaArchivoBloque, "w");
		fclose(bloque);
		free(rutaArchivoBloque);
	}

	closedir(directorioBlocks);
	free(rutaDirectorioBlocks);
}
