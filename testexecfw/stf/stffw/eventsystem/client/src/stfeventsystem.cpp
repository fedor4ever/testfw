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

#include <stfeventsystemdefs.h>
#include "stfeventsystem.h"
#include <stfasynceventactive.h>

// Implementation of session class

/**
Connects to event server.
*/
EXPORT_C TInt REventSystem::Connect()
    {
    //RDebug::Print(_L("STF [ESC]: REventSystem::Connect start"));
    TInt err;
    
    // Check if server is not running
    err = CreateSession(KEventSystemServName, Version());
    //RDebug::Print(_L("STF [ESC]: REventSystem::Connect session creation [%d]"), err);
    
    if(err == KErrNotFound) //Server is not yet started
        {
        RProcess p;
        err = p.Create(KEventSystemServNameExe, KEventSystemServNameExe);
        RDebug::Print(_L("STF [ESC]: REventSystem::Connect process creation [%d]"), err);
        if(err == KErrNone)
            {
            p.Resume();
            
            TRequestStatus eventServerStartup;
            p.Rendezvous(eventServerStartup);
            User::WaitForRequest(eventServerStartup);
            RDebug::Print(_L("STF [ESC]: REventSystem::Connect randezvous with server [%d]"), eventServerStartup.Int());
            p.Close();

            err = CreateSession(KEventSystemServName, Version());
            //RDebug::Print(_L("STF [ESC]: REventSystem::Connect session creation [%d]"), err);
            }
        }
    else if(err != KErrNone)
        {
        return err;
        }
    //RDebug::Print(_L("STF [ESC]: REventSystem::Connect end"));
    return err; 
    }

/**
Closes connection.
*/
EXPORT_C void REventSystem::Close()
    {
    //RDebug::Print(_L("STF [ESC]: REventSystem::Close()"));
    if(Handle())
        {
        SendReceive(EEventSystemServCloseSession);
        RHandleBase::Close();
        }
    //RDebug::Print(_L("STF [ESC]: REventSystem::Close end"));
    }

/**
Returns version of event system.
*/
EXPORT_C TVersion REventSystem::Version(void) const
    {
    return TVersion(KEventSystemServMajorVersionNumber, KEventSystemServMinorVersionNumber, KEventSystemServBuildVersionNumber);
    }

/**
Set indication event.
*/
EXPORT_C void REventSystem::SetIndicationEventL(const TDesC& aEventName)
    {
    SetIndicationEventL(aEventName, NULL);
    }

/**
Set indication event.
*/
EXPORT_C void REventSystem::SetIndicationEventL(const TDesC& aEventName, TRequestStatus *aStatus)
    {
    RDebug::Print(_L("STF [ESC]: REventSystem::SetIndicationEventL"));
    if(aStatus)
        *aStatus = KRequestPending;

    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName);
    TInt ret = SendReceive(EEventSystemSetIndicationEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::SetIndicationEventL completed with [%d]"), ret);
    Close();
    
    if(aStatus)
        User::RequestComplete(aStatus, ret);

    User::LeaveIfError(ret);
    }

/**
Wait for event
*/
EXPORT_C void REventSystem::WaitEventL(const TDesC& aEventName, const TInt aOwner)
    {    
    RDebug::Print(_L("STF [ESC]: REventSystem::WaitEventL eventname=[%S]"), &aEventName);
    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName, aOwner);
    TInt ret = SendReceive(EEventSystemWaitEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::WaitEventL completed with [%d]"), ret);
    Close();
    User::LeaveIfError(ret);
    }

/**
Set state event.
*/
EXPORT_C void REventSystem::SetEventL(const TDesC& aEventName)
    {
    SetEventL(aEventName, NULL);
    }

/**
Request event
*/
EXPORT_C void REventSystem::RequestEventL(const TDesC& aEventName, const TInt aOwner)
    {    
    RDebug::Print(_L("STF [ESC]: REventSystem::RequestEventL eventname=[%S]"), &aEventName);
    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName, aOwner);
    TInt ret = SendReceive(EEventSystemRequestEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::RequestEventL completed with [%d]"), ret);
    Close();
    User::LeaveIfError(ret);
    }

/**
Release event
*/
EXPORT_C void REventSystem::ReleaseEventL(const TDesC& aEventName, const TInt aOwner)
    {    
    RDebug::Print(_L("STF [ESC]: REventSystem::ReleaseEventL eventname=[%S]"), &aEventName);
    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName, aOwner);
    TInt ret = SendReceive(EEventSystemReleaseEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::ReleaseEventL completed with [%d]"), ret);
    Close();
    User::LeaveIfError(ret);
    }

/**
Set state event.
*/
EXPORT_C void REventSystem::SetEventL(const TDesC& aEventName, TRequestStatus *aStatus)
    {
    RDebug::Print(_L("STF [ESC]: REventSystem::SetStateEventL eventname=[%S]"), &aEventName);
    
    if(aStatus)
        *aStatus = KRequestPending;
    
    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName);
    TInt ret = SendReceive(EEventSystemSetEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::SetStateEventL completed with [%d]"), ret);
    Close();
    
    if(aStatus)
        User::RequestComplete(aStatus, ret);
        
    User::LeaveIfError(ret);
    }
    
/**
Unset state event.
*/
EXPORT_C void REventSystem::UnsetEventL(const TDesC& aEventName)
    {
    UnsetEventL(aEventName, NULL);
    }
    
/**
Unset state event.
*/
EXPORT_C void REventSystem::UnsetEventL(const TDesC& aEventName, TRequestStatus* aStatus)
    {
    RDebug::Print(_L("STF [ESC]: REventSystem::UnsetStateEventL eventname=[%S]"), &aEventName);
    
    if(aStatus)
        *aStatus = KRequestPending;
            
    User::LeaveIfError(Connect());
    TIpcArgs args(&aEventName);
    TInt ret = SendReceive(EEventSystemUnsetEvent, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::UnsetStateEventL completed with [%d]"), ret);
    Close();
    
    if(aStatus)
        User::RequestComplete(aStatus, ret);
    
    User::LeaveIfError(ret);
    }

/**
Cancel (remove) all requested events of given owner.
*/
EXPORT_C TInt REventSystem::RemoveInRequestedState(const TInt aOwner)
    {
    RDebug::Print(_L("STF [ESC]: REventSystem::RemoveInRequestedState"));

    User::LeaveIfError(Connect());
    TIpcArgs args(aOwner);
    TInt ret = SendReceive(EEventSystemRemoveInRequestedState, args);
    RDebug::Print(_L("STF [ESC]: REventSystem::RemoveInRequestedState completed with [%d]"), ret);
    Close();
    
    return ret;    
    }
    
// EOF
