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
* Description: This file contains TestScripter implementation.
*
*/

// INCLUDE FILES
#include "SubTestCaseRunner.h"
#include <TestEngineClient.h>
#include "TestScripter.h"
#include "Logging.h"
// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
#ifdef LOGGER
#undef LOGGER
#endif
#define LOGGER iTestRunner->GetLogger()

// LOCAL CONSTANTS AND MACROS
// None

// MODULE DATA STRUCTURES
// None

// LOCAL FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
// None

// ==================== LOCAL FUNCTIONS =======================================

// ================= MEMBER FUNCTIONS =========================================

/*
-------------------------------------------------------------------------------

     Class: CSubTestCaseRunner

     Method: CSubTestCaseRunner

     Description: Constructor

     Constructor.
     
     Parameters: CTestRunner* aTestRunner : in: Pointer to test runner which 
                         should be notified about test case end

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CSubTestCaseRunner::CSubTestCaseRunner()
:CActive( EPriorityStandard ), iState( ESTSIdle )
    {
    }

CSubTestCaseRunner::TSubTestCaseState CSubTestCaseRunner::GetState() const
    {
    return iState;
    }


/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: NewLC

     Description: Two-phased constructor.

     Two-phased constructor.
     
     Parameters: CTestRunner* aTestRunner : in: Pointer to test runner which 
                         should be notified about test case end

     Return Values: Pointer to newly created CLocalSubTestCaseRunner object.

     Errors/Exceptions: Leaves if there is some problem during method execution

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLocalSubTestCaseRunner* CLocalSubTestCaseRunner::NewLC( CTestRunner* aTestRunner )
    {
    CLocalSubTestCaseRunner* self = new(ELeave)CLocalSubTestCaseRunner( aTestRunner );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: NewL

     Description: Two-phased constructor.

     Two-phased constructor.
     
     Parameters: CTestRunner* aTestRunner : in: Pointer to test runner which 
                         should be notified about test case end

     Return Values: Pointer to newly created CLocalSubTestCaseRunner object.

     Errors/Exceptions: Leaves if there is some problem during method execution

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLocalSubTestCaseRunner* CLocalSubTestCaseRunner::NewL( CTestRunner* aTestRunner )
    {
    CLocalSubTestCaseRunner* self = NewL( aTestRunner );
    CleanupStack::Pop( self );
    return self;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: CLocalSubTestCaseRunner

     Description: Destructor

     Destructor
     
     Parameters: none

     Return Values: none

     Errors/Exceptions: none

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLocalSubTestCaseRunner::~CLocalSubTestCaseRunner()
    {
    CancelSubTestCaseL();
    
    delete iStartInfo;
    iTestCase.Close();
    iTestEngine.Close();
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: CLocalSubTestCaseRunner

     Description: Constructor

     Constructor.
     
     Parameters: CTestRunner* aTestRunner : in: Pointer to test runner which 
                         should be notified about test case end

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLocalSubTestCaseRunner::CLocalSubTestCaseRunner( CTestRunner* aTestRunner )
:iTestRunner( aTestRunner ), iResultPckg( iResult )
    {
    CActiveScheduler::Add( this );    
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: ConstructL

     Description: Second phase of two-phased constructor.

     Second phase of two-phased constructor.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::ConstructL()
    {
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: NewL

     Description: Handles test case execution end.

     Handles test case execution end and pass test case execution result to 
     proper CTestCaseRunner.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: Leaves if there is some problem during method execution

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::RunL()
    {
    // Store run result
    iRunResult = iStatus.Int();
    
    iTestCase.Close();
    iTestEngine.Close();    
    
    iState = ESTSIdle;
    
    // Report sub test case result to  CTestRunner
    iTestRunner->SubTestCaseFinishedL( this );
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: DoCancel

     Description: Cancels test case execution

     Cancels test case execution
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::DoCancel()
    {
    TInt ret = iTestCase.CancelAsyncRequest( RTestCase::ERunTestCase );
    if ( ret != KErrNone )
        {
        // There was some error during sub test case cancel
        // Only put message to log.
        __TRACE( KMessage, ( _L( "Unexpected error during sub test case testid=%S cancel." ), &iStartInfo->GetTestId() ) );
        }

    iTestCase.Close();
    iTestEngine.Close();    
    
    // Mart test case as canceled
    iResult.iTestResult.SetResult( KErrNone, KNullDesC );
    iResult.iCaseExecutionResultCode = KErrCancel;
    iResult.iCaseExecutionResultType = TFullTestResult::ECaseCancelled;
    iState = ESTSIdle;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: RunSubTestCaseL

     Description: Starts test case execution.

     Starts test case execution.
     
     Parameters: CStartInfo* aStartInfo : in: Pointer to structure which hold
                          information about test case which should be executed.

     Return Values: None

     Errors/Exceptions: Leaves if there is some problem during method execution

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::RunSubTestCaseL( CStartInfo& aStartTestCaseInfo )
    {
    // Check if this AO is already running some sub test case
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    
    // Reset runner internal state
    Reset();
    
    // Copy sub test case startup information
    CStartInfo* startInfo = CStartInfo::NewL();
    CleanupStack::PushL( startInfo );
    startInfo->CopyL( aStartTestCaseInfo );
    CleanupStack::Pop( startInfo );
    iStartInfo = startInfo;
    
    // Connect to TestEngine
    TInt ret = iTestEngine.Connect();
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    // Initialize TestEngine session
    CleanupClosePushL( iTestEngine );
    ret = iTestEngine.LoadConfiguration( KNullDesC() );    
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    // Load selected test module with specified ini file
    ret = iTestEngine.AddTestModule( iStartInfo->GetModuleName(),
            iStartInfo->GetIniFile() );
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    // Set optional cfg file
    if ( iStartInfo->GetConfig() != KNullDesC )
        {
        ret = iTestEngine.AddConfigFile( iStartInfo->GetModuleName(),
                iStartInfo->GetConfig() );
        if ( ret != KErrNone )
            {
            User::Leave( ret );
            }
        }

    // Get list of available test cases from test engine
    TRequestStatus enumerationRequestStatus;
    TCaseCount testCasesCount;
    iTestEngine.EnumerateTestCases( testCasesCount, enumerationRequestStatus );
    User::WaitForRequest( enumerationRequestStatus );
    if ( enumerationRequestStatus.Int() != KErrNone )
        {
        User::Leave( enumerationRequestStatus.Int() );
        }

    CFixedFlatArray<TTestInfo>* testCasesList = CFixedFlatArray<TTestInfo>::NewL( testCasesCount() );
    CleanupStack::PushL( testCasesList );

    ret = iTestEngine.GetTestCases( *testCasesList );
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    // Find requested test case in test cases enumerated from TestEngine 
    TBool findByTitle = ( iStartInfo->GetTitle() != KNullDesC );    
    TTestInfo testInfo;
    TBool foundTestCase = EFalse;
    if ( findByTitle )
        {
        iStartInfo->SetTestCaseNumber( -1 );
        }
    for ( TInt i = 0; i < testCasesList->Count(); i++ )
        {
        if ( ( findByTitle && ( (*testCasesList)[ i ].iTestCaseInfo.iTitle == iStartInfo->GetTitle() ) ) ||
             ( (*testCasesList)[ i ].iTestCaseInfo.iCaseNumber == iStartInfo->GetTestCaseNumber() ) )
            {
            testInfo = (*testCasesList)[ i ];
            foundTestCase = ETrue;
            }
        }

    // Store test case title
    iStartInfo->SetTitleL( testInfo.iTestCaseInfo.iTitle );

    // Check if requested test case is availabe or not
    if ( !foundTestCase )
        {
        User::Leave( KErrNotFound );
        }

    CleanupStack::PopAndDestroy( testCasesList );

    // Open test case subsession
    TTestInfoPckg testInfoPckg( testInfo );
    ret = iTestCase.Open( iTestEngine, testInfoPckg );
    if ( ret != KErrNone )
        {
        User::Leave( ret );
        }

    // Run sub test case
    CleanupClosePushL( iTestCase );
    iTestCase.RunTestCase( iResultPckg, iStatus );
    SetActive();
    CleanupStack::Pop(); // Remove iTestCase from cleanup stack
    CleanupStack::Pop(); // Remove iTestEngine from cleanup stack
    iState = ESTSRunning;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: PauseSubTestCaseL

     Description: Pauses selected sub test case.

     Pauses selected sub test case.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: Leaves if sub test case is not running

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::PauseSubTestCaseL()
    {
    // Check if sub test case is running
    if ( !IsActive() )
        {
        User::Leave( KErrNotReady );
        }
    
    User::LeaveIfError( iTestCase.Pause() );
    iState = ESTSPaused;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: ReasumeSubTestCaseL

     Description: Reasume selected sub test case.

     Reasume selected sub test case.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: Leaves if sub test case is not running

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::ResumeSubTestCaseL()
    {    
    // Check if sub test case is running
    if ( !IsActive() )
        {
        User::Leave( KErrNotReady );
        }
    
    User::LeaveIfError( iTestCase.Resume() );
    iState = ESTSRunning;
    }

void CLocalSubTestCaseRunner::CancelSubTestCaseL()
    {
    Cancel();
    }


/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: Reset

     Description: Resets sub test case runner internal state.

     Resets sub test case runner internal state.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLocalSubTestCaseRunner::Reset()
    {
    delete iStartInfo;
    iStartInfo = NULL;
        
    iRunResult = KErrNone;
    
    iResult.iCaseExecutionResultCode = KErrNone;
    iResult.iCaseExecutionResultType = TFullTestResult::ECaseOngoing;
    iResult.iTestResult.SetResult( KErrNone, KNullDesC );
    
    iState = ESTSIdle;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: GetRunResult

     Description: Returns execution result.

     Returns execution result.
     
     Parameters: None

     Return Values: Execution result

     Errors/Exceptions: 

     Status: Draft
    
-------------------------------------------------------------------------------
*/
TInt CLocalSubTestCaseRunner::GetRunResult() const
    {
    return iRunResult;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: GetTestCaseResult

     Description: Returns test case execution result.

     Returns test case execution result.
     
     Parameters: None

     Return Values: Test case execution result.

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
const TFullTestResult& CLocalSubTestCaseRunner::GetTestCaseResult() const    
    {
    return iResult;
    }

/*
-------------------------------------------------------------------------------

     Class: CLocalSubTestCaseRunner

     Method: GetStartInfo

     Description: Returns informations about started test case.

     Returns informations about started test case.
     
     Parameters: None

     Return Values: Returns information about started test case.

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
const CStartInfo* CLocalSubTestCaseRunner::GetStartInfo() const
    {
    return iStartInfo;
    }

CSubTestCaseRunner::TSubTestCaseType CLocalSubTestCaseRunner::GetType() const
    {
    return ESTTLocal;
    }




CRemoteSubTestCaseRunner* CRemoteSubTestCaseRunner::NewL( CTestRunner* aTestRunner, 
        CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy )
    {
    CRemoteSubTestCaseRunner* self = new(ELeave)CRemoteSubTestCaseRunner( 
            aTestRunner, aSlave, aRemoteCallsProxy );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;    
    }

CRemoteSubTestCaseRunner* CRemoteSubTestCaseRunner::NewLC( CTestRunner* aTestRunner, 
        CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy )
    {    
    CRemoteSubTestCaseRunner* self = new(ELeave)CRemoteSubTestCaseRunner( 
            aTestRunner, aSlave, aRemoteCallsProxy );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;    
    }

CRemoteSubTestCaseRunner::~CRemoteSubTestCaseRunner()
    {
    Cancel();
    
    iTimeoutTimer.Close();
    
    delete iNestedASLoop;
    iNestedASLoop = NULL;
    
    delete iStartInfo;
    iStartInfo = NULL;
    
    TRAPD( err, iSlave->UnregisterSubTestCaseL( this ) );
    if ( err != KErrNone )
        {
        RDebug::Print( _L("Unexpected error during slave subtestcase deregistration %d"), err );
        }
    }

void CRemoteSubTestCaseRunner::RunSubTestCaseL( CStartInfo& aStartTestCaseInfo )
    {
    // Check if this AO is already running some sub test case
    if ( IsActive() || ( iState != ESTSIdle ) )
        {
        User::Leave( KErrInUse );
        }
    
    // Reset runner internal state
    Reset();
    
    // Copy sub test case startup information
    CStartInfo* startInfo = CStartInfo::NewL();
    CleanupStack::PushL( startInfo );
    startInfo->CopyL( aStartTestCaseInfo );
    CleanupStack::Pop( startInfo );
    iStartInfo = startInfo;
    
    // Connect to TestEngine
    iRemoteCallsProxy->RunTestCaseL( iSlave->GetMasterId(), iSlave->GetSlaveId(), iStartInfo );
    
    iCurrentOperation = ECORunSubtestCase;
    
    iTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    
    iNestedASLoop->Start();
    
    User::LeaveIfError( iOperationResult );
    iState = ESTSRunning;
    }

void CRemoteSubTestCaseRunner::PauseSubTestCaseL()
    {
    // Check if sub test case is running
    if ( IsActive() || ( iState != ESTSRunning ) )
        {
        User::Leave( KErrNotReady );
        }

    iRemoteCallsProxy->PauseTestCaseL( iSlave->GetMasterId(), iSlave->GetSlaveId(), iTestCaseId );

    iCurrentOperation = ECOPauseSubtestCase;
    
    iTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    
    iNestedASLoop->Start();
    
    User::LeaveIfError( iOperationResult );
    iState = ESTSPaused;
    }

void CRemoteSubTestCaseRunner::ResumeSubTestCaseL()
    {    
    // Check if sub test case is running
    if ( IsActive() || ( iState != ESTSPaused ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iRemoteCallsProxy->ResumeTestCaseL( iSlave->GetMasterId(), iSlave->GetSlaveId(), iTestCaseId );

    iCurrentOperation = ECOResumeSubtestCase;
    
    iTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    
    iNestedASLoop->Start();
    
    User::LeaveIfError( iOperationResult );
    iState = ESTSRunning;
    }

void CRemoteSubTestCaseRunner::CancelSubTestCaseL()
    {
    // Check if sub test case is running
    if ( IsActive() || ( iState != ESTSIdle ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iRemoteCallsProxy->CancelTestCaseL( iSlave->GetMasterId(), iSlave->GetSlaveId(), iTestCaseId );

    iCurrentOperation = ECOCancelSubtestCase;
    
    iTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    
    iNestedASLoop->Start();
    
    User::LeaveIfError( iOperationResult );
    
    iRunResult = KErrCancel;
    // Mart test case as canceled
    iResult.iTestResult.SetResult( KErrNone, KNullDesC );
    iResult.iCaseExecutionResultCode = KErrCancel;
    iResult.iCaseExecutionResultType = TFullTestResult::ECaseCancelled;
    
    iState = ESTSIdle;
    }

TInt CRemoteSubTestCaseRunner::GetRunResult() const
    {
    return iRunResult;
    }

const TFullTestResult& CRemoteSubTestCaseRunner::GetTestCaseResult() const
    {
    return iResult;
    }

const CStartInfo* CRemoteSubTestCaseRunner::GetStartInfo() const
    {
    return iStartInfo;
    }

CSubTestCaseRunner::TSubTestCaseType CRemoteSubTestCaseRunner::GetType() const
    {
    return ESTTRemote;
    }

CRemoteSubTestCaseRunner::CRemoteSubTestCaseRunner( CTestRunner* aTestRunner, 
        CSlave* aSlave, CRemoteCallsProxy* aRemoteCallsProxy )
:iTestRunner( aTestRunner ), iSlave( aSlave ), 
 iRemoteCallsProxy( aRemoteCallsProxy ), iOperationTimeout( 30000000 )
    {
    CActiveScheduler::Add( this );
    }

void CRemoteSubTestCaseRunner::ConstructL()
    {
    User::LeaveIfError( iTimeoutTimer.CreateLocal() );
    iNestedASLoop = new(ELeave)CActiveSchedulerWait;
    iSlave->RegisterSubTestCaseL( this );
    }

void CRemoteSubTestCaseRunner::RunL()
    {
    // Test case timeouted.
    iCurrentOperation = ECONone;
    iOperationResult = KErrTimedOut;
    iNestedASLoop->AsyncStop();
    }

void CRemoteSubTestCaseRunner::DoCancel()
    {
    if ( GetState() == ESTSRunning )
        {
        TRAPD( err, CancelSubTestCaseL() );
        if ( err != KErrNone )
            {
            RDebug::Print( _L("Unexpected error during slave subtestcase cancel %d"), err );
            }
        }
    
    iTimeoutTimer.Cancel();
    }

void CRemoteSubTestCaseRunner::Reset()
    {
    delete iStartInfo;
    iStartInfo = NULL;
    iTestCaseId = 0;
    iRunResult = 0;
    iResult = TFullTestResult();
    }

CSlave* CRemoteSubTestCaseRunner::GetSlave()
    {
    return iSlave;
    }

TUint16 CRemoteSubTestCaseRunner::GetTestCaseId() const
    {
    return iTestCaseId;
    }

void CRemoteSubTestCaseRunner::NotifyTestCaseStartedL( TUint16 aTestCaseId )
    {
    if ( !iNestedASLoop->IsStarted() || ( iCurrentOperation != ECORunSubtestCase ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iTestCaseId = aTestCaseId;

    Cancel();
    
    iNestedASLoop->AsyncStop();
    iCurrentOperation = ECONone;
    iOperationResult = KErrNone;
    }

void CRemoteSubTestCaseRunner::NotifyTestCaseRunError( const TFullTestResult& aTestCaseResult )
    {
    if ( !iNestedASLoop->IsStarted() || ( iCurrentOperation != ECORunSubtestCase ) )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel();
    
    iResult = aTestCaseResult;
    
    iNestedASLoop->AsyncStop();
    iCurrentOperation = ECONone;
    iOperationResult = iResult.iCaseExecutionResultCode;
    }

void CRemoteSubTestCaseRunner::NotifyTestCasePausedL()
    {
    if ( !iNestedASLoop->IsStarted() || ( iCurrentOperation != ECOPauseSubtestCase ) )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel();
    
    iNestedASLoop->AsyncStop();
    iCurrentOperation = ECONone;
    iOperationResult = KErrNone;
    }

void CRemoteSubTestCaseRunner::NotifyTestCaseResumedL()
    {
    if ( !iNestedASLoop->IsStarted() || ( iCurrentOperation != ECOResumeSubtestCase ) )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel();
    
    iNestedASLoop->AsyncStop();
    iCurrentOperation = ECONone;
    iOperationResult = KErrNone;
    }

void CRemoteSubTestCaseRunner::NotifyTestCaseCancelledL()
    {
    if ( !iNestedASLoop->IsStarted() || ( iCurrentOperation != ECOCancelSubtestCase ) )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel();
    
    iNestedASLoop->AsyncStop();
    iCurrentOperation = ECONone;
    iOperationResult = KErrNone;
    }

void CRemoteSubTestCaseRunner::NotifyTestCaseFinishedL( const TFullTestResult& aTestCaseResult )
    {
    iRunResult = KErrNone;
    iResult = aTestCaseResult;
    
    iState = ESTSIdle;
    
    // Report sub test case result to  CTestRunner
    iTestRunner->SubTestCaseFinishedL( this );    
    }

TBool CRemoteSubTestCaseRunner::IsRunSubTestCaseRequestOngoing() const
    {
    if ( iCurrentOperation == ECORunSubtestCase )
        {
        return ETrue;
        }
    return EFalse;
    }

// EOF
