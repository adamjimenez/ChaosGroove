//april 29 2006
#ifndef		KS2_EMPTY
#include	<windows.h>

#include <al/al.h>
#include <al/alc.h>
#include <al/alu.h>
#include <al/alut.h>
#include	<vorbisfile.h>




#define		KS2_EMPTY					0
#define		KS2_SAMPLEWAV			1
#define		KS2_SAMPLEOGG			2
#define		KS2_STREAM				3
#define		KS2_MODULE				4

// Struct that contains the pointer to our file in memory
struct SOggFile
{
	char*	dataPtr;			// Pointer to the data in memoru
	int			dataSize;			// Sizeo fo the data
	int			dataRead;			// How much data we have read so far
	bool		loop;
};

struct		sModuleInfo
{
	void	*_moduleData ;
	void	*_moduleRenderer ;
	bool	_modCompleted ;
	bool	loop ;
};

class		KSound2
{
private:
	//used for streaming
	OggVorbis_File 	_vorbisFile; 
	ov_callbacks		_vorbisCallbacks;
	SOggFile			_oggInMemory ;	
	bool			 _isStreamPlaying ;  
	bool			_loopStream ;
	ALsizei		_oggFreq;
	ALenum		_oggFormat ;
	
	//modules
	sModuleInfo	_moduleData ;
	bool 				updateModule( void ) ;
	void				fillModBuffer(ALuint bufferId ) ;
	static			int			loopFunc( void	*dataPtr ) ;
	
	void			bindBufferToSources( bool loop ) ;
	bool			loadOggSample( const char *filename ) ;
	void			fillOggBuffer(ALuint bufferId ) ;
	void			emptyBuffers( void ) ;
	//thread
	static	int			pollingThread( void ) ;
	static	void			startThread( void ) ;
	static	void			closeThread( void ) ;
	
	
	static	HANDLE	_hThread ;
	static	unsigned	long			_threadID ;

	bool 			updateStream( void ) ;


	unsigned long		loadFileToRam( const char *filename ) ;

public:


	static	void			pollStreamList( void ) ;

	KSound2( ) ;
	~KSound2( ) ;

	unsigned long		_oggPosition , _oggFileLen ;
	char					*_rawData ;

ALuint 		*_soundBuffer;		//buffer holding the data
long			_bufferCount ;		//amount of buffers for that sound - always 4 for a stream

//sources for sound emitting
long			_sourcesCount ;		//amount of sources ( used to play multiple times the same sound )
ALuint		*_soundSource ;

//ALuint		_sndSource ;

//device to play sounds
static	ALCdevice* 	_defaultDevice ;
static	long		_instanceCount ;
long		_sourcePlaying ;


ALfloat _sourcePosition[3] ;
// Velocity of the source sound.
ALfloat _sourceVelocity[3] ;

	char		_errorMsg[256] ;


	short			_type ;
	
	static			bool			_initDone ;
	//global volumes
	static		float			_globalMusic , _globalSoundFx ;

	//global volumes 0-1.0f
	static		void			setGlobalVolumes( float music, float sfx ) ;

	//modifies the listener settings
	void 			setListenerValues( void ) ;


	//stream functions
	bool			loadStream( const	char*	filename , bool loop=true  ,  float volume=1.0f) ;
	void			playStream( void ) ;
	void			stop( void ) ;
	void			freeStream( void )  ;

	//modules  functions	
	bool			loadModule( const	char*	filename , bool loop=false , float volume=1.0f ) ;
	void			playModule( void ) ;
	void			freeModule( void ) ;
	void			setModulePosition(  int position ) ;

	//samples functions	
	bool			loadSample( const	char*	filename , bool loop=false , float volume=1.0f   , short		maxChannels =1 ) ;
	void			freeSample( void ) ;

	//returns the slot that's being played
	long			playSample( void ) ;
	//stops sample
	void			stopSample( long		channel ) ;
	bool			isPlaying( long channel = 0  ) ;
	
	//sets the volume of a sound  0-1.0f
	void			setVolume( float 	volume ) ;

	//pitch multiplier ( default is 1.0 )
	void			setPitch( float pitch ) ;

	//sets the position  (  x==-1 sets the sound completely on the left , x==1 on the right)
	void			setPosition( float x, float y , float z ) ;
};


#endif
