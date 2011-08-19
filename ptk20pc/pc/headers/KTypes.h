/******************************************************************************
 * PTK win32 type definitions
 ******************************************************************************
 * Rock And Roll (c) 2005 FunPause and Phelios Inc.
 ******************************************************************************
 * $Id: KTypes.h,v 1.5 2005/05/24 20:33:04 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_TYPES_H
#define  K_TYPES_H

/* Include definitions */
#define  NOMINMAX
#include <windows.h>
#include <ddraw.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <shlwapi.h>
#include	"KWindow.h"
#include	"KGraphic.h"
#include	"KMiscTools.h"
#include "KIniReader.h"
#include "KInput.h"
#include	"KPTK.h"
#include "KSound.h"

/** Type for string element */
typedef TCHAR K_CHAR;

/** Type for pointer to constant string */
typedef LPCTSTR K_LPCSTR;

/** Type for pointer to string */
typedef LPTSTR K_LPSTR;

/** Type for void pointer */
typedef LPVOID K_LPVOID;

/** Type for signed native integer */
typedef LONG K_LONG;

/** Type for unsigned native integer */
typedef ULONG K_ULONG;

/** Maximum path length */
#define K_MAXPATH MAX_PATH

extern "C" {

/** vsnprintf() */
#define K_vsnprintf _vsnprintf

/** snprintf() */
#define K_snprintf _snprintf

/** strncasecmp() */
#define K_strncasecmp _strnicmp

}  /* extern "C" */

#endif   /* K_TYPES_H */
