
#ifndef		DEFKSYSTEM

#define		DEFKSYSTEM

#define		CPUTYPE_PC					0
#define		CPUTYPE_MAC					1
#define		CPUTYPE_POCKETPC			2

//KTime struct
struct	KTime
{
unsigned long		hour,minute,second ;
unsigned long		day,month,year ;
unsigned long		milliseconds ;		
};


struct	KOSVersion
{
	short		computerType ; //0 == PC  1 == mac etc..		
	long		majorVersion; 
	long 		minorVersion ; 
	long		buildNumber ; 
	long		platformId; 
	char		miscInfo[256]; 
	char		ptkVersion[32] ;
};

class		KSystem
{
public:

	KSystem( ) ;
	~KSystem( ) ;


	void		getTime( KTime	*ktime ) ;
	void		getOSVersion( KOSVersion *kosver ) ;
	
	//video card
	char*	getVideoCardName( void ) ;
	char		videoCardName[512] ;
	//Audio driver name
	char*	getAudioDriverName( void ) ;
	//RAM installed
	long		memory ;				//total memory in KByte
	unsigned	long		getMemory( void ) ;

};


#endif
