/******************************************************************************
 * posix resource manager class definition
 ******************************************************************************
 * Garden Golf (c) 2005 FunPause
 ******************************************************************************
 * $Id: KResource.h,v 1.1 2005/04/15 15:32:22 emarty Exp $
 ******************************************************************************
 */

#ifndef  F_RESOURCE_PLATFORM_H
#define  F_RESOURCE_PLATFORM_H

/* Include definitions */
#include "KObjectListable.h"
#include "KList.h"

/** Resource opening mode */
typedef enum {
	K_RES_CLOSED = 0,				/**< No mode (closed) */
	K_RES_READ = 1000  			/**< Open for reads */
} KResourceMode;

/** Method results */
typedef enum {
	K_RES_OK = 0,					/**< Success */
	K_RES_STATE = 100,		   /**< Wrong state (no open resource) */
	K_RES_MODE,						/**< Wrong opening mode for operation */
	K_RES_PARAM,					/**< Bad parameters passed to method */
   K_RES_NOMEM,               /**< Out of memory for operation */
   K_RES_NOTFOUND,            /**< Error opening resource for reading */
   K_RES_CREATERR,            /**< Error creating resource for writing */
	K_RES_IOERR,     			   /**< Generic I/O failure */
   K_RES_BADFMT               /**< Resource in wrong format */
} KResourceResult;

/** How to intepret position for Seek(). */
typedef enum {
   K_RES_BEGIN = 0,           /**< From beginning of resource */
   K_RES_CURRENT,             /**< From current position in resource */
   K_RES_END                  /**< From end of resource */
} KResourceWhence;

/**
 * Generic class for loading and saving resources.
 *
 * The f_resource class provides a mechanism to load and save data, either
 * from independent files, or from packages (files containing more than
 * one resource)
 */

class KResource : public KObjectListable {
public:
	/** Constructor */
	KResource ();

	/** Destructor */
	~KResource ();

	/** Open resource from name
	 *
	 * \param lpszResName resource name
	 * \param nMode open mode (K_RES_READ or K_RES_WRITE)
	 *
	 * \return K_RES_OK for success,
	 *         K_RES_NOTFOUND if resource cannot be opened for reading,
    *         K_RES_CREATERR if resource cannot be created for writing,
	 *			  K_RES_STATE if resource is already opened,
	 *			  K_RES_PARAM if mode is invalid.
	 */
	KResourceResult open (const char *lpszResName, KResourceMode nMode);

   /** Open resource for reading from archive file. All resources opened
    * from the same file handle share the same read position.
    *
    * \param lpResource resource of archive file, open for reading,
    *              positioned on first byte of resource to be opened
    * \param nSize size of resource in bytes
    *
	 * \return K_RES_OK for success,
    *			  K_RES_STATE if resource is already opened, or supplied resource
    *                     is not open for reading,
	 *         K_RES_IOERR if I/O error
	 */
	KResourceResult open (KResource *lpResource, unsigned int nSize);

   /**
    * Open resource contained in the currently open archive for reading,
    * if this resource is an archive.
    *
    * \param lpszResName resource name
    * \param lpResource instance to open into, if successful
    *
    * \return K_RES_OK for success,
	 *         K_RES_NOTFOUND if resource cannot be opened for reading,
	 *         K_RES_STATE if failed due to no archive being currently open,
	 *         K_RES_IOERR if read error
	 */
   virtual KResourceResult openResource (const char *lpszResName,                          KResource *lpResource) {
			lpResource = lpResource ;
		lpszResName = lpszResName ;
		
      /* Base class does not model an archive but an actual resource */
      return K_RES_STATE;
   }

   /** Close currently open resource */
	void close (void);

	/** Change position in open resource, when reading. A subsequent
    * Read() will return data starting on the first byte of the specified
    * new position.
    *
    * \param nWhence how to interpret position (K_RES_BEGIN,CURRENT or END)
    * \param nPos new position
	 *
	 * \return K_RES_OK for success,
	 *         K_RES_STATE if failed due to no resource being currently open
    *                     for reading,
    *			  K_RES_PARAM if nWhence is invalid.
	 */
	KResourceResult seek (KResourceWhence nWhence, int nPos);

	/** Tell position in open resource, when reading.
	 *
	 * \param nPos returned current position
	 *
	 * \return K_RES_OK for success,
	 *         K_RES_STATE if failed due to no resource being currently open
    *                     for reading,
	 */
	KResourceResult tell (int &nPos);

	/** Read data from open resource
	 *
	 * \param lpBuffer pointer to first byte of buffer to read into
	 * \param nBytes size of data to be read, in bytes
	 *
	 * \return K_RES_OK for success,
	 *         K_RES_STATE if failed due to no resource being currently open,
	 *			  K_RES_MODE if failed due to resource being open for writing,
	 *         K_RES_IOERR if read error
	 */
	KResourceResult read (void *lpBuffer, size_t nBytes);

   /**
    * Add file to the list of archives that are looked up when a resource
    * is opened by Open(), before individual resource files are looked up.
    * This method enables the final product to package resources in one or
    * more archive files. The archive file remains open for the lifetime
    * of the product's process.
    *
    * \param lpszFileName name of archive file to be added
    *
    * \return K_RES_OK for success,
	 *         K_RES_NOTFOUND if archive cannot be opened for reading.
    */
   static KResourceResult addArchive (const char *lpszFileName);

private:
	/** Currently open file, NULL if none */
	FILE *_lpFile;

   /** true if file is contained in an archive (don't close) */
   bool _bFileInArchive;

	/** Mode of currently open file */
	KResourceMode _nMode;

   /** Position of beginning of resource in open file */
   unsigned int _nBeginPos;

   /** Size of resource (0 if unknown) */
   unsigned int _nSize;

   /** List of archives to open resources from, before looking at files */
   static KList<KResource> _lArchives;
};

#endif	/* F_RESOURCE_PLATFORM_H */
