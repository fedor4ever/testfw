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
#ifndef EVENTSYSTEMSESSION_H_
#define EVENTSYSTEMSESSION_H_

#include <e32base.h>
#include "waitingevent.h"


// Session class of TestEngine server    
class CEventSystemSession : public CSession2
    {
    public:
        // Create the session
        static CEventSystemSession* NewL();
    
    public:
        // Constructor
        CEventSystemSession();
    
        // Constructor
        ~CEventSystemSession();

        // Called by client/server framework after 
        // session has been successfully created
        void CreateL(); 
        
        // Service request
        void ServiceL(const RMessage2& aMessage);

    private:
        // Handles message
        void DispatchMessageL(const RMessage2& aMessage);
        
        // Closes the session
        void CloseSession();

        // Panics client
        void PanicClient(const RMessage2& aMessage, const TInt aPanic) const;

        // Set indication event
        void SetIndicationEventL(const RMessage2& aMessage);
        
        // Set state event
        void SetStateEventL(const RMessage2& aMessage);

        // Unset state event
        void UnsetStateEventL(const RMessage2& aMessage);

        // Request event
        void RequestEventL(const RMessage2& aMessage);
        
        // Release event
        void ReleaseEventL(const RMessage2& aMessage);
        
        // Wait for event
        void WaitForEventL(const RMessage2& aMessage);
        
        // Check if there is waiting event and complete if found
        void CompleteWaitingEventsL(const TDesC& aEventName);

        // Check list of requested events. If any of them is found, notify about event.
        void NotifyRequestedEventsL(const TDesC& aEventName);

        // Check list of state events. If any of them matches current waiting event, complete waiting event.
        //TBool CompleteFromStateEventL(const TDesC& aEventName, const RMessage2& aMessage);
        void CompleteFromStateEventL(CWaitingEvent* aEvent);

        // Add state event in the state events array
        void AddStateEventL(const TDesC& aEventName);

        // Cancel waiting message from the same session
        void CancelWaitingEventL(const RMessage2& aMessage);

        // Remove (kind of release) all requested events.
        void RemoveInRequestedStateL(const RMessage2& aMessage);

    private:
        // Object container for this session.
        CObjectCon *iContainer;
        
        // Waiting event (this is used in case when test case gets cancelled)
        CWaitingEvent *iWaitingEvent;
        
    }; /* CTestEngineSession */


#endif /* EVENTSYSTEMSESSION_H_ */
// EOF
