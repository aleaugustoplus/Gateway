//---------------------------------------------------------------------------



#include "BTP_Socket.h"
//---------------------------------------------------------------------------
BTP_Socket::BTP_Socket(char*nome,SOCKET temp):BTP_Objeto(nome)
{
	Sock=temp;
	// Cria evento com Reset Manual
	ovOut.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	// Seta flag no evento para que um packat não seja colocado
	// na fila da completion port quando uma operação de escrita
	// for completada
	ovOut.hEvent = (HANDLE)((DWORD)ovOut.hEvent | 0x1);
}
//---------------------------------------------------------------------------
BTP_Socket::~BTP_Socket()
{

}
//---------------------------------------------------------------------------
void __TROPICALL BTP_Socket::Objeto()
{

}
//---------------------------------------------------------------------------
void BTP_Socket::Ler()
{
  	int     numRead;
		// Solicita leitura com overlap de um caracter
		 ReadFile(
					(HANDLE)Sock,  // socket
					InBuffer,
					(DWORD)1,
					(LPDWORD)&numRead,
					&ovIn // Ponteiro para estrutura overlap	+ apêndices
				);

}


