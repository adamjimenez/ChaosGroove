//#include	"KMiscTools.h"

#ifndef	DEFLZMA

#define	DEFLZMA


class		K7Z
{
private:
// 	*outStream == dataptr
	unsigned 	long		_dataSize ;
	unsigned	char		*dataPtr ;
	
	char						_k7zError[256] ;
	void			freeInternalMemory( void ) ;
	bool		 	depackFile( char *filename ) ;
	 void 			*inStream, *lzmaInternalData;
	
public:
	K7Z( ) ;
	~K7Z( ) ;

	//depacks a file to disk ( also frees memory after operation is done )
	bool		depack( char *sourceFileName , char *destFileName ) ;

	//returns a pointer to the data buffer, buffersize contains the size of the data ( pass NULL if you don't want it )
	unsigned	char		*load(  char *fileName , unsigned long *buffersize ) ;

	char		*getLastError( void ) ;
	//release the memory
	void		release( void ) ;
};


#endif
