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
#include "TestFSPanicExample.h"

_LIT(KPanicString, "panicString");
_LIT(KPanicCode, "panicCode");

CTestFSPanicExample::CTestFSPanicExample()
    {
    // TODO Auto-generated constructor stub
    SetTestStepName(KPanicExample);
    }

CTestFSPanicExample::~CTestFSPanicExample()
    {
    // TODO Auto-generated destructor stub
    }


TVerdict CTestFSPanicExample::doTestStepL()
    {
    TInt panicCode = 0;
    if (GetIntFromConfig(ConfigSection(), KPanicCode, panicCode))
        {
        INFO_PRINTF2(_L("Expected Panic Code= %d"), panicCode);
        }
    else
        {
        ERR_PRINTF1(_L("Panic Code not set"));
        SetTestStepResult(EFail);
        }
    // Get file or directory name
    TPtrC panicString;
    if (GetStringFromConfig(ConfigSection(), KPanicString, panicString))
        {
        INFO_PRINTF2(_L("Expected Panic String= %S"), &panicString);
        }
    else
        {
        ERR_PRINTF1(_L("Panic String not set"));
        SetTestStepResult(EFail);
        }
    
    User::Panic(panicString, panicCode);    
    return TestStepResult();
    }
