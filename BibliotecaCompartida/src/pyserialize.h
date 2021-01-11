#ifndef PY_SERIALIZE_H
#define PY_SERIALIZE_H

#include "pycore.h"

py_enum py_send( sfd sfd_servidor, enum_mensaje operacion, buffer_t* buffer);
py_enum py_recv_buffer(sfd sfd_cliente, buffer_t* buffer);
py_enum py_recv_headers(sfd sfd_cliente, py_paquete_t* paquete);
py_enum py_recv_msg(sfd sfd_conexion, py_paquete_t* paquete);

buffer_t* serializar_modulo(enum_modulo modulo);
buffer_t* serializar_seleccionar_restaurante(nombre* nombre_restaurante, uint32_t id_cliente);
buffer_t* serializar_obtener_restaurante(nombre* nombre_restaurante);
buffer_t* serializar_consultar_platos(nombre* nombre_restaurante);
buffer_t* serializar_guardar_pedido(nombre* nombre_restaurante, uint32_t id_pedido);
buffer_t* serializar_aniadir_plato(nombre* nombre_plato, uint32_t id_pedido);
buffer_t* serializar_guardar_plato(nombre* nombre_restaurante, uint32_t id_pedido, nombre* nombre_plato, uint32_t cantidad_platos);
buffer_t* serializar_confirmar_pedido(uint32_t id_pedido);
buffer_t* serializar_plato_listo(nombre* nombre_plato, nombre* nombre_restaurante, uint32_t id_pedido);
buffer_t* serializar_consultar_pedido(uint32_t id_pedido);
buffer_t* serializar_obtener_pedido(nombre* nombre_restaurante, uint32_t id_pedido);
buffer_t* serializar_finalizar_pedido(nombre* nombre_restaurante, uint32_t id_pedido);
buffer_t* serializar_terminar_pedido(nombre* nombre_restaurante, uint32_t id_pedido);
buffer_t* serializar_obtener_receta(nombre* nombre_plato);
buffer_t* serializar_int(int32_t numero);

buffer_t* serializar_lista_nombres(t_list* lista_nombres_restaurantes);
buffer_t* serializar_registro_comida(registroComida registro);
buffer_t* serializar_lista_registros_comida(t_list* registros_de_comidas);
buffer_t* serializar_pedido(Estado_pedido estadoPedido,t_list* platos);
buffer_t* serializar_obtener_restaurante_rta(t_list* afinidades,CoordenadasXY coordenadas,t_list* recetasConPrecio,uint32_t cantDeHornos,uint32_t cantidadDePedidos, uint32_t cantidadDeCocineros);
buffer_t* serializar_restaurante(Restaurante* restaurante);
buffer_t* serializar_receta(t_list* pasos);
buffer_t* serializar_confirmar_pedido_con_restaurante(uint32_t id_pedido, nombre* nombre_restaurante);

enum_modulo deserealizar_modulo(buffer_t* buffer);
msg_seleccionar_restaurante* deserializar_seleccionar_restaurante (buffer_t* buffer);
msg_obtener_restaurante* deserializar_obtener_restaurante(buffer_t* buffer);
msg_consultar_platos* deserializar_consultar_platos(buffer_t* buffer);
msg_guardar_pedido* deserializar_guardar_pedido(buffer_t* buffer);
msg_aniadir_plato* deserializar_aniadir_plato(buffer_t* buffer);
msg_guardar_plato* deserializar_guardar_plato(buffer_t* buffer);
msg_confirmar_pedido* deserializar_confirmar_pedido(buffer_t* buffer);
msg_plato_listo* deserializar_plato_listo(buffer_t* buffer);
msg_consultar_pedido* deserializar_consultar_pedido(buffer_t* buffer);
msg_obtener_pedido* deserializar_obtener_pedido(buffer_t* buffer);
msg_finalizar_pedido* deserializar_finalizar_pedido(buffer_t* buffer);
msg_terminar_pedido* deserializar_terminar_pedido(buffer_t* buffer);
msg_obtener_receta* deserializar_obtener_receta(buffer_t* buffer);
msg_consultar_pedido_rta* deserializar_consultar_pedido_rta(buffer_t* buffer);


void serializarPlato(void* stream, Plato* unPlato, int* offset_ptr);
void serializarPlatos(void* stream, t_list* platos, int* offset_ptr);
void serializarRepartidor(void* stream, Repartidor repartidor, int* offset_ptr);
void serializarCoordenadas(void* stream, CoordenadasXY coordenadas, int* offset_ptr);
void serializarRestauranteSinNombre(void* stream, InfoRestaurante* restaurante, int* offset_ptr);
void serializarRestaurante(void* stream, Restaurante* restaurante, int* offset_ptr);
void serializarRecetas(void* stream, t_list* recetas, int*offset_ptr);
void serializarReceta(void* stream,t_list* pasos, int* offset_ptr);
void serializarNombres(void* stream, t_list* nombres, int* offset_ptr);
void serializarNombresReal(void* stream, t_list* nombres, int* offset_ptr);
void serializarRegistroComida(void* stream,registroComida* registro,int* offset_ptr);
void serializarListaDeRegistrosComidas(void* stream,t_list* registros_de_platos,int* offset_ptr);
void serializarPedido(void* stream, Pedido pedido, int* offset_ptr);
void db_copiar_uint32(void* stream, uint32_t entero, int* offset_ptr);
void db_copiar_int32(void* stream, int32_t entero, int* offset_ptr);
void db_copiar_string(void* stream, uint32_t size, const char* cadena, int* offset_ptr);

int32_t deserializar_int(buffer_t* buffer);
struct_lista* deserializar_lista_registros_de_comida(buffer_t* buffer);
Pedido* deserializar_pedido(buffer_t* buffer);
Receta* deserializar_receta(buffer_t* buffer);
struct_lista* deserializar_lista_nombres(buffer_t* buffer);
struct_lista* deserializar_lista_de_platos(buffer_t* buffer);
nombre* deserializar_nombre(buffer_t* buffer);
Restaurante* deserializar_restaurante(buffer_t* buffer);
InfoRestaurante* deserializar_restaurante_sin_nombre(buffer_t* buffer);

#endif
