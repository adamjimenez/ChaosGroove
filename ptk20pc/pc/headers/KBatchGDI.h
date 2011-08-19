
#ifndef		DEF_KBATCHGDI
#define		DEF_KBATCHGDI


#include	<windows.h>
#include	"KBatch.h"
#include	"KGraphic.h"




class		KBatchGDI : public KBatch
{
public:

	KBatchGDI( ) ;
	~KBatchGDI( ) ;
	
	KGraphic						*_texturePtr ;
	
	bool		allocateBuffer( long polygoncount ) ;
	void		beginBatch( KGraphic	*surface ) ;

	void		endBatch( void ) ;
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	void		blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY , bool flipx =false , bool flipy =false ) ;
	void		blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY, float angle, float zoom, float blend , bool flipx =false , bool flipy =false ) ;


};

#endif
