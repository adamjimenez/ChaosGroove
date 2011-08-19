

#ifndef	DEF_KSPLINE

#define	DEF_KSPLINE 0

struct	KPoint
{
	double	x , y ;
	double	coefx , coefy ;
};

struct	KSegment
{
	bool			active ;
	KPoint		p1 ,p2 ;
};

class		KSpline
{
private:


public:
	KSpline( ) ;
	~KSpline( ) ;
	
	//methode avec un point de controle unique
	void		getPointFromCtrlPoint( KPoint	*kPoint , KPoint	*controlP1, KPoint	*controlP2 , KPoint	*controlP3 , double position) ;
	
	//methode avec 2 points de controles
	void		getPoint( KPoint	*kPoint , double	x1,double y1	,double	x2,double y2 , double coefX1 , double coefY1 , double coefX2 , double coefY2 , double position) ;

};



#endif
