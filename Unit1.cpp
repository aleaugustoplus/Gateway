//---------------------------------------------------------------------------



//---------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include "BTP_CompletionServer.h"
#include "PLC_Controle.h"


int main(int argc, char* argv[])              
{
   BTP_CompletionServer Servidor("Servidor");
   Servidor.Iniciar();





	printf("\n");
	system("pause");
	return 0;
}
//---------------------------------------------------------------------------
