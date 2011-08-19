#define		DIRECTSOUND_VERSION 0x0300
//12
#ifndef KSOUND__H
#define KSOUND__H

#include <windows.h>

#include "dsound.h"
#include	<stdio.h>
#include	"vorbisfile.h"

//sampletype ( for loadSampleFromPtr )
#define		SAMPLE_WAV		0
#define		SAMPLE_OGG		1

struct		RiffPCMWaveHeader
{
	unsigned	long		riff ;
	unsigned	long		lengthAfterHeader ;
	unsigned	long		wave ;
	unsigned	long		fmt ;	
	unsigned	long		chunkSize  ;	
	
	short 					wFormatTag;         // Format category
	short 					wChannels;          // Number of channels
	unsigned long			dwSamplesPerSec;    // Sampling rate
	unsigned long			dwAvgBytesPerSec;   // For buffer estimation
	short 					wBlockAlign; 
	short						nBitsPerSample ;
	unsigned long			data ;
	unsigned long			dataLength ;
	
};


struct	sDataInfo
{
	unsigned long	data ;
	unsigned long	datalength ;
};

#define			MAX_SOUNDBUFFERS		9

struct PHELIOSWAVEFORMATEX
{
    WORD        wFormatTag;         /* format type */
    WORD        nChannels;          /* number of channels (i.e. mono, stereo...) */
    DWORD       nSamplesPerSec;     /* sample rate */
    DWORD       nAvgBytesPerSec;    /* for buffer estimation */
    WORD        nBlockAlign;        /* block size of data */
    WORD        wBitsPerSample;     /* number of bits per sample of mono data */
    WORD        cbSize;             /* the count in bytes of the size of */
                                    /* extra information (after cbSize) */
} ;


struct		sModuleInfo
{
	void	*_moduleData ;
	void	*_moduleRenderer ;
	bool	allowLooping ;
};

class KSound
{
private:
	short				_soundBufferCount ;
	static int		_instanceCount ;
	static 			LPDIRECTSOUND directSoundPtr ;  
	static long		_masterSFXvolume , _masterMUSICvolume ; //0 to 100
	long				_channelVolume		; //0-100
	long				_loopState ;
	char				_type ; //type of what's being played
	long				_whatPartIsPlaying  ;
	long				_currentBeingPlayed ;
	IDirectSoundBuffer			*createDSoundBuffer( short	bufferid , unsigned long	size ,bool withNotifier ) ;
	
	//frees a sound channel
	void						freeChannel( void ) ;
	
	unsigned long			loadFileToRam( const char *filename ) ;
	void						freeAllocatedData( void ) ;
	void						restoreSample( void ) ;
	bool						loadOggSample( const char *sampleName )  ;
	bool						loadOggSampleFromPtr(const char *dataSampePtr, unsigned long datalength )  ;
	void						clearSoundBuffer( short bufferID , long buffersize  ) ; 
	bool						startNotifiers( void ) ; //double buffering notifiers	
	static	void			precalcTable( void );
	unsigned long			transferFileToRam( char *dataPtr , unsigned long	dataLen ) ;

	//modules
	static	int				loopFunc( void	*dataPtr ) ;
	sModuleInfo			_moduleStorage ;  //module storage
	static	void			doubleBufferThread( void		*data ) ;
	void						killModule( void );
	void						killThread( void );
	
	PHELIOSWAVEFORMATEX			*_waveFormatEx ;		//format of the wav file

	unsigned	char				*_wavDataStart ;		//real start of wav data
	unsigned	long				_wavLength ;

	PHELIOSWAVEFORMATEX			_oggFormatEx ;		//format of the wav file
	OggVorbis_File 				_vorbisFile; 		//vorbis file for streaming
  	bool						_oggFile ;			//if an ogg stream is present and loaded in memory

	//for wav loading ;
	bool 	parseWav( ) ;

	long		 ParseWavHeader( unsigned char		*dataPtr , unsigned long dataLen ) ;
	
	long		flipLong( long value );
	short		flipShort( short value );
	float		flipFloat( float value ) ;

public:
	KSound();
	~KSound();

//only for expert users do not use if you don't know what you're doing !	
	IDirectSoundBuffer			*_directSoundBuffer[MAX_SOUNDBUFFERS]  ;
	LPDIRECTSOUNDNOTIFY _dsNotify ;	//notifiers
	HANDLE							hEvents[4] , hThread;
	unsigned long						threadID ;
	static	long						buffersFilled ;
	void									fillModBuffer( unsigned long bf  ) ;
	void									fillOggBuffer( unsigned long bf  ) ;
	bool									_terminate , _terminated , 	_ready  ; //ask for termination / is thread terminated ?
	bool								_oggCompleted ;
	unsigned	long				_oggPosition ;			//position in the buffer
	unsigned	long				_oggFileLen ; //ogg file length
	unsigned	char				*_wavData ;				//pointer on the wav
//****************************************************************

	bool						initDone() ;
	const char *			getError() ;
	bool						pollStream( bool forcefill) ;

//channels	
	bool 						isPlaying( void ) ;
	void						setVolume( int volume ) ;
	void						setPan( long pan ) ; //-100 / 100
//misc global control
	static	void						setGlobalVolumes( int volumeMusic , int volumeSFX ) ;

//modules  ( xm etc.. )
	bool						loadModule( const char *moduleName ) ;
	bool						isMusicPaused( void ) ;
	void						playModule( bool loop ) ;
	void						pauseModule( void ) ;
	void						continueModule( void ) ;
	void						stopModule(void ) ;
	void						setModulePosition(  int position ) ;	//jump dans les patterns
//wavs, aiff..
	bool						loadSample( const char *sampleName , int volume=100 , bool  loop=false, short nbchannels =1) ;
	bool						loadSampleFromPtr( const char *sampleDataPtr , unsigned long dataLength , long	sampletype , int volume=100 , bool  loop=false, short nbchannels =1) ;

	void						playSample( void ) ;
	void						stopSample( void ) ;

	KSound*						playSample( long rate, long vol, long pan ,  long loop  ) ;
	void						pauseSample( void ) {}
	void						continueSample( void ) {}

	void						setFrequency( long	frequency ) ;

//streams ( ogg only )
	bool						loadStream( const char *streamName  ) ;	
	void						playStream( bool loop = true ) ;
	void						stopStream( void ) ;

	bool						pollModule( void ) ;
	//writes a mod to disk, as raw PCM data 16bits 44khz
	//ON INTEL->raw 16 bits / signed / stereo / little endian / 0 byte offset
	bool						modToRaw( char		*filename , unsigned long maxbytes ) ;
	bool						loadStreamFromPtr( char	*oggPtr , unsigned long datasize ) ;
	
private:
	void						init();
} ;

#endif // KSOUND__H
