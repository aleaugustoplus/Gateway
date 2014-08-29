//---------------------------------------------------------------------------



#include "BTP_WorkerThread.h"
#include <stdio.h>
#include "BTP_Socket.h"
#include <stdlib.h>
//---------------------------------------------------------------------------
BTP_WorkerThread::BTP_WorkerThread(char*nome,BTP_CompletionPort*completionport,PLC_Controle*plc,
 BTP_ServerAtualiza *Server):BTP_Thread(nome)
{
 CompletionPort=completionport;
 PLC = plc;
 threadmsg = NULL;
 BTP_Thread::Ativa=true;
 Atualizador=Server;
}
//---------------------------------------------------------------------------
BTP_WorkerThread::~BTP_WorkerThread()
{

}
//---------------------------------------------------------------------------
void __TROPICALL BTP_WorkerThread::Executar()
{
	BOOL    bResult;
	DWORD   dwNumRead;
	LPOVERLAPPED lpOverlapped;


	for (;;)
	{	// Se associa � Completion Port
		bResult = GetQueuedCompletionStatus(
			   CompletionPort->PegarHandle(),
			   &dwNumRead,
			   &(DWORD)Cliente,
			   &lpOverlapped,
			   INFINITE
			);
//    	printf("\nTrabalhando!");
		if (bResult == FALSE && lpOverlapped == NULL) {
			printf(
				"ThreadFunc - Illegal call to GetQueuedCompletionStatus");
		}
		else if ((bResult == FALSE && lpOverlapped != NULL)||(dwNumRead == 0))
		{

		   Atualizador->RemoveCliente(Cliente);

		}
		else
		{


			Cliente->AddToBuffer();
//			printf("%c\n",SockCliente->OutBuffer[SockCliente->nOutBufIndex-1]);
//			printf("%c\n",SockCliente->InBuffer[0]);
			if (Cliente->InBuffer[0] == '$')
			{

				Cliente->OutBuffer[Cliente->nOutBufIndex]='\0';
				printf("%s\n",Cliente->OutBuffer);

				if(PLC->StatusPlc)
				{
				 NumESMsg();
				 AssinarSaidaD();
				 AssinarEntD();
				 AssinarSaidaA();
				 AssinarEntA();
				 PegarSaidaDigital();
				 PegarEntradaDigital();
				 PegarSaidaAnalogica();
				 PegarEntradaAnalogica();
				 AlterarSaidasDigitais();
				 AlterarTempoAssA();
				 AlterarSaidasAnalogicas();
				}
				else
				{
				  char Msg[30];
				  sprintf(Msg,"STPLC0%c%c",13,10);
				  Cliente->EscreverStr(Msg);
                }
				StatusMsg();
				DesligarMsg();



				Cliente->LimparBuffer();
			}

			Cliente->Ler();
		}
	}


}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AlterarSaidasDigitais()
{
char Msg[30];
  if (!(Cliente->OutBuffer[0]=='S' && Cliente->OutBuffer[1]=='D'))
	return;

  if(!Cliente->Escritor)
	return;


  for(int x=0;x<PLC->NumSaidasDigitais;x++)
   if((Cliente->OutBuffer[2]-48)==x)
   {
	  if(Cliente->OutBuffer[3]=='0')
	  {
		PLC->SaidasDigitais[x] = false;
		sprintf(Msg,"SD%d0%c%c",x,13,10);
		Cliente->EscreverStr(Msg);
	  }
	  else
	  {
	   PLC->SaidasDigitais[x] = true;
	   sprintf(Msg,"SD%d1%c%c",x,13,10);
	   Cliente->EscreverStr(Msg);
	  }
	  return;
   }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::StatusMsg()
{
 if(strcmp(Cliente->OutBuffer,"STPO$"))
  return;

char MsgED[50],MsgEA[50],MsgSD[50],MsgSA[50],Msg[200];
char *msg;


  if(PLC->StatusPlc)
  {
   msg = new char [PLC->NumEntradasDigitais+1];
   for(int x=0;x<PLC->NumEntradasDigitais;x++)
	if(PLC->EntradasDigitais[x])
	 msg[x] = '1';
	else
	 msg[x] = '0';

   msg[PLC->NumEntradasDigitais]='\0';
   sprintf(MsgED,"ED%s%c%c",msg,13,10);

   delete []msg;

   msg = new char [PLC->NumSaidasDigitais+1];
   for(int x=0;x<PLC->NumSaidasDigitais;x++)
	if(PLC->SaidasDigitais[x])
	 msg[x] = '1';
	else
	 msg[x] = '0';

   msg[PLC->NumSaidasDigitais]='\0';
   sprintf(MsgSD,"SD%s%c%c",msg,13,10);

   delete []msg;

   msg = new char [PLC->NumSaidasAnalogicas+1];
   for(int x=0;x<PLC->NumSaidasAnalogicas;x++)
	msg[x] = PLC->SaidasAnalogicas[x];

   msg[PLC->NumSaidasAnalogicas]='\0';
   sprintf(MsgSA,"SA%s%c%c",msg,13,10);

   delete []msg;

   msg = new char [PLC->NumEntradasAnalogicas+1];
   for(int x=0;x<PLC->NumEntradasAnalogicas;x++)
	msg[x] = PLC->EntradasAnalogicas[x];

   msg[PLC->NumEntradasAnalogicas]='\0';
   sprintf(MsgEA,"EA%s%c%c",msg,13,10);

   delete []msg;



   sprintf(Msg,"%s%s%s%sSTPLC1%c%c",MsgED,MsgSD,MsgEA,MsgSA,13,10);


  }
  else
  {
   sprintf(Msg,"STPLC0%c%c",13,10);

  }


   Cliente->EscreverStr(Msg);


}
//---------------------------------------------------------------------------
void BTP_WorkerThread::DesligarMsg()
{
  if(strcmp(Cliente->OutBuffer,"MSGOFF$"))
   return;

  if(!Cliente->Escritor)
	return;

 printf("\n\nO servidor sera desligado!!\n\n");

 Sleep(3000);

 exit(0);

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::NumESMsg()
{
 char Msg[100];
  if(strcmp(Cliente->OutBuffer,"NUM_ES$"))
   return;

 sprintf(Msg,"NUM_SD%d%c%cNUM_ED%d%c%cNUM_SA%d%c%cNUM_EA%d%c%c",
		 PLC->NumSaidasDigitais,13,10,
		 PLC->NumEntradasDigitais,13,10,
		 PLC->NumSaidasAnalogicas,13,10,
		 PLC->NumEntradasAnalogicas,13,10);
 Cliente->EscreverStr(Msg);
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AssinarSaidaD()
{
 if(Cliente->OutBuffer[0]!='A' || Cliente->OutBuffer[1]!='S' ||
	Cliente->OutBuffer[2]!='S' || Cliente->OutBuffer[3]!='D')
   return;

 Atualizador->Assinar();

 for(int x=0;x<PLC->NumSaidasDigitais;x++)
  if((Cliente->OutBuffer[4]-48)==x)
  {
   if(Cliente->OutBuffer[5]=='0')
	Cliente->AssinarVar.Saidas[x] = false;
   else
	Cliente->AssinarVar.Saidas[x] = true;

   return;
  }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AssinarEntD()
{
 if(Cliente->OutBuffer[0]!='A' || Cliente->OutBuffer[1]!='S' ||
	Cliente->OutBuffer[2]!='E' || Cliente->OutBuffer[3]!='D')
   return;

   Atualizador->Assinar();

 for(int x=0;x<PLC->NumEntradasDigitais;x++)
  if((Cliente->OutBuffer[4]-48)==x)
  {
   if(Cliente->OutBuffer[5]=='0')
	Cliente->AssinarVar.Entradas[x] = false;
   else
	Cliente->AssinarVar.Entradas[x] = true;

   return;
  }
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AssinarEntA()
{
  if(Cliente->OutBuffer[0]!='A' || Cliente->OutBuffer[1]!='S' ||
	 Cliente->OutBuffer[2]!='E' || Cliente->OutBuffer[3]!='A')
   return;

	Atualizador->Assinar();

 for(int x=0;x<PLC->NumEntradasAnalogicas;x++)
  if((Cliente->OutBuffer[4]-48)==x)
  {
   if(Cliente->OutBuffer[5]=='0')
	Cliente->AssinarVar.EntradasA[x] = false;
   else
	Cliente->AssinarVar.EntradasA[x] = true;

   return;
  }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AssinarSaidaA()
{
 if(Cliente->OutBuffer[0]!='A' || Cliente->OutBuffer[1]!='S' ||
	Cliente->OutBuffer[2]!='S' || Cliente->OutBuffer[3]!='A')
   return;

	Atualizador->Assinar();

 for(int x=0;x<PLC->NumSaidasAnalogicas;x++)
  if((Cliente->OutBuffer[4]-48)==x)
  {
   if(Cliente->OutBuffer[5]=='0')
	Cliente->AssinarVar.SaidasA[x] = false;
   else
	Cliente->AssinarVar.SaidasA[x] = true;

   return;
  }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AlterarSaidasAnalogicas()
{
char Msg[50];
 if(Cliente->OutBuffer[0]!='S' || Cliente->OutBuffer[1]!='A')
   return;

  Atualizador->Assinar();

 if(!Cliente->Escritor)
  return;


 for(int x=0;x<PLC->NumSaidasAnalogicas;x++)
  if((Cliente->OutBuffer[2]-48)==x)
  {
	PLC->SaidasAnalogicas[x]=Cliente->OutBuffer[3];
	sprintf(Msg,"SA%d%c%c%c",x,Cliente->OutBuffer[3],13,10);
	return;
  }

  Cliente->EscreverStr(Msg);
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::AlterarTempoAssA()
{
 if(Cliente->OutBuffer[0]!='A' || Cliente->OutBuffer[1]!='S' ||
	Cliente->OutBuffer[2]!='T')
   return;

   Atualizador->Assinar();
  if(!Cliente->Escritor)
   return;
   
   Cliente->AssinarVar.TempoA = (Cliente->OutBuffer[3]-48);
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::PegarSaidaDigital()
{
char Msg[100];

   if (!(Cliente->OutBuffer[0]=='P' && Cliente->OutBuffer[1]=='S' &&
										   Cliente->OutBuffer[2]=='D'))
	return;

  if(!Cliente->Escritor)
	return;


  for(int x=0;x<PLC->NumSaidasDigitais;x++)
   if((Cliente->OutBuffer[3]-48)==x)
   {
	  if(PLC->SaidasDigitais[x])
	  {
	   sprintf(Msg,"SD%d1%c%c",x,13,10);
	   Cliente->EscreverStr(Msg);
	  }
	  else
	  {
	   sprintf(Msg,"SD%d0%c%c",x,13,10);
	   Cliente->EscreverStr(Msg);
	  }
	  return;
   }
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::PegarEntradaDigital()
{
char Msg[100];

   if (!(Cliente->OutBuffer[0]=='P' && Cliente->OutBuffer[1]=='E' &&
										   Cliente->OutBuffer[2]=='D'))
	return;

  if(!Cliente->Escritor)
	return;


  for(int x=0;x<PLC->NumEntradasDigitais;x++)
   if((Cliente->OutBuffer[3]-48)==x)
   {
	  if(PLC->EntradasDigitais[x])
	  {
	   sprintf(Msg,"ED%d1%c%c",x,13,10);
	   Cliente->EscreverStr(Msg);
	  }
	  else
	  {
	   sprintf(Msg,"ED%d0%c%c",x,13,10);
	   Cliente->EscreverStr(Msg);
	  }
	  return;
   }
}
//---------------------------------------------------------------------------
void BTP_WorkerThread::PegarSaidaAnalogica()
{
char Msg[100];

   if (!(Cliente->OutBuffer[0]=='P' && Cliente->OutBuffer[1]=='S' &&
										   Cliente->OutBuffer[2]=='A'))
	return;

  if(!Cliente->Escritor)
	return;


  for(int x=0;x<PLC->NumSaidasAnalogicas;x++)
   if((Cliente->OutBuffer[3]-48)==x)
   {
	   sprintf(Msg,"SA%d%c%c%c",x,PLC->SaidasAnalogicas[x],13,10);
	   Cliente->EscreverStr(Msg);
	  return;
   }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::PegarEntradaAnalogica()
{
char Msg[100];

   if (!(Cliente->OutBuffer[0]=='P' && Cliente->OutBuffer[1]=='E' &&
										   Cliente->OutBuffer[2]=='A'))
	return;

  if(!Cliente->Escritor)
	return;


  for(int x=0;x<PLC->NumEntradasAnalogicas;x++)
   if((Cliente->OutBuffer[3]-48)==x)
   {
	   sprintf(Msg,"EA%d%c%c%c",x,PLC->EntradasAnalogicas[x],13,10);
	   Cliente->EscreverStr(Msg);
	  return;
   }

}
//---------------------------------------------------------------------------
void BTP_WorkerThread::EntregarMsg(unsigned int Msg)
{
 PostThreadMessage(threadmsg,Msg,0,0);
}
//--Propriedades-------------------------------------------------------------
void BTP_WorkerThread::AlterarThreadMsg(DWORD temp)
{
 threadmsg = temp;
}
//---------------------------------------------------------------------------

