//---------------------------------------------------------------------------

#include "BTP_CompletionPort.h"
#include "BTP_Excecao.h"

//---------------------------------------------------------------------------

__TROPICALL BTP_CompletionPort::BTP_CompletionPort(char*nome):BTP_Objeto(nome)
{
	// Cria Completion Port
	handleCompletionPort = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE,
			NULL,	// Completion port existente
			0,		// Chave
			0);		// N�mero de threads = N�mero de processadores
	if(handleCompletionPort == NULL)
	{
		throw new BTP_Excecao("Falha ao criar I/O CompletionPort!");
    }
}
//---------------------------------------------------------------------------
__TROPICALL BTP_CompletionPort::~BTP_CompletionPort()
{

}
//---------------------------------------------------------------------------
void __TROPICALL BTP_CompletionPort::Objeto()
{

}
//------------------------------------------------------------------------------
void BTP_CompletionPort::AssociarArquivo(BTP_PortaSerial* Porta)
{
		// Associa o socket da transa��o � completion port
		// V�rios arquivos (sockets) podem estar associados � mesma completion port
		CreateIoCompletionPort(
				(HANDLE)Porta->hPorta,
				handleCompletionPort,
				(DWORD)Porta->BufferPort,   // No key
				0              // Use default # of threads
			);

}
//--------------------------------------------------------------------------
void BTP_CompletionPort::AssociarArquivo(BTP_Socket* Porta)
{
		// Associa o socket da transa��o � completion port
		// V�rios arquivos (sockets) podem estar associados � mesma completion port
		CreateIoCompletionPort(
				(HANDLE)Porta->Sock,
				handleCompletionPort,
				(DWORD)Porta->BufferPort,   // No key
				0              // Use default # of threads
			);

}
//--------------------------------------------------------------------------
bool BTP_CompletionPort::AssociarThread(LPDWORD lpNumberOfBytesTransferred,
						PULONG_PTR lpCompletionKey,
						LPOVERLAPPED *lpOverlapped,
						unsigned int TimeOut )
{
//  return GetQueuedCompletionStatus(
//			   handleCompletionPort,
//			   &lpNumberOfBytesTransferred,
//			   &(DWORD)lpCompletionKey,
//			   &lpOverlapped,
//			   TimeOut
//			);


			

}
//---------------------------------------------------------------------------
HANDLE __TROPICALL BTP_CompletionPort::PegarHandle()
{
	return handleCompletionPort;
}
//---------------------------------------------------------------------------
