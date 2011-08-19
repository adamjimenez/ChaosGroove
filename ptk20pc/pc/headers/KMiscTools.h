#ifndef KMISCTOOLS__H
#define KMISCTOOLS__H
#include		<windows.h>

#define		PTK_MAXPATH			1024

#ifndef		MAX_PATH
	#define		MAX_PATH		1024
#endif


//return false if you want to END enumeration  // isFolder==true  if the returned path is a folder
typedef		 bool		( *enumProc ) (  char	*filename , bool isFolder , void *userData  );


#include "KLog.h"

class		KMiscTools
{
private:
	static	LARGE_INTEGER		_unitsPerSecond  ;
	static	LARGE_INTEGER		tstart;
	static	LARGE_INTEGER		tticks;
	static	LARGE_INTEGER		tnow;
	int 							Started;
	static	int 				_HighFreq;

	static	unsigned long		getRawMilliseconds( void ) ;

	static		unsigned long		_millisecondCounter  ;
	static		unsigned long		_starterMSTimer  ;

public:
	static	char						currentFilePath[PTK_MAXPATH] ;
	static  KLog      			_kLog ;

	KMiscTools( ) ;
	~KMiscTools( ) ;
	
	static	void						initMiscTools( void ) ;
	static	char		*				makeFilePath(const char	*filename ) ;	
	static	char		*				getUserFolder( void ) ;
	static	char		*				getCurrentPath( void ) ;
	static	char		*				getPreferencesFolder( void ) ;
	
	static	unsigned	long		getTheTicks( void ) ;
	static	void						messageBox(const char *title, const char *text )  ;
	static	void						pause( long	milliseconds , bool pollevents = false ) ;

	static	unsigned long			getMilliseconds( void ) ;
	static	void						initMillisecond( void ) ;
	static	long						getYear(void);
	static  void          				 logMessage( KLogType kLogType , const char* msg ) ;	

	static	void		launchURL( char	*url ,bool maximised = true) ;
	static	void		launchFile( char	*url ,bool maximised = false ) ;

	static	short		flipShort( short value ) ;	
	static	long		flipLong( long value ) ;
	static	float		flipFloat( float value ) ;
	static	double	flipDouble( double value ) { return value ; } 
	
	static	void		cleanPath( char		*texturePath )  ;
	static	void		enumerateFolder( char 	*folderpath , enumProc		enumerationProcPtr, void *userData = NULL ) ;
	static	char		*getLocale( void ) ;
	static	void		setFileSource( bool vall ) ;    //only used on mac ( to put the data in the bundle )
	//internal ( used to pause the milliseconds when the window is in the background )
	static	void		freezeTimer( void );
	static	bool		createDirectory( const char *folderpath ) ;
	//also works for folders
	static	bool		fileExists( const char *filepath ) ;
	
	
};





#endif // KMISCTOOLS__H
