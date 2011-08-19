#ifndef K_LOG__H
#define K_LOG__H

#include <stdio.h>

//---
//   Log type
typedef enum
{
			kInfo		=		1 ,
			kError	=		2 ,
			kFatal	=		3 ,
			kDebug	=		4 
} KLogType ;

//---
//		LogManager type
//			KeepFileOpen when program is bug free.
//			OpenAndCloseFiles when program is in debug mode.
typedef enum
{
			kLogKeepFilesOpen = 1 ,
			kLogOpenAndCloseFiles = 2
} KLogFileType ;

class KLog
{
private:
	bool					_logInfo ;
	FILE		*			_info ;
	
	bool					_logError ;
	FILE		*			_error ;
	
	bool					_logFatal ;
	FILE		*			_fatal;
	
	bool					_logDebug ;
	FILE		*			_debug;
	
	KLogFileType  _logFileType;
	
public:	
	KLog(KLogFileType kLogFileType=kLogOpenAndCloseFiles);
	~KLog() ;
	
	void		reset();
	void		logInfo( bool b ) ;
	void		logError( bool b ) ;
	void		logFatal( bool b ) ;
	void		logDebug( bool b ) ;
	
	void		logMessage( KLogType kLogType , const char* msg ) ;
} ;

#endif // K_LOG__H
