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
 * Description: This file contains the header file of the TestScripter 
 * module of STIF Test Framework.
 *
 */

#ifndef SUBTESTCASERUNNER_H_
#define SUBTESTCASERUNNER_H_

//  INCLUDES
#include <StifTestInterface.h>
#include <TestEngineClient.h>
#include "TestScripterUtils.h"

// CONSTANTS
// None

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
class CTestRunner;
class CSlave;
class CRemoteCallsProxy;

// CLASS DECLARATION
// DESCRIPTION
// CSubTestCaseRunner manages sub test cases execution and retuns 
// execution results to CTestRunner
NONSHARABLE_CLASS(CSubTestCaseRunner): public CActive
    {
public:
    enum TSubTestCaseType { ESTTLocal, ESTTRemote };
    enum TSubTestCaseState { ESTSIdle, ESTSRunning, ESTSPaused };
public:
    /**
     * Coinstructor.
     */
    CSubTestCaseRunner();

    /**
     * Starts execution of selected test case.
     */
    virtual void RunSubTestCaseL( CStartInfo& aStartTestCaseInfo ) = 0;
    
    /**
     * Pauses selected sub test case.
     */
    virtual void PauseSubTestCaseL() = 0;
    
    /**
     * Reasume selected sub test case.
     */
    virtual void ResumeSubTestCaseL() = 0;
    
    virtual void CancelSubTestCaseL() = 0;
    
    
    /**
     * Gets execution result.
     */
    virtual TInt GetRunResult() const = 0;

    /**
     * Gets test case execution result..
     */
    virtual const TFullTestResult& GetTestCaseResult() const = 0;
    
    /**
     * Gets executed test case info.
     */
    virtual const CStartInfo* GetStartInfo() const = 0;

    /**
     * Gets sub test case runner type.
     */
    virtual TSubTestCaseType GetType() const = 0;
    
    TSubTestCaseState GetState() const;
protected:
    TSubTestCaseState iState;
    }; // class CSubTestCaseRunner


// DESCRIPTION
// CSubTestCaseRunner manages sub test cases execution and retuns 
// execution results to CTestRunner
NONSHARABLE_CLASS(CLocalSubTestCaseRunner): public CSubTestCaseRunner
    {
public:
    /**
     * Two-phased constructor.
     */
    static CLocalSubTestCaseRunner* NewL( CTestRunner* aTestRunner );

    /**
     * Two-phased constructor.
     */
    static CLocalSubTestCaseRunner* NewLC( CTestRunner* aTestRunner );
public:
    /**
     * Destructor.
     */
    ~CLocalSubTestCaseRunner();

    /**
     * Starts execution of selected test case.
     */
    void RunSubTestCaseL( CStartInfo& aStartTestCaseInfo );
    
    /**
     * Pauses selected sub test case.
     */
    void PauseSubTestCaseL();
    
    /**
     * Reasume selected sub test case.
     */
    void ResumeSubTestCaseL();
    
    void CancelSubTestCaseL();
    
    /**
     * Gets execution result.
     */
    TInt GetRunResult() const;

    /**
     * Gets test case execution result..
     */
    const TFullTestResult& GetTestCaseResult() const;
    
    /**
     * Gets executed test case info.
     */
    const CStartInfo* GetStartInfo() const;

    /**
     * Gets sub test case runner type.
     */
    TSubTestCaseType GetType() const;    
private:
    /**
     * Constructor.
     */
    CLocalSubTestCaseRunner( CTestRunner* aTestRunner );

    /**
     * Second phase of two-phased constructor.
     */
    void ConstructL();
    
    /**
     * See CActive::RunL()
     */
    void RunL();

    /**
     * See CActive::DoCancel()
     */
    void DoCancel();
    
    /**
     * Resets runner internal state to default values.
     */
    void Reset();
private:
    // Pointer to test runner which will be notified about test case execution end.
    CTestRunner* iTestRunner;
    
    // Test engine session used to load test module and open RTestCase subsession
    RTestEngine iTestEngine;
    
    // Test case sub session used to run test case
    RTestCase iTestCase;

    // Copy of CStartInfo passed as an argument of RunSubTestCaseL call 
    CStartInfo* iStartInfo;
    
    // Value of iState.Int() after test case execution
    TInt iRunResult;
    
    // Result of test case execution
    TFullTestResult iResult;
    
    // Descriptor used to retrive result from TestEngine
    TFullTestResultPckg iResultPckg;
    }; // class CLocalSubTestCaseRunner


// DESCRIPTION
// CRemoteSubTestCaseRunner manages sub test cases execution and retuns 
// execution results to CTestRunner
NONSHARABLE_CLASS(CRemoteSubTestCaseRunner): public CSubTestCaseRunner
    {
    private:
        enum TCurrentOperation {    ECONone, 
                                    ECORunSubtestCase,
                                    ECOPauseSubtestCase,
                                    ECOResumeSubtestCase,
                                    ECOCancelSubtestCase };
    public:
        /**
         * Two-phased constructor.
         */
        static CRemoteSubTestCaseRunner* NewL( CTestRunner* aTestRunner, 
                CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy );
        
        /**
         * Two-phased constructor.
         */
        static CRemoteSubTestCaseRunner* NewLC( CTestRunner* aTestRunner, 
                CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy );
    public:
        /**
         * Destructor.
         */
        ~CRemoteSubTestCaseRunner();
    
        /**
         * Starts execution of selected test case.
         */
        void RunSubTestCaseL( CStartInfo& aStartTestCaseInfo );
        
        /**
         * Pauses selected sub test case.
         */
        void PauseSubTestCaseL();
        
        /**
         * Reasume selected sub test case.
         */
        void ResumeSubTestCaseL();
    
        void CancelSubTestCaseL();
        
        /**
         * Gets execution result.
         */
        TInt GetRunResult() const;
    
        /**
         * Gets test case execution result..
         */
        const TFullTestResult& GetTestCaseResult() const;
        
        /**
         * Gets executed test case info.
         */
        const CStartInfo* GetStartInfo() const;
    
        /**
         * Gets sub test case runner type.
         */
        TSubTestCaseType GetType() const;
        
        CSlave* GetSlave();
        
        TUint16 GetTestCaseId() const;
        
        void NotifyTestCaseStartedL( TUint16 aTestCaseId );
        void NotifyTestCaseRunError( const TFullTestResult& aTestCaseResult );
        
        void NotifyTestCasePausedL();
        void NotifyTestCaseResumedL();
        void NotifyTestCaseCancelledL();
        
        void NotifyTestCaseFinishedL( const TFullTestResult& aTestCaseResult );
        
        TBool IsRunSubTestCaseRequestOngoing() const;
    private:
        /**
         * Constructor.
         */
        CRemoteSubTestCaseRunner( CTestRunner* aTestRunner, 
                CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy );

        /**
         * Second phase of two-phased constructor.
         */
        void ConstructL();
        
        /**
         * See CActive::RunL()
         */
        void RunL();

        /**
         * See CActive::DoCancel()
         */
        void DoCancel();
        
        void Reset();
    private:
        // Pointer to test runner which will be notified about test case execution end.
        CTestRunner* iTestRunner;
        CSlave* iSlave;
        CRemoteCallsProxy* iRemoteCallsProxy;
                        
        // Copy of CStartInfo passed as an argument of RunSubTestCaseL call 
        CStartInfo* iStartInfo;
        
        TUint16 iTestCaseId;
                
        // Value of iState.Int() after test case execution
        TInt iRunResult;
        
        // Result of test case execution
        TFullTestResult iResult;
        
        TCurrentOperation iCurrentOperation;
        TInt iOperationResult;
        
        TTimeIntervalMicroSeconds32 iOperationTimeout;
        RTimer iTimeoutTimer;
        
        CActiveSchedulerWait* iNestedASLoop;
    }; // class CRemoteSubTestCaseRunner



#endif /* SUBTESTCASERUNNER_H_ */
// EOF
