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
#ifndef WAITINGEVENT_H
#define WAITINGEVENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION


/**
 *  CWaitingEvent - class representing waiting events 
 *  Object of that type is created when there was a request for the event.
 *  After event is requested, it can wait for another event.
 */
class CWaitingEvent : public CBase
    {
    public:
        // Destructor.
        ~CWaitingEvent();

        // Two-phased constructor.
        static CWaitingEvent* NewL(const TDesC& aEventName, const TInt aCallerId);

        // Two-phased constructor.
        static CWaitingEvent* NewLC(const TDesC& aEventName, const TInt aCallerId);
        
        // Wait for event (allowed only if event is in requested state)
        void WaitL(const RMessage2& aMessage, const CSession2* aSession);
        
        // Complete waiting event
        void CompleteEventL();
        
        // Cancel event
        void CancelEvent(void);
        
        // Checks if event name is the same as provided in argument
        TInt IsMatchingEvent(const TDesC& aEventName, const TInt aCallerId);

        // Checks if event name is the same as provided in argument
        TInt IsMatchingEvent(const TDesC& aEventName);

        // Checks if event's session is the same as provided in argument
        TInt IsMatchingSession(const CSession2* aSession);
        
        // Give event name
        const TDesC& EventName();
        
        // Check if event is in requested state
        TInt IsRequested();
        
        // Check if event is waitng
        TInt IsWaiting();
        
        // Notify requested event, that indicatin event 
        void NotifyRequestedEventL();

        // Give owner id 
        TInt OwnerId();
    private:
        // Shows current state of the event
        enum TEventState
            {
            EEventRequested,
            EEventWaiting
            };
            
        // Constructor for performing 1st stage construction
        CWaitingEvent(const TInt aCallerId);

        // Constructor for performing 2nd stage construction
        void ConstructL(const TDesC& aEventName);
        
        // Panics the client
        void PanicClient(const RMessage2& aMessage, const TInt aPanic) const;

    private:
        // Event name
        HBufC* iEventName;
        
        // Owner id. The same named waiting event may be requested by different callers - so we need to distinguish them
        const TInt iOwnerId;

        // Message to complete with event
        RMessage2* iMessage;
        
        // Session of the waiting event
        CSession2* iSession;
        
        // State of the event
        TEventState iState;
        
        // Defines if indication event was set in the moment when this waiting event was in requested state
        TBool iEventSetWhenRequested;
    };

#endif // WAITINGEVENT_H

// EOF
