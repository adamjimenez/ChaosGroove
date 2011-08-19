/******************************************************************************
 * PTK internal text definitions
 ******************************************************************************
 * Rock And Roll (c) 2005 FunPause and Phelios Inc.
 ******************************************************************************
 * $Id: KMessage.h,v 1.1 2005/05/28 07:53:27 emarty Exp $
 ******************************************************************************
 */

#ifndef	K_TEXT_H
#define	K_TEXT_H

/* Include definitions */
#include "KTypes.h"

/** Text IDs */
enum {
	/* Errors */
	K_TEXT_ERR_ASSERT = 0,        /**< Assertion failed */
	K_TEXT_ERR_NOMEM,				   /**< Out of memory */
	K_TEXT_ERR_EXCEPTION,			/**< Application exception */
	K_TEXT_ERR_EXCEPTION_REPORT,	/**< Application exception: report */
	K_TEXT_ERR_EXCEPTION_QUIT,	   /**< Application exception: quit */

	/* Captions */
	K_TEXT_CAPTION_ERROR,			/**< Application error message box */
	K_TEXT_CAPTION_EXCEPTION,		/**< Application exception dialog box */
   K_TEXT_CAPTION_INFO,          /**< Application info message box */

   /* Start of product-specific text */
   K_TEXT_PRODUCT_START
};

#endif	/* F_K_TEXT_H */
