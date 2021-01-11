#include "pymsg.h"

/*
   ____                                                       _
  / ___|_ __ ___  __ _ _ __   _ __ ___   ___ _ __  ___  __ _ (_) ___
 | |   | '__/ _ \/ _` | '__| | '_ ` _ \ / _ \ '_ \/ __|/ _` || |/ _ \
 | |___| | |  __/ (_| | |    | | | | | |  __/ | | \__ \ (_| || |  __/
  \____|_|  \___|\__,_|_|    |_| |_| |_|\___|_| |_|___/\__,_|/ |\___|

*/

int32_t calcularTamanioNombre(nombre* nombre){
	int32_t tamanio = 0;
	tamanio += nombre->largo_nombre;
	tamanio += sizeof(uint32_t);
	return tamanio;
}

int32_t calcularTamanioListaNombres(t_list* lista_nombres){
	int32_t tamanio_total = 0;
	Plato*nombre;
	for(int32_t i = 0; i < lista_nombres->elements_count; i++){
		nombre=list_get(lista_nombres, i);
		tamanio_total += calcularTamanioNombre(nombre->nombre);
	}
	tamanio_total += sizeof(uint32_t);
	return tamanio_total;
}

int32_t calcularTamanioListaNombresReal(t_list* lista_nombres){
	int32_t tamanio_total = 0;
	for(int32_t i = 0; i < lista_nombres->elements_count; i++){
		tamanio_total += calcularTamanioNombre(list_get(lista_nombres, i));
	}
	tamanio_total += sizeof(uint32_t);
	return tamanio_total;
}

int32_t calcularTamanioReceta(t_list* pasos){
	int32_t tamanio = 0;
	Paso* paso;
	tamanio+=sizeof(uint32_t);
	for(int32_t i = 0; i < pasos->elements_count; i++){
		paso = list_get(pasos, i);
		tamanio += paso->nombre->largo_nombre;
		tamanio += 2 * sizeof(uint32_t);
	}
	return tamanio;
}

int32_t calcularTamanioListaRecetas(t_list* recetas){
	int32_t tamanio_total = 0;
	Receta* receta;
	for(int32_t i = 0; i < recetas->elements_count; i++){
		receta = list_get(recetas, i);
		tamanio_total += calcularTamanioReceta(receta->Pasos);
	}
	return tamanio_total;
}

int32_t calcularTamanioPlatos(t_list* platos){
	int32_t tamanio_total = 0;
    Plato*unPlato;
    for(int i=0; i<platos->elements_count;i++){
    	unPlato=list_get(platos,i);
    	calcularTamanioPlato(unPlato);
    }
	return tamanio_total;
}

int32_t calcularTamanioPlato(Plato* plato){
	int32_t tamanio = 0;
	tamanio += plato->nombre->largo_nombre;
	tamanio += sizeof(uint32_t);
	tamanio += sizeof(uint32_t);
	return tamanio;
}

int32_t calcularTamanioListaRegistrosComida(t_list* registros_de_comidas){
	registroComida* registro;
	int32_t tamanio_total = 0;
	for(int32_t i = 0; i < registros_de_comidas->elements_count; i++){
		registro = list_get(registros_de_comidas, i);
		tamanio_total += registro->plato->largo_nombre;
		tamanio_total += sizeof(uint32_t)*3;
	}
	tamanio_total += sizeof(uint32_t);
	return tamanio_total;
}

int32_t calcularTamanioRestaurante(Restaurante* restaurante){
	int32_t tamanio_total = 0;
	tamanio_total += restaurante->nombre_restaurante->largo_nombre;
	tamanio_total += sizeof(uint32_t);
	nombre* afinidad;
	for(int32_t i = 0; i < restaurante->info_Restaurante.afinidades->elements_count; i++){
		afinidad = list_get(restaurante->info_Restaurante.afinidades, i);
		tamanio_total += calcularTamanioNombre(afinidad);
	}

	Plato*unPlato;
	for(int32_t i = 0; i < restaurante->info_Restaurante.Platos->elements_count; i++){
		unPlato = list_get(restaurante->info_Restaurante.Platos, i);
		tamanio_total += calcularTamanioNombre(unPlato->nombre);
		tamanio_total += sizeof(uint32_t);
	}
	tamanio_total += 6 * sizeof(uint32_t);
	tamanio_total+=sizeof(int32_t);
	return tamanio_total;
}

int32_t calcularTamanioPedido(Pedido pedido){
	int32_t tamanio_total = 0;
	tamanio_total += sizeof(int);         //ver si tiene que ser int32_t
	tamanio_total += calcularTamanioListaRegistrosComida(pedido.platos);

	return tamanio_total;
}

/*
 __  __                       _
|  \/  | ___ _ __  ___  __ _ (_) ___
| |\/| |/ _ \ '_ \/ __|/ _` || |/ _ \
| |  | |  __/ | | \__ \ (_| || |  __/
|_|  |_|\___|_| |_|___/\__,_|/ |\___|
                           |__/
*/

sfd conectar_y_enviar(char* ip_config, char* puerto_config, buffer_t* buffer, enum_mensaje tipo_mensaje){
	sfd sfd_servidor = client_create(ip_config, puerto_config);
	py_send(sfd_servidor, tipo_mensaje, buffer);
	return sfd_servidor;
}

sfd conectar_y_enviar_manual(char* ip, char* puerto, buffer_t* buffer, enum_mensaje tipo_mensaje){
	sfd sfd_servidor = client_create_manual(ip, puerto);
	py_send(sfd_servidor, tipo_mensaje, buffer);
	return sfd_servidor;
}

py_enum enviar_respuesta(sfd sfd_servidor, buffer_t* buffer, enum_mensaje tipo_mensaje){
	py_enum estado = py_send(sfd_servidor, tipo_mensaje ,buffer);
	close(sfd_servidor);
	return estado;
}

