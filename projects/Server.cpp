#include <Winsock2.h>
#include <stdio.h>
#include "Server.h"

Server::Server(int family, int type, int protocol, int portNumber) {
	iFamily = family; //"AF_INET" is IPv4
	iType = type; //"SOCK_DGRAM" is datagram
	iProtocol = protocol; //"IPPROTO_UDP" is UDP
	port_number = portNumber;
}
int Server::start() {
	/* Open windows connection */
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	/* Open a datagram socket */
	sock = socket(iFamily, iType, iProtocol);
	if (sock == INVALID_SOCKET)
	{
		wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}

	/* Clear out server struct */
	memset((void *)&server, '\0', sizeof(struct sockaddr_in));

	server.sin_family = iFamily;
	server.sin_port = htons(port_number);

	gethostname(host_name, sizeof(host_name));
	hp = gethostbyname(host_name);

	/* Check for NULL pointer */
	if (hp == NULL)
	{
		fprintf(stderr, "Could not get host name.\n");
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	/* Assign the address */
	server.sin_addr.S_un.S_un_b.s_b1 = hp->h_addr_list[0][0];
	server.sin_addr.S_un.S_un_b.s_b2 = hp->h_addr_list[0][1];
	server.sin_addr.S_un.S_un_b.s_b3 = hp->h_addr_list[0][2];
	server.sin_addr.S_un.S_un_b.s_b4 = hp->h_addr_list[0][3];

	/* Bind address to socket */
	if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1)
	{
		fprintf(stderr, "Could not bind name to socket.\n");
		closesocket(sock);
		WSACleanup();
		return 1;
	}

	printf("Server running on %u.%u.%u.%u\n", (unsigned char)server.sin_addr.S_un.S_un_b.s_b1,
		(unsigned char)server.sin_addr.S_un.S_un_b.s_b2,
		(unsigned char)server.sin_addr.S_un.S_un_b.s_b3,
		(unsigned char)server.sin_addr.S_un.S_un_b.s_b4);
	
	//After this, run either receive mode or send mode.
}

int Server::receiveMode() {
	while (1)
	{
		fprintf(stderr, "WHO AM I.\n");
		client_length = (int)sizeof(struct sockaddr_in);

		/* Receive bytes from client */
		bytes_received = recvfrom(sock, buffer, 4096, 0, (struct sockaddr *)&client, &client_length);
		if (bytes_received < 0)
		{
			fprintf(stderr, "Could not receive datagram.\n");
			return 1;
		}

		fprintf(stderr, "NONE OF YOUR BUSINESS.\n");
		/* TODO
		if (strcmp(buffer, "GET TIME\r\n") == 0)
		{
		current_time = time(NULL);
		if (sendto(sd, (char *)&current_time, (int)sizeof(current_time), 0, (struct sockaddr *)&client, client_length) != (int)sizeof(current_time))
		{
		fprintf(stderr, "Error sending datagram.\n");
		closesocket(sd);
		WSACleanup();
		exit(0);
		}
		}
		*/
	}
	return 0;
}

char* Server::send(sockaddr_in recipient, char message[]){
	/* Tranmsit data*/
	int recipient_length = sizeof(struct sockaddr_in);
	if (sendto(sock, message, (int)strlen(message) + 1, 0, (struct sockaddr *)&recipient, recipient_length) == -1)
	{
		fprintf(stderr, "Error transmitting data.\n");
		closesocket(sock);
		WSACleanup();
		return 0;
	}

	/* Receive data */
	char returnMessage[4096];
	if (recvfrom(sock, returnMessage, (int)sizeof(returnMessage), 0, (struct sockaddr *)&server, (int)sizeof(struct sockaddr_in) < 0))
	{
		fprintf(stderr, "Error receiving data.\n");
	}
	return returnMessage;
}

int Server::closeServer() {
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	WSACleanup();
	return 0;
}