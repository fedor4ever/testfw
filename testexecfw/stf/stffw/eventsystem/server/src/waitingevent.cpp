/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#include <e32svr.h>
#include <stfeventsystemerrorcodes.h>
#include "waitingevent.h"


/**
 * Constructor for performing 1st stage construction
 */
CWaitingEvent::CWaitingEvent(const TInt aOwnerId): iOwnerId(aOwnerId)
    {
    RDebug::Print(_L("STF [ESS]: CWaitingEvent::CWaitingEvent() iOwnerId=[%d]"), iOwnerId);
    }

/**
 * Destructor.
 */
CWaitingEvent::~CWaitingEvent()
    {
    RDebug::Print(_L("STF [ESS]: CWaitingEvent::~CWaitingEvent() iOwnerId=[%d]"), iOwnerId);
    if(iMessage && iState == EEventWaiting)
        {
        RDebug::Print(_L("STF [ESS]: Cancelling event [%S] in destructor"), iEventName);
        iMessage->Complete(KErrCancel);
        delete iMessage;
        }
    delete iEventName;
    }

/**
 * Two-phased constructor.
 */
CWaitingEvent* CWaitingEvent::NewLC(const TDesC& aEventName, const TInt aOwnerId)
    {
    //RDebug::Print(_L("STF [ESS]: CWaitingEvent::NewLC()"));
    CWaitingEvent* self = new (ELeave) CWaitingEvent(aOwnerId);
    CleanupStack::PushL(self);
    self->ConstructL(aEventName);
    return self;
    }

/**
 * Two-phased constructor.
 */
CWaitingEvent* CWaitingEvent::NewL(const TDesC& aEventName, const TInt aOwnerId)
    {
    //RDebug::Print(_L("STF [ESS]: CWaitingEvent::NewL()"));
    CWaitingEvent* self = CWaitingEvent::NewLC(aEventName, aOwnerId);
    CleanupStack::Pop();
    return self;
    }

/**
 * Constructor for performing 2nd stage construction
 */
void CWaitingEvent::ConstructL(const TDesC& aEventName)
    {
    //RDebug::Print(_L("STF [ESS]: CWaitingEvent::ConstructL()"));
    iEventName = aEventName.AllocL();
    RDebug::Print(_L("STF [ESS]: Event [%S] is set state to [EEventRequested]"), iEventName);
    iState = EEventRequested;
    RDebug::Print(_L("STF [ESS]: Event [%S] is set iEventSetWhenRequested notification to [EFalse]"), iEventName);    
    iEventSetWhenRequested = EFalse;
    }

/**
 * Checks if event name is the same as provided in argument
 */
TInt CWaitingEvent::IsMatchingEvent(const TDesC& aEventName)
    {
    return (aEventName.Compare(*iEventName) == 0) ? (1) : (0);
    }

/**
 * Checks if event name is the same as provided in argument
 */
TInt CWaitingEvent::IsMatchingEvent(const TDesC& aEventName, const TInt aOwnerId)
    {
    return (aEventName.Compare(*iEventName) == 0 && iOwnerId == aOwnerId) ? (1) : (0);
    }

/**
 * Checks if event's session is the same as provided in argument
 */
TInt CWaitingEvent::IsMatchingSession(const CSession2* aSession)
    {
    return (aSession == iSession);
    }

/**
 * Complete event
 */
void CWaitingEvent::CompleteEventL()
    {
    RDebug::Print(_L("STF [ESS]: CWaitingEvent::CompleteEventL(): Completing event [%S]"), iEventName);
    if(iState == EEventWaiting)
        {
        iMessage->Complete(KErrNone);
        delete iMessage;
        iMessage = NULL;
        RDebug::Print(_L("STF [ESS]: Event [%S] switches state to [EEventRequested]"), iEventName);
        iState = EEventRequested;
        if(iEventSetWhenRequested)
            {
            RDebug::Print(_L("STF [ESS]: Event [%S] switches iEventSetWhenRequested notification to [EFalse]"), iEventName);    
            iEventSetWhenRequested = EFalse;
            }
        }
    }
    
/**
 * Cancel event (because i.e. test case is being cancelled)
 */
void CWaitingEvent::CancelEvent()
    {
    RDebug::Print(_L("STF [ESS]: CWaitingEvent::CancelEvent()"));
    if(iMessage && iState == EEventWaiting)
        {
        iMessage->Complete(KErrCancel);
        delete iMessage;
        iMessage = NULL;
        RDebug::Print(_L("STF [ESS]: Event [%S] switches state to [EEventRequested]"), iEventName);
        iState = EEventRequested;
        }
    }

/**
 * Wait for event (allowed only if event is in requested state)
 */
void CWaitingEvent::WaitL(const RMessage2& aMessage, const CSession2* aSession)
    {
    __ASSERT_ALWAYS(aSession != NULL, PanicClient(aMessage, EEventSystemSessionNotDefined));

    // Check if we can wait for event
    if(iState != EEventRequested)
        {
        RDebug::Print(_L("STF [ESS]: CWaitingEvent::WaitL() Event not requested"));
        User::Leave(EEventSystemNotInRequestedState);
        }

    // Set values
    iMessage = new RMessage2(aMessage);
    iSession = const_cast<CSession2*>(aSession);
    
    // Switch to real waiting
    RDebug::Print(_L("STF [ESS]: Event [%S] switches state to [EEventWaiting]"), iEventName);
    iState = EEventWaiting;
    
    // If indication event was set when this event was in requested state, complete immediately
    if(iEventSetWhenRequested)
         {
         CompleteEventL();
         }
    }

/**
 * Give event name
 */
const TDesC& CWaitingEvent::EventName()
    {
    return *iEventName;
    }

/**
 * Check if event is in requested state
 */
TInt CWaitingEvent::IsRequested()
    {
    return iState == EEventRequested;
    }
        
/**
 * Check if event is waitng
 */
TInt CWaitingEvent::IsWaiting()
    {
    return iState == EEventWaiting;
    }

/**
Panics the client
*/
void CWaitingEvent::PanicClient(const RMessage2& aMessage, const TInt aPanic) const
    {
    RDebug::Print(_L("STF [ESS]: CWaitingEvent::PanicClient"));
    _LIT(KMessage, "CEventSystemSession");
    aMessage.Panic(KMessage, aPanic);
    }

// Notify requested event, that indicatin event 
void CWaitingEvent::NotifyRequestedEventL()
    {
    if(!iEventSetWhenRequested)
        {
        RDebug::Print(_L("STF [ESS]: Event [%S] switches iEventSetWhenRequested notification to [ETrue]"), iEventName);    
        iEventSetWhenRequested = ETrue;
        }
    }

// Give owner id 
TInt CWaitingEvent::OwnerId()
    {
    return iOwnerId;
    }     
// EOF
