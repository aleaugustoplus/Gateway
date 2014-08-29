//---------------------------------------------------------------------------

#ifndef BTP_ServerAtualizaH
#define BTP_ServerAtualizaH




#include "BTP_Thread.h"
#include "PLC_Controle.h"




class BTP_ServerAtualiza:public BTP_Thread
{
 PLC_Controle *PLC;
 unsigned int NumClientes;
 BTP_Socket* ultimo;
 BTP_Socket* primeiro;
 BTP_Socket* clienteatual;
 BTP_Socket* Escritor;
 UINT Timer;

void __TROPICALL Executar();


 




public:

	 BTP_ServerAtualiza(char*,PLC_Controle*plc);
	 ~BTP_ServerAtualiza();
	 void AddCliente(BTP_Socket*Cliente);
	 void RemoveCliente(BTP_Socket*Cliente);
	 void ProximoCliente();
	 void ClienteAnterior();
	 void Assinar();






};
//---------------------------------------------------------------------------
#endif
