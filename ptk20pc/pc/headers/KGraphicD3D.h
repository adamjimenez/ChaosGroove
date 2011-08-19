#define	D3D_OVERLOADS
//#define DIRECT3D_VERSION 0x0700

#include	"KGraphic.h"


#ifndef		DEFKGRAPHICD3DA

#define		DEFKGRAPHICD3DA

#include	<windows.h>
#include	"KWindowD3D.h"
#include <ddraw.h>
#include <d3d.h>
#ifdef __MWERKS__
#include	<vector.h>
#else
#include	<vector>
using namespace std;
#endif


class			KGraphicD3D : public KGraphic
{
public:
	float		_zoomX , _zoomY ;
	
	static	float		_texelSX  ;
	static	float		_texelSY  ;
	static	float		_texelDX  ;
	static	float		_texelDY  ;
	bool		_maskedUsed ;

	char			imageFileName[MAX_PATH] ;
	char			imageMaskFileName[MAX_PATH] ;
	
	bool			_currentState  ;
	unsigned	char*		LoadGeneric( const char	*filename ) ;

	unsigned	char*		LoadGenericFromPtr( char	*filename , unsigned char *buffer , unsigned long size ) ;
	bool					loadPicture16Ptr( const char	*filename , unsigned char *dataPtr , unsigned long size , bool hidef , bool hasAlpha ) ;
	bool					loadPicture32Ptr( const char	*filename , unsigned char	*dataPtr  , unsigned long size , bool hidef , bool hasAlpha ) ;
	bool					loadPictureWithMask( const	char	*filename , const	char	*filenameMask , bool hiDef ) ;
	bool					loadPicture2dFromPtr( const char	*filename , unsigned char	*dataPtr , unsigned long datasize , long x, long y )  ;

	D3DBLEND			_sourceAlpha,_destAlpha ;
	static	long						_screenWidth , _screenHeight , _screenDepth ;
	
	bool		reloadSurface( void ) ;

	IDirectDrawSurface7					*_surface ;
	//if a surface is used in 2d mode ( ddraw )
	 bool   _2dSurface ;

	static   LPDIRECT3DVERTEXBUFFER7	   _lpVertexBuffer;
   
   	long	_ckPixelX ;
	long	_ckPixelY ;



	//used for generated buffers
	unsigned long		*_userPixelPtr ;
	long				_userWidth ;
	bool					_userHiDef ;
	D3DTEXTUREMAGFILTER		_textureQualityD3D ;
	D3DTEXTUREADDRESS			_textureWrapD3D ;
	
	D3DTEXTUREMAGFILTER			_oldTextureQualityD3D ;
	D3DTEXTUREADDRESS				_oldTextureWrapD3D ;
	D3DBLEND								_oldsourceAlpha,_olddestAlpha ;


	bool			_hidef 	 ;
	bool			_hasAlpha	;
	static	vector<KGraphicD3D*>		surfaceList ;
	void		deleteD3DSurface( void ) ;

//	vector<KGraphicD3D*>		surfaceList ; //the list of surface  ( required to fix a non working managed surface in directx ) ->alt tab bug
	void			addSurfaceToList( void ) ;
	void			removeSurfaceFromList( void ) ;

	//load 32 bit texture
	bool			loadPicture32( const char	*filename  , const char	*filenameMask , bool hidef , bool hasAlpha ) ;
	//load 16 bit
	bool			loadPicture16( const char	*filename , const char	*filenamemask , bool hidef , bool hasAlpha ) ;
	short			_lineWidth ;
//	float			_ftextureHEIGHTM1 , _ftextureWIDTHM1 ;
	
public:
	static   LPDIRECTDRAW7						_directX ;
	static   LPDIRECTDRAWSURFACE7		_backBuffer  ;

	static   LPDIRECT3DDEVICE7				_d3d ;


	static	IDirectDrawSurface7		*_oldBindedText ;
  	char*  _pixels ;
  	float   _ftextureWIDTH   ;
  	float  _ftextureHEIGHT  ;
  	float  	fimageHeight ;
  	float  	fimageWidth ;
  	
 	bool   _ckActive ;	//color key activated ?
	bool    _textureWrap   ; //for texture wrapping
	bool	_textureQuality ;

	
    unsigned char	_ckr , _ckg , _ckb ;
	
	KGraphicD3D( ) ;
	~KGraphicD3D( ) ;

	

	float			getWidth( void );
	float			getHeight( void ) ;

	//ddraw usage for large or plain picture without effects ( 2 last values are the coordinnates of a color keyed pixel )
	bool			loadPicture2d( const char	*filename , long x=-1, long y=-1 ) ;
	void			blitRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  ) ;
	void			blitAlphaRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  ) ;

	//d3d
	bool			loadPictureFromPtr( const char	*filename ,unsigned char *dataPtr , unsigned long size ,  bool hidef , bool hasAlpha ) ;

	bool			loadPicture(  const	char	*filename , bool hiDef  , bool hasalpha )  ;	
	void			freePicture( void )  ;

	void			blitRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY , bool flipx=false , bool flipy=false ) ;
	void			blitRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, bool flipx=false , bool flipy=false) ;
	void			blitTiledRect(short	destX , short	destY , short		width , short	height ,  float tileFactor ,float angle ) ;

	void			blitAlphaRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, bool flipx = false , bool flipy = false ) ;
	void			blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend , bool flipx =false , bool flipy=false , float centerX = 0, float centerY = 0 )  ;

//blitting in sub pixels
	void			blitRectF( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY , bool flipx = false , bool flipy = false ) ;
	void			blitRectFxF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, float angle, float zoom, bool flipx=false , bool flipy=false) ;
	void			blitAlphaRectF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, bool flipx = false , bool flipy = false ) ;
	void			blitAlphaRectFxF( float	x1, float y1 , float	x2 , float y2 , float 	destX , float	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 )  ;



	void			stretchAlphaRect( float	sx1, float sy1 , float	sx2 , float sy2 , float	dx1, float dy1 , float	dx2 , float dy2, float blend =1, float angle = 0 , bool flipx = false , bool flipy= false  , float centerX= 0 , float centerY = 0)  ;
	
	void 			GenericBlit(long x1, long y1, long x2, long y2, float destX, float destY, bool flipx, bool flipy) ;
	void			genericStretchBlit( float	sx1 , float	sy1 , float	sx2 , float	sy2 , float	dx1 ,float	dy1 , float	dx2 , float	dy2  ) ;
	

   	void	 					setAlphaMode( long	 alphaMode  ) ;
 	void	 					setAlphaMode( long	 srcalphaMode , long	 dstalphaMode  ) ;
	static		void		setDirectX( LPDIRECTDRAW7	dd7 , LPDIRECT3DDEVICE7	d3dPtr ,LPDIRECTDRAWSURFACE7 backbuffer , long width,long	height , long depth ) ;


	void			setTextureQuality(bool	highquality ) ;
	//enable texture wrapping
	void			allowTextureWrap( bool	textWrapping );


	//draw line
	void			drawLine(float x1, float y1, float x2, float y2, float r, float g, float b, float blend) ;
	void			setLineWidth( short	linewidth ) ;

	void			drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void			plotPixel( float	x1, float y1, float r ,float g ,float b ,  float blend ) ;

	void			setColorKey( bool active,unsigned char r , unsigned char g ,  unsigned char b )  ;
	
	void			selectTexture( void )  ;


	bool           		grabBackBuffer(  pixelCallBackPtr	pixelTransformationPtr ) ;
	bool           		grabFrontBuffer( void ) ;
	
	
	//BROKEN		FUNCTIONS  NOT YET WORKING
	void			blitArbitraryQuad( 	float sx1 , float sy1 , float sx2 , float sy2 , float sx3 , float sy3 , float sx4 , float sy4 , 
														float dx1 , float dy1 , float dx2 , float dy2 , float dx3 , float dy3 , float dx4 , float dy4  ) ;

	//if you want to make pictures from buffers  HIDEF == true will use 32 bits textures in memory, else it'll use the current depth
	bool			makePictureFromArray( unsigned long *userPixelPtr , long width , long height , bool hiDef = false ) ;
	//if you want to "refresh" the texture created by makePictureFromArray you can just call poll 
	void			pollArray( void ) ;
	
	//replaces glColor4f  values, range from 0-1
	void			setBlitColor ( float	r , float	g , float	b , float a ) ;


	static	void			setTexelInfo( float sx , float sy , float dx , float dy ) ;

	//force reloads all the graphics
	static	void			forceReloadList( void ) ;	
	static	void			deleteList( void )  ;

	bool			ptk_bindTexture( void ) ;
	void			setStates( void ) ;

	void			setZoom( float zx, float zy );
	
	void		gpuFinish( void ) ;
	bool		grabBackBufferFast(  long  x, long  y, long  w, long  h )  ;
	bool		grabFrontBufferFast( void ) ; 
	
	bool				_blitColorChanged ;
	float				_blitRed ,_blitGreen , _blitBlue , _blitAlpha ;
	float				_blend ;
};

#endif
