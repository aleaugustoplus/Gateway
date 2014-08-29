//---------------------------------------------------------------------------

#ifndef BTP_SocketH
#define BTP_SocketH
//---------------------------------------------------------------------------


#include "BTP_Objeto.h"

#include <winsock.h>
#define MAXLINE			512	// Tamanho máximo da linha a ser editada pelo cliente

class BTP_Socket: public BTP_Objeto
{
 public:
 SOCKET Sock;
	// Input
 char        InBuffer[4];
 OVERLAPPED  ovIn;
 // Output
 int         nOutBufIndex;
 char        OutBuffer[MAXLINE];
 OVERLAPPED  ovOut;
 DWORD       dwWritten;

 void __TROPICALL Objeto();
 void Ler();

 public:
  BTP_Socket(char*,SOCKET);
  ~BTP_Socket();

};
#endif
