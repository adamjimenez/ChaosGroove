
#ifndef DEF_WINDOWDX9
#define DEF_WINDOWDX9

#include	<windows.h>
#include	<d3d9.h>
#include	<d3d9types.h>

#include	"KWindow.h"
#include	"KInput.h"
#include	"KGraphicDX9.h"

class		KWindowDX9 : public KWindow
{
public:

	IDirect3DSurface9		*_backbufferTarget ;

	D3DCOLOR	_clearColor ;

	KWindowDX9( ) ;
	~KWindowDX9( ) ;
	KWindowDX9( bool use3d ) ;

	LPDIRECT3D9            _d3d9Ptr	 ; //the D3DDevice
	LPDIRECT3DDEVICE9       _d3d9Device  ; //  rendering device
	D3DPRESENT_PARAMETERS d3dpp; 
 
	KGraphicDX9* _graphic;
	ptkWindowCallBack	_userCallback ;
	ptkNativeCallBack		_ptkCallback ;

	//keeps the global transformation
	float	_translateX , _translateY  , _rotation	,	_zoom	;


	long							_maxFrameRate ;
	bool							hasRectangleText ;
	bool							isAccelerated ;
	bool							_use3d ;

	float							_gamma ;
	bool							_isWaitVBLSupported;
	bool							_fullscreen  ;
	short							_sourceScreenDepth ;			//bpp originel , avant changement
	short							_gameScreenDepth ;			//bpp apres  changement


	HWND		_hwndPTK , _hwndParent ;
	bool		_quit ;


	long		_windowWidth , _windowHeight,_windowDepth ;
	char		_windowTitle[512];
	
	bool		initDx9( void ) ;
	void		releaseD3D9( void );


	bool		createGameWindow( short  width , short height , short depth , bool windowed , const char *windowTitle = GAMECLASSNAME )  ;

	//todo
	
	void			setDefaultWorldView( void ) ;
	void			setWorldView( float	translateX , float		translateY ,   float	rotation , float 	zoom, bool	clearworld  );

	void			setClearColor ( float r , float g , float b , float a ) ;

	
	//only available for windows  ( to create activex )
	void	 	setParentWindow( HWND	hwndParent ) ;


	bool		toggleFullScreen( bool 	fullscreen ) ;


	void		terminate( void )  ;
	bool		hasFocus(void) ;
	void		setTitle( const char * title ) ;
	void		flipBackBuffer(  bool	waitForBackWindow = true , bool restoreview = true) ;

	void		processEvents( void ) ;

	bool		isQuit( void ) ;
	bool		setQuit( bool quitstate ) ;
	HWND	getWindowHandle(void ) ;
	void		setMaxFrameRate( long	desiredFrameRate  ) ;

	bool		getRectangleTexCap( void ) ;		//returns true if rectangle is possible
	bool		getAccelerationCap( void );		//returns true if accelerated
	void		setRectangleTexCap( bool	forceRectangleCap );
	void		setGamma( float	gammai );
	long		getWindowWidth( void ) ;
	long		getWindowHeight( void ) ;
	void		restore( void ) ;
	void		minimize( void );
	void		displayMouse( bool mousestate ) {} 
	bool		saveBackBuffer( const char * cpcFileName , long	imageFormat = 0 , long resizeW=0 , long resizeH=0 );

	void		Resize(int width, int height);

	void		setClipping( long x1, long y1, long x2 , long y2 );

	//callback  for the window  ( differs on  mac )
	void		setCallBack( ptkWindowCallBack  userProc );

	//PTK callback
	void		setPTKCallBack( ptkNativeCallBack  userProc ) ;
	void		enumerateDisplays( ptkEnumDisplayCallBack  functionPtr ) {} 

	void		showWindow( bool show ) ;
	bool		isWindowVisible( void ) ;
	bool		getFullScreenState( void )  ;

	
	void SetRenderTarget(KGraphicDX9* graphic);
	
	static		LRESULT CALLBACK	GameWindowProc( HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam ) ;

	
};

#endif // DEF_WINDOWDX9
