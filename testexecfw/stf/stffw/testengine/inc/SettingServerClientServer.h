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
* Description: This file contains the header file of the 
* CSettingServer.
*
*/

#ifndef SETTINGSERVERCLIENTSERVER_H
#define SETTINGSERVERCLIENTSERVER_H

//  INCLUDES
#include <e32base.h>

// The server version.
// A version must be specified when creating a session with the server.
const TUint KSettingServerMajorVersionNumber=0;
const TUint KSettingServerMinorVersionNumber=1;
const TUint KSettingServerVersionNumber=1;

// Server heap size
const TUint KDefaultHeapSize = 0x10000;     // 64 K
const TUint KMaxHeapSize     = 0x20000;     // 128 K 

// MACROS
// none

// DATA TYPES

// Panic reasons
enum TSettingServerPanic
    {
    EBadRequest,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ECreateTrapCleanup,
    ENoStartupInformation,
    ETooManyCloseSessions,
    EPopFromEmptyStack
    };

// Opcodes used in message passing between client and server
enum TSettingServerRequests
    {
    // RSettingServer requests
    ESettingServerCloseSession,

    // RLoggerSetting requests
    ELoadLoggerSettingsFromIniFile,
    ELoadLoggerSettingsFromCommandLine,
    EGetLoggerSettings,

    // e.g. SetAttribute requests
    ESetLoggerOutputPath,
    
    // Reset logger settings to default value
    EResetLoggerSettings,
    };

// Struct to pass parameters to server thread
struct TThreadStartSetting
    {
    TFileName  iName;         // Server name
    RThread    iServerThread; // The server thread
    RSemaphore iStarted;      // Startup syncronisation semaphore   
    TInt       iStartupResult;// Start-up result
    };

#endif /* SETTINGSERVERCLIENTSERVER_H */
// End of file
