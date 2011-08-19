#include	"K3D.h"
#include	"KGraphicGL.h"



#ifndef		DEF_K3DGL


#define	DEF_K3DGL


class		K3DGL : public K3D
{
private:
	KGraphic				*surf_texture ;
	bool						_textureLoaded ;
	static	float			_screenWidth ;
	static	float			_screenHeight ;
	char						_textureFileName[MAX_PATH] ;
	bool						_blend 			 ;
	float						_blendFactor 	 ;

	float		 _scaleX , _scaleY , _scaleZ ;
	static	bool		_useScreenRes ;
	

	EShadeMode		_eShadeMode ;

	
	unsigned 	long		findChunk( char *chunkName , char *dataPtr , long startposition  , long bufferlen ) ; 
	unsigned	long		extractLongValue ( char *chunkName , char *dataPtr , long startposition  , long bufferlen )	;
	unsigned	long		extractNextFloatValue ( char *dataPtr , long startposition  , long bufferlen , float	*floatvalue)	;
	unsigned long			extractNextLongValue ( char *dataPtr , long startposition  , long bufferlen , unsigned long	*longValue ) ;
	void						extractTextureTable( char	*buffer , long	bufferLength );
	void						makeGouraud( void ) ;
	bool						isTouching( long	pointIndex , long		faceIndex ) ;
	unsigned long			extractString ( char *dataPtr , long startposition  , long bufferlen ,  char *destBuffer , long lendestBuffer) ; 
	
	
	//centers the object on load.
	void						centerObject( void ) ;
	
	GLenum					_sourceAlpha , _destAlpha ;
	
public:
	K3DGL( ) ;
	~K3DGL( ) ;
	
	
	/* use at your own risk */
	s3dPoint				*meshPtr ;
	sMeshFace			*meshFacesPtr ;
	unsigned	long		_pointCount ;
	unsigned	long		_faceCount ;
	/*          ***               */
	
	//called when the game window is created, you don't have to use that.
	static void	initK3D( long	width , long height ) ;
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
