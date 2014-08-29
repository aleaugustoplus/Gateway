//---------------------------------------------------------------------------



#include "BTP_ServerAtualiza.h"
#include <stdio.h>
#include "BTP_WorkerThread.h"
//---------------------------------------------------------------------------
BTP_ServerAtualiza::BTP_ServerAtualiza(char*nome ,PLC_Controle*plc):BTP_Thread(nome)
{
PLC=plc;
NumClientes=0;
ultimo=NULL;
primeiro=NULL;
clienteatual=NULL;
Escritor = NULL;
}
//---------------------------------------------------------------------------
BTP_ServerAtualiza::~BTP_ServerAtualiza()
{

}
//---------------------------------------------------------------------------
void __TROPICALL BTP_ServerAtualiza::Executar()
{
 PLC->ThreadMsg = GetCurrentThreadId();

 MSG Msg;
 char msg[100];


  Timer = SetTimer( NULL, // handle para janela
  NULL, // Identidade do temporizador
  1000, // Valor temporizado em ms
  NULL);


 for(;;)
 {
  GetMessageA(&Msg,NULL,NULL,NULL);

  switch(Msg.message)
  {
   case PLC_Controle::PLC_MUDOU_ENT:
	 if(!PLC->StatusPlc)
	  return;
	 clienteatual=primeiro;
	 if(clienteatual)
	  for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	   for(int x=0;x<PLC->NumEntradasDigitais;x++)
		if(clienteatual->AssinarVar.Entradas[x])
		{
		 if(PLC->EntradasDigitais[x])
		  sprintf(msg,"ED%d1%c%c",x,13,10);
		 else
		  sprintf(msg,"ED%d0%c%c",x,13,10);

		 clienteatual->EscreverStr(msg);
		}
   break;
   case PLC_Controle::PLC_MUDOU_SAIDA:
	 if(!PLC->StatusPlc)
	  return;
	 clienteatual=primeiro;
	 if(clienteatual)
	  for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	   for(int x=0;x<PLC->NumSaidasDigitais;x++)
		if(clienteatual->AssinarVar.Saidas[x])
		{
		 if(PLC->SaidasDigitais[x])
		  sprintf(msg,"SD%d1%c%c",x,13,10);
		 else
		  sprintf(msg,"SD%d0%c%c",x,13,10);

		 clienteatual->EscreverStr(msg);
		}



   break;
   case PLC_Controle::PLC_CONECTADO:
	printf("\nPLC - Conectou\n");
	for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	{
	 sprintf(msg,"STPLC1%c%c",13,10);
	 clienteatual->EscreverStr(msg);
	}

   break;
   case PLC_Controle::PLC_DESCONECTADO:
	printf("PLC - Desconectou\n");

	for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	{
	 sprintf(msg,"STPLC0%c%c",13,10);
	 clienteatual->EscreverStr(msg);
	}



   break;
   case PLC_Controle::PLC_BTP_EXCECAO:
//    printf("\n >Excecao %s<",)
   break;
   case WM_TIMER:


	 clienteatual=primeiro;
	 if(clienteatual)
	  for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	   for(int x=0;x<PLC->NumEntradasAnalogicas;x++)
		if(clienteatual->AssinarVar.EntradasA[x])
		{
		 sprintf(msg,"EA%d%c%c%c",x,PLC->EntradasAnalogicas[x],13,10);
		 clienteatual->EscreverStr(msg);
		}

	 clienteatual=primeiro;
	 if(clienteatual)
	  for(clienteatual = primeiro;clienteatual;this->ProximoCliente())
	   for(int x=0;x<PLC->NumSaidasAnalogicas;x++)
		if(clienteatual->AssinarVar.SaidasA[x])
		{
		 sprintf(msg,"SA%d%c%c%c",x,PLC->SaidasAnalogicas[x],13,10);
		 clienteatual->EscreverStr(msg);
		}
	 Timer = SetTimer( NULL, // handle para janela
	 Timer, // Identidade do temporizador
	 1000, // Valor temporizado em ms
	 NULL); // Ir� utilizar fun��o callback

   break;
   case PLC_Controle::PLC_EXCECAO:

   break;



  }


 }

}
//---------------------------------------------------------------------------
void BTP_ServerAtualiza::AddCliente(BTP_Socket*Cliente)
{
char Msg[50];
 if(!primeiro)
 {
  primeiro = Cliente;
  ultimo = Cliente;
  clienteatual = Cliente;
 }
 else if(ultimo==primeiro)
 {
  ultimo->Proximo = Cliente;
  Cliente->Anterior = ultimo;
  ultimo = Cliente;
  primeiro->Proximo = Cliente;

 }
 else
 {
  ultimo->Proximo = Cliente;
  Cliente->Anterior = ultimo;
  ultimo = Cliente;
 }

 if(!Escritor)
 {
  sprintf(Msg,"DIR_LER_ESC%c%c",13,10);
  Escritor = Cliente;
  Escritor->Escritor=true;
 }
 else
 {
  sprintf(Msg,"DIR_LER%c%c",13,10);
  Cliente->Escritor=false;
 }

 Cliente->EscreverStr(Msg);

 NumClientes++;
}
//---------------------------------------------------------------------------
void BTP_ServerAtualiza::RemoveCliente(BTP_Socket*Cliente)
{
BTP_Socket *Temp = primeiro;

 if(ultimo!=primeiro)
 {
  if(Cliente == primeiro)
  {
   primeiro->Proximo->Anterior = NULL;
   primeiro=Temp->Proximo;
  }
  if(Cliente == ultimo)
  {
   ultimo->Anterior->Proximo=NULL;
   ultimo=Temp;
  }
 }
 else
 {
  for(;Temp;)
  {

   if(Temp == Cliente && NumClientes != 1)
   {
	Temp->Anterior->Proximo = Temp->Proximo;
	Temp->Proximo->Anterior = Temp->Anterior;
	break;
   }
   Temp = Temp->Proximo;

  }
 }

 if(Cliente == Escritor)
  Escritor=NULL;

 delete Cliente;


 NumClientes--;

 if(!NumClientes)
 {
   primeiro = NULL;
   ultimo = NULL;

 }
}
//---------------------------------------------------------------------------
void BTP_ServerAtualiza::ProximoCliente()
{
 clienteatual=clienteatual->Proximo;
}
//---------------------------------------------------------------------------
void BTP_ServerAtualiza::ClienteAnterior()
{
 clienteatual=clienteatual->Anterior;
}
//---------------------------------------------------------------------------
void BTP_ServerAtualiza::Assinar()
{

	 printf("Assinou\n");
}
