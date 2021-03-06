/*##################################################################
  #                       Objeto Pai                               #
  #               Copyright�:                                      #
  #                Tropical� Corp.                                 #
  #----------------------------------------------------------------#
  #               Criada em:                                       #
  #               27/06/06 - Alexandre A. S. lopes                 #
  #               Descri��o:      								 #
  #                Classe Abstrata! para o controle dos objetos    #
  #      da Tropical� Corp.                                        #
  #----------------------------------------------------------------#
  #               Modificada em:                                   #
  #               31/07/2006 - Alexandre Oliveira Santos           #
  #----------------------------------------------------------------#
  #               12/08/2006 - Alexandre A. S. Lopes               #
  #           Introdu��o de diretivas de Pr�-Processador           #
  #----------------------------------------------------------------#
  #               18/01/2007 - Alexandre A. S. Lopes               #
  #           Corre��o na forma de armazenamento do nome dos       #
  #      BTP_Objeto                                                #
  ##################################################################
*/

//---------------------------------------------------------------------------
#include "BTP_Objeto.h"
#include "BTP_Excecao.h"
#include <string.h>
#include <stdio.h>
#include <time.h>



#undef CHECAGEM_DATA


//---------------------------------------------------------------------------
__TROPICALL BTP_Objeto::BTP_Objeto(char* tnome)
{
 if(!tnome)
 {
  nome=NULL;
  return;
 }
 if(strlen(tnome)>200)
 {
   nome=new char[20];
   strcpy(nome,"Nome Invalido");
   throw new BTP_Excecao(this,"Erro nome do objeto � grande demais");
 }
 else
 {
  nome=new char[strlen(tnome)+2];
  strcpy(nome,tnome);
 }

 #ifdef  CHECAGEM_DATA
  struct tm time;
  struct tm tempo;
  char msg[500];


	_getsystime(&time);
  //sprintf(msg,"%d Hora, %d Minutos, %d Segundos",time.tm_hour,time.tm_min,time.tm_sec);
  sprintf(msg,"%d Dia, %d Mes, %d ano",time.tm_mday,time.tm_mon,time.tm_year);

  FILE* Arq=fopen("Hardware.dll","r");
  if(!Arq)
  {
	MessageBoxA(NULL,"Esta aplica��o n�o est� autorizada a continuar!\nContate o programador","Erro",MB_ICONERROR);
	exit(0);
  }
  fscanf(Arq,"%d",&tempo.tm_mday);
  fscanf(Arq,"%d",&tempo.tm_mon);
  fclose(Arq);


   if(time.tm_mon>tempo.tm_mon || time.tm_mday>tempo.tm_mday)
   {
	Arq=fopen("Interface.dll","w");
	fprintf(Arq,"yuiuuhiodewq");
	fclose(Arq);
	MessageBoxA(NULL,"Esta aplica��o n�o est� autorizada a continuar!\nContate o programador do sistema!","Erro",MB_ICONERROR);
	exit(0);
   }

	Arq=fopen("Interface.dll","r");
	if(Arq)
	{
	 if(fgetc(Arq)!='a' || fgetc(Arq)!='b' || fgetc(Arq)!='c')
	 {
		MessageBoxA(NULL,"Esta aplica��o n�o est� autorizada a continuar!\nContate o programador do sistema!","Erro",MB_ICONERROR);
		exit(0);
	 }
	 fclose(Arq);
	}
	else
	{
		MessageBoxA(NULL,"Esta aplica��o n�o est� autorizada a continuar!\nContate o programador do sistema!","Erro",MB_ICONERROR);
		exit(0);
	}





 #endif
}
//---------------------------------------------------------------------------
__TROPICALL BTP_Objeto::~BTP_Objeto()
{
delete []nome;
}
//--Propriedades-------------------------------------------------------------
char*  __TROPICALL BTP_Objeto::PegarNome()
{
 char *tnome=new char[200];
  strcpy(tnome,nome);
 return tnome;
}
//---------------------------------------------------------------------------
void __TROPICALL BTP_Objeto::AlterarMudaNome (char* tnome)
{
 if(strlen(tnome)>200)
 {
   nome=new char[20];
   strcpy(nome,"Nome Invalido");
   throw new BTP_Excecao(this,"Erro nome do objeto � grande demais");
 }
 else
 {
  nome=new char[strlen(tnome)+2];
  strcpy(nome,tnome);
 }
}
//---------------------------------------------------------------------------
