#ifndef DB_MSG_H
#define DB_MSG_H

#include "pycore.h"

#define MSG_SELECCIONAR_RESTAURANTE(ptr) ((msg_seleccionar_restaurante*)(ptr))
#define MSG_OBTENER_RESTAURANTE(ptr)     ((msg_obtener_restaurante*)(ptr))
#define MSG_CONSULTAR_PLATOS(ptr)        ((msg_consultar_platos*)(ptr))
#define MSG_GUARDAR_PEDIDO(ptr)          ((msg_guardar_pedido*)(ptr))
#define MSG_ANIADIR_PLATO(ptr)           ((msg_aniadir_plato*)(ptr))
#define MSG_GUARDAR_PLATO(ptr)           ((msg_guardar_plato*)(ptr))
#define MSG_CONFIRMAR_PEDIDO(ptr)        ((msg_confirmar_pedido*)(ptr))
#define MSG_PLATO_LISTO(ptr)             ((msg_plato_listo*)(ptr))
#define MSG_CONSULTAR_PEDIDO(ptr)        ((msg_consultar_pedido*)(ptr))
#define MSG_OBTENER_PEDIDO(ptr)          ((msg_obtener_pedido*)(ptr))
#define MSG_FINALIZAR_PEDIDO(ptr)        ((msg_finalizar_pedido*)(ptr))
#define MSG_TERMINAR_PEDIDO(ptr)         ((msg_terminar_pedido*)(ptr))
#define MSG_OBTENER_RECETA(ptr)          ((msg_obtener_receta*)(ptr))

int32_t calcularTamanioNombre(nombre* nombre);
int32_t calcularTamanioListaNombres(t_list* lista_nombres);
int32_t calcularTamanioListaNombresReal(t_list* lista_nombres);
int32_t calcularTamanioReceta(t_list* pasos);
int32_t calcularTamanioListaRecetas(t_list* recetas);
int32_t calcularTamanioPlatos(t_list* recetas);
int32_t calcularTamanioPlato(Plato* plato);
int32_t calcularTamanioListaRegistrosComida(t_list* registros_de_comidas);
int32_t calcularTamanioRestaurante(Restaurante* restaurante);
int32_t calcularTamanioPedido(Pedido pedido);

sfd conectar_y_enviar(char* ip_config, char* puerto_config, buffer_t* buffer, enum_mensaje tipo_mensaje);
sfd conectar_y_enviar_manual(char* ip, char* puerto, buffer_t* buffer, enum_mensaje tipo_mensaje);
py_enum enviar_respuesta(sfd sfd_servidor, buffer_t* buffer, enum_mensaje tipo_mensaje);

#endif
