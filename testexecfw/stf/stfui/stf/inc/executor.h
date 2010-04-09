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
* Description: CExecutor is used to execute the testing via STIF engine.
*
*/

#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include <e32std.h>
#include <e32base.h>
#include <e32cons.h>
#include <e32svr.h>
#include "activetimer.h"
#include <TestEngineClient.h>
#include "TestModuleInfo.h"
#include <StifTestInterface.h>
#include "cmdparser.h"

class CCaseRunner;

class CExecutor : public CBase
    {
public:
    static CExecutor* NewL(CCmdParser* aParser,CConsoleBase* aConsole);
    ~CExecutor();
    
public:
    void RunTestsL();
    void TestCompleted(TInt aError);
    RTestEngine& TestEngine();
  
private:
    void ConstructL();
    CExecutor(CCmdParser* aParser,CConsoleBase* aConsole);
    void RunAllTestCasesL();
    void RunTestCaseL(TTestInfo& aTestInfo);
    TInt RunATestCaseL(CCaseRunner* aCaseRunner);
    void LogErrorAndLeaveL(const TDesC& aFunction, const TDesC& aDescription, const TInt aError);
    void ParseTestModulesL(CStifParser* aParser, CTestModuleList* aModuleList, const TDesC& aSectionStart, const TDesC& aSectionEnd);
    
private:
    CConsoleBase* iConsole;
    RTestEngine iTestEngine;
    TInt iTestCompletedError;
    CTestModuleList* iModuleList;
    CCmdParser* iParser;
    CFixedFlatArray<TTestInfo>* iTestCasesList;
    
    
    };

#endif /* EXECUTOR_H_ */
