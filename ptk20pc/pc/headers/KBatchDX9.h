
#ifndef		DEF_KBATCHDX9

#define		DEF_KBATCHDX9

#include	<windows.h>
#include	"KGraphicDx9.h"
#include	"KWindowDX9.h"
#include    "KGraphicDX9.h"
#include	"KBatch.h"
#include	"D3DX9MATH.h"
#include	"vector"

class		KBatchDX9 : public KBatch
{
private:
	static	float		_texelSX  ;
	static	float		_texelSY  ;
	static	float		_texelDX  ;
	static	float		_texelDY  ;
public:

	KBatchDX9( ) ;
	~KBatchDX9( ) ;
	
	KGraphicDX9	*_texturePtr;
	long _maxVertice, _verticeCount;
	IDirect3DVertexBuffer9	* _vertexBuffer;
	D3DXMATRIX _matWorld;
	PTKVERTEX _ptkVertex[4];
	void * vertexDataPtr;
	
	bool _blitColorChanged;
	float _blitRed, _blitGreen, _blitBlue, _blitAlpha;
	D3DBLEND _sourceAlpha, _destAlpha;
	//alpha additif ou normal ? ( false = noir transparent + ajout des couleurs )
	void setAlphaMode( long	alphamode );
	void setAlphaMode( long	alphamodeSource, long alphamodeDest );
	void setBlitColor( float r, float g, float b, float a );
	bool _textureflag;
	bool releaseBuffer();
	bool allocateBuffer( long polygoncount );
	void beginBatch( KGraphic* surface );
	void endBatch( void );
	
	
	//blit used for all the function , acts the same way as KGraphic::blitAlphaRect
	void blitAlphaRect(long x1, long y1, long x2, long y2, float destX, float destY, bool flipx, bool flipy );
	void blitAlphaRectFx( float	x1, float y1, float	x2, float y2, float	destX, float destY, float angle, float zoom, float blend, bool flipx, bool flipy, float centerX = 0, float centerY = 0 );
	void drawRect( float x1, float y1, float x2, float y2, float r, float g, float b,  float blend );
	
	int _quadCount;
	bool _allocated;

  	static	void setDirectX( LPDIRECT3D9 d3d9Ptr, LPDIRECT3DDEVICE9 d3d9Device );

	static LPDIRECT3D9 _d3d9Ptr; //the D3DDevice
	static LPDIRECT3DDEVICE9 _d3d9Device; //  rendering device

	static	void setTexelInfo( float sx, float sy, float dx, float dy );

	static std::vector<KBatchDX9*> currents;

	static void releaseCurrents();
	static void initCurrents();

	
};

#endif // DEF_KBATCHDX9
