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
* Description: This file contains the header file of the CTestEngine.
*
*/

#ifndef TESTENGINECLIENTSERVER_H
#define TESTENGINECLIENTSERVER_H

//  INCLUDES
//#include <e32std.h>
#include <e32base.h>

// CONSTANTS
// none

// Server name
_LIT( KTestEngineServerName, "TestFrameworkEngine" );
_LIT( KTestEngineServerExe,"STFTestEngineSrv.exe");      // EXE name
const TUid KTestEngineServerUid3 = {0x2002BC9C};

// The server version.
// A version must be specified when creating a session with the server.
const TUint KTestEngineMajorVersionNumber = 0;
const TUint KTestEngineMinorVersionNumber = 2;
const TUint KTestEngineBuildVersionNumber = 1;

// MACROS
// None

// DATA TYPES
// Opcodes used in message passing between client and server
enum TTestEngineRequests
    {
    // RTestEngine requests
    ETestEngineLoadConfiguration,
    ETestEngineUpdateLogConfiguration,
    ETestEngineUpdateEngineConfiguration,
    ETestEngineSetAttribute,
    ETestEngineAddTestModule,
    ETestEngineRemoveTestModule,
    ETestEngineAddConfigFile,
    ETestEngineRemoveConfigFile,
    ETestEngineEnumerateTestCases,
    ETestEngineGetTestCases,    
    ETestEngineCancelAsyncRequest,
    ETestEngineEvent,
    ETestEngineErrorNotification,
    ETestEngineLoggerSettings,
    ETestEngineReportTestCase, //Add info about executed test case to test report. Used by reboot mechanism

    // RTestCase requests
    ETestCaseCreateSubSession,
    ETestCaseCloseSubSession,
    ETestCaseRunTestCase,
    ETestCasePause,
    ETestCaseResume,
    ETestCaseNotifyProgress,
    ETestCaseNotifyRemoteType,
    ETestCaseNotifyRemoteMsg,
    ETestCaseCancelAsyncRequest,
    ETestCaseNotifyCommand,
    };

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// CLASS DECLARATION
// None

#endif /* TESTENGINECLIENTSERVER_H */

// End of file
