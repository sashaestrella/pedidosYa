#include "lecturaDeArchivos.h"

int calcularTamanioArchivo(char*ruta){
	FILE*info=fopen(ruta,"r");
	fseek(info,0,SEEK_END);
	int tamanioArchivo=ftell(info);
	fseek(info,0,SEEK_SET);
	fclose(info);
	return tamanioArchivo;
}
int cantidadBloques(size_t tamanioBloque,int tamanio){
	int tamanioFinal=tamanio/(tamanioBloque-5);
	if(tamanio%(tamanioBloque-5)!=0){
		tamanioFinal++;
	}
	return tamanioFinal;
}
archivo_Info infoArchivo(char*rutaInfo,char*sentencias,int tamanioArchivo){
	FILE* info=fopen(rutaInfo,"r");
	fseek(info,0,SEEK_SET);
	for(int i=0;i<tamanioArchivo;i++){
			sentencias[i]=fgetc(info);
		}
	fclose(info);
	sentencias[tamanioArchivo]='\0';
		int j=0;
		int f=0;
		int l=0;
		while(sentencias[j]!='=' && j<tamanioArchivo){
			j++;
			f=j;
		}
		j++;
		while(sentencias[j]!='\n' && j<tamanioArchivo){
			l++;
			j++;
		}
		char* tam=malloc(l+1);
		for(int z=0;z<l;z++){
			tam[z]=sentencias[f+1];
			f++;
		}
		tam[l]='\0';
		while(sentencias[j]!='=' && j<tamanioArchivo){
			j++;
			f=j;
		}
		l=0;
		j++;
		while(sentencias[j]!='\n' && j<tamanioArchivo){
			l++;
			j++;
		}
		char*init=malloc(l+1);
		for(int y=0;y<l;y++){
			init[y]=sentencias[f+1];
			f++;
		}
		init[l]='\0';
		archivo_Info archivo;
		archivo.tamanio=atoi(tam);
		archivo.bloqueInicial=atoi(init);
		free(tam);
		free(init);

		return archivo;
}
void leerBloques(int bloques[],int cantidadBloques,int tamanio){
	if (cantidadBloques==1){
	}
	else{
		int j=0;
		char*rutaBlocks=rutaDirectorio(afip,"Blocks");
		for (int i=1;i<cantidadBloques;i++){
			char*newBloque=string_itoa(bloques[i-1]);
			char*rutaBloque=rutaDirectorio(rutaBlocks,newBloque);
			FILE*bloque=fopen(rutaBloque,"r");
			fseek(bloque,0,SEEK_END);
			j=ftell(bloque);
			if(j>=60){
			fseek(bloque,0,SEEK_SET);
			fseek(bloque,tamanio-4,SEEK_CUR);

			char*stBloque=malloc(j-(tamanio-4)+1);
			for(int z=0;z<(j-(tamanio-4));z++){
				stBloque[z]=fgetc(bloque);
			}
			stBloque[j-(tamanio-4)]='\0';
			bloques[i]=atoi(stBloque);
            free(stBloque);
			}
            fclose(bloque);
            free(rutaBloque);
            free(newBloque);
		}
		free(rutaBlocks);
	}

}
void leerSentencias(int bloques[],int cantBloques,char*sentencias,int tamanioSentencias,int tamanioBloques){
	char*rutaBlocks=rutaDirectorio(afip,"Blocks");
	int f=0;
	for(int i=0;i<cantBloques;i++){
		    char*bloqueNew=string_itoa(bloques[i]);
			char*rutaBloque=rutaDirectorio(rutaBlocks,bloqueNew);
			FILE*bloque=fopen(rutaBloque,"r");
	    	for (int j=0;j<tamanioBloques-5;j++){
	    		if (f<tamanioSentencias-1){
	    			sentencias[f]=fgetc(bloque);
	    			f++;
	    		}
	    		else  {
	    			j=tamanioBloques-4;
	    			i=cantBloques;
	    		}
	    	}
	    	fclose(bloque);
	    	free(rutaBloque);
	    	free(bloqueNew);
	}
	sentencias[tamanioSentencias-1]='\0';
	free(rutaBlocks);
}
int leerLista(char*sentencias,int i,t_list* lista){
	i++;//[
	int l=0;
	int j=0;
	while(sentencias[i]!=']'){
		l=i;
		j=0;
		while (sentencias[i]!=',' && sentencias[i]!=']'){
			i++;
			j++;
		}
		if(sentencias[i]==','){
			i++;
		}
		char*elemento=malloc(j+1);
		for(int z=0;z<j;z++){
			elemento[z]=sentencias[l];
			l++;
		}
		elemento[j]='\0';
		list_add(lista,elemento);


	}
	i++;//te deja en el \n
	return i;
}
void asignarRestaurante(Restaurante* restaurante,char* sentencias,int tamanioSentencia){
	int i=0;
	int j=0;
	int l=0;
	while(sentencias[i]!='=' && i<tamanioSentencia-1){
		i++;
		j=i;
	}
	j++;
	i++;
	while(sentencias[i]!='\n' && i<tamanioSentencia-1){
		i++;
		l++;
	}
	char*cantCocineros=malloc(l+1);
	for(int z=0;z<l;z++){
		cantCocineros[z]=sentencias[j];
		j++;
	}
	cantCocineros[l]='\0';
	restaurante->info_Restaurante.cantidadDeCocineros=atoi(cantCocineros);
	free(cantCocineros);
	l=0;

	while(sentencias[i]!='=' && i<tamanioSentencia-1){
		i++;
	}
	i++;//=
	i++;//[
	j=i;
	while(sentencias[i]!=',' && i<tamanioSentencia-1){
		i++;
		l++;
	}
	char*posx=malloc(l+1);
	for(int z=0;z<l;z++){
		posx[z]=sentencias[j];
		j++;
	}
	posx[l]='\0';
	restaurante->info_Restaurante.Posicion.posicionX=atoi(posx);
	free(posx);
	i++;
	l=0;
	j=i;
	while(sentencias[i]!=']' && i<tamanioSentencia-1){
		i++;
		l++;
	}
	char*posy=malloc(l+1);
	for(int z=0;z<l;z++){
		posy[z]=sentencias[j];
		j++;
	}
	posy[l]='\0';
	restaurante->info_Restaurante.Posicion.posicionY=atoi(posy);
	free(posy);
	l=0;
    i++;//\n
	while(sentencias[i]!='=' && i<tamanioSentencia-1){
		i++;
	}
	i++;
    t_list*stringAfinidad;
    stringAfinidad=list_create();
    i=leerLista(sentencias,i,stringAfinidad);
    t_list*listaAfinidad;
    listaAfinidad=list_create();
    for(int f=0;f<stringAfinidad->elements_count;f++){
    	nombre*afinidad=malloc(sizeof(nombre));
    	afinidad->nombre=list_get(stringAfinidad,f);
    	afinidad->largo_nombre=strlen(afinidad->nombre);
    	list_add(listaAfinidad,afinidad);
    }
    list_destroy(stringAfinidad);
    restaurante->info_Restaurante.afinidades=listaAfinidad;
	while(sentencias[i]!='=' && i<tamanioSentencia){
		i++;
	}
	i++;
	t_list*recetas;
	recetas=list_create();
	i=leerLista(sentencias,i,recetas);
	while(sentencias[i]!='=' && i<tamanioSentencia-1){
		i++;
	}
	i++;
	t_list*precios;
	precios=list_create();
	i=leerLista(sentencias, i, precios);
	t_list*platos;
	platos=list_create();

	for(int z=0;z<recetas->elements_count;z++){
		Plato*plato=malloc(sizeof(Plato));
		plato->nombre=malloc(sizeof(nombre));
		plato->nombre->largo_nombre=strlen(list_get(recetas,z))+1;
		plato->nombre->nombre=list_get(recetas,z);
		plato->precio=atoi(list_get(precios,z));
		free(list_get(precios,z));
		list_add(platos,plato);
	}
	list_destroy(precios);
	list_destroy(recetas);
    restaurante->info_Restaurante.Platos=platos;
	while(sentencias[i]!='=' && i<tamanioSentencia-1){
		i++;
	}
	i++;
	j=i;
	l=0;
	while(sentencias[i]!='\n' && i<tamanioSentencia-1){
			i++;
			l++;
		}
		char*cantHornos=malloc(l+1);
		for(int z=0;z<l;z++){
			cantHornos[z]=sentencias[j];
			j++;
		}
		cantHornos[l]='\0';
		restaurante->info_Restaurante.cantidadDeHornos=atoi(cantHornos);
		free(cantHornos);
}
void leerRestaurante(archivo_Info archivo,Restaurante* restaurante,char*sentencias){
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	char*sentenciaNew=malloc(archivo.tamanio);
	leerSentencias(bloques,cantBloques,sentenciaNew,archivo.tamanio,archivoMet->block_size);
	asignarRestaurante(restaurante,sentenciaNew,archivo.tamanio);
	free(sentenciaNew);
	free(sentencias);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
}
Restaurante* buscarRestaurante(char*rutaInfoRestaurante,char*nombreRestaurante){
	Restaurante* restaurante=malloc(sizeof(Restaurante));
	restaurante->nombre_restaurante=malloc(sizeof(nombre));
	restaurante->nombre_restaurante->largo_nombre=strlen(nombreRestaurante)+1;
	restaurante->nombre_restaurante->nombre=malloc(restaurante->nombre_restaurante->largo_nombre);
	strcpy(restaurante->nombre_restaurante->nombre,nombreRestaurante);
    int tamanioArchivo=calcularTamanioArchivo(rutaInfoRestaurante);
	char*sentencias=malloc(tamanioArchivo+1);
	archivo_Info archivo=infoArchivo(rutaInfoRestaurante, sentencias, tamanioArchivo);
	leerRestaurante(archivo,restaurante,sentencias);
	return restaurante;

}
t_list* consultarPlatos(void*msg){
 msg_consultar_platos* mensaje=(msg_consultar_platos*)msg;
 char*rutaFiles=rutaDirectorio(afip,"Files");
 char*rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
 char*rutaRestaurante=rutaDirectorio(rutaRestaurantes, mensaje->restaurante->nombre);
 t_list*lista;
 lista=list_create();
 if(rutaRestaurante==NULL){
	 PY_LOG_INFO("no existe el restaurante. \n");
 }
 else{
	 char*rutaInfoRestaurante=rutaDirectorio(rutaRestaurante, "Info.AFIP");
	 Restaurante*restaurante=buscarRestaurante(rutaInfoRestaurante,mensaje->restaurante->nombre);
	 lista=restaurante->info_Restaurante.Platos;
 }
 return lista;
}
void asignarReceta (Receta*receta,char*sentencias,int tamanioSentencia){
	int i=0;
	while(sentencias[i]!='=' && i<tamanioSentencia-1){
			i++;
		}
		i++;
		t_list*nomPaso;
		nomPaso=list_create();
		i=leerLista(sentencias,i,nomPaso);
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
			i++;
		}
		i++;
		t_list*tiempo;
		tiempo=list_create();
		i=leerLista(sentencias, i, tiempo);
		t_list*pasos;
		pasos=list_create();

		for(int z=0;z<nomPaso->elements_count;z++){
			Paso*paso=malloc(sizeof(Paso));
			paso->nombre=malloc(sizeof(nombre));
			paso->nombre->largo_nombre=strlen(list_get(nomPaso,z))+1;
			paso->nombre->nombre=list_get(nomPaso,z);
			paso->tiempo=atoi(list_get(tiempo,z));
			free(list_get(tiempo,z));
			list_add(pasos,paso);
		}
		list_destroy(tiempo);
		list_destroy(nomPaso);
	    receta->Pasos=pasos;

}
void leerReceta(archivo_Info archivo,Receta*receta,char*sentencias){
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	char*sentenciaNew=malloc(archivo.tamanio+1);
	leerSentencias(bloques,cantBloques,sentenciaNew,archivo.tamanio,archivoMet->block_size);
	asignarReceta(receta,sentenciaNew,archivo.tamanio);
	free(sentenciaNew);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
}

t_list* buscarReceta(char*rutaReceta,char*nombrePlato){
	Receta* receta=malloc(sizeof(Receta));
	int tamanioArchivo=calcularTamanioArchivo(rutaReceta);
	PY_LOG_INFO("%i",tamanioArchivo);
	char*sentencias=malloc(tamanioArchivo+1);
	archivo_Info archivo=infoArchivo(rutaReceta, sentencias, tamanioArchivo);
	leerReceta(archivo,receta,sentencias);
	t_list*pasos=receta->Pasos;
	free(receta);
	return pasos;
}
t_list* consultarRecetas(void*msg){
	msg_obtener_receta* mensaje=(msg_obtener_receta*)msg;
	printf("La receta buscada es: %s \n", mensaje->plato->nombre);
	 char*rutaFiles=rutaDirectorio(afip,"Files");
	 char*rutaRecetas=rutaDirectorio(rutaFiles, "Recetas");
	 char*rutaReceta=malloc(mensaje->plato->largo_nombre + strlen(".AFIP")+1);
	 strcpy(rutaReceta,mensaje->plato->nombre);
	 strcat(rutaReceta,".AFIP");
	 char*rutreceta=rutaDirectorio(rutaRecetas, rutaReceta);
	 t_list*pasos;
	 int fd=open(rutreceta, O_RDONLY);
	 if (fd<0) {
		  PY_LOG_INFO("no existe esa receta \n");
	      }
	      else{
	      pasos=buscarReceta(rutreceta,mensaje->plato->nombre);
	     }
	 free(rutreceta);
	 free(rutaReceta);
	 free(rutaRecetas);
	 free(rutaFiles);
	 free(mensaje->plato->nombre);
	 free(mensaje->plato);
	 free(mensaje);
	 return pasos;
}
void asignarPedido(Pedido*pedido,char*sentencias,int tamanioSentencia){
	int i=0;
	int j=0;
	int l=0;
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
				i++;
			}
			i++;
			j=i;
		while(sentencias[i]!='\n' && i<tamanioSentencia-1){
			i++;
			l++;
		}
		char*estado=malloc(l+1);
		for(int z=0;z<l;z++){
			estado[z]=sentencias[j];
			j++;
		}
		estado[l]='\0';
		if(strcmp(estado,"CONFIRMADO")==0){
			pedido->estadoPedido=CONFIRMADO;
		}
		else if(strcmp(estado,"PENDIENTE")==0){
			pedido->estadoPedido=PENDIENTE;
		}
		else{
			pedido->estadoPedido=TERMINADO;
		}
		free(estado);
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
				i++;
			}
			i++;
		t_list*platos;
		platos=list_create();
		i=leerLista(sentencias, i, platos);
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
				i++;
			}
		i++;
		t_list*cantidadPlatos;
		cantidadPlatos=list_create();
		i=leerLista(sentencias, i, cantidadPlatos);
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
				i++;
		}
		i++;
		t_list*cantidadLista;
		cantidadLista=list_create();
		i=leerLista(sentencias, i, cantidadLista);
		t_list*registros;
		registros=list_create();
		for(int z=0;z<platos->elements_count;z++){
			registroComida* registro=malloc(sizeof(registroComida));
			registro->plato=malloc(sizeof(nombre));
			registro->plato->largo_nombre=strlen(list_get(platos,z))+1;
			registro->plato->nombre=list_get(platos,z);
			registro->cantTotal=atoi(list_get(cantidadPlatos,z));
			registro->cantLista=atoi(list_get(cantidadLista,z));
			free(list_get(cantidadPlatos,z));
			free(list_get(cantidadLista,z));
			list_add(registros,registro);
		}
		list_destroy(platos);
		list_destroy(cantidadPlatos);
		list_destroy(cantidadLista);
		pedido->platos=registros;
		while(sentencias[i]!='=' && i<tamanioSentencia-1){
			i++;
		}
		i++;
		j=i;
		l=0;
		while(sentencias[i]!='\n' && i<tamanioSentencia-1){
			i++;
			l++;
		}
		char*precioTotal=malloc(l+1);
		for (int z=0;z<l;z++){
			precioTotal[z]=sentencias[j];
			j++;
		}
		precioTotal[l]='\0';
		pedido->precioTotal=atoi(precioTotal);
        free(precioTotal);
}
void leerPedido(archivo_Info archivo,Pedido*pedido,char*sentencias){
	char* direcMetadata=rutaDirectorio(afip, "Metadata");
	char* archivoMetadata=rutaDirectorio(direcMetadata, "Metadata.AFIP");
	FILE*metadata=fopen(archivoMetadata,"r");
	t_metadata_archivo* archivoMet=malloc(sizeof(t_metadata_archivo));
	fread(archivoMet,sizeof(t_metadata_archivo),1,metadata);
	int cantBloques=cantidadBloques(archivoMet->block_size, archivo.tamanio);
	int bloques[cantBloques];
	bloques[0]=archivo.bloqueInicial;
	leerBloques(bloques,cantBloques,archivoMet->block_size);
	char* sentenciasNew=malloc(archivo.tamanio+1);
	leerSentencias(bloques,cantBloques,sentenciasNew,archivo.tamanio+1,archivoMet->block_size);
	asignarPedido(pedido,sentenciasNew,archivo.tamanio+1);
	free(sentenciasNew);
	free(archivoMet);
	fclose(metadata);
	free(archivoMetadata);
	free(direcMetadata);
}
Pedido* obtenerPedido(void*msg){
	msg_obtener_pedido* mensaje=(msg_obtener_pedido*)msg;
	DIR* files=abrirDirectorio(afip,"Files");
	verificarDirectorio(files);
	char* rutaFiles=rutaDirectorio(afip,"Files");
	DIR* restaurantes=abrirDirectorio(rutaFiles,"Restaurantes");
	verificarDirectorio(restaurantes);
	char* rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	DIR* restaurante=abrirDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	char* rutaRestaurante=rutaDirectorio(rutaRestaurantes,mensaje->restaurante->nombre);
	Pedido* pedido=malloc(sizeof(Pedido));
	pedido->estadoPedido=SINPEDIDO;
	if (restaurante==NULL){
		PY_LOG_INFO("no existe el restaurante \n");
		free(rutaRestaurante);
		closedir(restaurante);
		free(rutaRestaurantes);
		closedir(restaurantes);
		free(rutaFiles);
		closedir(files);
		free(mensaje->restaurante->nombre);
		free(mensaje->restaurante);
		free(mensaje);
		return pedido;
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
 		free(mensaje->restaurante->nombre);
 		free(mensaje->restaurante);
 		free(mensaje);
         return pedido;
     }
     else{
    		int tamanioArchivo=calcularTamanioArchivo(rutaPedido);
    		char*sentencias=malloc(tamanioArchivo+1);
    		archivo_Info archivo=infoArchivo(rutaPedido, sentencias, tamanioArchivo);
    		leerPedido(archivo,pedido,sentencias);
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
    		return pedido;
    }
	}
}
Restaurante* obtenerRestaurante (void*msg){
	msg_obtener_restaurante*mensaje=(msg_obtener_restaurante*)msg;
	 char*rutaFiles=rutaDirectorio(afip,"Files");
	 char*rutaRestaurantes=rutaDirectorio(rutaFiles, "Restaurantes");
	 char*rutaRestaurante=rutaDirectorio(rutaRestaurantes, mensaje->restaurante->nombre);
	 DIR* ruta=opendir(rutaRestaurante);
	 Restaurante*restaurante;
	 if(ruta==NULL){
		 PY_LOG_INFO("no existe el restaurante");
		 restaurante=malloc(sizeof(Restaurante));
		 restaurante->info_Restaurante.cantidadDePedidos=-1;
		 restaurante->info_Restaurante.cantidadDeCocineros=0;
		 restaurante->info_Restaurante.cantidadDeHornos=0;
		 restaurante->info_Restaurante.Posicion.posicionX=0;
		 restaurante->info_Restaurante.Posicion.posicionY=0;
		 restaurante->nombre_restaurante=malloc(sizeof(nombre));
		 restaurante->nombre_restaurante->largo_nombre=0;
		 restaurante->nombre_restaurante->nombre=malloc(1);
		 restaurante->nombre_restaurante->nombre="\0";
		 restaurante->info_Restaurante.Platos=list_create();
		 restaurante->info_Restaurante.afinidades=list_create();

	 }
	 else{
		 char*rutaInfoRestaurante=rutaDirectorio(rutaRestaurante, "Info.AFIP");
		 restaurante=buscarRestaurante(rutaInfoRestaurante,mensaje->restaurante->nombre);
	     free(rutaInfoRestaurante);

	 }
	 closedir(ruta);
     free(rutaRestaurante);
     free(rutaRestaurantes);
     free(rutaFiles);
     free(mensaje->restaurante->nombre);
     free(mensaje->restaurante);
     free(mensaje);
	 return restaurante;

}


