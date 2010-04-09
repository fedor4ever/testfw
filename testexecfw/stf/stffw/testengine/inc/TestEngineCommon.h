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

#ifndef TESTENGINECOMMON_H
#define TESTENGINECOMMON_H

//  INCLUDES
#include <StifLogger.h>

// DATA TYPES

// Panic reasons
enum TTestEnginePanic
    {
    EBadRequest,
    EBadDescriptor,
    EMainSchedulerError,
    ESvrCreateServer,
    ECreateTrapCleanup,
    EBadSubsessionHandle,
    ENoStartupInformation,
    EInvalidModuleName,
    EReqPending,
    EDoCancelDisorder,
    ECancelAsyncIPCError,
    EEventPanic
    };

// Struct to Test report settings
class CTestReportSettings
    {
    public:
    
        /**
        * NewL is two-phased constructor.
        */
        static CTestReportSettings* NewL();


       ~CTestReportSettings();
       
    private:
       CTestReportSettings();
       /**
       * By default Symbian OS constructor is private.
       */
       void ConstructL();

    public:

        TBool                   iCreateTestReport; // Test report creation mode
        HBufC*                  iPath;             // Test report path
        HBufC*                  iName;             // Test report name
        CStifLogger::TLoggerType iFormat;          // Test report file type
        CStifLogger::TOutput    iOutput;           // Test report output
        TBool                   iOverwrite;        // Test report overwrite
        TBool                   iXML;              // Test report in xml format

    };

#endif /* TESTENGINECOMMON_H */

// End of file
