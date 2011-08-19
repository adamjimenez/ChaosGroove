/******************************************************************************
 * PTK truetype text rendering class definition
/******************************************************************************/


#ifndef  K_TRUETEXT_H
#define  K_TRUETEXT_H


struct	sStringInfoKtt
{
long		startpos, endpos ;
bool		leftalign ;
};




/* Include definitions */
#include "KTypes.h"
#include "KObjectBase.h"
#ifdef _BUILDING_KTRUETYPE
#include <ft2build.h>
#include FT_FREETYPE_H
#else
typedef void *FT_Stream;
typedef void *FT_Face;
typedef void *FT_Library;
#endif

/** Char data */
struct KTrueTextChar {
   float fX;               /**< X coordinate in texture */
   float fY;               /**< Y coordinate in texture */
   float fWidth;           /**< Width in texture */
   float fHeight;          /**< Height in texture */
   int nXOffset;           /**< X offset for char */
   int nYOffset;           /**< Y offset for char */
   int nXAdvance;          /**< X displacement after drawing */
};

/**
 * Generic class for rendering TrueType text.
 *
 * The KTrueType class loads TrueType (.ttf) fonts and renders them to
 * the display.
 */

class KTrueText : public KObjectBase {
public:
   /**
    * Constructor
    *
    * \param font_Filename name of truetype font to load
    */
   KTrueText (const char *font_Filename);

   /** Destructor */
   ~KTrueText ();

   /**
    * Load a font file
    *
    * \param font_Filename name of file to be loaded
    */
   void loadFontFile (char *font_Filename); 

   /**
    * Draw a string from coordinates of top, left
    *
    * \param txt text to be drawn
    * \param x X coordinate of top, left of text
    * \param y Y coordinate of top, left of text
    * \param kering horizontal space between chars, in pixels
    */
   void drawStringFromLeft (const char *txt, long x, long y, float  kerning = 0);

   /**
    * Draw a string from coordinates of top, right
    *
    * \param txt text to be drawn
    * \param x X coordinate of top, right of text
    * \param y Y coordinate of top, right of text
    * \param kerning extra horizontal space between chars, in pixels
    */
   void drawStringFromRight (const char *txt, long x, long y, float kerning = 0);

   /**
    * Draw a centered string
    *
    * \param txt text to be drawn
    * \param x1 X coordinate of left of box to center text in
    * \param x2 X coordinate of right of box to center text in
    * \param y Y coordinate of top of text
    * \param kerning extra horizontal space between chars, in pixels
    */
   void drawStringCentered (const char *txt, long x1, long x2, long y, float  kerning = 0);

   /**
    * Get width of specified text in pixels, using loaded font
    *
    * \param txt text to get width of
    * \param kerning extra horizontal space between chars, in pixels
    *
    * \return width in pixels
    */    
   float    getStringWidth (const char *txt, float kerning = 0);

   /**
    * Get this font's height in pixels, independently of how
    * it was set (points or pixels)
    *
    * \return height in pixels
    */
   long getHeightPix (void);

   /**
    * Set this font's height in pixels
    *
    * \param nPixHeight font height in pixels
    */
   void setHeightPix (long nHeightPix);

   /**
    * Set this font's height in points
    *
    * \param nPtHeight font size in points for 96 dpp
    */
   void setHeightPt (long nHeightPt);

   /**
    * Set text color
    *
    * \param r red component (0-1)
    * \param g red component (0-1)
    * \param b red component (0-1)
    * \param alpha component (0-1)
    */
   void setColor (float  r, float g, float b , float alpha);

   /**
    * Set the text opacity
    *
    * \param blendFactor new opacity (0..1)
    */
   void setBlend (float blendFactor) { _fBlendFactor = blendFactor; }

   /** Texture for current face and size */
   KGraphic *_lpFontGraphics;
   KBatch *_lpFontBatch;

	//width of N chars
	float  getNCharWidth (char *txt, long charcount , float  kerning = 0 )  ;
	
	void	setSpaceAdjustment( float pixels ) ; //allows to change the size of the space character
	long	getSpaceCount( char *textPtr )  ;


	long		drawMultiline(const char *myText , long x1, long x2 , long y , long textstyle = 0 ,  float kerning = 0 ) ;
	void		setYSpacing( float yspacing ) ;
	KGraphic	*getKGraphicPtr( void ) ;

	long		getMultilineHeight( const char *myText , long x1, long x2 , long textstyle = 0 ,  float kerning = 0 ) ;

   /** Character coordinates in texture, for current face and size */
   struct KTrueTextChar _char[256-32];


private:   
  	float  _spaceAdjustment  , _ySpacing ;
 
	bool		movePosToNextChar( char *tmpBuffer ,  sStringInfoKtt  *strInfo )  ;
	bool		findStringMatchWidth( char  *strbuffer , long matchwidth , sStringInfoKtt  *strinfoptr , float kerning ) ;
	long		drawJustifiedMultilineText(  const char *myText , long x1, long x2 , long y , float kerning = 0 )  ;
	long		getJustifiedMultilineTextHeight( const char *myText , long x1, long x2 , long y , float kerning ) ;

   /**
    * Render current font and size to a texture
    */
   void render (void);

   /** FreeType stream for accessing font face through _resFont */
   FT_Stream _lpFtStream;

   /** FreeType font face */
   FT_Face _lpFreeTypeFace;



   /** Text color: red component */
   float _nTextR;

   /** Text color: green component */
   float _nTextG;

   /** Text color: blue component */
   float _nTextB;

   /** Text blending factor */
   float _fBlendFactor;


   /** Global instance of FreeType library */
   static FT_Library _lpFreeTypeLib;
};

#endif   /* K_TRUETEXT_H */
