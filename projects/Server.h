#ifndef _SERVER_H_
#define _SERVER_H_

#include <Winsock2.h>

class Server {
public:
	Server(int family, int type, int protocol, int portNumber);
	int start();
	int receiveMode();
	char* send(sockaddr_in recipient, char message[]);
	int closeServer();
private:
	WSADATA wsaData = { 0 };
	int iResult = 0;
	SOCKET sock = INVALID_SOCKET;
	int iFamily = AF_UNSPEC;
	int iType = 0;
	int iProtocol = 0;
	char buffer[4096];
	unsigned short port_number;			/* Port number to use */
	int ip;				/* Components of address in xxx.xxx.xxx.xxx form */
	int client_length;					/* Length of client struct */
	int bytes_received;					/* Bytes received from client */
	struct sockaddr_in server;			/* Information about the server */
	struct sockaddr_in client;			/* Information about the client */
	struct hostent *hp;					/* Information about this computer */
	char host_name[256];				/* Name of the server */
};

#endif