/*##################################################################
  #                Objeto Controle de PLC                          #
  #               Copyright�:                                      #
  #                Noname� Corp.                                   #
  #----------------------------------------------------------------#
  #                Modificado em:                                  #
  #                24/06/2008 - Alexandre A. S. Lopes              #
  #----------------------------------------------------------------#
  #                25/06/2008 - Alexandre A. S. Lopes              #
  ##################################################################*/

//---------------------------------------------------------------------------

#include "PLC_Controle.h"
#include <stdio.h>



//---------------------------------------------------------------------------
__TROPICALL PLC_Controle::PLC_Controle(char*Nome):BTP_Thread(Nome,false)
{
 numsaidasdigitais =   NUM_SAIDAS_D;
 numentradasdigitais = NUM_ENTRADAS_D;
 numentradasanalogicas = NUM_ENTRADAS_A;
 numsaidasanalogicas = NUM_SAIDAS_A;
 excecao = NULL;

 janelamsg=NULL;
 threadmsg=NULL;



 for(unsigned short x=0;x<numsaidasdigitais;x++)
  Portas.Saidas[x]=false;

 for(unsigned short x=0;x<numentradasdigitais;x++)
  Portas.Entradas[x]=false;

 for(unsigned short x=0;x<numentradasanalogicas;x++)
  Portas.EntradasA[x]=0;

 for(unsigned short x=0;x<numsaidasanalogicas;x++)
  Portas.SaidasA[x]=0;


 PortaCom = new BTP_PortaSerial("Porta de comunicacao");
 CompletionPort = new BTP_CompletionPort("Completion Port");
 CompletionPort->AssociarArquivo(PortaCom);



 BTP_Thread::Ativa = true;
 statusplc = false;


 Sleep(500);




 PortaCom->LeituraAsinc();
}
//---------------------------------------------------------------------------
__TROPICALL PLC_Controle::~PLC_Controle()
{
 delete CompletionPort;
 delete PortaCom;
}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::Executar()
{

  BOOL    bResult;
  DWORD   NumRead;
  BTP_BufferPorta *Buffer;
  OVERLAPPED *lpOver;


 try
 {
  for(;;)
  {
	bResult = GetQueuedCompletionStatus(
			   CompletionPort->hHandle,
			   &NumRead,
			   &(DWORD)Buffer,
			   &lpOver,
			   INFINITE
			);

		if (bResult == FALSE && lpOver == NULL) {
			//IssueRead(pCntx);
//			FatalError(
//				"ThreadFunc - Illegal call to GetQueuedCompletionStatus");
		}
		else if (bResult == FALSE && lpOver != NULL) {
			//de;
			//fprintf(stderr,"ThreadFunc - I/O operation failed\n");
		}
		else if (NumRead == 0)
		{

			if(statusplc)
			{
			 statusplc = false;
			 EntregarMsg(PLC_DESCONECTADO);

			}

			PortaCom->LeituraAsinc();
			continue;
		}
		else
		{
			Buffer->AddInBufferStr();
			if (Buffer->Entrada == 10 )
			{

			 if(Buffer->BufferStr[0]=='E'&&Buffer->BufferStr[1]=='D')
			  AtualizarEntradasD(2);

			 if(Buffer->BufferStr[0]=='S'&&Buffer->BufferStr[1]=='D')
			  AtualizarSaidasD(2);

			 if(Buffer->BufferStr[0]=='E'&&Buffer->BufferStr[1]=='A')
			  AtualizarEntradasA(2);

			 if(Buffer->BufferStr[0]=='S'&&Buffer->BufferStr[1]=='A')
			  AtualizarSaidasA(2);

			 Buffer->LimparBufferStr();

			 if(!statusplc)
			 {
			  statusplc = true;
			  EntregarMsg(PLC_CONECTADO);

			 }

			}

			PortaCom->LeituraAsinc();

		}
	}
  }
  catch(BTP_Excecao*Erro)
  {
	  excecao=Erro;
	  EntregarMsg(PLC_BTP_EXCECAO);
  }
  catch(...)
  {
	  EntregarMsg(PLC_EXCECAO);
  }



}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AtualizarEntradasD(unsigned int BufferPos)
{
 unsigned short y =0;
 bool Temp;

 for(unsigned int x = BufferPos;;x++)
 {
  Temp = Portas.Entradas[y];
  if(PortaCom->BufferPort->BufferStr[x]=='\0')
   break;


  if(PortaCom->BufferPort->BufferStr[x]=='0')
  {
   Portas.Entradas[y] = false;

   if(Temp)
	EntregarMsg(PLC_MUDOU_ENT);

  }
  else
  {
   Portas.Entradas[y] = true;

   if(!Temp)
	EntregarMsg(PLC_MUDOU_ENT);

  }



  y++;
  if(y >= numentradasdigitais )
   break;
 }


}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AtualizarSaidasD(unsigned int BufferPos)
{
 unsigned short y =0;
 bool Temp;
 for(unsigned int x = BufferPos;;x++)
 {

  if(PortaCom->BufferPort->BufferStr[x]=='\0')
   break;

  Temp=Portas.Saidas[y];
  if(PortaCom->BufferPort->BufferStr[x]=='0')
  {
   Portas.Saidas[y] = false;

   if(Temp)
	EntregarMsg(PLC_MUDOU_SAIDA);
  }
  else
  {
   Portas.Saidas[y] = true;
	if(!Temp)
	EntregarMsg(PLC_MUDOU_SAIDA);
  }

  y++;
  if(y >= numsaidasdigitais )
   break;
 }


}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AtualizarEntradasA(unsigned int BufferPos)
{
 unsigned short y =0;

 for(unsigned int x = BufferPos;;x++)
 {

  if(PortaCom->BufferPort->BufferStr[x]=='\0')
   break;


   Portas.EntradasA[y] = PortaCom->BufferPort->BufferStr[x];

  y++;
  if(y >= numentradasanalogicas )
   break;
 }


}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AtualizarSaidasA(unsigned int BufferPos)
{
 unsigned short y =0;

 for(unsigned int x = BufferPos;;x++)
 {

  if(PortaCom->BufferPort->BufferStr[x]=='\0')
   break;


   Portas.SaidasA[y] = PortaCom->BufferPort->BufferStr[x];

  y++;
  if(y >= numsaidasanalogicas )
   break;
 }


}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::EntregarMsg(unsigned int Msg)
{
 if(janelamsg)
  SendMessageA(janelamsg,Msg,0,0);
 if(threadmsg)
  PostThreadMessage(threadmsg,Msg,0,0);
}
//--Propriedades-------------------------------------------------------------
unsigned short __TROPICALL PLC_Controle::PegarNumSaidasDigitais()
{
 return numsaidasdigitais;
}
//---------------------------------------------------------------------------
bool __TROPICALL PLC_Controle::PegarSaidasDigitais(unsigned short Num)
{
 return Portas.Saidas[Num];
}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AlterarSaidasDigitais(unsigned short Num,bool temp)
{
 if(Portas.Saidas[Num]!=temp)
 {
  char Msg[15];
  if(temp)
   sprintf(Msg,"CMDS%dT%c%c",Num,13,10);
  else
   sprintf(Msg,"CMDS%dF%c%c",Num,13,10);
//  BTP_Thread::Ativa = false;
   PortaCom->TransmitirDados(Msg,strlen(Msg));
//  BTP_Thread::Ativa = true;
 }

// Portas.Saidas[Num]=temp;
}
//---------------------------------------------------------------------------
unsigned short __TROPICALL PLC_Controle::PegarNumSaidasAnalogicas()
{
 return numsaidasanalogicas;
}
//---------------------------------------------------------------------------
unsigned char __TROPICALL PLC_Controle::PegarSaidasAnalogicas(unsigned short Num)
{
 return Portas.SaidasA[Num];
}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AlterarSaidasanalogicas(unsigned short Num, unsigned char Valor)
{
 if(Portas.Saidas[Num]!=Valor)
 {
  char Msg[15];
  sprintf(Msg,"CMDA%d%c%c%c",Num,Valor,13,10);
  printf("Msg: %s",Msg);
  PortaCom->TransmitirDados(Msg,strlen(Msg));
 }

 Portas.Saidas[Num]=Valor;
}
//---------------------------------------------------------------------------
unsigned short __TROPICALL PLC_Controle::PegarNumEntradasDigitais()
{
 return numentradasdigitais;
}
//---------------------------------------------------------------------------
bool __TROPICALL PLC_Controle::PegarEntradasDigitais(unsigned short Num)
{
 return Portas.Entradas[Num];
}
//---------------------------------------------------------------------------
unsigned short __TROPICALL PLC_Controle::PegarNumEntradasAnalogicas()
{
 return numentradasanalogicas;
}
//---------------------------------------------------------------------------
unsigned char __TROPICALL  PLC_Controle::PegarEntradasAnalogicas(unsigned short Num)
{
 return Portas.EntradasA[Num];
}
//---------------------------------------------------------------------------
BTP_Excecao * __TROPICALL PLC_Controle::PegarExcecao()
{
  BTP_Excecao *temp = excecao;
  excecao = NULL;
  return temp;
}
//---------------------------------------------------------------------------
DWORD __TROPICALL PLC_Controle::PegarThreadMsg()
{
  return threadmsg;
}
//---------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AlterarThreadMsg(DWORD temp)
{
 threadmsg=temp;
}
//---------------------------------------------------------------------------
HWND __TROPICALL PLC_Controle::PegarJanelaMsg()
{
 return janelamsg;
}
//--------------------------------------------------------------------------
void __TROPICALL PLC_Controle::AlterarJanelaMsg(HWND temp)
{
 janelamsg = temp;
}
//---------------------------------------------------------------------------
bool __TROPICALL PLC_Controle::PegarSatusPlc()
{
 return statusplc;
}
//---------------------------------------------------------------------------
