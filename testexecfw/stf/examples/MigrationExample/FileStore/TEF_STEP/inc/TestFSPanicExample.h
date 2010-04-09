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

#ifndef TESTFSPANICEXAMPLE_H_
#define TESTFSPANICEXAMPLE_H_
#include <TestExecuteStepBase.h>

// Literal constant for test step name
_LIT(KPanicExample,"PanicExample");

class CTestFSPanicExample : public CTestStep
    {
public:
    CTestFSPanicExample();
    ~CTestFSPanicExample();
    virtual TVerdict doTestStepL();
    };

#endif /* TESTFSPANICEXAMPLE_H_ */
