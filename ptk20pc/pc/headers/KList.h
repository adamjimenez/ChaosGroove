/******************************************************************************
 * List template definition
 ******************************************************************************
 * (c) 2005 FunPause
 ******************************************************************************
 * $Id: KList.h,v 1.1 2005/06/02 05:11:50 emarty Exp $
 ******************************************************************************
 */

#ifndef  K_LIST_H
#define  K_LIST_H

/* Include definitions */
//#include "KDebug.h"

/* Forward declaration */
class KObjectListable;

/**
 * Template for a list of objects
 *
 * The KList class enumerates KObjectListable objects using a doubly
 * linked list. Any object (derived from KObjectListable) can be listed.
 */

template <class T> class KList : public KObjectListable {
public:
   /** Constructor */
   KList (void) :
      _nObjects (0)      	    /* Reset number of objects in list */
      {
         /* Point list head and tail at marker */
         this->_lpListNext = this;
         this->_lpListPred = this;
      }
      
      /** Destructor */
      ~KList (void) {
         /* Remove all elements left in list */
         while (!isEmpty ()) {
            T *lpCurHead;
            
            getHead (lpCurHead);
            remove (lpCurHead);
         }
         
         /* Make sure we counted right */
      //   KAssert (!_nObjects);
         
         /* Reset list head and tail */
         this->_lpListNext = (KObjectListable *) NULL;
         this->_lpListPred = (KObjectListable *) NULL;
      }
      
      /**
       * Add object to head of list.
       *
       * \param lpObject pointer to object to be added
       */   
      
      void addToHead (T *lpObject) {
         T *lpPrevHead;
         
         /* Make sure object isn't already in a list */
         KAssert (lpObject->_lpListNext == (T *) NULL);
         KAssert (lpObject->_lpListPred == (T *) NULL);
         
         /* Add object between head marker and head node */
         lpPrevHead = (T *) this->_lpListNext;
         lpObject->_lpListNext = lpPrevHead;
         lpObject->_lpListPred = this;
         this->_lpListNext = lpObject;
         lpPrevHead->_lpListPred = lpObject;
         
         /* Check head and tail linkage */
         KAssert (_lpListNext->_lpListPred == this);
         KAssert (_lpListPred->_lpListNext == this);
         
         /* Increase number of objects in list */
         _nObjects++;
      }
      
      /**
       * Add object to tail of list.
       *
       * \param lpObject pointer to object to be added
       */
      
      void addToTail (T *lpObject) {
         T *lpPrevTail;
         
         /* Make sure object isn't already in a list */
    //     KAssert (lpObject->_lpListNext == (T *) NULL);
//KAssert (lpObject->_lpListPred == (T *) NULL);
      
         /* Add object between tail node and tail marker */
         lpPrevTail = (T *) this->_lpListPred;
         lpObject->_lpListNext = this;
         lpObject->_lpListPred = lpPrevTail;
         this->_lpListPred = lpObject;
         lpPrevTail->_lpListNext = lpObject;
         
         /* Check head and tail linkage */
   //      KAssert (_lpListNext->_lpListPred == this);
   //      KAssert (_lpListPred->_lpListNext == this);
         
         /* Increase number of objects in list */
         _nObjects++;
      } 
      
      /**
       * Add object in the list nefore a specified object.
       *
       * \param pRefObject pointer to object to add object before
       * \param lpObject pointer to object to be added
       */
      
      void addBefore (T *pRefObject, T *lpObject) {
         T *pRefPred;
         
         /* Make sure refObject is in a list */
         KAssert (pRefObject->_lpListNext != (T *) NULL);
         KAssert (pRefObject->_lpListPred != (T *) NULL);
         
         /* Make sure object isn't already in a list */
         KAssert (lpObject->_lpListNext == (T *) NULL);
         KAssert (lpObject->_lpListPred == (T *) NULL);
         
         /* Add object between reference object and its predecessor */
         pRefPred = (T *) pRefObject->_lpListPred;
         lpObject->_lpListNext = pRefObject;
         lpObject->_lpListPred = pRefPred;
         pRefPred->_lpListNext = lpObject;
         pRefObject->_lpListPred = lpObject;
         
         /* Check head and tail linkage */
         KAssert (_lpListNext->_lpListPred == this);
         KAssert (_lpListPred->_lpListNext == this);
         
         /* Increase number of objects in list */
         _nObjects++;
      } 
      
      /**
       * Add object in the list after a specified object.
       *
       * \param pRefObject pointer to object to add object after
       * \param lpObject pointer to object to be added
       */
      
      void addAfter (T *pRefObject, T *lpObject) {
         T *pRefNext;
         
         /* Make sure refObject is in a list */
         KAssert (pRefObject->_lpListNext != (T *) NULL);
         KAssert (pRefObject->_lpListPred != (T *) NULL);
         
         /* Make sure object isn't already in a list */
         KAssert (lpObject->_lpListNext == (T *) NULL);
         KAssert (lpObject->_lpListPred == (T *) NULL);
         
         /* Add object between reference object and its successor */	
         pRefNext = (T *) pRefObject->_lpListNext;
         lpObject->_lpListNext = pRefNext;
         lpObject->_lpListPred = pRefObject;
         pRefObject->_lpListNext = lpObject;
         pRefNext->_lpListPred = lpObject;
         
         /* Check head and tail linkage */
         KAssert (_lpListNext->_lpListPred == this);
         KAssert (_lpListPred->_lpListNext == this);
         
         /* Increase number of objects in list */
         _nObjects++;
      } 
      
      /**
       * Remove object from list.
       *
       * \param lpObject pointer to object to be removed
       */
      
      void remove (T *lpObject) {
         T *pNext, *pPred;
         
         /* Make sure object is in a list */
     //    KAssert (lpObject->_lpListNext);
  //       KAssert (lpObject->_lpListPred);
  //       KAssert (lpObject->_lpListNext->_lpListPred == lpObject);
 //        KAssert (lpObject->_lpListPred->_lpListNext == lpObject);
         
         /* Point at successor and predecessor */
         pNext = (T *) lpObject->_lpListNext;
         pPred = (T *) lpObject->_lpListPred;
         
         /* Make sure the node was the successor's predecessor */
       //  KAssert (lpObject == pNext->_lpListPred);
         
         /* Make sure the node was the predecessor's successor */
     //    KAssert (lpObject == pPred->_lpListNext);
         
         /* Remove node */
         pPred->_lpListNext = pNext;
         pNext->_lpListPred = pPred;
         
         /* Clear node's links */
         lpObject->_lpListNext = (T *) NULL;	 
         lpObject->_lpListPred = (T *) NULL;
         
         /* Check head and tail linkage */
     //    KAssert (_lpListNext->_lpListPred == this);
      //   KAssert (_lpListPred->_lpListNext == this);
         
         /* Decrease number of objects in list */
         _nObjects--;
      }
      
      /** Does list have objects
       *
       * \return true if list contains at least one object
       */
      bool isEmpty (void) {
         return (this->_lpListNext == this);
      }
      
      /**
       * Is object at head of list.
       *
       * \param rObject reference to object to be checked
       *
       * \return true if specified object is at head of list
       */
      bool isHead (T *&rObject) {
         return (rObject == this->_lpListNext);
      }
      
      /**
       * Is object at tail of list.
       * \param rObject reference to object to be checked
       *
       * \return true if specified object is at tail of list
       */
      bool isTail (T *&rObject) {
         return (rObject == this->_lpListPred);
      }
      
      /**
       * Get object at head of list
       *
       * \param rObject returned reference to object at head 
       *
       * \return true if list had at least one object and head was returned
       */
      bool getHead (T *&rObject) {
         /* Give up if no objects in list */
         if (this->_lpListNext == this) return false; 
         
         /* Return head object */
         rObject = (T *) this->_lpListNext;
         return true;
      }
      
      /**
       * Get object at tail of list
       *
       * \param rObject returned reference to object at tail 
       *
       * \return true if list had at least one object and tail was returned
       */
      bool getTail (T *&rObject) {
         /* Give up if no objects in list */
         if (this->_lpListPred == this) return false; 
         
         /* Return tail object */
         rObject = (T *) this->_lpListPred;
         return true;
      }      
      
      /**
       * Get next object in list
       *
       * \param rObject returned reference to next object
       *
       * \return true if wasn't end of list, and next was returned
       */
      bool getNext (T *&rObject) {
         T *lpNext = (T *) rObject->_lpListNext;
         
         /* Give up if it was last object in list */
         if (lpNext == (T *) this) return false;
         
         /* Make sure we're not looping over one object due to broken links */
   //      KAssert (lpNext != rObject);
         
         /* Make sure we were not passed an invalid pointer */
   //      KAssert (lpNext->_lpListPred == rObject);
         
         /* Return it */
         rObject = lpNext;
         return true;
      }
      
      /**
       * Get previous object in list
       *
       * \param rObject returned reference to next object
       *
       * \return true if wasn't end of list, and previous was returned
       */
      bool getPred (T *&rObject) {
         T *lpPred = (T *) rObject->_lpListPred;
         
         /* Give up if it was first object in list */
         if (lpPred == (T *) this) return false;
         
         /* Make sure we're not looping over one object due to broken links */
         KAssert (lpPred != rObject);
         
         /* Make sure we were not passed an invalid pointer */
         KAssert (lpPred->_lpListNext == rObject);
         
         /* Return it */
         rObject = lpPred;
         return true;
      }
      
      /**
       * Get number of objects
       */
      
      int getCount (void) {
         return _nObjects;
      }
      
private:
      /** Number of objects in list */
      int _nObjects;           
};

#endif   /* K_LIST_H */
