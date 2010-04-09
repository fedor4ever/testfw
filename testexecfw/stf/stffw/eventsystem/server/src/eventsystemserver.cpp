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
#include "eventsystemserver.h"
#include "eventsystemsession.h"


/**
First phase construction
*/
CEventSystemServer* CEventSystemServer::NewL(CActive::TPriority aActiveObjectPriority)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::NewL"));
    CEventSystemServer* self = NewLC(aActiveObjectPriority);
    CleanupStack::Pop();
    return self;
    }

/**
First phase construction
*/
CEventSystemServer* CEventSystemServer::NewLC(CActive::TPriority aActiveObjectPriority)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::NewLC"));
    CEventSystemServer* self = new (ELeave) CEventSystemServer(aActiveObjectPriority);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }    

/**
Creates and returns a new object container using the server's object container index.
This is a service that is used by a session.
*/
CObjectCon* CEventSystemServer::NewContainerL()
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::NewContainerL"));
    return iContainerIndex->CreateL();
    }

/**
Removes session object from container.
*/
void CEventSystemServer::RemoveContainer(CObjectCon *aObj)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::RemoveContainer aObj=[%x]"), aObj);
    iContainerIndex->Remove(aObj);
    }


/**
Constructor takes the server priority value. 

The server is an active object, and the priority value is the priority
of this active object.

It passes the priority value to the base class in the Ctor list.
By default, the session is not sharable, which is what we want here
so no second parameter is passed to the CServer2 constructor.
*/
CEventSystemServer::CEventSystemServer(CActive::TPriority aActiveObjectPriority)
    : CServer2(aActiveObjectPriority)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::CEventSystemServer"));
    }


/**
Second-phase constructor - creates the object container index.
*/
void CEventSystemServer::ConstructL()
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::ConstructL"));
    StartL(KEventSystemServName);
    iContainerIndex = CObjectConIx::NewL();
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::ConstructL created iContainerIndex=[%x]"), iContainerIndex);
    iShutdown.ConstructL();
    iShutdown.Start();
    }


/**
Desctructor - deletes the object container index.
*/
CEventSystemServer::~CEventSystemServer()
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::~CEventSystemServer iContainerIndex=[%x]"), iContainerIndex);
    delete iContainerIndex;
    iWaitingEvents.ResetAndDestroy();
    iWaitingEvents.Close();
    iStateEvents.ResetAndDestroy();
    iStateEvents.Close();
    }


/**
Creates a new session with the server.
*/
CSession2* CEventSystemServer::NewSessionL(const TVersion &aVersion, const RMessage2& /*aMessage*/) const
    {
    RDebug::Print(_L("STF [ESS]: CEventSystemServer::NewSessionL"));
    // Check that the version is OK
    TVersion version(KEventSystemServMajorVersionNumber, KEventSystemServMinorVersionNumber, KEventSystemServBuildVersionNumber);
    if(!User::QueryVersionSupported(version, aVersion))
        {
        User::Leave(EEventSystemVersionNotSupported);
        }
    
    // Create the session.
    CSession2* session = new (ELeave) CEventSystemSession;

    // Return session        
    return session;
    }

/**
Adds session.
*/
void CEventSystemServer::AddSession(void)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::AddSession"));
    iSessionCounter++;
    RDebug::Print(_L("STF [ESS]: CEventSystemServer::AddSession iSessionCounter increased to %d"), iSessionCounter);
    iShutdown.Cancel();
    }
    
/**
Closes session and starts shutdown timer in case no more sessions are opened.
*/
void CEventSystemServer::RemoveSession(void)
    {
    //RDebug::Print(_L("STF [ESS]: CEventSystemServer::RemoveSession"));
    __ASSERT_ALWAYS((iSessionCounter > 0), PanicClass(EInvalidSessionCounter));
    iSessionCounter--;
    RDebug::Print(_L("STF [ESS]: CEventSystemServer::RemoveSession iSessionCounter decreased to %d"), iSessionCounter);
    CheckAndStartShutdownTimer();
    }

/**
Check condition and start shudown timer.
*/
void CEventSystemServer::CheckAndStartShutdownTimer(void)
    {
    RDebug::Print(_L("STF [ESS]: CEventSystemServer::CheckAndStartShutdownTimer counter=[%d] state count=[%d] waiting count=[%d]"), iSessionCounter, iStateEvents.Count(), iWaitingEvents.Count());
    // Timer can be started if there is no connected session and there are no state events set
    if(iSessionCounter == 0 && iStateEvents.Count() == 0 && iWaitingEvents.Count() == 0)
        {
        RDebug::Print(_L("STF [ESS]: starting shutdown timer"));
        iShutdown.Start();
        }
    }

/**
Panic class with given error code.
*/
void CEventSystemServer::PanicClass(const TInt aErrorCode)
    {
    RDebug::Print(_L("STF [ESS]: Panic class CEventSystemServer [%x] with code [%d]"), this, aErrorCode);
    _LIT(KMessage, "CEventSystemServer");
    User::Panic(KMessage, aErrorCode);
    }

/**
Search for state event and give its index.
*/
TInt CEventSystemServer::SearchForState(const TDesC& aEventName)
    {
    TInt i;
    for(i = 0; i < iStateEvents.Count(); i++)
        {
        // Check if this event matches
        if(iStateEvents[i]->IsMatchingEvent(aEventName))
            {
            return i;
            }
        }
    return KErrNotFound;
    }
        
/**
Search for waiting event, starting from given index.
aIndex will show found position and method return true.
*/
TBool CEventSystemServer::SearchForWaiting(const TDesC& aEventName, const TInt aOwnerId, TInt& aIndex)
    {
    if(aIndex < 0)
        aIndex = 0;
        
    for(; aIndex < iWaitingEvents.Count(); aIndex++)
        {
        // Check if this event matches
        if(iWaitingEvents[aIndex]->IsMatchingEvent(aEventName, aOwnerId))
            {
            return ETrue;
            }
        }
    return EFalse;
    }

/**
Search for waiting event, starting from given index.
aIndex will show found position and method return true.
*/
TBool CEventSystemServer::SearchForWaiting(const TDesC& aEventName, TInt& aIndex)
    {
    if(aIndex < 0)
        aIndex = 0;
        
    for(; aIndex < iWaitingEvents.Count(); aIndex++)
        {
        // Check if this event matches
        if(iWaitingEvents[aIndex]->IsMatchingEvent(aEventName))
            {
            return ETrue;
            }
        }

    return EFalse;
    }

/**
Constructor of shutdown class.
*/
inline CShutdown::CShutdown(): CTimer(-1)
    {
    //RDebug::Print(_L("STF [ESS]: CShutdown::CShutdown"));
    CActiveScheduler::Add(this);
    }
    
/**
2nd phase of object construction.
*/
inline void CShutdown::ConstructL()
    {
    //RDebug::Print(_L("STF [ESS]: CShutdown::ConstructL"));
    CTimer::ConstructL();
    }
    
/**
Start shutdown timer.
*/
inline void CShutdown::Start()
    {
    RDebug::Print(_L("STF [ESS]: CShutdown::Start"));
    After(KShutdownDelay);
    }
    
/**
Shutdown server.
*/
void CShutdown::RunL()
    {
    RDebug::Print(_L("STF [ESS]: CShutdown::RunL"));
    CActiveScheduler::Stop();
    }
        
// EOF
