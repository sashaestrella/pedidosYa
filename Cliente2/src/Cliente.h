#ifndef CLIENTE_H_
#define CLIENTE_H_

#include <src/pycore.h>
#include <src/pyconfig.h>
#include <src/pylog.h>
#include <src/pyconexiones.h>
#include <src/pyserialize.h>
#include <src/pyhandshake.h>
#include <src/pysockets.h>
#include <src/pymsg.h>
#include <readline/readline.h>
#include <readline/history.h>

t_log* logger;

msg_seleccionar_restaurante* parsearSeleccionarRestaurante(int32_t idCliente,char* nombreRestaurante);
msg_aniadir_plato* parsearAniadirPlato(char* plato,char* idPedido);
msg_confirmar_pedido* parsearConfirmarPedido(char* idPedido,char*nombreRestaurante);
msg_confirmar_pedido* parsearConfirmarPedidoConRestaurante(char* idPedido,char* nombreRestaurante);
msg_plato_listo* parsearPlatoListo(char* nombreRestaurante,char* idPedido,char* nombrePlato);
msg_consultar_pedido* parsearConsultarPedido(char* idPedido);
msg_guardar_pedido* parsearGuardarPedido(char* idPedido,char* nombreRestaurante);
msg_obtener_pedido* parsearObtenerPedido(char* idPedido,char* nombreRestaurante);
msg_finalizar_pedido* parsearFinalizarPedido(char* idPedido,char* nombreRestaurante);
msg_terminar_pedido* parsearTerminarPedido(char* idPedido,char* nombreRestaurante);
msg_guardar_plato* parsearGuardarPlato(char* idPedido,char* plato,char* cantidad_platos,char* nombreRestaurante);
msg_obtener_restaurante* parsearObtenerRestaurante(char* nombreRestaurante);
msg_obtener_receta* parsearObtenerReceta(char* nombrePlato);



void py_config_delete(void);

#endif /* CLIENTE_H_ */
