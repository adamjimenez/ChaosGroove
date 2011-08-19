#ifndef K_DATA_ITEM__H
#define K_DATA_ITEM__H

typedef enum
{
	kCopyData				=		1 ,		// Use this if class must alloc memory to copy datas
	kOnlyStoreData	=		2			// Use this if class don't have to allocate memory, only store pointer.
} KStoreType ;

typedef enum
{
	kTcp						=		1 ,
	kUdp						=		2
} KPacketType ;

class KDataItem
{
protected:
	char		*				_buffer			 ;
	long						_size				 ;
	KStoreType			_kStoreType  ;
	KPacketType			_kPacketType ;
	
public:
	KDataItem( KStoreType kStoreType ) ;
	
	virtual ~KDataItem() ;

	KDataItem( const KDataItem & rkDataItem ) ;
	KDataItem &operator=( const KDataItem & rkDataItem ) ;
	
	void				set( KPacketType kPacketType, void *buffer , long size ) ;
	void	*			get( ) ;
	long				getSize();
	
	KPacketType getPacketType();
	bool				isUdpPacket( ) ;
	bool				isTcpPacket( ) ;
} ;


#endif // K_DATA_ITEM__H
