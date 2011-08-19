
#ifndef		DEF_KBATCHGL

#define		DEF_KBATCHGL


#include	<windows.h>
#include	"KBatch.h"
#include	"KGraphic.h"




class		KBatchGL : public KBatch
{
public:

	KBatchGL( ) ;
	~KBatchGL( ) ;
	
	KGraphic						*_texturePtr ;
	
	bool		allocateBuffer( long polygoncount ) ;
	void		beginBatch( KGraphic	*surface ) ;

	void		endBatch( void ) ;
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	void		blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY , bool flipx =false , bool flipy =false ) ;
	void		blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY, float angle, float zoom, float blend , bool flipx =false , bool flipy =false , float centerX= 0  , float centerY =0 ) ;
	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void		drawLine( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void		freeBatchBuffer( void ) ;		//only call if you want to free the buffer ( not necessary for most users )


};

#endif
