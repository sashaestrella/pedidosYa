#ifndef BIBLIOTECAS_SOCKETS_H_
#define BIBLIOTECAS_SOCKETS_H_

#include "pycore.h"

sfd client_create_manual(char* ip, char* puerto);
sfd client_create(char* ip_config, char* puerto_config);
sfd server_create(char* puerto_config);


#endif
