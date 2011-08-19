#include	<windows.h>
//#include	"KGraphic.h"
#include	"KGraphic.h"
#include	"KBatch.h"



#ifndef		DEF_KFONT


#define		DEF_KFONT

#define		TEXTSTYLE_LEFT			0
#define		TEXTSTYLE_RIGHT		1
#define		TEXTSTYLE_CENTER	2
#define		TEXTSTYLE_JUSTIFIED	3

struct	sStringInfo
{
long		startpos, endpos ;
bool		leftAlign ;
};



//KFont

struct	KFont
{
  unsigned char _c  ;
  short  _x1 ;
  short  _y1 ;
  short  _x2 ;
 short  _y2 ;
 short  _w  ;
 short  _h  ;
}  ;


class		KText
{
	private:
		KBatch			*_textBatcher ; //makes texts operations much faster
		KGraphic		*_surf_Fonts ;	
		KFont			*_fontTable ;

		//retourne l'index dans la table de fonte
		long	getCharIndex( char	c ) ;
	
	  	float  _spaceAdjustment  , _ySpacing ;
 
		bool		movePosToNextChar( char *tmpBuffer ,  sStringInfo  *strInfo )  ;
		bool		findStringMatchWidth( char  *strbuffer , long matchwidth , sStringInfo  *strinfoptr , float kerning ) ;
		long		drawJustifiedMultilineText(  const char *myText , long x1, long x2 , long y , float kerning = 0 )  ;
		long		getSpaceCount( char *textPtr ) ;

	public:
		KText( ) ;
		KText( const char	*fontName , KFont	*table ) ;
		~KText( ) ;

		float	_blendFactor ,_r , _g , _b ;
		
		//attache une table de fonte
		void	setFontTable( KFont	*fontTable ) ;
		void	loadFontBitmap( char	*font_Filename ) ;
		//if you use several fonts on the same bitmap, that can be useful
		void	setKGraphic( KGraphic	*picPtr ) ;
		// le parametre kerning permet d'ajuster l'espace entre les lettres
		// un kerning negatif rapprochera les caracteres entre eux.
		
		//write from left to right
		void	drawStringFromLeft( const	char	*txt , long x , long y, float kerning = 0  ) ;
		//write, right aligned ( x is the rightmost coordinnate )
		void	drawStringFromRight( const	char	*txt , long x , long y, float kerning = 0  ) ;
		//affiche le texte, centré dans x1-x2
		void	drawStringCentered( const	char	*txt , long x1,long x2 , long y , float kerning = 0 ) ;


		//affiche le texte en partant de la gauche
		void	drawStringFromLeft( long		value , long x , long y, float kerning = 0  ) ;
		//affiche le texte en partant de la droite
		void	drawStringFromRight(long		value, long x , long y, float kerning = 0  ) ;
		//affiche le texte, centré dans x1-x2
		void	drawStringCentered( long		value, long x1,long x2 , long y , float kerning = 0 ) ;

		
		//alpha blending  ( useful for fades )
		void	setColor( float r , float g , float b , float		blendFactor ) ;
	
		///returns the picture associated with the bitmap
		KGraphic*getKGraphicPtr( void ) ;
		
		float	getStringWidth( const	char	*txt , float kerning = 0 ) ;
		float	getNCharWidth( const	char	*txt ,long ccount ,  float kerning  )  ;

		void	drawNCharFromLeft( char	*txt , long nchar , long x , long y,float  kerning )  ;
		void	setYSpacing( float yspacing ) ;
		void	setSpaceAdjustment( float pixels )  ;  //kerns the space character ( so you can have bigger/smaller  spaces between words )
		long	drawMultiline( const char *myText , long x1, long x2 , long y, long textstyle=TEXTSTYLE_LEFT , float kerning = 0  ) ;

};


#endif
