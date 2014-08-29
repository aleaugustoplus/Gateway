//---------------------------------------------------------------------------

#ifndef BTP_WorkerThreadH
#define BTP_WorkerThreadH


 #include "BTP_Thread.h"
 #include "BTP_CompletionPort.h"
 #include "PLC_Controle.h"
 #include "BTP_ServerAtualiza.h"

class BTP_WorkerThread:public BTP_Thread
{
 public:
 enum SOCK_CTRL_MSG{
					 CLIENTE_DESCONECTOU = 20000,
					 

 };

 private:
 BTP_Socket * Cliente;
 BTP_CompletionPort *CompletionPort;
 PLC_Controle *PLC;
 BTP_ServerAtualiza* Atualizador;
 void __TROPICALL Executar();

 void StatusMsg();
 void DesligarMsg();
 void NumESMsg();
 void AssinarSaidaD();
 void AssinarEntD();
 void AssinarSaidaA();
 void AssinarEntA();
 void AlterarSaidasDigitais();
 void AlterarSaidasAnalogicas();
 void AlterarTempoAssA();
 void PegarSaidaDigital();
 void PegarEntradaDigital();
 void PegarSaidaAnalogica();
 void PegarEntradaAnalogica();

 void EntregarMsg(unsigned int Msg);
 DWORD threadmsg;


 public:
 BTP_WorkerThread(char*,BTP_CompletionPort*,PLC_Controle*,BTP_ServerAtualiza*);
 ~BTP_WorkerThread();

 __property DWORD ThreadMsg = {write = AlterarThreadMsg};

 void AlterarThreadMsg(DWORD); 


};
//---------------------------------------------------------------------------
#endif

