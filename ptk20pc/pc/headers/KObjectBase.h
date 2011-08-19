/******************************************************************************
 * PTK base object class definition
 ******************************************************************************
 * Rock And Roll (c) 2005 FunPause and Phelios Inc.
 ******************************************************************************
 * $Id: KObjectBase.h,v 1.2 2005/05/19 16:00:32 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_OBJECTBASE_H
#define  K_OBJECTBASE_H

/* Include definitions */
#include "KTypes.h"

/** Get minimum of _a and _b */
#define K_MIN(_a,_b) ((_a) < (_b)) ? (_a) : (_b)

/** Get maximum of _a and _b */
#define K_MAX(_a,_b) ((_a) > (_b)) ? (_a) : (_b)

/** Get sign of _a */
#define K_SIGN(_a) ((_a) < 0) ? -1 : 1

/**
 * Generic base class for all objects.
 *
 * The KObjectBase class is the base class of all objects, generic or
 * game-specific. It doesn't provide anything except for a virtual destructor.
 */

class KObjectBase {
public:
   /** Destructor */
   virtual ~KObjectBase ();
};

#endif   /* K_OBJECTBASE_H */
