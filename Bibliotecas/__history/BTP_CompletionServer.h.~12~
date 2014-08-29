//---------------------------------------------------------------------------

#ifndef BTP_CompletionServerH
#define BTP_CompletionServerH


#include "BTP_Thread.h"
#include "BTP_SocketServer.h"
#include "BTP_WorkerThread.h"
#include "BTP_CompletionPort.h"
#include "PLC_Controle.h"
#include "BTP_ServerAtualiza.h"


class BTP_CompletionServer :public BTP_Thread
{
 BTP_WorkerThread *Threads[10];
 PLC_Controle *PLC;
 BTP_SocketServer *Sock;
 BTP_CompletionPort *CompletionPort;
 BTP_ServerAtualiza * Atualizador;
 void __TROPICALL Executar();
 
 public:
  BTP_CompletionServer(char*);
  ~BTP_CompletionServer();
  void Iniciar();

};


//---------------------------------------------------------------------------
#endif
