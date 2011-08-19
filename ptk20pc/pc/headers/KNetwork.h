#ifndef K__NETWORK__H
#define K__NETWORK__H

#include 	"openPlay.h"
#include	"KDataItem.h"

#include "KDataExchange.h"

//---
//   KNetwork header, by ptrognon(ptrognon@free.fr), for Phelios.
//
//		Retour au FR_fr pour les explications de dev.
//		Dans la fonction qui passe la class en mode server, il manque le system de callback
//		pour accepter ou refuser une demande de connection.
//		Soit cette callback est passée en paramètre de la fonction serverMode(), soit elle
//		est interne a la class, pour l'instant je ne sais pas, a voir en codant ce qui est le
//		mieux, les 2 solutions se defendent.
//		J'ai donc laissé serverMode() sans parametre, a completer.
//---

typedef enum
{
	K_TCP		=		1 ,
	K_APL		=		2
} KProtocolType ;

const NMUInt32  kAppleTalk_ProtoTag  = 'Atlk';
const NMUInt32  kTCP_ProtoTag        = 'Inet';

void OpenPlay_Callback( PEndpointRef inEndpoint, void* callbackData, NMCallbackCode inCode, NMErr inError, void* inCookie )	;

class KNetwork
{
protected:
	int									_gameId					;		// game ID
	char								_gameName[256]	;		// game name.
	KProtocolType				_kProtocolType  ;		// K_TCP(Tcp/Ip) K_APL(AppleTalk)
	PConfigRef					_configuration  ;		// internal OpenPlay Configuration.
	
	long								_packetSize			;		// Packet Size. (sizeof(struct)), ONLY for stream messages.

	long								_timeout				;		// network timeout, default to 10 secs.

	PEndpointRef				_pClient				;		// Server : sender Endpoint
	PEndpointRef				_pSocket				;		// Client : EndPoint sender/receiver.
																					// Server : listen EndPoint

	bool								_bInitOk				;		// true if all init are ok, false else, _error is set with the error message.
	char								_error[256] 		;		// error message.
	
	int									_hostsCount			;		// game servers hosts count.
	NMEnumerationItem		_hosts[ 100 ]		;		// game servers hosts array.
	
	char								_status[ 1024 ] ;

	KDataExchange			*	_pkDataExchange	;
	
	bool								_bServerMode 		;		// true if server, false if client.
	
public:
	//---
	//   Constructor
	//		packetsize is used only for stream messages.
	KNetwork(KProtocolType kProtocolType, int gameId, const char *pcGameName, const char *hostName, int port, bool bServerMode, long packetSize, int stackSize = 1024 );

	//---
	//   free all OpenPlay structures.
	~KNetwork();

	//---
	//   close the endpoint.
	void				disconnect( ) ;

	bool				isServer();
	bool				isClientConnected( );
	bool 				isServerConnected( );

	//---
	//		Call this functions to send/recv datagram message.
	//		Be carful, there is no receive confirmation with datagrams.
	bool				sendDatagramMessage( short messageLen, const char * bytes , char *errorMsg = NULL ) ;
	bool				recvDatagramMessage( char *errorMsg = NULL ) ;


	//---
	//		Call this function to send/recv Stream message.
	bool				sendStreamMessage( const char * bytes , char *errorMsg = NULL ) ;
	bool				recvStreamMessage( char *errorMsg = NULL ) ;
	
		
	void				setTimetout( long timeout ) ;
	long				getTimeout() ;

	bool				isInitOk() ;
	const char* getErrorMessage();
	
	void				displayStatus();
	
	void				accept( void* inCookie ) ;

	bool				getLastNetworkMessageType( KPacketType *pkPacketType ) ;
	void				getLastNetworkMessage( KDataItem & rDataItem );

	//******************************
	//KNetwork pour les NULS :
	//******************************
	bool						getTCPData( void*	buffer );
	bool						getUDPData( char*	buffer , unsigned long	buffersize);
	unsigned long		getUDPDataSize( void );
	//fin pk
	
	static		bool	getIpAddress( char *ipAddress ) ;
	
protected	:
	void				setClientEndPoint( PEndpointRef pEndPoint ) ;
	void				endPointDied( PEndpointRef deadEndPoint ) ;

	friend void OpenPlay_Callback( PEndpointRef inEndpoint, void* callbackData, NMCallbackCode inCode, NMErr inError, void* inCookie )	;
	friend void Enumeration_Callback( void *callbackData, NMEnumerationCommand inCommand, NMEnumerationItem *item ) ;

} ;

#endif // K__NETWORK__H
