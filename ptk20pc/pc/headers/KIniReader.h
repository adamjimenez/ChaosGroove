#include	<stdio.h>
#include	<string.h>


#ifndef 	DEF_INIREADER

#define 	DEF_INIREADER


class		KIniReader
{
private:
	char		iniFilename[261] ;
	char		*memIni ;
	long		memIniLen ;
public:
	KIniReader( ) ;
	~KIniReader( ) ;

	//set an ini from a file
	bool	setIni( char	*iniFile) ;
	//set an ini from a memory location
	bool	setIni( char	*iniFile , long datalength) ;
	long	getValue( char	*section , char 	*variable , long defaultValue = 0 ) ;
	bool	getString( char	*section , char 	*variable, char	*buffer,long	bufferlen ) ;

};


#endif
