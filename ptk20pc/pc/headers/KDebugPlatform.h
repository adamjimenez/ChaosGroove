/******************************************************************************
 * Win32 debugging class definition
 ******************************************************************************
 * Garden Golf (c) 2005 FunPause
 ******************************************************************************
 * $Id: KDebugPlatform.h,v 1.1 2005/05/10 14:23:57 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_DEBUG_PLATFORM_H
#define  K_DEBUG_PLATFORM_H

/* Include definitions */
#include "KObjectBase.h"
#include	"KText.h"

/**
 * Generic class for handling crashes and failed assertions on Win32.
 *
 * The f_debug class installs unhandled exception handlers and supports
 * displaying failed assertion messages as well as stack traces, that
 * can be reported by users as a big help to fixing bugs.
 */

class KDebugPlatform : public KObjectBase {
public:
   /** Constructor */
   KDebugPlatform ();

   /** Destructor */
   ~KDebugPlatform ();

	/** Display assertion failure
	 *
	 * \param lpszFileName name of source file where assertion failed
	 * \param nLine line number where assertion failed
	 */
	static void assertionFailed (K_LPCSTR lpszFileName, unsigned int nLine);

private:
   /** Unhandled exception filter */
   static LONG WINAPI unhandledExceptionFilter (
      struct _EXCEPTION_POINTERS *lpExceptionInfo);

	/**
	 * Show exception dialog box
	 *
	 * \param lpszMessage message to be displayed in dialog box
	 */
	static void exceptionDialog (LPCTSTR lpszMessage);

   /**
    * Produce text representation of stack trace
    *
    * \param lpSite pointer to exception location
	 * \param lpStack pointer to top of stack
    * \param lpszBuffer buffer to store text representation in
	 *			            (must be at least LINE_SIZE * MAX_DEPTH bytes)
    */
   static void stackTrace (UINT32 *lpSite, UINT32 *lpStack,
		                     LPTSTR lpszBuffer);

	/**
	 * Quote message so as to e-mail it via mailto:
	 *
	 * \param lpszMessage message to be quoted
	 * \param lpszBuffer buffer to store quoted representation in
	 */
	static void quoteMessage (LPCTSTR lpszMessage, LPTSTR lpszBuffer);

   /**
    * Dialog box procedure for Exception box
    */
   static int CALLBACK exceptionDialogProc (HWND hwndDlg, UINT uMsg,
                                            WPARAM wParam, LPARAM lParam);
};

#endif   /* K_DEBUG_PLATFORM_H */
