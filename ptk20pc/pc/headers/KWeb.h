#include	<windows.h>
#include	<Wininet.h>


#ifndef		DEF_KWEB

#define		DEF_KWEB		

class		KWeb
{
private:
	HINTERNET		hInternet ;
	HINTERNET		hIurl ;
	char					*_dataBuffer ;
	unsigned long		_dataBufferSize	;
	
	char*	openURL( char	*urlStr , bool 	asynchronousFlags )  ;
	void		freeInternetHandles( void ) ;
	void		freeDataBuffer( void ) ;
		
public:
	KWeb( ) ;
	~KWeb( ) ;
	
	char*		callURL( char	*urlStr , bool 	asynchronousFlags ) ;

	unsigned long		getBufferSize( void ) ;
	
};

#endif
