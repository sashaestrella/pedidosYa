#include "pysockets.h"

#define DB_CAST_TO_IPV4(address)	((struct sockaddr_in*)(address))
#define DB_CAST_TO_IPV6(address)	((struct sockaddr_in6*)(address))

sfd server_create(char* puerto_config)
{
	char* puerto;
	puerto = py_config_get_string(puerto_config);

	struct addrinfo  hints;
	struct addrinfo* serverInfo;
	
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int err = getaddrinfo(NULL, puerto, &hints, &serverInfo);

	sfd sfd_escucha = socket(serverInfo->ai_family ,serverInfo->ai_socktype, serverInfo->ai_protocol);

	int verdadero = 1;
	setsockopt(sfd_escucha, SOL_SOCKET, SO_REUSEADDR, &verdadero, sizeof(verdadero));

	if(bind(sfd_escucha,serverInfo->ai_addr,serverInfo->ai_addrlen))
	{
		//PY_LOG_DEBUG("%s #(%d): Fallo en bind", db_enum_to_str(BIND_ERROR), BIND_ERROR);
		exit(BIND_ERROR);
	}

	freeaddrinfo(serverInfo);

	if(listen(sfd_escucha, SOMAXCONN) == -1)
	{
		//PY_LOG_DEBUG("%s #(%d): Fallo en listen", db_enum_to_str(LISTEN_ERROR), LISTEN_ERROR);
		exit(LISTEN_ERROR);
	}

	//PY_LOG_TRACE("Servidor creado");

	return sfd_escucha;
}

sfd client_create_manual(char* ip, char* puerto)
{
	struct addrinfo hints = { 0 };
		struct addrinfo *clientInfo;

		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		if(getaddrinfo(ip, puerto, &hints, &clientInfo) < 0)
		{
			//PY_LOG_DEBUG("%s #(%d): Falla en getaddrinfo", db_enum_to_str(LOOK_UP_ERROR), LOOK_UP_ERROR);
			freeaddrinfo(clientInfo);
			return -1;
		}

		sfd sfd_cliente = socket(clientInfo->ai_family,clientInfo->ai_socktype,clientInfo->ai_protocol);

		if(connect(sfd_cliente,clientInfo->ai_addr,clientInfo->ai_addrlen))
		{
			//PY_LOG_DEBUG("%s #(%d): No se pudo conectar al servidor", db_enum_to_str(CONN_ERROR), CONN_ERROR);
			freeaddrinfo(clientInfo);
		    	return -1;
		}

		//PY_LOG_TRACE("Conectado al servidor");
		freeaddrinfo(clientInfo);

		return sfd_cliente;
}

sfd client_create(char* ip_config, char* puerto_config)
{
	char* ip;
	char* puerto;
	ip = py_config_get_string(ip_config);
	puerto = py_config_get_string(puerto_config);
	
	struct addrinfo hints = { 0 };
	struct addrinfo *clientInfo;
	
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if(getaddrinfo(ip, puerto, &hints, &clientInfo) < 0)
	{
		//PY_LOG_DEBUG("%s #(%d): Falla en getaddrinfo", db_enum_to_str(LOOK_UP_ERROR), LOOK_UP_ERROR);
		freeaddrinfo(clientInfo);
		return -1;
	}

	sfd sfd_cliente = socket(clientInfo->ai_family,clientInfo->ai_socktype,clientInfo->ai_protocol);

	if(connect(sfd_cliente,clientInfo->ai_addr,clientInfo->ai_addrlen))
	{
		//PY_LOG_DEBUG("%s #(%d): No se pudo conectar al servidor", db_enum_to_str(CONN_ERROR), CONN_ERROR);
		freeaddrinfo(clientInfo);
	    	return -1;
	}
	
	//PY_LOG_TRACE("Conectado al servidor");
	freeaddrinfo(clientInfo);

	return sfd_cliente;
}
