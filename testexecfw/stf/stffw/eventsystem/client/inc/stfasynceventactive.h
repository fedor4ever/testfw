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
#ifndef STFASYNCEVENTACTIVE_H_
#define STFASYNCEVENTACTIVE_H_

#include <e32base.h>

#include <stfeventsystemdefs.h>
#include "stfasynceventlist.h"
#include <stfeventsystem.h>

// This class defines active object which represents asynchronous request
// to Event Server.
// On RunL each objects deletes itself.
class CAsyncEventActive: public CActive 
    {
    public:
        /**
        * NewL is first phase of two-phased constructor.
        */
        IMPORT_C static CAsyncEventActive* NewL(const TInt aOwnerId);

        /**
        * Destructor.
        */
        ~CAsyncEventActive();

        /** 
        * Starts active object
        */
        IMPORT_C void StartL(TRequestStatus* aStatus, TThreadId aStatusThreadId, const TDesC& aEventName, CAsyncEventList* aAsyncEventList);
        
        /** 
        * Checks if request to event server is in pending state
        */
        IMPORT_C TBool IsPending();

    private:
        /** 
        * C++ default constructor.
        */
        CAsyncEventActive(const TInt aOwnerId);

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();

        /**
        * Handles active object and deletes the object!
        */
        void RunL();

        /** 
        * DoCancel
        */
        void DoCancel();

        /** 
        * RunError
        */
        TInt RunError(TInt aError);
        
        /**
        * Complete caller status with specified code
        */
        TInt CompleteCallerStatus(TInt aCode);


    private:
        // List of active objects
        CAsyncEventList* iAsyncEventList;
        
        // Event name
        HBufC* iEventName;
        
        // Event system client
        REventSystem iEventSystem;
        
        // Status of caller
        TRequestStatus *iCallerStatus;
        
        // If of thread which owns caller status
        TThreadId iStatusThreadId;
        
        // Identifier of the owner of waiting event
        const TInt iOwnerId;
        
    }; /* class CAsyncEventActive */
    

#endif /* STFASYNCEVENTACTIVE_H_ */
// EOF
