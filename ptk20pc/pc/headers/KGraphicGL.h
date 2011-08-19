#ifndef KGRAPHICGL__H
#define KGRAPHICGL__H

#include		<windows.h>
#include		<glu.h>
#include	<olectl.h>
#include 	<ocidl.h>
#include 	<crtdbg.h>
#include	"KGraphic.h"


class		KGraphicGL : public KGraphic
{
public:			
	//used for transparent pixel ( for loadPicture2d ) 
  short  _transPixX ,   _transPixY  ;
	float		_zoomX , _zoomY ;
	
	static	void		setRectangleTexSupport( bool rsupport ) ;
	static	bool	_rectangleTexCap ;

	bool		_blitColorChanged ;
	float		_blitRed , _blitGreen , _blitBlue , _blitAlpha ;

//	unsigned	char*		LoadTGA( char		*filename ) ;
//	unsigned	char*		LoadBMP( char	*filename ) ;

	//direct draw blitting ( useful for large backgrounds that won't use any effects ) not used in GL ( it calls regular fonctions behind the scene )
	bool			loadPicture2d( const char	*filename , long x, long y )  ;
	bool			loadPicture2dFromPtr( const char	*filename , unsigned char	*dataPtr , unsigned long datasize , long x, long y )  ;
	


	void			blitRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  )  ;
	void			blitAlphaRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  )  ;

	bool					loadPictureWithMask( const	char	*filename , const	char	*filenameMask , bool hiDef ) ;

	unsigned	char*		LoadGenericFromPtr( char	*filename , unsigned char *buffer , unsigned long size ) ;
	bool							loadPictureFromPtr( const	char	*filename , unsigned char	*dataPtr , unsigned long datasize , bool hiDef , bool hasalpha ) ;
	
	void			GenericBlit(  float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY , bool flipx , bool flipy )  ;
	void			GenericBlitF(  float	x1, float y1 , float	x2 , float y2 , float destX , float destY , bool flipx , bool flipy )  ;
	void			genericArbitraryBlit( 	float	sx1 , float	sy1 , float	sx2 , float	sy2 , float	sx3 ,float	sy3 , float	sx4 , float	sy4 , 
																		float	dx1 , float	dy1 , float	dx2 , float	dy2 , float	dx3 ,float	dy3 , float	dx4 , float	dy4 )	;

	static		short			screenWidth , screenHeight ,screenDepth ;	
	static		long			videoMemory	;		//vram used	
	static		bool		_graphicQuality  ;

	GLuint		texture ;
	static		GLint 		_oldBindedText ;
	void			ptk_glBindTexture(void  )  ;
	
	char			filename[MAX_PATH] ;		//sauvegarde le nom de fichier de l'image

	unsigned 	char			*_pixels ;

	unsigned short			_sourceAlpha  , 	_destAlpha ;
	
	//load PC jpeg, bmp, gif, wmf
	unsigned	char*	LoadGeneric( char	*filename ) ;
	unsigned	char*	RenderPicture( LPPICTURE 		picture  )  ;
	float			_ftextureWIDTH , _ftextureHEIGHT ;
	GLenum			_glEnum ;
	bool				_textureWrap ; //allow texture wrapping ( default is false )

	//color key
	bool					_ckActive ;
	unsigned char 	_ckr , _ckg , _ckb ;
	float					_centerX , _centerY ;

	float			picX1,picY1 ;								//storage for the coordinates of the bitmap
	float			 fimageWidth,fimageHeight ;  						//size of the square containing the texture
		
	KGraphicGL( ) ;
	~KGraphicGL( ) ;
		
	bool			loadPicture(  const	char	*filename , bool hiDef = true , bool hasalpha  = K_ALPHA )  ;	
	bool			loadPicture3D( const	char	*filename , bool hiDef = true , bool hasalpha  = K_ALPHA )  ;

	void			freePicture( void ) ;
	float			getWidth( void ) ; 
	float			getHeight( void ) ;

	void			setTextureQuality( bool quality ) ;
	
//drawing functions	
	//non static version ( setAlphaMode compliant )
	void			drawAlphaRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend );

	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void		drawLine( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) ;
	void		plotPixel( float	x1, float y1, float r ,float g ,float b ,  float blend ) ;
//blitting
	void			blitRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY , bool flipx = false , bool flipy = false ) ;
	void			blitRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, bool flipx=false , bool flipy=false) ;
	void			blitAlphaRect( void );
	void			blitAlphaRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, bool flipx = false , bool flipy = false ) ;
	void			blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 ) ;
	void			blitTiledRect(short	destX , short	destY , short		width , short	height ,  float tileFactor ,float angle  ) ;
	void			blitAlphaRectFxCenterAdjust( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend ) ;

//blitting in sub pixels
	void			blitRectF( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY , bool flipx = false , bool flipy = false ) ;
	void			blitRectFxF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, float angle, float zoom, bool flipx=false , bool flipy=false) ;
	void			blitAlphaRectF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, bool flipx = false , bool flipy = false ) ;
	void			blitAlphaRectFxF( float	x1, float y1 , float	x2 , float y2 , float 	destX , float	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 )  ;


	//arbitraty blits
	void			blitArbitraryQuad( 	float sx1 , float sy1 , float sx2 , float sy2 , float sx3 , float sy3 , float sx4 , float sy4 , 
														float dx1 , float dy1 , float dx2 , float dy2 , float dx3 , float dy3 , float dx4 , float dy4  ) ;

	//alpha additif ou normal ? ( false = noir transparent + ajout des couleurs )
	void			setAlphaMode( long 	alphamode ) ;
	//sets any value as you like
	void			setAlphaMode( long	alphamodeSource, long alphamodeDest ) ;

	//stretching
	void			genericStretchBlit( float	sx1 , float	 sy1 , float	 sx2 , float	 sy2 ,float	 dx1 ,float	 dy1 , float	 dx2 ,float	 dy2  , bool flipx,bool flipy ) ;
	void			stretchAlphaRect( float	sx1, float sy1 , float	sx2 , float sy2 , float	dx1, float dy1 , float	dx2 , float dy2, float blend = 1.0f, float angle = 0.0f , bool flipx = false , bool flipy= false  , float centerX= 0 , float centerY = 0) ;

//recupere le buffer et le mets dans une texture	
	bool			grabBackBuffer( pixelCallBackPtr		pixelFunc ) ;
	bool			grabFrontBuffer( void ) ;

//color key ( generates an alpha channel from a color key, must be called before loading the bitmap
	void			setColorKey( bool active,unsigned char r , unsigned char g ,  unsigned char b ) ;
	void			allowTextureWrap( bool textureWrap) ; //allow texture wrapping ( only works with square textures )
	
	
	
	//unused in gl
	void			setTexelInfo( float sx , float sy , float dx , float dy )  ;
	
	//important
	static		void	setScreenSize(  short	swidth,  short	sheight , short sdepth )  ;
	static		void	blitBackToBack( float	x1, float y1 , float	x2 , float y2 , float dx,float dy ) ;

	void			setBlitColor( float r , float g , float b , float a ) ;
	
	//used for generated buffers
	unsigned long		*_userPixelPtr ;
	bool					_userHiDef ;
	//if you want to make pictures from buffers  HIDEF == true will use 32 bits textures in memory, else it'll use the current depth
	bool			makePictureFromArray( unsigned long *userPixelPtr , long width , long height , bool hiDef = false ) ;
	//if you want to "refresh" the texture created by makePictureFromArray you can just call poll 
	void			pollArray( void ) ;

	void			setZoom( float zx, float zy ) ;

	void		gpuFinish( void ) ;
	bool		grabBackBufferFast(  long  x, long  y, long  w, long  h )  ;
	bool		grabFrontBufferFast( void ) ; 
	
	//used by K3D ( does a glBind )
	void			selectTexture( void ) ;
	
};	

#endif // KGRAPHIC__H
