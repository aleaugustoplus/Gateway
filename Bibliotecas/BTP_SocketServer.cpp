//---------------------------------------------------------------------------



#include "BTP_SocketServer.h"
//---------------------------------------------------------------------------

#include <windows.h>
#include <stdio.h>
// Pick a port number that seems to be away from all others
#define SERV_TCP_PORT	5554
#define MAXLINE			512	// Tamanho máximo da linha a ser editada pelo cliente


BTP_SocketServer::BTP_SocketServer(char*nome):BTP_Objeto(nome)
{
	WSADATA WsaData;
	SockP=new BTP_Socket("SockPrinicipal",0);

	struct	sockaddr_in serverAddress;
//	struct	sockaddr_in clientAddress;




	if (WSAStartup (0x0101, &WsaData) == SOCKET_ERROR)
	{
		printf("WSAStartup Failed");
		return ;
	}

	SockP->Sock = socket(AF_INET, SOCK_STREAM, 0);


	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(SERV_TCP_PORT);

	if (bind(SockP->Sock,
		(struct sockaddr *)&serverAddress,
		sizeof(serverAddress)
		) < 0)
		printf("bind() failed");

	 	// coverte socket em socket de escuta
	listen(SockP->Sock, 5);	// Máximo de 5 conexões simultaneas



}
//---------------------------------------------------------------------------
BTP_SocketServer::~BTP_SocketServer()
{

}
//---------------------------------------------------------------------------
void __TROPICALL BTP_SocketServer::Objeto()
{

}
//---------------------------------------------------------------------------
BTP_Socket* BTP_SocketServer::AguardarConexao()
{
		int     clientAddressLength;
		struct	sockaddr_in clientAddress;

		clientAddressLength = sizeof(clientAddress);

		return new BTP_Socket("Cliente", accept(SockP->Sock,
							(struct sockaddr *)&clientAddress,
							&clientAddressLength));







		/*if (newsocket < 0) {
			FatalError("accept() Failed");
			return ;
		} */

}
