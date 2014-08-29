//---------------------------------------------------------------------------

#ifndef BTP_SocketServerH
#define BTP_SocketServerH
//---------------------------------------------------------------------------

#include "BTP_Objeto.h"
#include <winsock.h>
#include "BTP_Socket.h"


class BTP_SocketServer:public BTP_Objeto
{
 public:
 BTP_Socket  *SockP;

 void __TROPICALL Objeto();

 public:
 BTP_SocketServer(char*);
 ~BTP_SocketServer();

 BTP_Socket* AguardarConexao();

};
#endif
