#include	<ddraw.h>
#include	<d3d.h>

#include	"KGraphic.h"
#include	"KGraphicD3D.h"
#include	"K3D.h"


#ifndef		DEF_K3Dd3d


#define	DEF_K3Dd3d


class		K3Dd3d : public K3D
{
private:
	KGraphicD3D			*surf_texture ;
	bool						_textureLoaded ;
	static	float			_screenWidth ;
	static	float			_screenHeight ;
   static 	LPDIRECT3DDEVICE7			_lpD3DDevice ;
	char						_textureFileName[MAX_PATH] ;
	bool						_blend 			 ;
	float						_blendFactor 	 ;

	float		 _scaleX , _scaleY , _scaleZ ;
	static	bool		_useScreenRes ;
	

	EShadeMode		_eShadeMode ;

   D3DVERTEX *_lpVertexFlat;
   D3DVERTEX *_lpVertexGouraud;

	
	unsigned 	long		findChunk( char *chunkName , char *dataPtr , long startposition  , long bufferlen ) ; 
	unsigned	long		extractLongValue ( char *chunkName , char *dataPtr , long startposition  , long bufferlen )	;
	unsigned	long		extractNextFloatValue ( char *dataPtr , long startposition  , long bufferlen , float	*floatvalue)	;
	unsigned long			extractNextLongValue ( char *dataPtr , long startposition  , long bufferlen , unsigned long	*longValue ) ;
	void						extractTextureTable( char	*buffer , long	bufferLength );
	void						makeGouraud( void ) ;
	void       		        makeD3DVertexFlat ( void ) ;
	void		         		    makeD3DVertexGouraud ( void ) ;
	bool						isTouching( long	pointIndex , long		faceIndex ) ;
	unsigned long			extractString ( char *dataPtr , long startposition  , long bufferlen ,  char *destBuffer , long lendestBuffer) ; 
	
	//centers the object on load.
	void						centerObject( void ) ;
	
	D3DBLEND			_sourceAlpha , _destAlpha ;
	
public:
	K3Dd3d( ) ;
	~K3Dd3d( ) ;

	D3DMATRIX			matProjection ;
	
	/* use at your own risk */
	s3dPoint				*meshPtr ;
	sMeshFace			*meshFacesPtr ;
	unsigned	long		_pointCount ;
	unsigned	long		_faceCount ;
	/*          ***               */
	
	//called when the game window is created, you don't have to use that.
	static void	initK3Dd3d( long	width , long height , LPDIRECT3DDEVICE7	D3DDevice ) ;
//	void	initK3D( long	width , long height ) ;
	void	setPerspective( double  fovy, double aspect, double zNear, double zFar ) ;
	
	//clear scene only clears the Zbuffer , use KGraphic::setWorldView to clear the screen.
	void		beginScene( bool		clearScene , bool useScreenRes) ;
	void		endScene( void ) ;

	void		scale( float sx , float  sy , float  sz ) ;
	
	//flat / gouraud  
	void		setShadeMode( EShadeMode	esm ) ;
		
	void		display( float	x, float y , float z   , float	rx=0 , float ry=0 , float rz=0  ) ;
	//you can use display with the correct shade mode or use  directly the function that you prefer
	void		displayFlat( float	x , float y , float z   , float	rx=0 , float ry=0 , float rz=0   ) ;
	void		displayGouraud( float	x , float y , float z  , float	rx=0 , float ry=0 , float rz=0  ) ;

	//false for additive
	void		setAlphaMode( bool	regular ) ;
	void		setTexture( KGraphic	*texturePtr ) ;
	//enables alpha blending, blendFactor is a float from 0 ( completely transparent to 1 :  solid )
	void		enableBlending( bool	allowBlend , float	blendFactor ) ;

	//you can specify if you want to scale or load the texture
	//if you don't load the texture you can specify one with setTexture, this is useful if you
	//have several models sharing the same texture.
	bool		load( char	*filename , float scaleFactor = 1 , bool		loadTexture = true ) ;

	//loads a milkshape model
	bool		loadMk( char		*filename ,float scaleFactor = 1 , bool		loadTexture = true ) ;

	//loads a k3d model
	bool		loadK3D( char *filename , bool loadTexture= true ) ;
	bool		saveK3D( char *filename ) ;

	void		freeModel( void ) ;
	
};

#endif
