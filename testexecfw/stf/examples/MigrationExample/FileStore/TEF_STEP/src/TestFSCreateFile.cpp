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
* Description:
*
*/

// User includes
#include "TestFSCreateFile.h"
// Literal constants 
_LIT(KFileName, "filename");

CTestFSCreateFile::CTestFSCreateFile()
/**
 Constructor.

 Sets the test step name.
 */
    {
    // Call base class method to set up the human readable name for logging
    SetTestStepName(KCreateFile);
    }

CTestFSCreateFile::~CTestFSCreateFile()
/**
 Destructor
 */
    {
    }

TVerdict CTestFSCreateFile::doTestStepL()
/**
 This is implementation of the pure virtual function of CTestStep.
 It creates files or directories
 @return TVerdict - the test result
 */
    {
    INFO_PRINTF1(_L("In test step CreateFile"));

    if (TestStepResult() == EPass)
        {

        // Get file or directory name
        TPtrC file;
        if (GetStringFromConfig(ConfigSection(), KFileName, file))
            {
            INFO_PRINTF2(_L("File name %S"), &file);
            }
        else
            {
            ERR_PRINTF1(_L("No File Name"));
            SetTestStepResult(EFail);
            }
        
        RFile rf;
        RFs fileSession;
        CleanupClosePushL(rf);
        TRAPD(error, fileSession.Connect())
        if ( error != KErrNone)
            {
            ERR_PRINTF2(_L("File Session connect error, %d"), error);
            SetTestStepResult(EFail);
            }
        CleanupClosePushL(fileSession);
        TInt errCode = rf.Create(fileSession, file, EFileWrite);
        CleanupStack::PopAndDestroy(&fileSession);
        CleanupStack::PopAndDestroy(&rf);
                
        
        if (KErrNone != errCode)
            {
            SetTestStepError(errCode);
            ERR_PRINTF2(_L("--->Error in Creating file, get result %D"), errCode);
            SetTestStepResult(EFail);
            }
        
        }
        return TestStepResult();
    }

