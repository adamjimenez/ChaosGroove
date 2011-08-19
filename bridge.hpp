// bridge (.Hpp)
// -----------------

#ifndef __Bridge_hpp__
#define __Bridge_hpp__ 
     
#define MIN(x,y)     (((x) < (y)) ? (x) : (y))
#define MAX(x,y)     (((x) > (y)) ? (x) : (y))
#define MID(x,y,z)   MAX((x), MIN((y), (z)))
     
#undef ABS
#define ABS(x)       (((x) >= 0) ? (x) : (-(x)))
     
#undef SGN
#define SGN(x)       (((x) >= 0) ? 1 : -1)

#define BLENDER_ADD			    							0
#define BLENDER_ALPHA											1
#define BLENDER_SUB												2
#define BLENDER_COPY											3

class Rgba {
public:
   // Construct the color from float (0.0 ... 1.0) or integer color components (0..255) /
   // or using the 24-bit packed color value and an alpha value //
   Rgba( float  r, float  g, float  b, float  a = 1.0 )              : r( r ), g( g ), b( b ), a( a ) {}
   Rgba( double r, double g, double b, double a = 1.0 )              : r( r ), g( g ), b( b ), a( a ) {}
   Rgba( int    r, int    g, int    b, int    a = 255 )              : r( Rgba::CompToF(r)), g( Rgba::CompToF(g)), b( Rgba::CompToF(b)), a( Rgba::CompToF(a)) {}
   
   Rgba( const std::string &hexPresentation );
   
   Rgba() : r( 0.0 ), g( 0.0 ), b( 0.0 ), a( 0.0 ) {}
   
   // Color components //
   float r, g, b, a;
   
   // Some default colors //
   static Rgba BLACK, WHITE, RED, YELLOW, GREEN, BLUE;
   static Rgba INVISIBLE; // Has zero alpha //
   
   // Returns a color interpolated between this and otherColor //
   // using the factor (0...1) such that if factor is zero, it //
   // returns the calling color, if factor is 1.0 it returns otherColor //
   // and otherwise it returns a color between this and otherColor //
   Rgba MixWith( const Rgba &otherColor, float factor ) const;
   
   
   // Returns a color which has the same color componenets as this one //
   // except a different alpha value //
   inline Rgba WithAlpha( float newAlpha ) const                           { return Rgba( r, g, b, newAlpha ); }
   
   
   // Returns the color in a packed 32-bit integer //
   int Packed() const;
   
   
   // A previously used name for the function MixWith //
   inline Rgba InterpolateWith( const Rgba &otherColor, float factor ) const {
      return MixWith( otherColor, factor );
   }
   
   
private:

   Rgba( bool invalidiated );
   Rgba( int specialPackedColor, bool notUsed );
   
   static inline int CompToI( float c ) { return int( 255.0 * c ); }
   static inline float CompToF( int c ) { return float(c)/255.0; }
   int SpecialPacked() const;
   
   bool IsValid();
};

extern KWindow *gwin;

struct rand_info
{
 unsigned long seed;
};

extern struct rand_info rand_info;

unsigned long Rand();
void Seed(unsigned int newSeed);
unsigned int SeedFromTime(void);
float FRand( float min, float max );
int IRand(int min, int max);

vector < KGraphic *> LoadListOfBitmaps( string filenameBegin, string extension, unsigned int numNumbers);
int LoadAndAddBitmaps(vector< KGraphic *> *bitmapList, string filenameBegin, string extension);

bool CreateGameScreen(int width, int height, bool window, bool dx);
void ListScreenModes(void);
bool ListDisplays(KDisplay * kd);
int get_desktop_resolution(int *width, int *height);

int ScreenWidth(void);
int ScreenHeight(void);
void ClearScreen(void);
bool CheckQuit(void);
bool CheckWindowFocus(void);
void RestoreWindow(void);
void MessageBox(char *title, char *maintext);
void Rest(int time);
void UpdateScreen(void);
bool Key(int k);
void GetMouseMickeys(int *x, int *y);
void AccelerateMouseMickeys(float *x, float *y, float scale, float scale_accel);
bool LeftMouseButton(void);
bool RightMouseButton(void);

long FileSize(char *FileName);

void BlitTransform(KGraphic *bmp, float x, float y, float w, float h, float angle, float alpha) ;
void DrawShadedRect(int x1, int y1, int x2, int y2, Rgba c1, Rgba c2, Rgba c3, Rgba c4);
void SetSolidColour(KGraphic *bmp, Rgba col);
void CancelSolidColour(KGraphic *bmp);

void CheckGfxExists(int g, char *file, int line);
int slprintf(char *buffer, size_t count, const char *fmt, ...);

#endif