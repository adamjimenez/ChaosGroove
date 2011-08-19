/******************************************************************************
 * Definitions for handling tape archives (.tar) of resources
 ******************************************************************************
 * (c) 2005 FunPause
 ******************************************************************************
 * $Id:$
 ******************************************************************************
 */

#ifndef  K_RESOURCE_ARCHIVE_H
#define  K_RESOURCE_ARCHIVE_H

/*
 * Header file for public domain tar (tape archive) program.
 *
 * @(#)tar.h 1.20 86/10/29	Public Domain.
 *
 * Created 25 August 1985 by John Gilmore, ihnp4!hoptoad!gnu.
 */

/*
 * Header block on tape.
 *
 * I'm going to use traditional DP naming conventions here.
 * A "block" is a big chunk of stuff that we do I/O on.
 * A "record" is a piece of info that we care about.
 * Typically many "record"s fit into a "block".
 */
#define	RECORDSIZE	512
#define	NAMSIZ	100
#define	TUNMLEN	32
#define	TGNMLEN	32

union k_tar_record {
	char		charptr[RECORDSIZE];
	struct header {
		char	name[NAMSIZ];
		char	mode[8];
		char	uid[8];
		char	gid[8];
		char	size[12];
		char	mtime[12];
		char	chksum[8];
		char	linkflag;
		char	linkname[NAMSIZ];
		char	magic[8];
		char	uname[TUNMLEN];
		char	gname[TGNMLEN];
		char	devmajor[8];
		char	devminor[8];
	} header;
};

/* The checksum field is filled with this while the checksum is computed. */
#define	CHKBLANKS	"        "	/* 8 blanks, no null */

/* The magic field is filled with this if uname and gname are valid. */
#define	TMAGIC		"ustar  "	/* 7 chars and a null */

/* The linkflag defines the type of file */
#define	LF_OLDNORMAL	'\0'		/* Normal disk file, Unix compat */
#define	LF_NORMAL	'0'		/* Normal disk file */
#define	LF_LINK		'1'		/* Link to previously dumped file */
#define	LF_SYMLINK	'2'		/* Symbolic link */
#define	LF_CHR		'3'		/* Character special file */
#define	LF_BLK		'4'		/* Block special file */
#define	LF_DIR		'5'		/* Directory */
#define	LF_FIFO		'6'		/* FIFO special file */
#define	LF_CONTIG	'7'		/* Contiguous file */

/* End of John Gilmore's public domain header */

/* Include definitions */
#include "KResource.h"

/**
 * Class for reading resources packaged in a Tape Archive
 *
 * The KResourceArchive class searches through a tape archive for a resource by name,
 * and allows to read the resource contents. The archive's extension may
 * be .tar or a product-specific extension such as .gpk, but in any case
 * the file is created with the tar utility as part of the product
 * packaging and delivery process.
 */

class KResourceArchive : public KResource {
public:
   /** Constructor */
   KResourceArchive ();

   /** Destructor */
   ~KResourceArchive ();

	/** Open archive from name
	 *
	 * \param lpszArchiveName archive name
	 *
	 * \return F_RES_OK for success,
	 *         F_RES_NOTFOUND if archive cannot be opened,
	 *			  F_RES_STATE if an archive is already opened.
	 */
	KResourceResult open (const char *lpszArchiveName);

   /**
    * Open resource contained in the currently open archive for reading,
    * if this resource is an archive.
    *
    * \param lpszResName resource name
    * \param lpResource instance to open into, if successful
    *
    * \return F_RES_OK for success,
	 *         F_RES_NOTFOUND if resource cannot be opened for reading,
	 *         F_RES_STATE if failed due to no archive being currently open,
	 *         F_RES_IOERR if read error
	 */
   virtual KResourceResult openResource (const char *lpszResName,
                                         KResource *lpResource);
};

#endif   /* K_RESOURCE_ARCHIVE_H */
