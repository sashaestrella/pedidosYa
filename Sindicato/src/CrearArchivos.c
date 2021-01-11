#include "crearArchivos.h"

static const char* ENUM_TO_STR_ESTADOS[] =
{
	"PENDIENTE",
	"CONFIRMADO",
	"TERMINADO" ,
	"SINPEDIDO"
};

const char* enum_to_str(Estado_pedido value)
{
	return ENUM_TO_STR_ESTADOS[(int)value];
}


int calcularTamanioRestauranteSin(char**array){
	int tamanio=0;
	tamanio+=strlen("CANTIDAD_COCINEROS=");
	tamanio+=strlen(array[2])+1;
	tamanio+=strlen("POSICION=");
	tamanio+=strlen(array[3])+1;
	tamanio+=strlen("AFINIDAD_COCINEROS=");
	tamanio+=strlen(array[4])+1;
	tamanio+=strlen("RECETAS=");
	tamanio+=strlen(array[5])+1;
	tamanio+=strlen("PRECIO_RECETAS=");
	tamanio+=strlen(array[6])+1;
	tamanio+=strlen("CANTIDAD_HORNOS=");
	tamanio+=strlen(array[7])+1;
	return tamanio;

}

void asignarBloques(int bloques[],int cant) {
    char* directorioMetadata=rutaDirectorio(afip,"Metadata");
    char* rutaBitmap=rutaDirectorio(directorioMetadata,"Bitmap.bin");
    FILE* bitmapR=fopen(rutaBitmap,"r");
    t_bitarray* bitarray=malloc(sizeof(t_bitarray));
    //bitarray->size=tamanioBitarray;
   // bitarray->bitarray=malloc(tamanioBitarray);
    fread(bitarray,sizeof(t_bitarray),1,bitmapR);
    fclose(bitmapR);
    int j=0;
    int i=0;
    FILE* bitmapW=fopen(rutaBitmap,"w");
    while(j<bitarray->size && i<cant){
    	if(bitarray->bitarray[j]=='0'){
    		bloques[i]=j+1;
    		bitarray->bitarray[j]='1';
    		i++;
    	}
    	j++;
    }
    fwrite(bitarray,sizeof(t_bitarray),1,bitmapW);
    fclose(bitmapW);
    //free(bitarray->bitarray);
    free(bitarray);
    free(rutaBitmap);
    free(directorioMetadata);


}
void crearUnicaSentenciaRestaurante(char*sentencias,char** array){
	strcpy(sentencias,"CANTIDAD_COCINEROS=");
	strcat(sentencias,array[2]);
	strcat(sentencias,"\n");
	strcat(sentencias,"POSICION=");
	strcat(sentencias,array[3]);
	strcat(sentencias,"\n");
	strcat(sentencias,"AFINIDAD_COCINEROS=");
	strcat(sentencias,array[4]);
	strcat(sentencias,"\n");
	strcat(sentencias,"RECETAS=");
	strcat(sentencias,array[5]);
	strcat(sentencias,"\n");
	strcat(sentencias,"PRECIO_RECETAS=");
	strcat(sentencias,array[6]);
	strcat(sentencias,"\n");
	strcat(sentencias,"CANTIDAD_HORNOS=");
	strcat(sentencias,array[7]);


}
void escribirEnBloques(char*sentencias,int bloques[],int cantBloques,int tamanio){
	char* directorioMetadata=rutaDirectorio(afip, "Metadata");
	char* rutaMetadata=rutaDirectorio(directorioMetadata, "Metadata.AFIP");
	FILE* metadata=fopen(rutaMetadata,"r");
	t_metadata_archivo* estructuraMetadata=malloc(sizeof(t_metadata_archivo));
	fread(estructuraMetadata,sizeof(t_metadata_archivo),1,metadata);
	fclose(metadata);
	int f=0;
	int n=0;
	char* rutaBloques=rutaDirectorio(afip, "Blocks");
	    for(int l=0;l<cantBloques;l++){
	    	char*bloqueNuevo=string_itoa(bloques[l]);
	    	char*rutaBloque=rutaDirectorio(rutaBloques,bloqueNuevo);
	    	FILE*bloque=fopen(rutaBloque,"w");
	    	for (int j=0;j<estructuraMetadata->block_size-5;j++){
	    		if (f<tamanio-1){
	    			fputc(sentencias[f], bloque);
	    			f++;
	    		}
	    		else  {
	    			j=estructuraMetadata->block_size-5;
	    			l=cantBloques;
	    		}
	    	}
	    	if(f<tamanio-1){
	    		fputc('\n',bloque);
	    	char* bloquee=string_itoa(bloques[l+1]);
	    	while(n<strlen(bloquee)){
	    	fputc(bloquee[n],bloque);
	    	n++;
	    	}
	    	free(bloquee);
	    	}
	    	free(rutaBloque);
	    	free(bloqueNuevo);
	    	fclose(bloque);
	    	n=0;
	    }
	    free(rutaBloques);
	    free(estructuraMetadata);
	    free(rutaMetadata);
	    free(directorioMetadata);
}
int calcularTamanioSentencia(int tamanio,int bloque){
	int tam=0;
	char*stringTam=string_itoa(tamanio);
	char*stringBlo=string_itoa(bloque);
	tam+=strlen("SIZE=");
	tam+=strlen(stringTam)+1;
	tam+=strlen("INITIAL_BLOCK=");
	tam+=strlen(stringBlo)+1;
	free(stringTam);
	free(stringBlo);
	return tam;
}
void crearSentenciaInfo(char*sentencia,int tamanio,int bloque){
	char*stringTam=string_itoa(tamanio);
	char*stringBlo=string_itoa(bloque);
	strcpy(sentencia,"SIZE=");
	strcat(sentencia,stringTam);
	strcat(sentencia,"\n");
	strcat(sentencia,"INITIAL_BLOCK=");
	strcat(sentencia,stringBlo);
	free(stringTam);
	free(stringBlo);
}
void crearInfo(int tamanio,int bloque,char*ruta){//la ruta es del archivo a escribirse
	int tamanioSent=calcularTamanioSentencia(tamanio,bloque);
	char*sentencia=malloc(tamanioSent);
	crearSentenciaInfo(sentencia,tamanio,bloque);
	FILE* info=fopen(ruta,"w");
	for(int i=0;i<tamanioSent-1;i++){
		fputc(sentencia[i],info);
	}
	fclose(info);
	free(sentencia);

}
int calcularTamanioRecetaSin(char**array){
	int tamanio=0;
	tamanio+=strlen("PASOS=");
	tamanio+=strlen(array[2])+1;
	tamanio+=strlen("TIEMPO_PASOS=");
	tamanio+=strlen(array[3])+1;
	return tamanio;
}
void crearUnicaSentenciaReceta(char*sentencias,char**array){
	strcpy(sentencias,"PASOS=");
	strcat(sentencias,array[2]);
	strcat(sentencias,"\n");
	strcat(sentencias,"TIEMPO_PASOS=");
	strcat(sentencias,array[3]);

}
void crearYasginarBloques(char**array,char*info,int tipo){
	char*directorioMetadata=rutaDirectorio(afip, "Metadata");
	char*rutaMetadata=rutaDirectorio(directorioMetadata, "Metadata.AFIP");
	FILE* metadata=fopen(rutaMetadata,"r");
	t_metadata_archivo* estructuraMetadata=malloc(sizeof(t_metadata_archivo));
	fread(estructuraMetadata,sizeof(t_metadata_archivo),1,metadata);
	fclose(metadata);
	free(rutaMetadata);
	free(directorioMetadata);
	if(tipo==CrearRESTAURANTE){
	int tamanio=calcularTamanioRestauranteSin(array);
    int cantBloques=cantidadBloques(estructuraMetadata->block_size,tamanio);
    int bloques[cantBloques];
    asignarBloques(bloques,cantBloques);
    char* sentencias=malloc(tamanio);
    crearInfo(tamanio,bloques[0],info);
    crearUnicaSentenciaRestaurante(sentencias,array);
    escribirEnBloques(sentencias,bloques,cantBloques,tamanio);
    free(sentencias);
	}
	else if(tipo==CrearRECETA){
		int tamanio=calcularTamanioRecetaSin(array);
		int cantBloques=cantidadBloques(estructuraMetadata->block_size,tamanio);
		int bloques[cantBloques];
		asignarBloques(bloques,cantBloques);
		char* sentencias=malloc(tamanio);
	    crearInfo(tamanio,bloques[0],info);
	    crearUnicaSentenciaReceta(sentencias,array);
	    escribirEnBloques(sentencias,bloques,cantBloques,tamanio);
	    free(sentencias);

	}
	free(estructuraMetadata);
}

void crearRestaurante(char**array){
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,array[1]);
    char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,array[1]);
    if(restaurante!=NULL){
    	PY_LOG_INFO("ya existe ese restaurante \n");
    }
    else{
    mkdir(rutaRestaurante,0777);
    char*infoRestaurante=rutaDirectorio(rutaRestaurante,"Info.AFIP");
    crearYasginarBloques(array,infoRestaurante,CrearRESTAURANTE);
    free(infoRestaurante);
    }

    free(rutaRestaurante);
    closedir(restaurante);
    free(rutaRestaurantes);
    closedir(restaurantes);
    free(rutaFiles);
    closedir(files);

}
void crearReceta(char**array){
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* recetas=abrirDirectorio(rutaFiles,"Recetas");
	verificarDirectorio(recetas);
	char* rutaRecetas=rutaDirectorio(rutaFiles, "Recetas");
	char*archivoReceta=malloc(strlen(array[1])+strlen(".AFIP")+1);
	strcpy(archivoReceta,array[1]);
	strcat(archivoReceta,".AFIP");
	DIR* receta=abrirDirectorio(rutaRecetas,archivoReceta);
    char* rutaReceta=rutaDirectorio(rutaRecetas,archivoReceta);
    int fd=open(rutaReceta, O_RDONLY);
     if (fd<0) {
    	 crearYasginarBloques(array,rutaReceta,CrearRECETA);

     }
     else{
    	printf("ya existe esa receta \n");
    }

    free(rutaReceta);
    closedir(receta);
    free(archivoReceta);
    free(rutaRecetas);
    closedir(recetas);
    free(rutaFiles);
    closedir(files);

}
int tamanioPedidoVacio(){
	int tamanio=0;
	tamanio+=strlen("ESTADO_PEDIDO=");
	tamanio+=strlen("Pendiente")+1;
	tamanio+=strlen("LISTA_PLATOS=");
	tamanio+=strlen("[]")+1;
	tamanio+=strlen("CANTIDAD_PLATOS=");
	tamanio+=strlen("[]")+1;
	tamanio+=strlen("CANTIDAD_LISTA=");
	tamanio+=strlen("[]")+1;
	tamanio+=strlen("PRECIO_TOTAL=");
	tamanio+=strlen("0");
	return tamanio;
}
void crearSentenciaPedidoVacio(char*sentencia){
    strcpy(sentencia,"ESTADO_PEDIDO=");
    strcat(sentencia,"PENDIENTE");
    strcat(sentencia,"\n");
    strcat(sentencia,"LISTA_PLATOS=");
    strcat(sentencia,"[]");
    strcat(sentencia,"\n");
    strcat(sentencia,"CANTIDAD_PLATOS=");
    strcat(sentencia,"[]");
    strcat(sentencia,"\n");
    strcat(sentencia,"CANTIDAD_LISTA=");
    strcat(sentencia,"[]");
    strcat(sentencia,"\n");
    strcat(sentencia,"PRECIO_TOTAL=");
    strcat(sentencia,"0");
}
void crearYAsignarPedidos(char*rutaPedido){
	char*directorioMetadata=rutaDirectorio(afip, "Metadata");
	char*rutaMetadata=rutaDirectorio(directorioMetadata, "Metadata.AFIP");
	FILE* metadata=fopen(rutaMetadata,"r");
	t_metadata_archivo* estructuraMetadata=malloc(sizeof(t_metadata_archivo));
	fread(estructuraMetadata,sizeof(t_metadata_archivo),1,metadata);
	fclose(metadata);
	free(rutaMetadata);
	free(directorioMetadata);
	int tamanioSent=tamanioPedidoVacio();
	char*sentencias=malloc(tamanioSent+1);
	crearSentenciaPedidoVacio(sentencias);
	int cantBloques=cantidadBloques(estructuraMetadata->block_size,tamanioSent+1);
	int bloques[cantBloques];
	asignarBloques(bloques,cantBloques);
	crearInfo(tamanioSent,bloques[0],rutaPedido);
	escribirEnBloques(sentencias,bloques,cantBloques,tamanioSent+1);
	free(sentencias);
	free(estructuraMetadata);
}

int32_t crearPedido(void*msg){
	msg_guardar_pedido* mensaje=(msg_guardar_pedido*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	if (restaurante==NULL){
		printf("no existe el restaurante \n");
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		return 1;
	}
	else {
		char* stringPedido=string_itoa(mensaje->id_pedido);
		char*nombrePedido=malloc(strlen("Pedido")+strlen(stringPedido)+1);
		strcpy(nombrePedido,"Pedido");
		strcat(nombrePedido,stringPedido);
		strcat(nombrePedido,"\0");
		char*rutaPedido=rutaDirectorio(rutaRestaurante, nombrePedido);
		int fd=open(rutaPedido,O_RDONLY);
     if (fd<0) {
    	crearYAsignarPedidos(rutaPedido);
    	free(stringPedido);
    	free(nombrePedido);
    	free(rutaPedido);
 		free(rutaRestaurante);
 		closedir(restaurante);
 		free(rutaRestaurantes);
 		closedir(restaurantes);
 		free(rutaFiles);
 		closedir(files);
 		free(mensaje->restaurante->nombre);
 		free(mensaje->restaurante);
 		free(mensaje);
         return 0;
     }
     else{
    	PY_LOG_INFO("ya existe ese Pedido \n");
    	free(stringPedido);
    	free(nombrePedido);
        free(rutaPedido);
    	free(rutaRestaurante);
    	closedir(restaurante);
    	free(rutaRestaurantes);
    	closedir(restaurantes);
    	free(rutaFiles);
    	closedir(files);
    	free(mensaje->restaurante->nombre);
    	free(mensaje->restaurante);
    	free(mensaje);
    	return 1;
    }

}
}

int tamanioPedido(Pedido*pedido){
	int tamanio=0;
	tamanio+=strlen("ESTADO_PEDIDO=");
	tamanio+=strlen(enum_to_str(pedido->estadoPedido))+1;
	tamanio+=strlen("LISTA_PLATOS=");
	tamanio+=strlen("[");
    for (int z=0;z<pedido->platos->elements_count-1;z++){
    registroComida*registro;
    registro=list_get(pedido->platos,z);
    tamanio+=strlen(registro->plato->nombre);
    tamanio+=strlen(",");
    }
    if(pedido->platos->elements_count>0){
    registroComida*ultimoRegistro;
    ultimoRegistro=list_get(pedido->platos,pedido->platos->elements_count-1);
    tamanio+=strlen(ultimoRegistro->plato->nombre);
    tamanio+=strlen("]")+1;
	tamanio+=strlen("CANTIDAD_PLATOS=");
	tamanio+=strlen("[");
    for (int z=0;z<pedido->platos->elements_count-1;z++){
    registroComida*registro;
    registro=list_get(pedido->platos,z);
    char*cantTOTA=string_itoa(registro->cantTotal);
    tamanio+=strlen(cantTOTA);
    tamanio+=strlen(",");
    free(cantTOTA);
    }
    char*cantTOT=string_itoa(ultimoRegistro->cantTotal);
    tamanio+=strlen(cantTOT);
    tamanio+=strlen("]")+1;
	tamanio+=strlen("CANTIDAD_LISTA=");
	tamanio+=strlen("[");
	free(cantTOT);
    for (int z=0;z<pedido->platos->elements_count-1;z++){
    registroComida*registro;
    registro=list_get(pedido->platos,z);
    char*cantLIST=string_itoa(registro->cantLista);
    tamanio+=strlen(cantLIST);
    tamanio+=strlen(",");
    free(cantLIST);
    }
    char*cantLISTA=string_itoa(ultimoRegistro->cantLista);
    tamanio+=strlen(cantLISTA);
    tamanio+=strlen("]")+1;
    free(cantLISTA);
    }
    tamanio+=strlen("PRECIO_TOTAL=");
	tamanio+=strlen("0");
	return tamanio;
}
void crearSentenciaPedido(char* sentencia,Pedido* pedido){
    strcpy(sentencia,"ESTADO_PEDIDO=");
    strcat(sentencia,enum_to_str(pedido->estadoPedido));
    strcat(sentencia,"\n");
    strcat(sentencia,"LISTA_PLATOS=");
    strcat(sentencia,"[");
    for (int z=0;z<pedido->platos->elements_count-1;z++){
    registroComida*registro;
    registro=list_get(pedido->platos,z);
    strcat(sentencia,registro->plato->nombre);
    strcat(sentencia,",");
    }
    registroComida*ultimoRegistro;
    ultimoRegistro=list_get(pedido->platos,pedido->platos->elements_count-1);
    strcat(sentencia,ultimoRegistro->plato->nombre);
    strcat(sentencia,"]");
    strcat(sentencia,"\n");
    strcat(sentencia,"CANTIDAD_PLATOS=");
    strcat(sentencia,"[");
    for(int z=0;z<pedido->platos->elements_count-1;z++){
    	registroComida*registroCant;
    	registroCant=list_get(pedido->platos,z);
    	char*cantTOTA=string_itoa(registroCant->cantTotal);
    	strcat(sentencia,cantTOTA);
    	strcat(sentencia,",");
    	free(cantTOTA);
    }
    char*cantTOT=string_itoa(ultimoRegistro->cantTotal);
    strcat(sentencia,cantTOT);
    strcat(sentencia,"]");
    strcat(sentencia,"\n");
    strcat(sentencia,"CANTIDAD_LISTA=");
    strcat(sentencia,"[");
    free(cantTOT);
    for(int z=0;z<pedido->platos->elements_count-1;z++){
    	registroComida*registroLista;
    	registroLista=list_get(pedido->platos,z);
    	char*registroLIS=string_itoa(registroLista->cantLista);
    	strcat(sentencia,registroLIS);
    	strcat(sentencia,",");
    	free(registroLIS);
    }
    char* ultimoREG=string_itoa(ultimoRegistro->cantLista);
    strcat(sentencia,ultimoREG);
    strcat(sentencia,"]");
    strcat(sentencia,"\n");
    strcat(sentencia,"PRECIO_TOTAL=");
    char* precioTOT=string_itoa(pedido->precioTotal);
    strcat(sentencia,precioTOT);
    strcat(sentencia,"\0");
    free(precioTOT);
    free(ultimoREG);

}
void modificarBitarray(int bloques[],int cantBloques) {
    char* directorioMetadata=rutaDirectorio(afip,"Metadata");
    char* rutaBitmap=rutaDirectorio(directorioMetadata,"Bitmap.bin");
    FILE* bitmapR=fopen(rutaBitmap,"r");
    t_bitarray* bitarray=malloc(sizeof(t_bitarray));
    //bitarray->size=tamanioBitarray;
    //bitarray->bitarray=malloc(tamanioBitarray);
    fread(bitarray,sizeof(t_bitarray),1,bitmapR);
    fclose(bitmapR);
    FILE* bitmapW=fopen(rutaBitmap,"w");
    for(int i=0;i<cantBloques;i++){
    	bitarray->bitarray[bloques[i]-1]='0';
    	PY_LOG_INFO("Se modifico el bloque %i ",bloques[i]);
    }
    fwrite(bitarray,sizeof(t_bitarray),1,bitmapW);
    fclose(bitmapW);
    //free(bitarray->bitarray);
    free(bitarray);
    free(rutaBitmap);
    free(directorioMetadata);

}
void borrarContenidoBloques(int bloques[],int cantBloques){
	char*directorioBlocks=rutaDirectorio(afip, "Blocks");
	for(int i=0;i<cantBloques;i++){
	char*bloqueNew=string_itoa(bloques[i]);
	char*bloque=rutaDirectorio(directorioBlocks,bloqueNew);
	FILE*archBloque=fopen(bloque,"w");
	fclose(archBloque);
	free(bloque);
	free(bloqueNew);
	}
	free(directorioBlocks);
}

void reescribirPedido(Pedido*pedido,archivo_Info archivo,char*rutaPedido,int bloquesIni[],int cantiBloques){
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	fclose(metadata);
	modificarBitarray(bloquesIni,cantiBloques);
	borrarContenidoBloques(bloquesIni,cantiBloques);
	int tamanioSent=tamanioPedido(pedido);
	char*sentencias=malloc(tamanioSent+1);
	crearSentenciaPedido(sentencias,pedido);
	int cantBloques=cantidadBloques(archivoMet->block_size,tamanioSent);
	int newBloques[cantBloques];
	asignarBloques(newBloques, cantBloques);
	crearInfo(tamanioSent, newBloques[0],rutaPedido);
    escribirEnBloques(sentencias,newBloques,cantBloques,tamanioSent+1);
    free(sentencias);
    free(archivoMet);
    free(archivoMetadata);
    free(direcMetadata);
}
int32_t modificarPedido(msg_guardar_plato* mensaje,char*rutaPedido){
	Pedido* pedido=malloc(sizeof(Pedido));
	int tamanioArchivo=calcularTamanioArchivo(rutaPedido);
	char*sentencias=malloc(tamanioArchivo+1);
	archivo_Info archivo=infoArchivo(rutaPedido, sentencias, tamanioArchivo);
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
	leerPedido(archivo,pedido,sentencias);
	free(sentencias);
	int f=0;
	int j=0;
	if(pedido->estadoPedido==PENDIENTE){
	for(int z=0;z<pedido->platos->elements_count;z++){
		registroComida*comida;
		comida=list_get(pedido->platos,z);
		if(strcmp(comida->plato->nombre,mensaje->plato->nombre)==0){
			f++;
			j=z;
		}
	}
	registroComida*comidaNew=malloc(sizeof(registroComida));
	if(f==1){
		comidaNew=list_get(pedido->platos,j);
		comidaNew->cantTotal+=mensaje->cantidad_platos;
		list_remove(pedido->platos,j);
		list_add(pedido->platos,comidaNew);
	}
	else{
		comidaNew->plato=mensaje->plato;
		comidaNew->cantTotal=mensaje->cantidad_platos;
		comidaNew->cantLista=0;
		list_add(pedido->platos,comidaNew);
	}
	reescribirPedido(pedido,archivo,rutaPedido,bloques,cantBloques);
	free(comidaNew);
	free(pedido);
	return 0;
    }
	else{
		return 1;
	}
}
int32_t guardarPlato(void*msg){
	msg_guardar_plato*mensaje=(msg_guardar_plato*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	if (restaurante==NULL){
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje);
		printf("no existe el restaurante \n");
		return 1;

	}
	else {
		char* stringPedido=string_itoa(mensaje->id_pedido);
		char*nombrePedido=malloc(strlen("Pedido")+strlen(stringPedido)+1);
		strcpy(nombrePedido,"Pedido\0");
		strcat(nombrePedido,stringPedido);
		strcat(nombrePedido,"\0");
		char*rutaPedido=rutaDirectorio(rutaRestaurante, nombrePedido);
		int fd=open(rutaPedido,O_RDONLY);
     if (fd<0) {
    	 PY_LOG_INFO("no existe ese Pedido \n");
    	 free(rutaPedido);
    	 free(nombrePedido);
    	 free(stringPedido);
 		free(rutaRestaurante);
 		closedir(restaurante);
 		free(rutaRestaurantes);
 		closedir(restaurantes);
 		free(rutaFiles);
 		closedir(files);
 		free(mensaje->restaurante->nombre);
 		free(mensaje->restaurante);
 		free(mensaje->plato->nombre);
 		free(mensaje->plato);
 		free(mensaje);
    	 return 1;
     }
     else{
    	int32_t valor=modificarPedido(mensaje,rutaPedido);
   	    free(rutaPedido);
   	    free(nombrePedido);
   	    free(stringPedido);
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje);
    	return valor;
    }
     free(rutaPedido);
     free(nombrePedido);
     free(stringPedido);
	}
	free(rutaRestaurante);
	closedir(restaurante);
	free(rutaRestaurantes);
	closedir(restaurantes);
	free(rutaFiles);
	closedir(files);
}
int32_t modificarEstadoPedido(msg_confirmar_pedido*mensaje,char*rutaPedido,Estado_pedido estadoActual,Estado_pedido estadoNew){
	Pedido* pedido=malloc(sizeof(Pedido));
	int tamanioArchivo=calcularTamanioArchivo(rutaPedido);
	char*sentencias=malloc(tamanioArchivo+1);
	archivo_Info archivo=infoArchivo(rutaPedido, sentencias, tamanioArchivo);
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
	leerPedido(archivo,pedido,sentencias);
	free(sentencias);
	if(pedido->estadoPedido==estadoActual){
	pedido->estadoPedido=estadoNew;
	reescribirPedido(pedido,archivo,rutaPedido,bloques,cantBloques);
	free(pedido);
	return 0;
    }
	else{
		return 1;
	}


}
int32_t confirmarPedido(void*msg) {
	msg_confirmar_pedido*mensaje=(msg_confirmar_pedido*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->nombre_restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->nombre_restaurante->nombre);
	if (restaurante==NULL){
		PY_LOG_INFO("no existe el restaurante \n");
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->nombre_restaurante->nombre);
		free(mensaje->nombre_restaurante);
		free(mensaje);

		return 1;

	}
	else {
		char* stringPedido=string_itoa(mensaje->id_pedido);
		char*nombrePedido=malloc(strlen("Pedido")+strlen(stringPedido)+1);
		strcpy(nombrePedido,"Pedido");
		strcat(nombrePedido,stringPedido);
		char*rutaPedido=rutaDirectorio(rutaRestaurante, nombrePedido);
		int fd=open(rutaPedido,O_RDONLY);
     if (fd<0) {
    	PY_LOG_INFO("no existe ese Pedido \n");
    	free(stringPedido);
    	free(nombrePedido);
    	free(rutaPedido);
 		free(rutaRestaurante);
 		closedir(restaurante);
 		free(rutaRestaurantes);
 		closedir(restaurantes);
 		free(rutaFiles);
 		closedir(files);
 		free(mensaje->nombre_restaurante->nombre);
 		free(mensaje->nombre_restaurante);
 		free(mensaje);
    	 return 1;
     }
     else{
    	int32_t valor=modificarEstadoPedido(mensaje,rutaPedido,PENDIENTE,CONFIRMADO);
    	free(stringPedido);
        free(nombrePedido);
    	free(rutaPedido);
    	free(rutaRestaurante);
    	closedir(restaurante);
    	free(rutaRestaurantes);
    	closedir(restaurantes);
    	free(rutaFiles);
    	closedir(files);
    	free(mensaje->nombre_restaurante->nombre);
    	free(mensaje->nombre_restaurante);
    	free(mensaje);

    	return valor;
    }
	}
	free(rutaRestaurante);
	closedir(restaurante);
	free(rutaRestaurantes);
	closedir(restaurantes);
	free(rutaFiles);
	closedir(files);
}

int32_t terminarPedido(void*msg) {
	msg_confirmar_pedido*mensaje=(msg_confirmar_pedido*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->nombre_restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->nombre_restaurante->nombre);
	if (restaurante==NULL){
		PY_LOG_INFO("no existe el restaurante \n");
		return 1;

	}
	else {
		char* stringPedido=string_itoa(mensaje->id_pedido);
		char*nombrePedido=malloc(strlen("Pedido")+strlen(stringPedido)+1);
		strcpy(nombrePedido,"Pedido");
		strcat(nombrePedido,stringPedido);
		char*rutaPedido=rutaDirectorio(rutaRestaurante, nombrePedido);
		int fd=open(rutaPedido,O_RDONLY);
     if (fd<0) {
    	 PY_LOG_INFO("no existe ese Pedido \n");
    	 return 1;
     }
     else{
    	int32_t valor=modificarEstadoPedido(mensaje,rutaPedido,CONFIRMADO,TERMINADO);
    	return valor;
    }
     free(rutaPedido);
     free(nombrePedido);
     free(stringPedido);
	}

}

int32_t modificarPlato (msg_plato_listo*mensaje,char*rutaPedido){
	Pedido* pedido=malloc(sizeof(Pedido));
	int tamanioArchivo=calcularTamanioArchivo(rutaPedido);
	char*sentencias=malloc(tamanioArchivo+1);
	archivo_Info archivo=infoArchivo(rutaPedido, sentencias, tamanioArchivo);
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
	leerPedido(archivo,pedido,sentencias);
	free(sentencias);
	if(pedido->estadoPedido==CONFIRMADO){
		int j=0;
		registroComida* platoAModificar;
		for(int i=0;i<pedido->platos->elements_count;i++){
			registroComida*unPlato;
			unPlato=list_get(pedido->platos,i);
            if(!strcmp(unPlato->plato->nombre,mensaje->plato->nombre)){
            	platoAModificar=unPlato;
            	list_remove(pedido->platos,i);
            	j=1;
            }
		}
		if(j==1){
			platoAModificar->cantLista+=1;
			list_add(pedido->platos,platoAModificar);
			reescribirPedido(pedido, archivo, rutaPedido,bloques,cantBloques);
			liberarPlatosSin(pedido->platos);
			free(pedido);
			return 0;
		}
		else {
			PY_LOG_INFO("no existe el plato. \n");
			liberarPlatosSin(pedido->platos);
			free(pedido);
			return 1;
		}
	}
	else{
		PY_LOG_INFO("el pedido no esta en estado Confirmado. \n");
		liberarPlatosSin(pedido->platos);
		free(pedido);
		return 1;
	}

}
int32_t platoListo(void*msg){
	msg_plato_listo*mensaje=(msg_plato_listo*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	if (restaurante==NULL){
		PY_LOG_INFO("no existe el restaurante \n");
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->plato->nombre);
		free(mensaje->plato);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		return 1;

	}
	else {
		char* stringPedido=string_itoa(mensaje->id_pedido);
		char*nombrePedido=malloc(strlen("Pedido")+strlen(stringPedido)+1);
		strcpy(nombrePedido,"Pedido");
		strcat(nombrePedido,stringPedido);
		char*rutaPedido=rutaDirectorio(rutaRestaurante, nombrePedido);
		int fd=open(rutaPedido,O_RDONLY);
     if (fd<0) {
    	 PY_LOG_INFO("no existe ese Pedido \n");
    	free(rutaPedido);
    	free(nombrePedido);
    	free(stringPedido);
 		free(rutaRestaurante);
 		closedir(restaurante);
 		free(rutaRestaurantes);
 		closedir(restaurantes);
 		free(rutaFiles);
 		closedir(files);
 		free(mensaje->plato->nombre);
 		free(mensaje->plato);
 		free(mensaje->restaurante->nombre);
 		free(mensaje->restaurante);
 		free(mensaje);
    	 return 1;
     }
     else{
    	int32_t valor=modificarPlato(mensaje,rutaPedido);
    	free(rutaPedido);
    	free(nombrePedido);
    	free(stringPedido);
    	free(rutaRestaurante);
    	closedir(restaurante);
    	free(rutaRestaurantes);
    	closedir(restaurantes);
    	free(rutaFiles);
    	closedir(files);
    	free(mensaje->plato->nombre);
    	free(mensaje->plato);
    	free(mensaje->restaurante->nombre);
    	free(mensaje->restaurante);
    	free(mensaje);

    	return valor;
    }
	}
}
