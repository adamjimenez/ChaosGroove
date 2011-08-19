// bridge (.Cpp)
// -----------------

// Bridge code (used to add some handy RGBA code as well as wrap around some PTK functions)

#include <ptk.h>
#include <vector>
#include <list>
#include <sstream>
#include "KPTK.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "time.h"
#include "gl.h"

using namespace std;

#include "bridge.hpp"
#include "scene.hpp"
#include "game.hpp"
#include "log.hpp"

// Necessary objects.
KWindow *gwin;

// Rgba class:
Rgba Rgba::BLACK = Rgba( 0.0, 0.0, 0.0, 1.0 );
Rgba Rgba::WHITE = Rgba( 1.0, 1.0, 1.0, 1.0 );
Rgba Rgba::RED = Rgba( 1.0, 0.0, 0.0, 1.0 );
Rgba Rgba::YELLOW = Rgba( 1.0, 1.0, 0.0, 1.0 );
Rgba Rgba::GREEN = Rgba( 0.0, 1.0, 0.0, 1.0 );
Rgba Rgba::BLUE = Rgba( 0.0, 0.0, 1.0, 1.0 );
Rgba Rgba::INVISIBLE = Rgba( 0.0, 0.0, 0.0, 0.0 );


Rgba Rgba::
MixWith( const Rgba &other, float factor ) const {
	float red = r + (other.r - r) * factor;
	float green = g + (other.g - g) * factor;
	float blue = b + (other.b - b) * factor;
	float alpha = a + (other.a - a) * factor;

	return Rgba( red, green, blue, alpha );
}


int Rgba::
SpecialPacked() const {
	return (Rgba::CompToI(a) << 24) | (Rgba::CompToI(r) << 16) | (Rgba::CompToI(g) << 8) | Rgba::CompToI(b);
}


Rgba::
Rgba( int col, bool notUsed )
: r( CompToF(( col >> 16 ) & 0xff)), g( CompToF(( col >> 8 ) & 0xff)),
b( CompToF( col & 0xff)), a( CompToF(( col >> 24 ) & 0xff)) {
	notUsed = true;
}


Rgba::
Rgba( bool invalidiate )
: r( -1.0 ), g( -1.0 ), b( -1.0 ), a( -1.0 ) {
	invalidiate = true;
}


bool Rgba::
IsValid() {
	return r >= 0.0 && g >= 0.0 && b >= 0.0 && a >= 0.0;
}


int Rgba::
Packed() const {
	return NULL;
}

struct rand_info rand_info;

// RandomNum code:
void Seed(unsigned int newSeed)
{
	rand_info.seed = newSeed;
}

unsigned int SeedFromTime(void)
{
 rand_info.seed = time(0);

 return rand_info.seed;
}

unsigned long Rand()
{
 rand_info.seed = (201 * rand_info.seed + 11) % 4294967295;

 return (ABS(rand_info.seed) % 65535);
}

// Returns a Floating point random number between min and max.
float FRand( float min, float max )
{
 return min + float(Rand())/float(RAND_MAX) * (max-min);
}

int IRand(int min, int max)
{
 if (min > max) return max + (Rand() % (min + 1 - max));

 return min + (Rand() % (max + 1 - min));
}

vector < KGraphic *> LoadListOfBitmaps( string filenameBegin, string extension, unsigned int numNumbers)
{
   vector< KGraphic *> bitmapList;
   
   for( int index = 1;; index++ )
   {
      stringstream stream;
      stream << index;
      string numbers = stream.str();
      
      while( numbers.size() < numNumbers )
	  {
         numbers = string("0") + numbers;
      }
      
      string filename = filenameBegin + numbers + string(".") + extension;

      KGraphic *frame = NULL;
	  frame = KPTK::createKGraphic() ;
      
	  if( !frame->loadPicture( KMiscTools::makeFilePath(filename.c_str()), true, true ))
	  {
         delete frame;
         break;
      }
      
	  frame->setTextureQuality( true );
      bitmapList.push_back( frame );
   }
   
   return bitmapList;
}

int LoadAndAddBitmaps(vector< KGraphic *> *bitmapList, string filenameBegin, string extension)
{
   int num = 0;

   for( int index = 0;; index++ )
   {
      stringstream stream;
      stream << index;
      string numbers = stream.str();
      
      string filename = filenameBegin + numbers + string(".") + extension;

      KGraphic *frame = NULL;
	  frame = KPTK::createKGraphic() ;
      
	  if( !frame->loadPicture( KMiscTools::makeFilePath(filename.c_str()), true, true ))
	  {
         delete frame;
         break;
      }
      
	  frame->setTextureQuality( true );
	  //frame->setAlphaMode( 0 );
      bitmapList->push_back( frame );
			num++;
   }
   
   return num;
}

bool CheckQuit(void)
{
 return gwin->isQuit();
}

bool CheckWindowFocus(void)
{
 return gwin->hasFocus();
}

void RestoreWindow(void)
{
 gwin->restore();
}

void Rest(int time)
{
 #if !defined __APPLE__
 Sleep(time);
 #endif
}

bool CreateGameScreen(int width, int height, bool window, bool dx)
{
	// Open DirectX or OpenGL gfx context.
	if (dx == false) gwin = KPTK::createKWindow( K_OPENGL );
	if ( dx == true) gwin = KPTK::createKWindow( K_DIRECTX );

	// Now try to open screen with requested details.
	if (!gwin->createGameWindow( width, height, 32, window, "Chaos Groove" )) return false;

	// Clear newly created screen.
	ClearScreen();

	game.opengl = 1 - dx;
	return true;
}

void ListScreenModes(void)
{
 gwin->enumerateDisplays(ListDisplays);
}


bool ListDisplays(KDisplay * kd)
{
 log( "%d %d %d bpp - %d hz" , kd->width , kd->height , kd->depth, kd->frequency ) ;
 return true ;
}

int get_desktop_resolution(int *width, int *height)
{
   /* same thing for the desktop resolution:
    *
    *   DEVMODE display_mode;
    *
    *   display_mode.dmSize = sizeof(DEVMODE);
    *   display_mode.dmDriverExtra = 0;
    *   if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &display_mode) == 0)
    *      return -1;
    *
    *   *width  = display_mode.dmPelsWidth;
    *   *height = display_mode.dmPelsHeight;
    *
    *   return 0;
    */
 #if !defined __APPLE__
   HDC dc;

   dc = GetDC(NULL);
   *width  = GetDeviceCaps(dc, HORZRES);
   *height = GetDeviceCaps(dc, VERTRES);
   ReleaseDC(NULL, dc);

   return 0;
 #endif
}

int ScreenWidth(void)
{
 return gwin->getWindowWidth();
}

int ScreenHeight(void)
{
 return gwin->getWindowHeight();
}

void ClearScreen(void)
{
	// Clear newly created screen.
	gwin->setWorldView(0, 0, 0, 1.0, true);
}

void MessageBox(char *title, char *maintext)
{
	// Display messagebox with error details.
	KMiscTools::messageBox(title, maintext);

	// Record error in logfile too.
	log("");
	log("Messagebox error :");
	log(title);
	log("--");
	log(maintext);
	log("");
}

void UpdateScreen(void)
{
 // Flip screen buffer.
 gwin->flipBackBuffer( false ) ;
 gwin->processEvents( ) ;
}

bool Key(int k)
{
 return KInput::isPressed( EKeyboardLayout(k) );
}

bool LeftMouseButton(void)
{
 return KInput::getLeftButtonState();
}

bool RightMouseButton(void)
{
 return KInput::getRightButtonState();
}

void GetMouseMickeys(int *x, int *y)
{
 *x = (KInput::getMouseX() - (ScreenWidth() / 2)) * 4;
 *y = (KInput::getMouseY() - (ScreenHeight() / 2)) * 4;

 if (gwin->hasFocus()) KInput::mousePointerTo(ScreenWidth() / 2, ScreenHeight() / 2);
}

void AccelerateMouseMickeys(float *x, float *y, float scale, float scale_accel)
{
 float dist;

 scale = scale * 0.015;
 scale_accel = (scale_accel * 0.01) + 1.0;
 
 dist = *x * *x + *y * *y;
 
 // Acceleration Threshold 1
 if (dist > 7*7)
 {
  *x *= (scale_accel * 0.8); // Scale movements.
  *y *= (scale_accel * 0.8);
 }
 
 // Acceleration Threshold 2
 if (dist > 16*16)
 {
  *x *= scale_accel; // Scale movements.
  *y *= scale_accel;  
 }
 
 // Scale overall speed (while stopping scale being so low that movement is stopped).
 if (*x > 0) *x = MAX(0.1, *x * scale);
 if (*x < 0) *x = MIN(-0.1, *x * scale);
  
 if (*y > 0) *y = MAX(0.1, *y * scale);
 if (*y < 0) *y = MIN(-0.1, *y * scale);

}

long FileSize(char *FileName)
{   
 struct stat file;
     
 if(!stat(FileName,&file))
 {
  return file.st_size;
 }
   
 return 0;
}

void BlitTransform(KGraphic *bmp, float x, float y, float w, float h, float angle, float alpha) 
{
 int width, height;
 float zoom;
 
 width = bmp->getWidth();
 height = bmp->getHeight();

 // Different Width and Height.
 x -= (w / 2);
 y -= (h / 2);

 // need -1 for dx2 and dy2 in directX, but not in OpenGL! PTK bug..
 bmp->stretchAlphaRect( 0, 0, width, height, x, y, (x + w) - (1 - game.opengl), (y + h) - (1 - game.opengl), 
 alpha, 360.0 - RAD_TO_DEG( angle ));

 return;

/*
 if (w == h)
 {
  if (angle != 0.0 || alpha < 1.0)
	{
	 // Rotated..
   // Same Width and Height, so we can use zoom.
   x -= (float)width / 2;
   y -= (float)height / 2;

   zoom = w / (float)width;

   bmp->blitAlphaRectFxF( 0, 0, (float)width, (float)height, x, y, 360.0 - RAD_TO_DEG( angle ), zoom, alpha, false, false, 0, 0 );
	}
	else
	{
	 // full alpha, no rotation, possible stretching.
   x -= (float)width / 2;
   y -= (float)height / 2;

   zoom = w / (float)width;

   bmp->blitAlphaRectFxF( 0, 0, (float)width, (float)height, (int)x, (int)y, 0.0, zoom, alpha, false, false, 0, 0 );
	}
 }
 else
 {
  // Different Width and Height.
  x -= (w / 2);
  y -= (h / 2);

  // need -1 for dx2 and dy2 in directX, but not in OpenGL! PTK bug..
	bmp->stretchAlphaRect( 0, 0, width, height, x, y, (x + w) - (1 - game.opengl), (y + h) - (1 - game.opengl), 
	alpha, 360.0 - RAD_TO_DEG( angle ));
 }
 */
}

void SetSolidColour(KGraphic *bmp, Rgba col)
{
 GLfloat texcols[4];

 bmp->setBlitColor(col.r, col.g, col.b, 1.0);

 if (game.opengl)
 {
  glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);

  texcols[0] = col.r;
  texcols[1] = col.g;
  texcols[2] = col.b;
			
  glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texcols);
 }
 else
 {
  // Demo version of PTK doesn't allow D3D handle.. :'(

  //KGraphicD3D::_d3d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	//KGraphicD3D::_d3d->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
 }
}

void CancelSolidColour(KGraphic *bmp)
{
 GLfloat texcols[4];

 bmp->setBlitColor(1.0, 1.0, 1.0, 1.0);

 if (game.opengl)
 {	 
  texcols[0] = 1.0;
  texcols[1] = 1.0;
  texcols[2] = 1.0;

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, texcols);
 }
 else
 {
  //KGraphicD3D::_d3d->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	//KGraphicD3D::_d3d->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
 }
}

void DrawShadedRect(int x1, int y1, int x2, int y2, Rgba c1, Rgba c2, Rgba c3, Rgba c4)
{
 if (game.opengl)
 {
  glDisable( GL_TEXTURE_2D) ;
	glDisable( GL_BLEND ) ;
	glShadeModel( GL_SMOOTH );

	glBegin( GL_TRIANGLE_STRIP);

	y1 = ScreenHeight() - y1;
	y2 = ScreenHeight() - y2;

	glColor4f(c3.r, c3.g, c3.b, c3.a);
	glVertex3f(x1, y2, 0);
	
	glColor4f(c1.r, c1.g, c1.b, c1.a);
	glVertex3f(x1, y1, 0);

	glColor4f(c4.r, c4.g, c4.b, c4.a);
	glVertex3f(x2, y2, 0);

	glColor4f(c2.r, c2.g, c2.b, c2.a);
	glVertex3f(x2, y1, 0);

	glEnd();

	glEnable( GL_TEXTURE_2D) ;
 }
 else
 {
  #if !defined __APPLE__
/*
  y1++;
	y2++;
 	y2 = -y2;
	y1 = -y1;
  if ( KGraphicD3D::_directX->TestCooperativeLevel() != DD_OK)		return ;
	
	KGraphicD3D::_d3d->SetRenderState(D3DRENDERSTATE_ALPHABLENDENABLE, FALSE);
	KGraphicD3D::_d3d->SetRenderState(D3DRENDERSTATE_SHADEMODE, D3DSHADE_GOURAUD);
	KGraphicD3D::_d3d->SetRenderState(D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
	KGraphicD3D::_d3d->SetRenderState(D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	D3DCOLOR col1 = D3DRGBA( c1.r, c1.g, c1.b, c1.a );
  D3DCOLOR col2 = D3DRGBA( c2.r, c2.g, c2.b, c2.a );
  D3DCOLOR col3 = D3DRGBA( c3.r, c3.g, c3.b, c3.a );
  D3DCOLOR col4 = D3DRGBA( c4.r, c4.g, c4.b, c4.a );

	D3DLVERTEX v[4];
	v[0] = D3DLVERTEX(D3DVECTOR(x1,y2,0), col3, 0, 0, 0);
	v[1] = D3DLVERTEX(D3DVECTOR(x1,y1,0), col1, 0, 0, 0);
	v[2] = D3DLVERTEX(D3DVECTOR(x2,y2,0), col4, 0, 0, 0);
	v[3] = D3DLVERTEX(D3DVECTOR(x2,y1,0), col2, 0, 0, 0);

	KGraphicD3D::_d3d->SetTexture(0, NULL);

	KGraphicD3D::_d3d->DrawPrimitive(D3DPT_TRIANGLESTRIP, D3DFVF_LVERTEX, &v[0], 4, 0);
*/
  #endif
 }
}

void CheckGfxExists(int g, char *file, int line)
{
 char txt[MAX_STRING];

 if (g > gfx.size())
 {
  sprintf(txt, "Gfx: %d in %s at line %d", g, file, line);
	MessageBox("Gfx number does not exist!", txt);
 }
}

int slprintf(char *buffer, size_t count, const char *fmt, ...)
{
	va_list ap;
	int ret;

	va_start(ap, fmt);
#if !defined __APPLE__	
	ret = _vsnprintf(buffer, count-1, fmt, ap);
#else
	ret = vsnprintf(buffer, count-1, fmt, ap);
#endif	
	if (ret < 0)
		buffer[count-1] = '\0';
	va_end(ap);
	return ret;
}