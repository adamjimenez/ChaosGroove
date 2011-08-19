#include	"KGraphic.h"

#ifndef		DEF_K3D


#define	DEF_K3D

#define	K_PI	 3.1415926535897932384626433832795f

enum		EShadeMode {	ESM_FLAT , ESM_GOURAUD 	};


//struct	3d point
struct	s3dPoint
{
	float		x,y,z ;
	float		nx,ny,nz ;		//normals for gouraud shading
};

//we only support triangles
struct	sMeshFace
{
	//indexes of the points
	long		pt1,pt2,pt3 ;
	float		nx,ny,nz ; //normal for per face shading ( displayFlat )
	s3dPoint	texCoords[3] ;		//texture coordinnates
};

struct	sK3DHeader
{
	long		tag ;
	char		textureFilename[MAX_PATH] ;
	unsigned	long		_pointCount ;
	unsigned	long		_faceCount ;
//	s3dPoint				*meshPtr ;
//	sMeshFace			*meshFacesPtr ;
};


struct		sTextureCoord
{
	float	u,v ;
};	

class		K3D
{
public:
	K3D( ) ;
	virtual	~K3D( ) ;
	
		
	//called when the game window is created, you don't have to use that.
	virtual	void	setPerspective( double  fovy, double aspect, double zNear, double zFar ) = 0 ;
	
	//clear scene only clears the Zbuffer , use KGraphic::setWorldView to clear the screen.
	virtual	void		beginScene( bool		clearScene , bool useScreenRes) = 0 ;
	virtual	void		endScene( void ) = 0 ;

	virtual	void		scale( float sx , float  sy , float  sz ) = 0 ;
	
	//flat / gouraud  
	virtual	void		setShadeMode( EShadeMode	esm ) = 0 ;
		
	virtual	void		display( float	x, float y , float z   , float	rx=0 , float ry=0 , float rz=0  ) = 0 ;
	//you can use display with the correct shade mode or use  directly the function that you prefer
	virtual	void		displayFlat( float	x , float y , float z   , float	rx=0 , float ry=0 , float rz=0   ) = 0 ;
	virtual	void		displayGouraud( float	x , float y , float z  , float	rx=0 , float ry=0 , float rz=0  ) = 0 ;

	//false for additive
	virtual	void		setAlphaMode( bool	regular ) = 0 ;
	virtual	void		setTexture( KGraphic	*texturePtr ) = 0 ;
	//enables alpha blending, blendFactor is a float from 0 ( completely transparent to 1 :  solid )
	virtual	void		enableBlending( bool	allowBlend , float	blendFactor ) = 0 ;

	//you can specify if you want to scale or load the texture
	//if you don't load the texture you can specify one with setTexture, this is useful if you
	//have several models sharing the same texture.
	virtual	bool		load( char	*filename , float scaleFactor = 1 , bool		loadTexture = true ) = 0 ;

	//loads a milkshape model
	virtual	bool		loadMk( char		*filename ,float scaleFactor = 1 , bool		loadTexture = true ) = 0 ;

	//loads a k3d model
	virtual	bool		loadK3D( char *filename , bool loadTexture= true ) = 0 ;
	virtual	bool		saveK3D( char *filename ) = 0 ;

	virtual	void		freeModel( void ) = 0 ;
	
	
};

#endif
