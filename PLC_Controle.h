/*##################################################################
  #                Objeto Controle de PLC                          #
  #               Copyright�:                                      #
  #                Noname� Corp.                                   #
  #----------------------------------------------------------------#
  #               Criada em:                                       #
  #               24/06/08 - Alexandre A. S. lopes                 #
  #               Descri��o:                                       #
  #                Classe de controle de PLC                       #
  ##################################################################*/

//---------------------------------------------------------------------------

#ifndef PLC_ControleH
#define PLC_ControleH


#include "BTP_Thread.h"
#include "BTP_PortaSerial.h"
#include "BTP_CompletionPort.h"


#define NUM_SAIDAS_D   5
#define NUM_ENTRADAS_D 4
#define NUM_ENTRADAS_A 4
#define NUM_SAIDAS_A   4


class PLC_Controle: public BTP_Thread
{
 public:
	enum PLC_CTRL_MSG{
					   PLC_BTP_EXCECAO = 10000,
					   PLC_EXCECAO =     10001,
					   PLC_MUDOU_ENT = 10003,
					   PLC_MUDOU_SAIDA = 10004,
					   PLC_CONECTADO = 10005,
					   PLC_DESCONECTADO = 10006,


	};



 private:
 typedef struct _StatusPortas {
	 bool Saidas[NUM_SAIDAS_D];
	 bool Entradas[NUM_ENTRADAS_D];
	 unsigned char SaidasA[NUM_SAIDAS_A];
	 unsigned char EntradasA[NUM_ENTRADAS_A];

}StatusPortas;

 StatusPortas Portas;
 bool statusplc;

 BTP_Excecao * excecao;
 BTP_PortaSerial *PortaCom;
 BTP_CompletionPort * CompletionPort;
 DWORD threadmsg;
 HWND  janelamsg;


 unsigned short numsaidasdigitais,numentradasdigitais;
 unsigned short numsaidasanalogicas,numentradasanalogicas;

 void __TROPICALL Executar();
 void __TROPICALL AtualizarEntradasD(unsigned int BufferPos);
 void __TROPICALL AtualizarSaidasD(unsigned int BufferPos);
  void __TROPICALL AtualizarEntradasA(unsigned int BufferPos);
 void __TROPICALL AtualizarSaidasA(unsigned int BufferPos);

 void __TROPICALL EntregarMsg(unsigned int);

 public:

 __TROPICALL PLC_Controle(char*);
 __TROPICALL ~PLC_Controle();

 __property unsigned short NumSaidasDigitais = {read = PegarNumSaidasDigitais};
 __property bool SaidasDigitais [unsigned short Num] = {read = PegarSaidasDigitais , write =
								   AlterarSaidasDigitais};
 __property unsigned short NumSaidasAnalogicas = {read = PegarNumSaidasAnalogicas};

 __property unsigned char SaidasAnalogicas [unsigned short Num] = {read = PegarSaidasAnalogicas , write =
								   AlterarSaidasanalogicas};


 __property unsigned short NumEntradasDigitais = {read = PegarNumEntradasDigitais};

 __property bool EntradasDigitais [unsigned short Num] = {read = PegarEntradasDigitais};

 __property unsigned short NumEntradasAnalogicas = {read = PegarNumEntradasAnalogicas};

 __property unsigned char EntradasAnalogicas [unsigned short Num] = {read = PegarEntradasAnalogicas};

 __property BTP_Excecao * Excecao = {read = PegarExcecao};
 __property DWORD ThreadMsg = {read = PegarThreadMsg , write = AlterarThreadMsg };
 __property HWND  JanelaMsg = {read = PegarJanelaMsg , write = AlterarJanelaMsg};
 __property bool StatusPlc = {read = PegarSatusPlc};


 //--Propriedades--------------------------
 private:

 unsigned short __TROPICALL PegarNumSaidasDigitais();
 bool __TROPICALL PegarSaidasDigitais(unsigned short Num);
 void __TROPICALL AlterarSaidasDigitais(unsigned short Num,bool);

  unsigned short __TROPICALL PegarNumSaidasAnalogicas();


 unsigned char __TROPICALL PegarSaidasAnalogicas(unsigned short Num);
 void __TROPICALL AlterarSaidasanalogicas(unsigned short Num, unsigned char);


 unsigned short __TROPICALL PegarNumEntradasDigitais();
 bool __TROPICALL PegarEntradasDigitais(unsigned short Num);
 unsigned short __TROPICALL PegarNumEntradasAnalogicas();

 unsigned char __TROPICALL PegarEntradasAnalogicas(unsigned short Num);






 BTP_Excecao * __TROPICALL PegarExcecao();

 DWORD __TROPICALL PegarThreadMsg();
 void __TROPICALL AlterarThreadMsg(DWORD);
 HWND __TROPICALL PegarJanelaMsg();
 void __TROPICALL AlterarJanelaMsg(HWND);
 bool __TROPICALL PegarSatusPlc();
 char *__TROPICALL PegarMsg();

 };
//---------------------------------------------------------------------------
#endif
