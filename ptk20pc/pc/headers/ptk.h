#include	<windows.h>
#define	D3D_OVERLOADS
//#define DIRECT3D_VERSION 0x0700
//#define DIRECTDRAW_VERSION 0x0700

#include	"KPTK.h"
#include 	"KWindow.h"
#include 	"KGraphic.h"
#include 	"KInput.h"
#include 	"KText.h"
#include 	"KTrueText.h"
#include 	"KResourceArchive.h"
#include 	"KMiscTools.h"

#include 	"KSpline.h"
#include 	"KIniReader.h"
//#include	"K3D.h"
#include	"K7Z.h"
#include 	"KSound.h"

#ifndef		PTK_FREEPTR

#define		PTK_FREEPTR( x )  if ( x ) { delete x ; x =NULL ; }
/* for ms vc you can automate lib inclusion
#pragma comment  ( lib,"opengl32.lib")
#pragma comment  ( lib,"d3dx.lib")
#pragma comment  ( lib,"wininet.lib")
#pragma comment  ( lib,"winmm.lib")
#pragma comment  ( lib,"ddraw.lib")
#pragma comment  ( lib,"dxguid.lib")
*/

#ifndef 	K_PI
#define	K_PI  3.14159265
#endif

#endif

