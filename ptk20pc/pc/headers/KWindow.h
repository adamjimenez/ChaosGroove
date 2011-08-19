#ifndef KWINDOW__H
#define KWINDOW__H

//gestion d'ouverture de fenetre.
// debut : 9 octobre 2002

#include <windows.h>
#include	"KKeyTable.h"

#define	GAMECLASSNAME		"http://www.phelios.com  PTK Game Library."

#define	K_LBUTTON					1
#define	K_MBUTTON					3
#define	K_RBUTTON					2


#define	K_EVENT_NULL						0
//move
#define	K_EVENT_MOUSEMOVE			1
//generic mouse up down
#define	K_EVENT_MOUSEUP					2
#define	K_EVENT_MOUSEDOWN			3
//char event
#define	K_EVENT_CHAR						5
//keys up/down
#define	K_EVENT_KEYDOWN					6
#define	K_EVENT_KEYUP						7
#define	K_EVENT_DBLCLICK					8
//wheel
#define	K_EVENT_WHEEL						11
#define	K_EVENT_CONTEXTMENU		12
#define	K_EVENT_ACTIVATED				13
#define	K_EVENT_DEACTIVATED			14
#define	K_EVENT_MONITORPOWEROFF			15



struct	KEvent
{
long		type ;
long		mouseX , mouseY ;
long		buttonIndex ;
short		wheel ;			//wheel displacement
char		character ;
long		virtualKey ;		//required for keydown/up events
long		nativeMessage ;
EKeyboardLayout	ptkKey ;
long		nativeData ;  //lparam on windows
};


struct		KDisplay
{
long			width ;
long			height ;
long			depth ;
long			frequency ;
};

//return false if you don't want the event to be processed
typedef		 bool		( *ptkWindowCallBack ) (  HWND hWnd, UINT messg,	WPARAM wParam, LPARAM lParam  );
typedef		 bool		( *ptkNativeCallBack ) (  KEvent  *keventPtr);

//return false to abord the enumeration
typedef		 bool		( *ptkEnumDisplayCallBack ) (  KDisplay  *kdisplayPtr );




class		KWindow
{
public:

	virtual	void			setDefaultWorldView( void )  = 0;
	virtual	void			setWorldView( float	translateX , float		translateY ,   float	rotation , float 	zoom, bool	clearworld  ) = 0;
	virtual	void			setClearColor ( float r , float g , float b , float a ) = 0;
	
	KWindow( ) {};			//doesn't allow a Zbuffer
	KWindow( bool 	use3D) { use3D = use3D ;};  //gives your the choice to set a Zbuffer
	virtual	~KWindow( ) {} ;
	
	virtual	bool		createGameWindow( short  width , short height , short depth , bool windowed , const char *windowTitle = GAMECLASSNAME )  = 0;

	//only available for windows  ( to create activex )
	virtual	void	 	setParentWindow( HWND	hwndParent ) = 0 ;

	virtual	void		terminate( void )  = 0;
	virtual	bool		toggleFullScreen( bool 	fullscreen ) = 0;
	virtual	bool		hasFocus()= 0;
	virtual	void		setTitle( const char * title ) = 0;
	virtual	void		flipBackBuffer(  bool	waitForBackWindow = true , bool restoreview = true) = 0;
	virtual	void		processEvents( void ) = 0;
	virtual	bool		isQuit( void ) = 0;
	virtual	bool		setQuit( bool quitstate ) = 0;
	virtual	HWND	getWindowHandle(void ) = 0;
	virtual	void		setMaxFrameRate( long	desiredFrameRate  ) = 0;
	virtual	bool		getRectangleTexCap( void ) = 0;		//returns true if rectangle is possible
	virtual	bool		getAccelerationCap( void )= 0 ;		//returns true if accelerated
	virtual	void		setRectangleTexCap( bool	forceRectangleCap ) = 0	;
	virtual	void		setGamma( float	gammai ) = 0;
	virtual	long		getWindowWidth( void ) = 0 ;
	virtual	long		getWindowHeight( void ) = 0 ;
	virtual	void		restore( void )= 0;
	virtual	void		minimize( void )= 0;
	virtual	void		displayMouse( bool mousestate ) =0 ;
	virtual	bool		saveBackBuffer( const char * cpcFileName , long	imageFormat = 0 , long resizeW=0 , long resizeH=0 ) = 0 ;
	virtual	void		setClipping( long x1, long y1, long x2 , long y2 ) = 0 ;

	//callback  for the window  ( differs on  mac )
	virtual	void		setCallBack( ptkWindowCallBack  userProc ) = 0;

	//PTK callback
	virtual	void		setPTKCallBack( ptkNativeCallBack  userProc )= 0 ;
	virtual	void		enumerateDisplays( ptkEnumDisplayCallBack  functionPtr ) = 0;
	virtual	void		showWindow( bool show ) = 0 ;
	virtual	bool		isWindowVisible( void ) = 0 ;
	virtual	bool		getFullScreenState( void ) = 0 ;

};

#endif // KWINDOW__H
