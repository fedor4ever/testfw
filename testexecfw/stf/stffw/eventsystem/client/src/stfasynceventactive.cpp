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
#include <e32std.h>
#include <e32svr.h>

#include <stfeventsystemerrorcodes.h>
#include "stfasynceventactive.h"
#include <stfasynceventlist.h>


// Implementation of asynchronous event Active Object

/**
* NewL is first phase of two-phased constructor.
*/
EXPORT_C CAsyncEventActive* CAsyncEventActive::NewL(const TInt aOwnerId)
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventActive::NewL()"));
    CAsyncEventActive* self = new (ELeave) CAsyncEventActive(aOwnerId);
    CleanupStack::PushL(self);

    // Construct the object
    self->ConstructL();

    // Remove from cleanup stack
    CleanupStack::Pop(self);

    return self;
    }

/** 
* C++ default constructor.
*/
CAsyncEventActive::CAsyncEventActive(const TInt aOwnerId): CActive(CActive::EPriorityStandard), iOwnerId(aOwnerId)
    {
    CActiveScheduler::Add(this);
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::CAsyncEventActive() creating active object [%x]"), this);
    }

/**
* By default Symbian OS constructor is private.
*/
void CAsyncEventActive::ConstructL()
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventActive::ConstructL()"));
    }

/**
* Destructor.
*/
CAsyncEventActive::~CAsyncEventActive()
    {
    Cancel();
    iEventSystem.Close();
    delete iEventName;
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::~CAsyncEventActive() deleting active object [%x]"), this);
    }

/** 
* Start active object.
* If list is provided, active object will cooperate with it adding and deleting itself from the list.
* Also active object will delete itself if it's on the list.
*/
EXPORT_C void CAsyncEventActive::StartL(TRequestStatus* aStatus, TThreadId aStatusThreadId, const TDesC& aEventName, CAsyncEventList* aAsyncEventList)
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::StartL() eventName=[%S]"), &aEventName);
    // Check if list of asynchronous objects and event name are not set.
    // It would mean that method StartL was already called.
    __ASSERT_ALWAYS(iAsyncEventList == NULL, User::Panic(_L("CAsyncEventActive1"), EEventSystemListAlreadySet));
    __ASSERT_ALWAYS(iEventName == NULL, User::Panic(_L("CAsyncEventActive2"), EEventSystemListAlreadySet));
    
    // Set caller's status and other data
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::StartL() registering status [%x]"), aStatus);
    iCallerStatus = aStatus;
    *iCallerStatus = KRequestPending;
    iStatusThreadId = aStatusThreadId;
    iAsyncEventList = aAsyncEventList;
    iEventName = aEventName.AllocL();
     
    // Open connection to server
    User::LeaveIfError(iEventSystem.Connect());
    
    // Add object to list
    if(iAsyncEventList)
        {
        iAsyncEventList->AddAsyncEventL(this);
        }
    
    // Call wait event
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::StartL() SendReceive eventName=[%S]"), iEventName);
    TIpcArgs args(iEventName, iOwnerId);
    iEventSystem.SendReceive(EEventSystemWaitEvent, args, iStatus);
    
    // Activate object    
    SetActive();
    }

/**
* Handles active object and deletes itself!
*/
void CAsyncEventActive::RunL()
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::RunL()"));
    // Complete request from caller
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::RunL() completing status [%x] with [%d]"), iCallerStatus, iStatus.Int());
    User::LeaveIfError(CompleteCallerStatus(iStatus.Int()));
    
    // Remove from list
    if(iAsyncEventList)
        {
        iAsyncEventList->RemoveAsyncEventL(this);
        iAsyncEventList = NULL;
        }
    
    // Close connection to server, as it's not longer required
    iEventSystem.Close();
    }

/** 
* DoCancel
*/
void CAsyncEventActive::DoCancel()
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::DoCancel()"));

    // Cancelling wait event on the event server
    TIpcArgs args(iEventName, iOwnerId);
    if(iEventSystem.Handle())
        {
        TInt ret = iEventSystem.SendReceive(EEventSystemCancelWaitEvent, args);
        }

    // Complete request from caller
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::DoCancel() completing (KErrCancel) status [%x]"), iCallerStatus);
    CompleteCallerStatus(KErrCancel);

    // Remove from list
    if(iAsyncEventList)
        {
        iAsyncEventList->RemoveAsyncEventL(this);
        iAsyncEventList = NULL;
        }
    }

/** 
* RunError
*/
TInt CAsyncEventActive::RunError(TInt aError)
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventActive::RunError() aError=[%d]"), aError);    
    return aError;
    }

/**
* Checks if request to event server is in pending state
*/
EXPORT_C TBool CAsyncEventActive::IsPending()
    {
    return (iCallerStatus != NULL);
    }

/**
* Complete caller status with specified code
*/
TInt CAsyncEventActive::CompleteCallerStatus(TInt aCode)
    {
    RThread thread;
    TInt ret = thread.Open(iStatusThreadId);
    
    if(ret == KErrNone)
        {
        thread.RequestComplete(iCallerStatus, aCode);
        iCallerStatus = NULL;
        thread.Close();
        }
        
    return ret;
    }
    
