
#ifndef KWINDOWGDI__H
#define KWINDOWGDI__H



//ptk pure gdi ( no ddraw )
//20 july 2006 / by patrice

#include <windows.h>

#include	"KKeyTable.h"
#include		"KWindow.h"
#include		"KGraphicGDI.h"


class		KWindowGDI :  public KWindow
{
public:
	HDC							_windowDC ;
	
	KGraphicGDI				*_surf_backBuffer ;
	static	KGraphicGDI				*_surf_colorHolder ;

	float							_clearR , _clearG , _clearB , _clearA ;
	DEVMODE				dv ;
	HDC							_hDC ; 
	HWND						_hwnd ;
	ptkWindowCallBack				_userCallback ;
	ptkNativeCallBack				_ptkCallback ;
	bool							_silent ;
	bool							_capsDetected ;

	void	 	setParentWindow( HWND	hwndParent ) {}
	bool		setQuit(bool quit ) { _quit = quit ; return _quit ;} 
	
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

	static	LRESULT CALLBACK		GameWindowProc( HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam )  ;
	bool		SwitchToResolution( short	width , short	height , short depth ) ;
	void		RestoreResolution( void ) ;
	void		doGammaAdjustment( void ) ;

	void			setDefaultWorldView( void )  ;
	void			setWorldView( float	translateX , float		translateY ,   float	rotation , float 	zoom, bool	clearworld  ) ;

	unsigned long		_maxFrameRate ;
	bool		hasRectangleText ;
	bool		isAccelerated ;
	bool		_use3d ;
	float		_gamma ;


	KWindowGDI( )  ;			//doesn't allow a Zbuffer
	KWindowGDI( bool 	use3D) ;  //gives your the choice to set a Zbuffer

	~KWindowGDI( ) ;
	
	bool							_quit ;
	
	bool		_displayPTKMouse ;
	void		displayMouse( bool mousestate ) ;
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
};

#endif // KWINDOW__H
