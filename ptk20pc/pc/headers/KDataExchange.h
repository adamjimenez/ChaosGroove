#ifndef K_DATA_EXCHANGE__H
#define K_DATA_EXCHANGE__H

#include "KDataItem.h"

class KDataItemContainer
{
private:
	KDataItemContainer		*		_next		;
	KDataItemContainer		*		_prev		;
	
	KDataItem							*		_pkDataItem ;

	//---
	//   don't authorize copy constructor and operator=();
	KDataItemContainer( const KDataItemContainer & rkDataItem ) ;
	KDataItemContainer &operator=( const KDataItemContainer & rkDataItem ) ;
	
public:	
	KDataItemContainer( ) ;
	KDataItemContainer( KDataItem * pkDataItem ) ;
	KDataItemContainer( KDataItemContainer *pkNext , KDataItem * pkDataItem ) ;
	KDataItemContainer( KDataItemContainer *pkPrev , KDataItemContainer *pkNext , KDataItem * pkDataItem ) ;
	
	~KDataItemContainer( ) ;
	
	void										setKDataItem( KDataItem * item );
	void										setNext( KDataItemContainer * pkNext ) ;
	void										setPrev( KDataItemContainer * pkPrev ) ;
	
	KDataItem						*		getKDataItem( ) ;
	KDataItemContainer	*		getNext( ) ;
	KDataItemContainer	*		getPrev( ) ;
} ;

class KDataExchange
{
private:
	KDataItemContainer		*		_head  ;
	KDataItemContainer		*		_foot  ;
	int												_count ;
	
	int												_stacksize ;
	
public:
	KDataExchange( int stackSize ) ;
	~KDataExchange() ;
	
	void					push( KDataItem *item );
	KDataItem		*	peek( ) ;
	KDataItem		*	pop( ) ;
} ;

#endif // _DATA_EXCHANGE__H
