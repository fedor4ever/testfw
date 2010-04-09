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
#include <e32svr.h>

#include <stfeventsystemerrorcodes.h>
#include "eventsystemsession.h"
#include "eventsystemserver.h"


/**
Constructor
*/
CEventSystemSession::CEventSystemSession()
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemSession::CEventSystemSession"));
    }

/**
Destructor
*/
CEventSystemSession::~CEventSystemSession()
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemSession::~CEventSystemSession"));
    // Check if there is waiting event registered 
    if(iWaitingEvent)
        {
        RDebug::Print(_L("STF [ESS]: CEventSystemSession::~CEventSystemSession iWaitingEvent is defined, will be handled"));
        TInt i;
    
        for(i = 0; i < ((CEventSystemServer*)Server())->iWaitingEvents.Count(); i++)
            {
            CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
            // Check if this event matches
            if(event == iWaitingEvent)
                {
                RDebug::Print(_L("STF [ESS]: CEventSystemSession::~CEventSystemSession iWaitingEvent found in the array"));
                if(event->IsWaiting())
                    {
                    // Complete event and remove it from list of waiting events
                    RDebug::Print(_L("STF [ESS]: CEventSystemSession::~CEventSystemSession iWaitingEvent in EEventWait state, cancelling"));
                    event->CancelEvent();
                    ((CEventSystemServer*)Server())->iWaitingEvents.Remove(i);
                    RDebug::Print(_L("STF [ESS]: CEventSystemSession::~CEventSystemSession deleting iWaitingEvent"));
                    delete event;
                    event = NULL;
                    iWaitingEvent = NULL;
                    }
                break;
                }
            }
        }
        
    // Close session if not closed properly
    if(iContainer)
        {
        CloseSession();
        }
    }


/**
Called by client/server framework after 
session has been successfully created.

In effect, a second-phase constructor.

Creates:

1. the object index
2. the object container for this session.

We are then ready for subsessions.
*/
void CEventSystemSession::CreateL()
    {
//    RDebug::Print(_L("STF [ESS]: CEventSystemSession::CreateL"));
//        // Create new object index
//        iTestCaseRunnerObjectIndex = CObjectIx::NewL();
    
    // Initialize the object container
    // using the object container index in the server.
    iContainer = ((CEventSystemServer*)Server())->NewContainerL();
    
    // Adds session (updates counter)
    ((CEventSystemServer*)Server())->AddSession();
    }


/**
Closes the session.

It deletes the object index and object container.

This could be done in the destructor, but it seems neater to do it here. 
*/
void CEventSystemSession::CloseSession()
    {
//    RDebug::Print(_L("STF [ESS]: CEventSystemSession::CloseSession"));
    // Deletes the object index.
//        delete iTestCaseRunnerObjectIndex;
//        iTestCaseRunnerObjectIndex = NULL;
    
    // Deletes the object container
    ((CEventSystemServer*)Server())->RemoveContainer(iContainer);
    //delete iContainer; // RemoveContainer deletes the object
    iContainer = NULL;
    
    // Removes session
    ((CEventSystemServer*)Server())->RemoveSession();
    }

/**
First line servicing of a client request.

This function dispatches requests to the appropriate handler.
Some messages are handled by the session itself, and are
implemented as CCountSession member functions, while 
other messages are handled by the subsession, and are
implemented as CCountSubSession member functions.
*/
void CEventSystemSession::ServiceL(const RMessage2& aMessage)
    {
//    RDebug::Print(_L("STF [ESS]: CEventSystemSession::ServiceL"));
    DispatchMessageL(aMessage);
    }


/**
Called by ServiceL()

It tests the function code and then delegates to
the appropriate function.
*/
void CEventSystemSession::DispatchMessageL(const RMessage2& aMessage)
    {
//    RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL function=[%d] message=[%x]"), aMessage.Function(), &aMessage);
    TInt err;

    // First check for session-relative requests
    switch(aMessage.Function())
        {
        // Session relevant commands
        case EEventSystemServCloseSession:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemServCloseSession"));
            CloseSession();
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemServCloseSession"));
            aMessage.Complete(KErrNone);
            break;
            
        case EEventSystemSetEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemSetEvent"));
            TRAP(err, SetStateEventL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemSetEvent with [%d]"), err);
            aMessage.Complete(err);
            break;
            
        case EEventSystemUnsetEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemUnsetEvent"));
            TRAP(err, UnsetStateEventL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemUnsetEvent with [%d]"), err);
            aMessage.Complete(err);
            break;
            
        case EEventSystemSetIndicationEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemSetIndicationEvent"));
            TRAP(err, SetIndicationEventL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemSetIndicationEvent with [%d]"), err);
            aMessage.Complete(err);
            break;
            
        case EEventSystemWaitEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemWaitEvent"));
            TRAP(err, WaitForEventL(aMessage));
            if(err != KErrNone)
                {
                RDebug::Print(_L("STF [ESS]: Completing message EEventSystemWaitEvent with [%d]"), err);
                aMessage.Complete(err);
                }
            break;
            
        case EEventSystemCancelWaitEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemCancelWaitEvent"));
            TRAP(err, CancelWaitingEventL(aMessage));
            if(err != KErrNone)
                {
                RDebug::Print(_L("STF [ESS]: Completing message EEventSystemCancelWaitEvent with [%d]"), err);
                aMessage.Complete(err);
                }
            break;

        case EEventSystemRequestEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemRequestEvent"));
            TRAP(err, RequestEventL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemRequestEvent with [%d]"), err);
            aMessage.Complete(err);
            break;

        case EEventSystemReleaseEvent:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemReleaseEvent"));
            TRAP(err, ReleaseEventL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemReleaseEvent with [%d]"), err);
            aMessage.Complete(err);
            break;

        case EEventSystemRemoveInRequestedState:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL EEventSystemRemoveInRequestedState"));
            TRAP(err, RemoveInRequestedStateL(aMessage));
            RDebug::Print(_L("STF [ESS]: Completing message EEventSystemRemoveInRequestedState with [%d]"), err);
            aMessage.Complete(err);
            break;

                    
        default:
            RDebug::Print(_L("STF [ESS]: CEventSystemSession::DispatchMessageL unknown function"));
            PanicClient(aMessage, EEventSystemSessionUnknownMessage);
        }
    }

/**
Panics the client
*/
void CEventSystemSession::PanicClient(const RMessage2& aMessage, const TInt aPanic) const
    {
    RDebug::Print(_L("STF [ESS]: CEventSystemSession::PanicClient"));
    _LIT(KMessage, "CEventSystemSession");
    aMessage.Panic(KMessage, aPanic);
    }


/**
Set indication event
*/
void CEventSystemSession::SetIndicationEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);
    
    RDebug::Print(_L("STF [ESS]: setting indication event [%S]"), &eventName);
    
    // Notify requested events
    NotifyRequestedEventsL(eventName);
    
    // Complete waiting events
    CompleteWaitingEventsL(eventName);
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Request event
*/
void CEventSystemSession::RequestEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);
    
    TInt ownerId = aMessage.Int1();

    // Check if event is not already requested
    TInt i = 0;
    if(((CEventSystemServer*)Server())->SearchForWaiting(eventName, ownerId, i))
        {
        RDebug::Print(_L("STF [ESS]: Requested event [%S] already exists, leaving"), &eventName);
        User::Leave(KErrAlreadyExists);
        }
    
    // Create waiting event - it's first state will be EEventRequested
    RDebug::Print(_L("STF [ESS]: Waiting event registered [%S]"), &eventName);
    iWaitingEvent = CWaitingEvent::NewL(eventName, ownerId);
    ((CEventSystemServer*)Server())->iWaitingEvents.Append(iWaitingEvent);
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Release event
*/
void CEventSystemSession::ReleaseEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);

    TInt ownerId = aMessage.Int1();

    // Check if event is requested
    TInt i = 0;
    if(((CEventSystemServer*)Server())->SearchForWaiting(eventName, ownerId, i))
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        
        // If event is waiting, cancel it
        if(event->IsWaiting())
            {
            event->CancelEvent();
            }

        // Check if there is any candidate to be unset
        TInt j = ((CEventSystemServer*)Server())->SearchForState(eventName);
        if(j != KErrNotFound)
            {
            CStateEvent* state = ((CEventSystemServer*)Server())->iStateEvents[j];
            if(state->IsToBeUnset())
                {
                ((CEventSystemServer*)Server())->iStateEvents.Remove(j);
                delete state;
                state = NULL;
                }
            }
        
        
        // Delete waiting event
        ((CEventSystemServer*)Server())->iWaitingEvents.Remove(i);
        delete event;
        event = NULL;
        }
    else // Event was not requested
        {
        RDebug::Print(_L("STF [ESS]: Event [%S] to be releases doesn't exist, leaving"), &eventName);
        User::Leave(KErrNotFound);
        }
        
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Wait for event
*/
void CEventSystemSession::WaitForEventL(const RMessage2& aMessage)
    {
    __ASSERT_ALWAYS(iWaitingEvent == NULL, PanicClient(aMessage, EEventSystemSessionAlreadyWaiting));
    
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);

    TInt ownerId = aMessage.Int1();

    // Check if event is requested
    TInt i = 0;
    if(((CEventSystemServer*)Server())->SearchForWaiting(eventName, ownerId, i))
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        
        // Set in waiting state
        iWaitingEvent = event;
        event->WaitL(aMessage, this);
        CompleteFromStateEventL(event);
        }
    else
        {
        RDebug::Print(_L("STF [ESS]: Event [%S] not requested"), &eventName);
        User::Leave(EEventSystemNotRequested);
        }
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Cancel waiting message from the same session
*/
void CEventSystemSession::CancelWaitingEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);

    TInt ownerId = aMessage.Int1();

    // Search for waiting events
    TInt i = 0;
    while(((CEventSystemServer*)Server())->SearchForWaiting(eventName, ownerId, i))
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        if(event->IsMatchingSession(this))
            {
            // Complete event and remove it from list of waiting events
            event->CancelEvent();
            ((CEventSystemServer*)Server())->iWaitingEvents.Remove(i);
            // Delete waiting event
            delete event;
            event = NULL;
            break;
            }
        //Increase index, so SearchForWaiting will start searching with following entries
        i++; 
        }
    aMessage.Complete(KErrNone);
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Check list of waiting events. If any of them is found, complete it.
*/
void CEventSystemSession::CompleteWaitingEventsL(const TDesC& aEventName)
    {
    TInt i = 0;
    while(((CEventSystemServer*)Server())->SearchForWaiting(aEventName, i))
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        // Check if this event matches
        if(event->IsWaiting())
            {
            // Complete event
            event->CompleteEventL();
            }
        //Increase index, so SearchForWaiting will start searching with following entries
        i++; 
        }            
    }

/**
Check list of requested events. If any of them is found, notify about event.
*/
void CEventSystemSession::NotifyRequestedEventsL(const TDesC& aEventName)
    {
    TInt i = 0;
    while(((CEventSystemServer*)Server())->SearchForWaiting(aEventName, i))
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        // Check if this event matches
        if(event->IsRequested())
            {
            // Complete event
            event->NotifyRequestedEventL();
            }
        //Increase index, so SearchForWaiting will start searching with following entries
        i++; 
        }            
    }

/**
Check list of state events. If any of them matches current waiting event, complete waiting event.
*/
void CEventSystemSession::CompleteFromStateEventL(CWaitingEvent* aEvent)
    {
    TInt i = ((CEventSystemServer*)Server())->SearchForState(aEvent->EventName());
    if(i != KErrNotFound)
        {
        RDebug::Print(_L("STF [ESS]: CEventSystemSession::CompleteFromStateEventL event [%S]"), &aEvent->EventName());
        aEvent->CompleteEventL();
        }
    }
    
/**
Set state event
*/
void CEventSystemSession::SetStateEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);

    // Setting state event
    RDebug::Print(_L("STF [ESS]: setting state event [%S]"), &eventName);
    AddStateEventL(eventName);
    
    // Complete waiting events
    CompleteWaitingEventsL(eventName);
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);
    }

/**
Unset state event
*/
void CEventSystemSession::UnsetStateEventL(const RMessage2& aMessage)
    {
    // Get event data
    RBuf eventName;
    eventName.CreateL(KMaxEventName);
    eventName.CleanupClosePushL();
    aMessage.ReadL(0, eventName);

    // Search for state event
    TBool cannotBeUnset = EFalse;
    CStateEvent* event = NULL;
    
    TInt i = ((CEventSystemServer*)Server())->SearchForState(eventName);
    if(i != KErrNotFound)
        {
        // State event found, do further checking with requested events
        event = ((CEventSystemServer*)Server())->iStateEvents[i];
        TInt j = 0;
        while(((CEventSystemServer*)Server())->SearchForWaiting(eventName, j))
            {
            CWaitingEvent* waiting = ((CEventSystemServer*)Server())->iWaitingEvents[j];
            // If waiting event is in requested state, we can't unset our event, we can only mark it to be unset later
            if(waiting->IsRequested())
                {
                // Set the flag
                event->NotifyToBeUnset(ETrue);
                cannotBeUnset = ETrue;
                }
            //Increase index, so SearchForWaiting will start searching with following entries
            j++; 
            }
        }
    else
        {
        // State event was not found
        RDebug::Print(_L("STF [ESS]: unset: state event [%S] not set, leaving with [%d]"), &eventName, KErrNotFound);
        User::Leave(KErrNotFound);
        }

    // Unsetting event because we have no found any requested events
    if(!cannotBeUnset)
        {
        // Unsetting state event
        RDebug::Print(_L("STF [ESS]: unsetting state event [%S]"), &eventName);
        ((CEventSystemServer*)Server())->iStateEvents.Remove(i);
        delete event;
        event = NULL;
        }
    
    // Clean data
    CleanupStack::PopAndDestroy(&eventName);    
    }

/**
Add state event in the state events array
*/
void CEventSystemSession::AddStateEventL(const TDesC& aEventName)
    {
    CStateEvent *event;

    TInt i = ((CEventSystemServer*)Server())->SearchForState(aEventName);
    if(i != KErrNotFound)
        {
        event = ((CEventSystemServer*)Server())->iStateEvents[i];
        // Event found, leave (or not, if event was unset but could not be due to some not released wait)
        if(event->IsToBeUnset())
            {
            // Reset the flag (equivalent to situation when theoretically unset event is set again)
            event->NotifyToBeUnset(EFalse);
            }
        else
            {
            // Leave
            RDebug::Print(_L("STF [ESS]: set: state event [%S] already set, leaving with [%d]"), &aEventName, KErrAlreadyExists);
            User::Leave(KErrAlreadyExists);
            }
        }
    else
        {
        // Event not found, add it
        event = CStateEvent::NewL(aEventName);
        ((CEventSystemServer*)Server())->iStateEvents.Append(event);
        }
    }


/**
Remove (like would be released) all requested events.
Waiting events will be handled via destructor of opened session
*/
void CEventSystemSession::RemoveInRequestedStateL(const RMessage2& aMessage)
    {
    // Get event data
    TInt ownerId = aMessage.Int0();
    RDebug::Print(_L("STF [ESS]: Removing all requested events of owner [%d], count=[%d]"), ownerId, ((CEventSystemServer*)Server())->iWaitingEvents.Count());

    // Check if event is requested
    TInt i = 0;
    while(i < ((CEventSystemServer*)Server())->iWaitingEvents.Count())
        {
        CWaitingEvent* event = ((CEventSystemServer*)Server())->iWaitingEvents[i];
        
        if(event->OwnerId() == ownerId && event->IsRequested())
            {
            // Check if there is any candidate to be unset
            TInt j = ((CEventSystemServer*)Server())->SearchForState(event->EventName());
            if(j != KErrNotFound)
                {
                CStateEvent* state = ((CEventSystemServer*)Server())->iStateEvents[j];
                if(state->IsToBeUnset())
                    {
                    ((CEventSystemServer*)Server())->iStateEvents.Remove(j);
                    delete state;
                    state = NULL;
                    }
                }

            // And remove and delete
            ((CEventSystemServer*)Server())->iWaitingEvents.Remove(i);
            delete event;
            event = NULL;
            }
        else
            {
            i++;
            }
        }
    }
// EOF

