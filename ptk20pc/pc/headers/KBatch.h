
#ifndef		DEF_KBATCH

#define		DEF_KBATCH

#include	"KGraphic.h"

class		KBatch
{
public:

	KBatch( ) {};
	virtual	~KBatch( ) {};
	
	virtual	bool		allocateBuffer( long polygoncount ) =0;
	virtual	void		beginBatch( KGraphic	*surface )=0 ;

	virtual	void		endBatch( void ) =0;
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	virtual	void		blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY ,  bool flipx =false , bool flipy =false ) =0 ;
	virtual	void		blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY, float angle, float zoom, float blend  , bool flipx =false , bool flipy =false , float centerX = 0 , float centerY = 0) = 0 ;
	virtual	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend )=0 ;
	//virtual	void		freeBatchBuffer( void ) =0 ;		//only call if you want to free the buffer ( not necessary for most users )

};

#endif
