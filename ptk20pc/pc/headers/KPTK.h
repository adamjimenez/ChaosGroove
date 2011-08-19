//#include	<d3d9.h>
#include	<windows.h>
#include	"KWindow.h"
#include	"KGraphic.h"
#include	"K3D.h"
#include	"KBatch.h"


#ifndef		DEFKPTK

#define		DEFKPTK

enum Erenderer		{	K_DIRECTX , K_DIRECTX9 , K_OPENGL , K_GDI  };

class			KPTK
{
public:
	KPTK( ) ;
	~KPTK( ) ;

static	Erenderer		_renderer ;

static	KWindow	*createKWindow( Erenderer er, bool use3d = false ) ;
static	KGraphic	*createKGraphic( char	*filename )  ;
static	KGraphic	*createKGraphic(void) ;
static	K3D			*createK3D( void ) ;
static	KBatch		*createKBatch( void ) ;

static	void			deleteKBatch( KBatch *kBatchPtr ) ;
static	void			deleteKGraphic( KGraphic *kGraphicPtr ); 
static	void			deleteKWindow( KWindow *kWindowPtr ) ;

};

#endif
