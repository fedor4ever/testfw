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
#ifndef EVENTSYSTEMSERVER_H_
#define EVENTSYSTEMSERVER_H_

#include "stfeventsystemdefs.h"
#include "waitingevent.h"
#include "stateevent.h"

// Constant values
TInt const KShutdownDelay = 60000000; //Automatic shutdown delay: 60s

// Class for shutting down server when inactive
class CShutdown: public CTimer
    {
    public:
        inline CShutdown();
        inline void ConstructL();
        inline void Start();
        
    private:
        void RunL();
    };

// Main class of TestEngine server
class CEventSystemServer : public CServer2
    {
    public:    
        // Creats a new server object
        static CEventSystemServer* NewL(CActive::TPriority aActiveObjectPriority);
        static CEventSystemServer* NewLC(CActive::TPriority aActiveObjectPriority);

        // Creates a new session with the server; the function implements 
        // pure virtual function defined in class CServer2
        CSession2* NewSessionL(const TVersion &aVersion, const RMessage2& aMessage) const;
        
        // Adds session
        void AddSession(void);

        // Removes session
        void RemoveSession(void);
        
    public :
        // Constructor
        CEventSystemServer(CActive::TPriority aActiveObjectPriority);
    
        // Second phase constructor
        void ConstructL();

        // Returns an object container, and guaranteed to produce object 
        // containers with unique ids within the server.
        // Called by a new session to create a container 
        CObjectCon* NewContainerL();
        
        // Remove session object from container
        void RemoveContainer(CObjectCon *aObj);
    
        // Destructor
        ~CEventSystemServer();
        
        // Search for state event and give its index
        TInt SearchForState(const TDesC& aEventName);
        
        // Search for waiting event, starting from aIndex
        TBool SearchForWaiting(const TDesC& aEventName, const TInt aOwnerId, TInt& aIndex);
        
        // Search for waiting event, starting from aIndex
        TBool SearchForWaiting(const TDesC& aEventName, TInt& aIndex);
        
        // List of waiting events
        RPointerArray<CWaitingEvent> iWaitingEvents;

        // State events container
        RPointerArray<CStateEvent> iStateEvents;
        
    private:
        // Panic class with given error code
        void PanicClass(const TInt aErrorCode);
        
        // Check condition and start shudown timer.
        void CheckAndStartShutdownTimer(void);
        
        // The server has an object container index that
        // creates an object container for each session.
        CObjectConIx* iContainerIndex;
        
        // Session counter
        TInt iSessionCounter;
        
        // Shutdown object
        CShutdown iShutdown;
    };
        

#endif /* EVENTSYSTEMSERVER_H_ */
// EOF
