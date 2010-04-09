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
#ifndef EVENTSYSTEMDEFS_H_
#define EVENTSYSTEMDEFS_H_

#include <e32base.h>


// Server name    
_LIT(KEventSystemServName, "STFEventSystemServ");
_LIT(KEventSystemServNameExe, "stfeventsystemsrv.exe");

// Server version. A version must be specifyed when creating a session with the server
const TUint KEventSystemServMajorVersionNumber = 1;
const TUint KEventSystemServMinorVersionNumber = 0;
const TUint KEventSystemServBuildVersionNumber = 0;

// Max length
const TInt KMaxEventName = 50;
const TInt KMaxEventMessage = 200;

// Operation codes used in message passing between client and server
enum TEventSystemServRqst
    {
    EEventSystemServCloseSession       = 1,
    EEventSystemSetEvent               = 2,
    EEventSystemUnsetEvent             = 3,
    EEventSystemSetIndicationEvent     = 4,
    EEventSystemWaitEvent              = 5,
    EEventSystemCancelWaitEvent        = 6,
    EEventSystemRequestEvent           = 7,
    EEventSystemReleaseEvent           = 8,
    EEventSystemRemoveInRequestedState = 9
    };
    

#endif /* EVENTSYSTEMDEFS_H_ */
// EOF
