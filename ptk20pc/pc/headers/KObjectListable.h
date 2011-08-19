/******************************************************************************
 * Listable object class definition
 ******************************************************************************
 * Atlantis (c) 2005 FunPause
 ******************************************************************************
 * $Id: KObjectListable.h,v 1.2 2005/06/06 16:01:03 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_OBJECT_LISTABLE_H
#define  K_OBJECT_LISTABLE_H

/* Include definitions */
#include "KObjectBase.h"

/** Class for listable objects
 *
 * The KObjectListable class enhances KObjectBase with list membership.
 */

class KObjectListable : public KObjectBase {
public:   
   /** Constructor */
   KObjectListable (void);

   /** Destructor */
   virtual ~KObjectListable ();

   /** Copy constructor */
   KObjectListable (const KObjectListable &refObject);

   /** Copy operator */
   KObjectListable &operator = (KObjectListable refObject);

   /** Pointer to next in list */
   KObjectListable *_lpListNext;

   /** Pointer to predecessor in list */
   KObjectListable *_lpListPred;
};

#endif   /* K_OBJECT_LISTABLE_H */
