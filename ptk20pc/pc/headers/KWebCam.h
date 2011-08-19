#include	<windows.h>
#include	<vfw.h>



class		KWebCam
{
private:
	unsigned short			*	_screenshotData ;
	bool									_cameraInitialised ;
	HWND									_hwndCamera ;
	long									_cameraIndex ;
	
	bool		detachGrabber( void ) ;
	bool		attachGrabber( void ) ;
	bool		connectCamera(  void ) ;
	bool		disconnectCamera( void ) ;
	bool		startCamera( void ) ;
	
	static	LRESULT CALLBACK		grabFrameProc( HWND hwnd , LPVIDEOHDR  videoHDR ) ;
	
public:
	KWebCam( ) ;
	~KWebCam( ) ;

	bool			setCamera( HWND	parentWindow , long		index ) ;
	char*		getCameraInfo( long	index ) ;

	unsigned	short	*getPixels( void ) ;

	bool	startCapture( long		FrameEveryMicroseconds ) ;
	void	stopCapture( void ) ;

};