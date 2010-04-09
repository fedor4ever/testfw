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

//  Include Files  

#include <e32base.h>
#include <e32std.h>
#include <e32svr.h>

#include "stfeventsystemdefs.h"
#include "eventsystemserver.h"

//  Local Functions

// TestEngine server starter
LOCAL_C void RunEventSystemServerL()
    {
    RDebug::Print(_L("STF [ESS]: RunEventSystemServerL start"));
    // Naming the server thread after the server helps to debug panics
    User::LeaveIfError(RProcess::RenameMe(KEventSystemServName));
    
    // Create and install active scheduler
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    
    // Create TestEngine server and leave it on the cleanup stack
    CEventSystemServer* eventSystemServer = CEventSystemServer::NewLC(CActive::EPriorityStandard);
    
    // Initialisation complete, now signal the client
    RProcess::Rendezvous(KErrNone);
    
    // Ready to run
    CActiveScheduler::Start();
    
    // Cleanup the server and scheduler
    CleanupStack::PopAndDestroy(eventSystemServer);
    CleanupStack::PopAndDestroy(scheduler);
    RDebug::Print(_L("STF [ESS]: RunEventSystemServerL end"));
    }

//  Global Functions
GLDEF_C TInt E32Main()
    {
    RDebug::Print(_L("STF [ESS]: E32Main of EventServer start"));
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt r = KErrNoMemory;
    if(cleanup)
        {
        TRAP(r, RunEventSystemServerL());
        delete cleanup;
        }
    __UHEAP_MARKEND;
    RDebug::Print(_L("STF [ESS]: E32Main of EventServer end result=[%d]"), r);
    return r;
    }

// EOF
