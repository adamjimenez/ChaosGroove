/******************************************************************************
 * PTK debugging class definition
 ******************************************************************************
 * Rock And Roll (c) 2005 FunPause and Phelios Inc.
 ******************************************************************************
 * $Id: KDebug.h,v 1.2 2005/05/10 20:04:35 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_DEBUG_H
#define  K_DEBUG_H

/* Include definitions */
#include "KTypes.h"

/** Check assertions */
#define KAssert(__cond) \
   if (!(__cond)) KDebug::assertionFailed (__FILE__, __LINE__);

/** Product support e-mail address */
#define	SUPPORT_EMAIL	"support@funpause.com"

/* Include platform-specific class */
#include "KDebugPlatform.h"

/**
 * Generic class for handling crashes and failed assertions.
 *
 * The KDebug class installs unhandled exception handlers and supports
 * displaying failed assertion messages as well as stack traces, that
 * can be reported by users as a big help to fixing bugs.
 */

class KDebug : public KDebugPlatform {
public:
   /** Constructor */
   KDebug ();

   /** Destructor */
   ~KDebug ();

   /** Display information message.
    *
    * \param lpszMsg message to be displayed (msg includes formatting)
    * \param ... Data to be inserted in message according to format
    */
   static void info (K_LPCSTR lpszMsg, ...);

   /** Display error message and exit.
    *
    * \param nMsg index of message to be displayed (msg includes formatting)
    * \param ... Data to be inserted in message according to format
    */
   static void abort (unsigned int nMsg, ...);
};

#endif   /* K_DEBUG_H */
