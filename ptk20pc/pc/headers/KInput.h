#ifndef KINPUT__H
#define KINPUT__H

#include	<windows.h>
#include	<mmsystem.h>
#include	"KGraphic.h"
/*class CInput 
	started 30 / 09 /2002
*/

//the default build in mouse.
//you can change it to match your needs.
extern	unsigned	long	mousePattern[] ;

//#ifdef WIN32
	#include		<windows.h>

	#define	K_JOYID1		JOYSTICKID1 
	#define	K_JOYID2		JOYSTICKID2

	#define		BUTTON_1		JOY_BUTTON1
	#define		BUTTON_2		JOY_BUTTON2
	#define		BUTTON_3		JOY_BUTTON3
	#define		BUTTON_4		JOY_BUTTON4	
	
	/* constants used with JOYINFOEX */
	#define BUTTON_5         0x00000010l
	#define BUTTON_6         0x00000020l
	#define BUTTON_7         0x00000040l
	#define BUTTON_8         0x00000080l
	#define BUTTON_9         0x00000100l
	#define BUTTON_10        0x00000200l
	#define BUTTON_11        0x00000400l
	#define BUTTON_12        0x00000800l
	#define BUTTON_13        0x00001000l
	#define BUTTON_14        0x00002000l
	#define BUTTON_15        0x00004000l
	#define BUTTON_16        0x00008000l
	#define BUTTON_17        0x00010000l
	#define BUTTON_18        0x00020000l
	#define BUTTON_19        0x00040000l
	#define BUTTON_20        0x00080000l
	#define BUTTON_21        0x00100000l
	#define BUTTON_22        0x00200000l
	#define BUTTON_23        0x00400000l
	#define BUTTON_24        0x00800000l
	#define BUTTON_25        0x01000000l
	#define BUTTON_26        0x02000000l
	#define BUTTON_27        0x04000000l
	#define BUTTON_28        0x08000000l
	#define BUTTON_29        0x10000000l
	#define BUTTON_30        0x20000000l
	#define BUTTON_31        0x40000000l
	#define BUTTON_32        0x80000000l
//#endif

#include		"KKeyTable.h"


#define		ISDOWN			true
#define		ISUP					false

struct		sMouseState
{
long lx,ly,lz ; 
char buttons[4]; 
} ;

class		KInput
{
private:
	//used for multi mouse mode
	
	static	HKL		hKL ;
	static	HWND		_hwnd ;
	static	unsigned	char			keystatetable[256] ;
	static	bool							joystick[2] ;		//etat d'activation du joystick
	static	bool							_is9x ;				//if it's a 9x system
	//built in mouse cursor
	static	unsigned long		*_mousePointerBuffer ;
	static	void		generateMouse( void ) ;

public:	
	KInput( ) ;
	~KInput( ) ;

	static	KGraphic	*surf_mouse ;
	static	bool			_fullScreen;

	//required to be called to intialiaze the coordinnates space. ( called when a window is created, you shouldn't have to call it )
	//NULL  for fullscreen
	static	void		setMousePointerBuffer( unsigned long *dataPtr ) ;
	
	static	void		initInput(  HWND		hwnd ) ;
	static	void		displayMouse( void ) ;
	static	void		freeMouse( void ) ;
	//souris , position et boutons
	static	short		getMouseX( void ) ;
	static	short		getMouseY( void ) ;
	static	bool		getLeftButtonState( void ) ;
	static	bool		getRightButtonState( void ) ;
	static	bool		getMiddleButtonState( void )  ;
	static	void		mousePointerTo( long x , long y ) ;
	
	//clipping, cursors etc..
	static	void		showPointer( void ) ;
	static	void		hidePointer( void ) ;
	static	void		clipPointer( bool clipToWindow ) ;		//maintains the pointer inside the window
	
	//touche du clavier ( creer une table de touches virtuelles pour le mac VK_XXXX -> equivalent pc )
	static	void		waitForKeyRelease( EKeyboardLayout	 keyCode ) ;
	static	bool		isPressed( EKeyboardLayout	keyCode ) ;

	//function speciales joystick  //joyID = numero du joystick (0,1,2etc.. )
	static	bool		joyEnable( char	joyID ) ;		//active le joystick, return true si ok
	static	void		joyDisable( char	joyID ) ;	//desactive le joystick
	
	//detection de mouvement, retourne ISDOWN si le joy est dans la position indiquee
	static	bool		joyLeft( char		joyID ) ;
	static	bool		joyRight( char		joyID ) ;
	static	bool		joyUp( char		joyID ) ;
	static	bool		joyDown( char		joyID ) ;
	static	long		joyY( char		joyID , char	hatID )  ;
	static	long		joyX( char		joyID , char	hatID )  ;
	static	long		joyPOV( char		joyID 	 ) ;
	
	static	bool		joyButtonN( char		joyID , long		buttonID ) ;
	static	void		joyWaitForJoyButtonUp( char		joyID , long		buttonID ) ;
	static	void		setLastWheelValue( short	value ) ;
	static	short		getWheelValue( void ) ;
	//returns an EKeyboardLayout given a native virtual key
	static	EKeyboardLayout getPTKKey( long wparam ) ; 

	static	short		_lastWheelValue ;
};

#endif // KINPUT__H
