/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/
#ifndef STFASYNCEVENTLIST_H_
#define STFASYNCEVENTLIST_H_

#include <e32base.h>

#include <stfeventsystemdefs.h>

class CAsyncEventActive;

// This class defines list of asynchronous event calls.
// It is planned that only wait events will be realised in a real asynchronous
// way. All others will be synchronous, even they pretend to be asynchronous.
class CAsyncEventList: public CBase 
    {
    public:
        /**
        * NewL is two-phased constructor.
        */
        IMPORT_C static CAsyncEventList* NewL();
        
        /**
        * Destructor.
        */
        ~CAsyncEventList();

        /**
        * Add active object to the list.
        */
        IMPORT_C void AddAsyncEventL(CAsyncEventActive* aAsyncEvent);
        
        /**
        * Remove active object from the list.
        */
        IMPORT_C void RemoveAsyncEventL(CAsyncEventActive* aAsyncEvent);

    private:
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Default C++ constructor.
        */
        CAsyncEventList();
    
        /**
        * Cancel and remove all active objects from the list.
        */
        void CancelAllAsyncEvents();
        
        
    private:
        /**
        * Array of all existing active objects waiting for asynchronous 
        * completion.
        */
        RPointerArray<CAsyncEventActive> iAsyncEvents;

    }; /* class CAsyncEventList */
    

#endif /* STFASYNCEVENTLIST_H_ */
// EOF
