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
#include "caserunner.h"

CCaseRunner::CCaseRunner(CExecutor* aExecutor, TTestInfo& aTestInfo) :
    CActive(CActive::EPriorityStandard),
    iExecutor(aExecutor),
    iTestInfo(aTestInfo),
    iTestInfoPckg(iTestInfo),
    iFullTestResultPckg(iFullTestResult)
    {
    __ASSERT_ALWAYS( aExecutor, User::Panic( _L("Null pointer"), KErrGeneral ) );
    CActiveScheduler::Add( this );
    }

void CCaseRunner::ContructL()
    {
    iTestEngine = iExecutor->TestEngine();
    User::LeaveIfError(iTestCase.Open(iTestEngine, iTestInfoPckg));
    }

CCaseRunner* CCaseRunner::NewL(CExecutor* aExecutor, TTestInfo& aTestInfo)
    {
    CCaseRunner* self = new(ELeave) CCaseRunner(aExecutor, aTestInfo);
    CleanupStack::PushL(self);
    self->ContructL();
    CleanupStack::Pop(self);
    return self;
    }

CCaseRunner::~CCaseRunner()
    {
    Cancel();
    iTestCase.Close();
    }

void CCaseRunner::StartTestL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("Start test case %d: "), iTestInfo.iTestCaseInfo.iCaseNumber );
#endif
    if(IsActive())
        {
        Cancel();
        }
    iTestCase.RunTestCase(iFullTestResultPckg, iStatus);
    SetActive();
    
    }

void CCaseRunner::RunL()
    {
#ifdef _DEBUG
    RDebug::Print(_L("RunTestCase completed: [%d]"), iStatus.Int());
#endif
    User::LeaveIfError(iStatus.Int());
    
    if(iFullTestResult.iCaseExecutionResultType != TFullTestResult::ECaseExecuted)
        {
#ifdef _DEBUG
        RDebug::Print(_L("Test case execution failed: [%d] "),iFullTestResult.iCaseExecutionResultCode );
#endif
        iExecutor->TestCompleted(iFullTestResult.iCaseExecutionResultCode);
        }
    else
        {
#ifdef _DEBUG
        // Debug test result
        RDebug::Print( _L("Test case execution completed[%d]: %S"), 
                            iFullTestResult.iTestResult.iResult,  
                            &iFullTestResult.iTestResult.iResultDes);
#endif
        iExecutor->TestCompleted( iFullTestResult.iTestResult.iResult );
        }
    
    
    }

void CCaseRunner::DoCancel()
    {
    iTestCase.CancelAsyncRequest(RTestCase::ERunTestCase);
    }

TInt CCaseRunner::RunError(TInt aError)
    {
#ifdef _DEBUG
    RDebug::Print(_L("Test case execution failed: [%d] "),aError );
#endif
    iExecutor->TestCompleted( aError );
    return KErrNone;       
    }
