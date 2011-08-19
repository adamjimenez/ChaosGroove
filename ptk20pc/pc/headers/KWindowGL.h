
#ifndef KWINDOWGL__H
#define KWINDOWGL__H



//gestion d'ouverture de fenetre.
// debut : 9 octobre 2002

#include <windows.h>
#include "gl.h"
#include	"KKeyTable.h"
#include		"KWindow.h"


class		KWindowGL :  public KWindow
{
public:
   HWND	 _hwndParent  ;



	float							_clearR , _clearG , _clearB , _clearA ;
	DEVMODE				dv ;
	HGLRC						_openGLContext ;  //handle on my gl Context
	HDC							_hDC ; 
	HWND						_hwnd ;
	ptkWindowCallBack				_userCallback ;
	ptkNativeCallBack				_ptkCallback ;
	bool							_silent ;
	bool							_capsDetected ;

	
/*	static	bool		_gammaChanged ;
	static	void 		saveGammaRamp( void ) ;
	static	void 		fadeGammaRamp( void ) ;
	static	void 		restoreGammaRamp( void ) ;
	static	short		gammaTable[3*256] ;
*/	
	 bool		_isWaitVBLSupported;
	bool		_fullscreen ;
	short		_sourceScreenDepth ;			//bpp originel , avant changement
	short		_gameScreenDepth ;			//bpp apres  changement
	int			_windowID ;
	short		_windowWidth , _windowHeight , _windowDepth ;
		
	void		SwitchScreenToDepth( short		depth ) ;
	int			FillAndSelectPixelFormat( HDC		hdc , PIXELFORMATDESCRIPTOR		*pixelFormatDesc , short depth ) ;
	bool		AttachOpenGLToWindow( short depth ) ;


	static	LRESULT CALLBACK		GameWindowProc( HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam )  ;
	bool		SwitchToResolution( short	width , short	height , short depth ) ;
	void		RestoreResolution( void ) ;
	void		doGammaAdjustment( void ) ;
	bool		setQuit( bool quitstate ) ;
	void			setDefaultWorldView( void )  ;
	void			setWorldView( float	translateX , float		translateY ,   float	rotation , float 	zoom, bool	clearworld  ) ;

	long		_maxFrameRate ;
	bool		hasRectangleText ;
	bool		isAccelerated ;
	bool		_use3d ;
	float		_gamma ;


	KWindowGL( )  ;			//doesn't allow a Zbuffer
	KWindowGL( bool 	use3D) ;  //gives your the choice to set a Zbuffer

	~KWindowGL( ) ;
	
	bool							_quit ;
	
	bool		_displayPTKMouse ;
	void		displayMouse( bool mousestate ) ;

	void		setParentWindow( HWND	hwndParent ) ;
	bool		createGameWindow( short  width , short height , short depth , bool windowed , const char *windowTitle = GAMECLASSNAME )  ;
	void		terminate( void )  ;
	bool		toggleFullScreen( bool 	fullscreen ) ;
	bool		hasFocus();
	void		setTitle( const char * title ) ;
	void		flipBackBuffer(  bool	waitForBackWindow = true , bool restoreview = true) ;
	void		processEvents( void ) ;
	bool		isQuit( void ) ;
	HWND	getWindowHandle(void ) ;
	void		setMaxFrameRate( long	desiredFrameRate  ) ;
	bool		getRectangleTexCap( void ) ;		//returns true if rectangle is possible
	bool		getAccelerationCap( void ) ;		//returns true if accelerated
	void		setRectangleTexCap( bool	forceRectangleCap ) 	;
	void		setGamma( float	gammai ) ;
	void		setClearColor( float r , float g , float b , float a ) ;
	bool		saveBackBuffer( const char * cpcFileName , long	imageFormat , long resizeW , long resizeH ) ;

	void		restore( void );
	void		minimize( void );

	long		getWindowWidth( void )  ;
	long		getWindowHeight( void )  ;


	void		setClipping( long x1, long y1, long x2 , long y2 ) ;

	//callback  for the window  ( differs on  mac )
	void		setCallBack( ptkWindowCallBack  userProc ) ;

	//PTK callback
	void		setPTKCallBack( ptkNativeCallBack  userProc ) ;
	
	void		enumerateDisplays( ptkEnumDisplayCallBack  functionPtr ) ;
	
	void		showWindow( bool visible ) ;
	bool		isWindowVisible( void ) ;
	bool		getFullScreenState( void )  ;
};

#endif // KWINDOW__H
