
#ifndef		DEF_KBATCHD3D

#define		DEF_KBATCHD3D

#define	D3D_OVERLOADS
#define DIRECT3D_VERSION 0x0700

#include	<windows.h>
#include	"KGraphicD3D.h"
#include	"KWindowD3D.h"
#include	"KBatch.h"




class		KBatchD3D : public KBatch
{
private:
	static	float		_texelSX  ;
	static	float		_texelSY  ;
	static	float		_texelDX  ;
	static	float		_texelDY  ;
public:

	KBatchD3D( ) ;
	~KBatchD3D( ) ;
	
//	IDirect3DVertexBuffer7 *_vertexBuffer ;
	//once we lock the vertex buffer data:
 
	KGraphicD3D						*_texturePtr ;
	long									_maxVertice , _verticeCount ;
	IDirect3DVertexBuffer7				*_d3dlvertexBuffer ;
	D3DMATRIX 					_mat3;

	bool		allocateBuffer( long polygoncount ) ;
	void		beginBatch( KGraphic	*surface ) ;

	void		endBatch( void ) ;
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	void		blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY , bool flipx , bool flipy)  ;
	void		blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY, float angle, float zoom, float blend  , bool flipx , bool flipy , float centerX = 0 , float centerY = 0 )  ;
	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;

	void		freeBatchBuffer( void ) ;		//only call if you want to free the buffer ( not necessary for most users )

//internal stuff
  	static	void		setDirectX( LPDIRECT3D7 d3d7ptr , LPDIRECT3DDEVICE7 d3device ) ;
	static	LPDIRECT3D7							_direct3d7 ;
	static	LPDIRECT3DDEVICE7				_d3dDevice ;

	static	void			setTexelInfo( float sx , float sy , float dx , float dy ) ;

	//force reloads all the graphics
	static	vector<KBatchD3D*>		batchList ;
	static	void			forceReloadList( void );	
	static	void			deleteList( void );
	void			addToList( void );
	void			removeFromList( void );

};

#endif
