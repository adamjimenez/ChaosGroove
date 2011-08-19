#ifndef	DEFKWINDOWD3D

#define	DEFKWINDOWD3D


#define D3D_OVERLOADS
#define DIRECT3D_VERSION 0x0700
#include <ddraw.h>
#include <d3d.h>
#include	"KWindow.h"
#include	"KInput.h"

//#define D3DCOLOR_ARGB(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))



class		KWindowD3D : public KWindow
{
private:
	DEVMODE	dv ;
	void					testCooperative(void ) ;

	bool					_internalFocus ;
	bool					_visibleState ;
	bool					_scrActive ;
	unsigned	long	_startSCR , _scrTimer ;
	char					_screenSaverPath[512] ;
	unsigned long		_screenTimeOut ;
	 bool					_screenTimeOutActive ;
	bool					_monitorOn ;

	bool	 			createRealWindow( short	width , short	height , short	depth ,  const char	*windowTitle ) ;
	bool				attachD3DToWindow( void ) ;
	static			LRESULT CALLBACK	GameWindowProc( HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam )  ;
	bool				SwitchToResolution( short	width , short	height , short depth )  ;
	void				RestoreResolution( void )  ;
	bool				releaseDirectContexts( void ) ;
 	bool           createZBuffer( void ) ;
	bool				createDirectContextsWindow( void ) ;
	bool				createDirectContextsFullScreen( void ) ;
	bool				regenerateD3DDevice( void ) ;
	void				ClearSurface(LPDIRECTDRAWSURFACE7  surfacePTR) ;
	bool				_displayPTKMouse ;
	char				_windowTitle[256] ;
	
	HWND			_hwndParent ;

	bool	 			createGameWindowSpecial( short	width , short	height , short	depth ,  const char	*windowTitle )  ;


public:
	static			bool		isWindowActive ;
	HWND						_hWndPTK ;
	
	LPDIRECTDRAW7					_lpDD;
	LPDIRECTDRAWSURFACE7		_lpDDSPrimary;
	LPDIRECTDRAWSURFACE7		_lpDDSBack;
	LPDIRECTDRAWSURFACE7		_lpDDSZBuffer;
	LPDIRECT3D7					_lpD3D;
	LPDIRECT3DDEVICE7 			_lpD3DDevice;
	D3DCOLOR					_clearColor;
	
	
	


	ptkWindowCallBack	_userCallback ;
	ptkNativeCallBack		_ptkCallback ;
	long							_maxFrameRate ;
	bool							hasRectangleText ;
	bool							isAccelerated ;
	bool							_use3d ;
	float							_gamma ;
	bool							_isWaitVBLSupported;
	bool							_fullscreen ;
	short							_sourceScreenDepth ;			//bpp originel , avant changement
	short							_gameScreenDepth ;			//bpp apres  changement
	int								_windowID ;
	short							_windowWidth , _windowHeight , _windowDepth ;
	
	
	KWindowD3D( ) ;
    KWindowD3D(bool use3d) ;
	~KWindowD3D( ) ;

	bool		_quit ;


	bool		createGameWindow( short  width , short height , short depth , bool windowed , const char *windowTitle = GAMECLASSNAME )  ;
	void	 	setParentWindow( HWND	hwndParent )  ;
	
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
	void		doGammaAdjustment( void ) ;
	void		setClearColor(float r , float g , float b  , float a )  ;
	bool		saveBackBuffer( const char * cpcFileName , long	imageFormat , long resizeW , long resizeH ) ;

	void		restore( void );
	void		minimize( void );
	long		getWindowWidth( void )  ;
	long		getWindowHeight( void )  ;
	void		displayMouse( bool mousestate ) ;
	bool		setQuit( bool quitstate ) ;

	
	//callback  for the window  ( differs on  mac )
	void		setCallBack( ptkWindowCallBack  userProc ) ;

	//PTK callback
	void		setPTKCallBack( ptkNativeCallBack  userProc ) ;

	void				setClipping( long x1 ,	long y1 , long x2 , long y2 ) ;


	void	setWorldView( float	translateX , float		translateY ,   float	rotation , float 	zoom, bool	clearworld  ) ;
	void	setDefaultWorldView( void ) ;
	
	void			enumerateDisplays( ptkEnumDisplayCallBack  functionPtr ) ;
	void			showWindow( bool show )  ;
	bool			isWindowVisible( void )  ;
	bool			getFullScreenState( void ) ;

};



#endif
