#pragma once

#include	"KGraphic.h"
#include	<d3d9.h>
#include "vector"

struct PTKVERTEX
{
	FLOAT	x, y, z;//, rhw; // The transformed position for the vertex (can not be used : incompatibility with world transforms)
	DWORD	color;
	float	u;
	float	v;
};
#define D3DFVF_PTKVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 )





class		KGraphicDX9	: public KGraphic
{

	friend class KBatchDX9;
private:

	bool createTexture( long width, long height ,  long bpp ) ;
	bool loadPicture32( const char	*filename  , const char	*filenameMask , bool hidef , bool hasAlpha ) ;
	unsigned char* LoadGeneric( const char	*filename ) ;

	float _ftextureWIDTH , _ftextureHEIGHT ;
	float fimageWidth , fimageHeight ;
	long _imageHeight , _imageWidth ;
	// project in vertex buffer
	void Project( int nbVertexs, int nbIndices, D3DPRIMITIVETYPE type);

	//blitting in integer
	void PrepareRect( float x1, float y1, float x2, float y2, float destX, float destY, bool flipx = false, bool flipy = false );
	void PrepareFx(float x1, float y1, float x2, float y2, float destX, float destY, float angle, float zoom, float centerX = 0, float centerY = 0);
	void SetStates(bool textured, bool alpha);

public:
	KGraphicDX9() ;
	~KGraphicDX9( ) ;
	bool CreateRenderTarget(int width, int height);
	bool startRenderToTexture();
	bool endRenderToTexture();

	//used to store lost surface etc..
	char _imageMaskFileName[512];
	char _imageFileName[512];
	bool _maskedUsed;
	bool _quality ; //texture quality, false = nearest , true = bilinear
	DWORD _textureQualityD3D; // nearest ou bilinear
	bool _ckActive; // color key
	unsigned char _ckr, _ckg, _ckb;

	bool _textureWrap; //for texture wrapping
	D3DTEXTUREADDRESS _textureWrapD3D;

	bool _blitColorChanged;
	float _blitRed, _blitGreen, _blitBlue, _blitAlpha;
	float _blend;

	float _zoomX, _zoomY;

	D3DBLEND _sourceAlpha, _destAlpha;

	bool _hidef;
	bool _hasAlpha;

	int _targetWidth, _targetHeight;
	IDirect3DTexture9 * _pTargetTexture;
	IDirect3DSurface9* _pTargetSurface;

	IDirect3DTexture9 *_texture, *_oldTexture;
	IDirect3DVertexBuffer9 *_vertexBuffer;
	PTKVERTEX _ptkVertex[4];
	bool createBuffer();
	bool releaseBuffer();


	static LPDIRECT3D9            _d3d9Ptr	 ; //the D3DDevice
	static LPDIRECT3DDEVICE9      _d3d9Device  ; //  rendering device

	static void setDirectX( LPDIRECT3D9  dx9Ptr , LPDIRECT3DDEVICE9  dx9Device ) ;

	static std::vector<KGraphicDX9*> currents;

	static void releaseCurrents();
	static void initCurrents();

	//direct draw blitting ( useful for large backgrounds that won't use any effects )
	bool loadPicture2d( const char	*filename , long x = -1, long y = -1 ) ;
	void blitRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  )  ;
	bool loadPicture2dFromPtr( const char	*filename , unsigned char	*dataPtr , unsigned long datasize , long tx, long ty )   ;
	void blitAlphaRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  ) ;


	//3d stuff
	bool loadPictureFromPtr( const	char	*filename , unsigned char	*dataPtr , unsigned long datasize , bool hiDef , bool hasalpha )  ;

	bool loadPictureWithMask( const	char	*filename , const	char	*filenameMask , bool hiDef ) ;

	bool loadPicture(  const	char	*filename , bool hiDef , bool hasalpha )  ;	
	void freePicture( void ) ;
	float getWidth( void ) ; 
	float getHeight( void );
	void setTextureQuality( bool quality ) ;

	void drawRect( float x1, float y1, float x2, float y2, float r, float g, float b, float blend );
	void drawLine( float x1, float y1, float x2, float y2, float r, float g, float b, float blend );
	void plotPixel( float x1, float y1, float r, float g, float b, float blend );

	void blitRect( float x1, float y1, float x2, float y2, short destX, short destY, bool flipx = false , bool flipy = false );
	void blitRectFx( float x1, float y1, float x2, float y2, short destX, short	destY, float angle, float zoom, bool flipx = false, bool flipy = false );

	void blitAlphaRect( float x1, float y1, float x2, float y2, short destX, short destY, bool flipx = false, bool flipy = false );
	void blitAlphaRectFx( float	x1, float y1, float	x2, float y2, short	destX, short destY, float angle, float zoom, float blend, bool flipx = false, bool flipy = false, float centerX = 0, float centerY = 0 );

	//blitting in sub pixels
	void blitRectF( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY , bool flipx = false , bool flipy = false ) ;
	void blitRectFxF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, float angle, float zoom, bool flipx=false , bool flipy=false) ;
	void blitAlphaRectF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, bool flipx = false , bool flipy = false ) ;
	void blitAlphaRectFxF( float	x1, float y1 , float	x2 , float y2 , float 	destX , float	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 ) ;

	void blitTiledRect(short	destX , short	destY , short		width , short	height ,  float tileFactor ,float angle  ) ;
	//arbitraty blits
	void blitArbitraryQuad( 	float sx1 , float sy1 , float sx2 , float sy2 , float sx3 , float sy3 , float sx4 , float sy4 , 
		float dx1 , float dy1 , float dx2 , float dy2 , float dx3 , float dy3 , float dx4 , float dy4  ) ;

	//alpha additif ou normal ? ( false = noir transparent + ajout des couleurs )
	void setAlphaMode( long	alphamode );
	void setAlphaMode( long	alphamodeSource, long alphamodeDest );

	//stretching
	void stretchAlphaRect( float	sx1, float sy1 , float	sx2 , float sy2 , float	dx1, float dy1 , float	dx2 , float dy2, float blend = 1.0f, float angle = 0.0f , bool flipx = false , bool flipy= false  , float centerX= 0 , float centerY = 0) ;
	//recupere le buffer et le mets dans une texture	
	bool grabBackBuffer( pixelCallBackPtr		pixelFunc = NULL ) ;
	bool grabFrontBuffer( void ) ;

	//color key ( generates an alpha channel from a color key, must be called before loading the bitmap
	void setColorKey( bool active,unsigned char r , unsigned char g ,  unsigned char b ) ;
	void allowTextureWrap( bool textureWrap) ; //allow texture wrapping ( only works with square textures )


	//regle la couleur de blit
	void setBlitColor( float r, float g, float b, float a );

	//if you want to make pictures from buffers  HIDEF == true will use 32 bits textures in memory, else it'll use the current depth
	bool makePictureFromArray( unsigned long *userPixelPtr , long width , long height , bool hiDef = false ) ;
	//if you want to "refresh" the texture created by makePictureFromArray you can just call poll 
	void pollArray( void );

	void gpuFinish( void ) ;
	bool grabBackBufferFast(  long  x, long  y, long  w, long  h )   ;
	bool grabFrontBufferFast( void ) ; 

	//used by K3D ( does a glBind )
	void selectTexture( void ) ;
	void setZoom( float zx, float zy )  ;

	void bindTexture( void ) ;
};


