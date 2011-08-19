#ifndef KGRAPHICNEUTRAL__H
#define KGRAPHICNEUTRAL__H

#include		<windows.h>


#define		K_NOALPHA		false
#define		K_ALPHA			true

#define K_IMAGE_BMP 0
#define K_IMAGE_JPG 1
#define K_IMAGE_TGA 2
#define K_IMAGE_PNG 3


//structure qui contient les informations sur la bitmap
struct	tgaBitmap
{
	unsigned	char			idField ;
	unsigned	char			colorMapType ;
	unsigned	char			imageType ;
	
	unsigned	short			colorMapSpec1 ; //offset 3
	unsigned	char			colr ; //o 5
//	short			xstart ; // o 7	
	unsigned	short			xstart ;             // image x origin
    //short 		ystart ;
    unsigned 	short 		ystart;
    
	unsigned 	short			width ;	
	unsigned 	short			height ;	
	unsigned	char			bpp ;
	unsigned	char			imageDescriptor ;
};



typedef		 void		( *pixelCallBackPtr ) (  long x , long y , long  *r, long  *g, long  *b);


class		KGraphic
{
public:			

	KGraphic() {};
	virtual	~KGraphic() {};

	//direct draw blitting ( useful for large backgrounds that won't use any effects )
	virtual	bool			loadPicture2d( const char	*filename , long x = -1, long y = -1 ) = 0;
	virtual	void			blitRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  ) = 0 ;
	virtual	void			blitAlphaRect2d( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY  ) = 0 ;


	//3d stuff
	virtual	bool			loadPictureFromPtr( const	char	*filename , unsigned char	*dataPtr , unsigned long datasize , bool hiDef , bool hasalpha ) = 0 ;
	virtual	bool			loadPicture2dFromPtr( const char	*filename , unsigned char	*dataPtr , unsigned long datasize , long tx, long ty )  = 0 ;

	virtual	bool			loadPictureWithMask( const	char	*filename , const	char	*filenameMask , bool hiDef ) = 0 ;

	virtual	bool			loadPicture(  const	char	*filename , bool hiDef , bool hasalpha )  =0;	
	virtual	void			freePicture( void ) =0;
	virtual	float			getWidth( void ) =0; 
	virtual	float			getHeight( void )=0 ;
	virtual	void			setTextureQuality( bool quality ) =0;
	

	virtual	void		drawRect( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) =0;
	virtual	void		drawLine( float	x1, float y1 , float	x2 , float y2 , float r ,float g ,float b ,  float blend ) =0;
	virtual	void		plotPixel( float	x1, float y1, float r ,float g ,float b ,  float blend ) =0;

//blitting in integer
	virtual	void			blitRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY , bool flipx = false , bool flipy = false ) =0;
	virtual	void			blitRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, bool flipx=false , bool flipy=false) =0;
	virtual	void			blitAlphaRect( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, bool flipx = false , bool flipy = false ) =0;
	virtual	void			blitAlphaRectFx( float	x1, float y1 , float	x2 , float y2 , short	destX , short	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 )  =0;

//blitting in sub pixels
	virtual	void			blitRectF( float	x1, float y1 , float	x2 , float y2 , float	destX , float	destY , bool flipx = false , bool flipy = false ) =0;
	virtual	void			blitRectFxF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, float angle, float zoom, bool flipx=false , bool flipy=false) =0;
	virtual	void			blitAlphaRectF( float	x1, float y1 , float	x2 , float y2 , float destX , float	destY, bool flipx = false , bool flipy = false ) =0;
	virtual	void			blitAlphaRectFxF( float	x1, float y1 , float	x2 , float y2 , float 	destX , float	destY, float angle, float zoom, float blend , bool flipx = false , bool flipy=false , float centerX = 0, float centerY = 0 )  =0;



	virtual	void			blitTiledRect(short	destX , short	destY , short		width , short	height ,  float tileFactor ,float angle  ) =0;
	//arbitraty blits
	virtual	void			blitArbitraryQuad( 	float sx1 , float sy1 , float sx2 , float sy2 , float sx3 , float sy3 , float sx4 , float sy4 , 
														float dx1 , float dy1 , float dx2 , float dy2 , float dx3 , float dy3 , float dx4 , float dy4  ) =0;

	//alpha additif ou normal ? ( false = noir transparent + ajout des couleurs )
	virtual	void			setAlphaMode( long	alphamode ) =0;
	virtual	void			setAlphaMode( long	alphamodeSource, long alphamodeDest ) = 0 ;

	//stretching
	virtual	void			stretchAlphaRect( float	sx1, float sy1 , float	sx2 , float sy2 , float	dx1, float dy1 , float	dx2 , float dy2, float blend = 1.0f, float angle = 0.0f , bool flipx = false , bool flipy= false  , float centerX= 0 , float centerY = 0)  =0;
//recupere le buffer et le mets dans une texture	
	virtual	bool			grabBackBuffer( pixelCallBackPtr		pixelFunc = NULL ) =0;
	virtual	bool			grabFrontBuffer( void ) =0;

//color key ( generates an alpha channel from a color key, must be called before loading the bitmap
	virtual	void			setColorKey( bool active,unsigned char r , unsigned char g ,  unsigned char b ) =0;
	virtual	void			allowTextureWrap( bool textureWrap)=0 ; //allow texture wrapping ( only works with square textures )
	

	//regle la couleur de blit
	virtual	void			setBlitColor( float r , float g , float b , float a ) =0;
	
	//used for generated buffers
	unsigned long		*_userPixelPtr ;
	bool					_userHiDef ;
	//if you want to make pictures from buffers  HIDEF == true will use 32 bits textures in memory, else it'll use the current depth
	virtual	bool			makePictureFromArray( unsigned long *userPixelPtr , long width , long height , bool hiDef = false ) =0;
	//if you want to "refresh" the texture created by makePictureFromArray you can just call poll 
	virtual	void			pollArray( void ) =0;

	virtual	void		gpuFinish( void ) = 0 ;
	virtual	bool		grabBackBufferFast(  long  x, long  y, long  w, long  h ) = 0  ;
	virtual	bool		grabFrontBufferFast( void ) = 0 ; 
	
	//used by K3D ( does a glBind )
	virtual	void			selectTexture( void ) =0;
	virtual	void			setZoom( float zx, float zy ) = 0 ;
	
	char		_usedForTTF ;			//when a lost surface was used for truetype we need to know in order to regenerate it.   0 = no/  1 = yes/ 2 = needs refresh

};	

#endif //KGRAPHICNEUTRAL__H
