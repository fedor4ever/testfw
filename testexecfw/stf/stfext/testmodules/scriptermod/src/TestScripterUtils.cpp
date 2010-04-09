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
#include "TestScripterUtils.h"
#include <f32file.h>
#include "TestScripter.h"
#include "Logging.h"

// EXTERNAL DATA STRUCTURES
// None

// EXTERNAL FUNCTION PROTOTYPES  
// None

// CONSTANTS
// None

// MACROS
// None

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

     Class: CStartInfo

     Method: CStartInfo

     Description: Default constructor

     C++ default constructor can NOT contain any code, that
     might leave.
     
     Parameters: None

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CStartInfo::CStartInfo():
iCategory( TFullTestResult::ECaseExecuted ) 
    {
    }
     
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: ConstructL

     Description: Symbian OS second phase constructor

     Symbian OS default constructor can leave.

     Parameters:    None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CStartInfo::ConstructL()
    {
    iModule = HBufC::NewL( 0 );
    iIniFile = HBufC::NewL( 0 );
    iConfig = HBufC::NewL( 0 );
    iTestId = HBufC::NewL( 0 );
    iTitle = HBufC::NewL( 0 );
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: NewL

     Description: Two-phased constructor.
          
     Parameters:    None
     
     Return Values: CStartInfo*: new object

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CStartInfo* CStartInfo::NewL()
    {
    CStartInfo* self = new (ELeave) CStartInfo();
     
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }    
    
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: ~CStartInfo

     Description: Destructor
     
     Parameters:    None

     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/     
CStartInfo::~CStartInfo()
    {
    delete iModule;
    delete iIniFile;
    delete iConfig;
    delete iTestId;
    delete iTitle;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetModuleNameL

     Description: Set module name.
     
     Parameters: TDesC& aModule: in: Module name 
                 TInt aExtLength: in: Extra length reserved for buffer
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/
 void CStartInfo::SetModuleNameL( const TDesC& aModule, TInt aExtLength )
    {
    HBufC* module = HBufC::NewL( aModule.Length() + aExtLength );
    CleanupStack::PushL( module );
    TPtr modulePtr( module->Des() );
    modulePtr.Append( aModule );
    
    // Remove optional '.DLL' from file name
    modulePtr.LowerCase();
    TParse parse;
    parse.Set( *module, NULL, NULL );
    
    if ( parse.Ext() == _L(".dll") )
        {
        const TInt len = parse.Ext().Length();
        modulePtr.Delete ( modulePtr.Length() - len, len );
        }
    CleanupStack::Pop( module );
    delete iModule;
    iModule = module;
    };
            
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetIniFileL

     Description: Set initialization file name name.
     
     Parameters: TDesC& aIni: in: Initialization file name
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetIniFileL( const TDesC& aIni )
    {
    HBufC* inifile = aIni.AllocL();
    delete iIniFile;
    iIniFile = inifile;
    };
            
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetConfigL

     Description: Set configuration file name name.
     
     Parameters: TDesC& aConfig: in: Configuration file name
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetConfigL( const TDesC& aConfig )
    {
    HBufC* config = aConfig.AllocL();
    delete iConfig;
    iConfig = config;
    };
            
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetTestIdL

     Description: Set test identifier.
     
     Parameters: TDesC& aTestId: in: test identifier
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetTestIdL( const TDesC& aTestId )
    {
    HBufC* testId = aTestId.AllocL();
    delete iTestId;
    iTestId = testId;
    };

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetTitleL

     Description: Set title.
     
     Parameters: TDesC& aTitle: in: Test case title
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetTitleL( const TDesC& aTitle)
    {
    HBufC* title = aTitle.AllocL();
    delete iTitle;
    iTitle = title;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetTestCaseNumber

     Description: Sets test case index.
     
     Parameters: TInt aTestCaseNumber: in: Test case index
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetTestCaseNumber( TInt aTestCaseNumber )
    {
    iCaseNum = aTestCaseNumber;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetExpectedResult

     Description: Sets test case expected result
     
     Parameters: TInt aExpectedResult: in: Test case expected result
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetExpectedResult( TInt aExpectedResult )
    {    
    iExpectedResult = aExpectedResult;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetExpectedResultCategory

     Description: Sets test case expected result category
     
     Parameters: TFullTestResult::TCaseExecutionResult aCategory: in: Test case expected result category
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetExpectedResultCategory( TFullTestResult::TCaseExecutionResult aCategory )
    {
    iCategory = aCategory;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: SetTimeout

     Description: Sets test case timeout value
     
     Parameters:  TInt aTimeout: in: Test case timeout value
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::SetTimeout( TInt aTimeout )
    {    
    iTimeout = aTimeout;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetModuleName

     Description: Gets test module name.
     
     Parameters:  None
     
     Return Values: Test module name

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
const TDesC& CStartInfo::GetModuleName() const
    {
    return *iModule;
    }
    
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetIniFile

     Description: Gets ini file path
     
     Parameters:  None
     
     Return Values: Ini file path

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
const TDesC& CStartInfo::GetIniFile() const
    {
    return *iIniFile;
    }
    
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetConfig

     Description: Gets config file path
     
     Parameters:  None
     
     Return Values: Config file path

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
const TDesC& CStartInfo::GetConfig() const
    {
    return *iConfig;
    }
    
/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetTestId

     Description: Gets test case id
     
     Parameters:  None
     
     Return Values: Test case id

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
const TDesC& CStartInfo::GetTestId() const
    {
    return *iTestId;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetTitle

     Description: Gets test case title
     
     Parameters:  None
     
     Return Values: Test case title

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
const TDesC& CStartInfo::GetTitle() const
    {
    return *iTitle;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetTestCaseNumber

     Description: Gets test case index
     
     Parameters:  None
     
     Return Values: Test case index

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
TInt CStartInfo::GetTestCaseNumber() const
    {
    return iCaseNum;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetExpectedResult

     Description: Gets test case expected result
     
     Parameters:  None
     
     Return Values: Test case expected result

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
TInt CStartInfo::GetExpectedResult() const
    {
    return iExpectedResult;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetExpectedResultCategory

     Description: Gets test case expected result category
     
     Parameters:  None
     
     Return Values: Test case expected result category

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
TFullTestResult::TCaseExecutionResult CStartInfo::GetExpectedResultCategory() const
    {    
    return iCategory;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: GetTimeout

     Description: Gets test case timeout value
     
     Parameters:  None
     
     Return Values: Test case timeout value

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
TInt CStartInfo::GetTimeout() const
    {
    return iTimeout;
    }

/*
-------------------------------------------------------------------------------

     Class: CStartInfo

     Method: CopyL

     Description: Copy values from other CStartInfo instance.
     
     Parameters:  const CStartInfo& aStartInfo: in: CStartInfo instance
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/        
void CStartInfo::CopyL( const CStartInfo& aStartInfo )
    {
    SetConfigL( aStartInfo.GetConfig() );
    SetExpectedResult( aStartInfo.GetExpectedResult() );
    SetExpectedResultCategory( aStartInfo.GetExpectedResultCategory() );
    SetIniFileL( aStartInfo.GetIniFile() );
    SetModuleNameL( aStartInfo.GetModuleName() );
    SetTestCaseNumber( aStartInfo.GetTestCaseNumber() );
    SetTestIdL( aStartInfo.GetTestId() );
    SetTimeout( aStartInfo.GetTimeout() );
    SetTitleL( aStartInfo.GetTitle() );    
    }


/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: NewL

     Description: Two-phased constructor.
          
     Parameters: CTestRunner* aTestRunner: in: Pointer to CTestRunner
     
     Return Values: CLoopHelper*: new object

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLoopHelper* CLoopHelper::NewL( CTestRunner* aTestRunner )
    {    
    CLoopHelper* self = new(ELeave)CLoopHelper( aTestRunner );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: ~CLoopHelper

     Description: Destructor.
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLoopHelper::~CLoopHelper()
    {
    Cancel();
    iInLoopSubTestCases.Reset();
    iInLoopSubTestCases.Close();    
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: CLoopHelper

     Description: Constructor.
          
     Parameters: CTestRunner* aTestRunner: in: Pointer to CTestRunner
     
     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
CLoopHelper::CLoopHelper( CTestRunner* aTestRunner )
:CActive( EPriorityStandard ), iTestRunner( aTestRunner )
    {    
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: ConstructL

     Description: Two-phased constructor.
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::ConstructL()
    {    
    CActiveScheduler::Add( this );
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: LoopStartL

     Description: Should be to indicate loop start.
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::LoopStartL()
    {
    IterationBeginL( ETrue );
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: IterationEndStartNextIterationL

     Description: Should be called to indicate end of iteration and begining of next iteration
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::IterationEndStartNextIterationL()
    {
    IterationEndL( EFalse );
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: LoopEndL

     Description: Should be called to indicate loop end
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::LoopEndL()
    {
    IterationEndL( ETrue );
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: IterationBeginL

     Description: Indicate begining of the loop iteration
          
     Parameters: TBool aFirstIteration: in: Indicates if it is first iteration
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::IterationBeginL( TBool aFirstIteration )
    {
    // Check if it is first iteration. If yes then reset CLoopHelper state
    if ( aFirstIteration )
        {
        if ( iInProgress )
            {
            User::Leave( KErrInUse );
            }
        iPassedIterationsCounter = 0;
        iInProgress = ETrue;
        iLoopEnd = EFalse;
        }

    // Check for invalid calls order
    if ( iIterationOngoing )
        {
        User::Leave( KErrInUse );
        }
    iIterationOngoing = ETrue;
    iPassCurrentIteration = ETrue;    
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: IterationEndL

     Description: Indicate end of the loop iteration
          
     Parameters: TBool aLastIteration: in: Indicates if it is last iteration
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::IterationEndL( TBool aLastIteration )
    {
    // Perform tests to check if methods were called in proper order
    if ( IsActive() )
        {
        User::Leave( KErrInUse );
        }
    
    if ( !( iInProgress && iIterationOngoing ) )
        {
        User::Leave( KErrNotReady );
        }
    
    if ( aLastIteration )
        {
        iLoopEnd = ETrue;
        }

    // It is end of loop iteration. Wait for all sub test cases executed inside loop
    // to finish its execution.
    iStatus = KRequestPending;
    SetActive();
    if ( iInLoopSubTestCases.Count() == 0 )
        {
        // All sub test cases executed inside loop are already finished.
        // Schedule activation of CTestRunner to continue script execution after endloop keyword
        // ( it is done in CLoopHelper::RunL )
        TRequestStatus* status = &iStatus;
        User::RequestComplete( status, KErrNone );
        }
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: RegisterInLoopSubTestCaseL

     Description: Should be called to register sub test case executed inside loop
          
     Parameters: CSubTestCaseRunner* aSubTestCaseRunner: in: Sub test case runner 
                 to be registered
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::RegisterInLoopSubTestCaseL( CSubTestCaseRunner* aSubTestCaseRunner )
    {
    // Check if loop iteration is ongoing
    if ( !( iInProgress && iIterationOngoing ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iInLoopSubTestCases.AppendL( aSubTestCaseRunner );
    }

void CLoopHelper::UnregisterInLoopSubTestCaseL( CSubTestCaseRunner* aSubTestCaseRunner )
    {
    // Check if loop iteration is ongoing
    if ( !( iInProgress && iIterationOngoing ) )
        {
        User::Leave( KErrNotReady );
        }
    
    TInt idx = iInLoopSubTestCases.Find( aSubTestCaseRunner );
    if ( idx >= 0 )
        {
        iInLoopSubTestCases.Remove( idx );
        }    
    }

void CLoopHelper::RegisterInLoopSlaveL( CSlave* aSlave )
    {
    // Check if loop iteration is ongoing
    if ( !( iInProgress && iIterationOngoing ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iInLoopSlaves.AppendL( aSlave );
    }

void CLoopHelper::UnregisterInLoopSlaveL( CSlave* aSlave )
    {
    // Check if loop iteration is ongoing
    if ( !( iInProgress && iIterationOngoing ) )
        {
        User::Leave( KErrNotReady );
        }
    TInt idx = iInLoopSlaves.Find( aSlave );
    if ( idx >= 0 )
        {
        iInLoopSlaves.Remove( idx );
        }
    }

RPointerArray<CSlave>& CLoopHelper::GetRegisteredInLoopSlaves()
    {
    return iInLoopSlaves;
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: NotifySubTestCaseEndL

     Description: Should be called to indicate end of sub test case executed inside loop
          
     Parameters:  CSubTestCaseRunner* aSubTestCaseRunner: in: Sub test case which ended
                  TBool aPassed: in: Indicates if test case passed or not
     
     Return Values: None

     Errors/Exceptions: Leaves if new or ConstructL leaves.

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::NotifySubTestCaseEndL( CSubTestCaseRunner* aSubTestCaseRunner, TBool aPassed )
    {
    // Check if execution of the loop is ongoing
    if ( !iInProgress )
        {
        User::Leave( KErrNotReady );
        }
    
    // Check if selected sub test case was executed inside loop
    TInt idx = iInLoopSubTestCases.Find( aSubTestCaseRunner );
    if ( idx >= 0 )
        {
        iInLoopSubTestCases.Remove( idx );
        if ( !aPassed )
            {
            iPassCurrentIteration = EFalse;
            }
        
        // Check if there are other ont finished sub test cases executed inside loop
        if ( iInLoopSubTestCases.Count() == 0 )
            {
            // Check if CLoopHelper is active.
            if ( IsActive() )
                {
                // CLoopHelper is active. It means that IterationEndL method was called 
                // and CTestRunner currently waiting for sub test end before executing next
                // iteration of the loop. Since all sub test cases executed inside lopp are 
                // already finished we can activate CTestRunner ( it is done in CLoopHelper::RunL )
                TRequestStatus* status = &iStatus;
                User::RequestComplete( status, KErrNone );
                }
            }
        }
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: LoopInProgress

     Description: Indicates if loop execution is in progress
          
     Parameters: None
     
     Return Values: True if loop is in progress.

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
TBool CLoopHelper::LoopInProgress() const
    {
    return iInProgress;
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: DoCancel

     Description: See CActive::DoCancel
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::DoCancel()
    {
    TRequestStatus* status = &iStatus;
    User::RequestComplete( status, KErrCancel );
    }

/*
-------------------------------------------------------------------------------

     Class: CLoopHelper

     Method: RunL

     Description: See CActive::RunL
          
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
void CLoopHelper::RunL()
    {
    // Iteration has ended and all sub test cases executed inside loop has ended.
    User::LeaveIfError( iStatus.Int() );
    iIterationOngoing = EFalse;

    if ( iPassCurrentIteration )
        {
        iPassedIterationsCounter++;
        }

    // Activate CTestRunner to continue test case execution
    iTestRunner->SetRunnerActive();
    
    if ( iLoopEnd )
        {
        iTestRunner->ReleaseRemoteResourcesAllocatedInLoopL();
        // It was last iteration. Report loop result to CTestRunner
        iTestRunner->ReportLoopEndResultL( iPassedIterationsCounter );
        // Reset CLoopHelper state
        iLoopEnd = EFalse;
        iInProgress = EFalse;
        }
    else
        {
        // Begin next loop iteration
        IterationBeginL( EFalse );    
        }
    }
    
/*
-------------------------------------------------------------------------------

     Class: CSlaveInfo

     Method: GetEvent

     Description: Returns event with given name.

     Parameters:  TDesC& aEventName: in; Event name

     Return Values: TEventTc: Event structure

     Errors/Exceptions: None

     Status: Draft
    
-------------------------------------------------------------------------------
*/
//TEventTc* CSlaveInfo::GetEvent( TDesC& aEventName )
//    {
//    
//    TInt count = iEvents.Count();
//    for( TInt i = 0; i < count; i++ )
//        {
//        if( iEvents[i]->Name() == aEventName )
//            {
//            return iEvents[i];
//            }
//        }
//    return NULL;
//    
//    }

#ifdef LOGGER
#undef LOGGER
#endif

#define LOGGER iLogger

/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: TEventTS

     Description: Default constructor

     C++ default constructor can NOT contain any code, that
     might leave.
     
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/
TEventTS::TEventTS()
:iClientReqStatus( NULL )
    {
    }
        
/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: TEventTS

     Description: Parametric constructor

     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/ 
TEventTS::TEventTS( TName& aEventName )
:iClientReqStatus( NULL )
    {
    SetName( aEventName );
    SetType( EReqEvent );
    }

/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: ~TEventTS

     Description: Destructor

     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/ 
TEventTS::~TEventTS()
    { 
    Complete( KErrNone ); 
    }
/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: SetRequestStatus

     Description: Set request status member.

     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/ 
void TEventTS::SetRequestStatus( TRequestStatus* aStatus )
    { 
    iClientReqStatus = aStatus; 
    *iClientReqStatus = KRequestPending;
    }

/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: Complete

     Description: Complete request status member.
     
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/ 
void TEventTS::Complete( TInt aError )
    { 
    if( iClientReqStatus )
        { 
        User::RequestComplete( iClientReqStatus, aError ); 
        }
    }

/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: SetEvent

     Description: Set event pending.
     
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/ 
void TEventTS::SetEvent( TEventType aEventType )
    {
    SetEventType( aEventType );
    if( iClientReqStatus )
        {
        Complete( KErrNone );
        if( EventType() == EState )
            {
            SetType( ESetEvent ) ;
            }
        }
    else 
        {
        SetType( ESetEvent ) ;
        }
    }
   
/*
-------------------------------------------------------------------------------

     Class: TEventTS

     Method: WaitEvent

     Description: Wait event.
     
     Parameters: None
     
     Return Values: None

     Errors/Exceptions: None

     Status: Proposal
    
-------------------------------------------------------------------------------
*/
void TEventTS::WaitEvent( TRequestStatus& aStatus )
    {
    SetRequestStatus( &aStatus );
    if( Type() == ESetEvent )
        {
        Complete( KErrNone );
        if( EventType() == EIndication )
            {
            SetType( EReqEvent );
            }
        }
    }

CRemoteCallsProxy* CRemoteCallsProxy::NewL( CTestModuleIf& aTestModuleIf, CStifLogger* aLogger )
    {
    CRemoteCallsProxy* self = new(ELeave)CRemoteCallsProxy( aTestModuleIf, aLogger );
    CleanupStack::PushL( self );
    self->ConstructL(); 
    CleanupStack::Pop( self );
    return self;
    }

CRemoteCallsProxy::~CRemoteCallsProxy()
    {
    Cancel();
    
    iLogger = NULL;
    iSlaveAllocateFreeMonitor = NULL;
    iRemoteTestCasesMonitor = NULL;
    iRemoteEventsMonitor = NULL;
    iRemoteSendReceiveMonitor = NULL;
    iReceiveErrorHandler = NULL;    
    }

CRemoteCallsProxy::CRemoteCallsProxy( CTestModuleIf& aTestModuleIf, CStifLogger* aLogger )
: CActive( EPriorityNormal ), iLogger( aLogger ), iTestModuleIf( aTestModuleIf )
    {
    CActiveScheduler::Add( this );
    }

void CRemoteCallsProxy::ConstructL()
    {
    StartReceivingL();
    }

void CRemoteCallsProxy::DoCancel()
    {
    iTestModuleIf.RemoteReceiveCancel();
    }

void CRemoteCallsProxy::RunL()
    {
    RDebug::Print( _L("[STIF Master received response] %S"), &iReceivedRemoteMsg );    
    
    DispatchReceivedRemoteMsgL();
    StartReceivingL();
    }

TInt CRemoteCallsProxy::RunError( TInt aError )
    {
    if ( iReceiveErrorHandler != NULL )
        {
        iReceiveErrorHandler->HandleRemoteReceiveError( aError, iReceiveErrorDescription );
        return KErrNone;
        }
    return aError;
    }

void CRemoteCallsProxy::AllocateL( TUint32 aMasterId, const TDesC& aType )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();    
    // Reserve message
    User::LeaveIfError(
            remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgReserve ) );
    // Srcid. i.e. master id
    User::LeaveIfError(
            remoteRequest->AppendId( aMasterId ) );
    // DstId, broacast id
    User::LeaveIfError(
            remoteRequest->AppendId( 0 ) );
    // Slave type
    User::LeaveIfError( 
            remoteRequest->Append( aType ) );

    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );
    
    User::LeaveIfError( 
        iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
    
    CleanupStack::PopAndDestroy( remoteRequest );
    }

void CRemoteCallsProxy::FreeL( TUint32 aMasterId, TUint16 aSlaveId )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    
    // Release message
    User::LeaveIfError( 
            remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRelease ) );
    // Srcid. i.e. master id
    User::LeaveIfError( 
            remoteRequest->AppendId( aMasterId ) );
    // DstId is device broadcast
    User::LeaveIfError( 
            remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
        iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
        
    CleanupStack::PopAndDestroy( remoteRequest );
    }

void CRemoteCallsProxy::SendReceiveL(  TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aMessage )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError(
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    // Run command
    User::LeaveIfError( remoteRequest->Append( 
        CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdSendReceive ) );
    // asynchronous sendreceive's parameters    
    if ( aMessage != KNullDesC )
        {        
        // Append parameters
        User::LeaveIfError( remoteRequest->Append( aMessage ) );  
        }
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );
    User::LeaveIfError( 
        iTestModuleIf.RemoteSend( remoteRequest->Message() ) );

    CleanupStack::PopAndDestroy( remoteRequest );
    }

void CRemoteCallsProxy::SendUnknownL(  TUint32 aMasterId, TUint16 aSlaveId, 
        const TDesC& aCommand, const TDesC& aMessage )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError(
            remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    
    // Append command name
    User::LeaveIfError( remoteRequest->Append( aCommand ) );
    
    if ( aMessage != KNullDesC )
        {        
        // Append parameters
        User::LeaveIfError( remoteRequest->Append( aMessage ) );  
        }
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
        iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
                
    CleanupStack::PopAndDestroy( remoteRequest );
    }


void CRemoteCallsProxy::RunTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, CStartInfo* aStartInfo )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    remoteRequest->CreateL();
    
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId is device broadcast
    User::LeaveIfError( 
        remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    // Run command
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdRun ) );
    // Run parameters
    User::LeaveIfError(
        remoteRequest->Append( CStifTFwIfProt::RunParams, 
                CStifTFwIfProt::ERunModule, 
                aStartInfo->GetModuleName() ) );
    User::LeaveIfError(
        remoteRequest->Append( CStifTFwIfProt::RunParams, 
                CStifTFwIfProt::ERunTestcasenum, 
                aStartInfo->GetTestCaseNumber() ));
    if( aStartInfo->GetIniFile().Length() > 0 )
        {
        // Initialization file
        __TRACE( KMessage, (_L("ini: %S"), &aStartInfo->GetIniFile() ));     
        User::LeaveIfError(
            remoteRequest->Append( CStifTFwIfProt::RunParams, 
                    CStifTFwIfProt::ERunInifile, 
                    aStartInfo->GetIniFile() ) );
        }
    if( aStartInfo->GetConfig().Length() > 0 )
        {
        // Initialization file
        __TRACE( KMessage, (_L("config: %S"), &aStartInfo->GetConfig() ));     
        User::LeaveIfError(
            remoteRequest->Append( CStifTFwIfProt::RunParams, 
                    CStifTFwIfProt::ERunTestcasefile, 
                    aStartInfo->GetConfig() ));
        } 
    //Title (must be given between quotation marks in case of any spaces inside
    if( aStartInfo->GetTitle().Length() > 0 )
        {
        __TRACE(KMessage, (_L("title: %S"), &aStartInfo->GetTitle() ) );
        TName title;
        title.Format(_L("\"title=%S\""), &aStartInfo->GetTitle() );
        User::LeaveIfError( remoteRequest->Append( title ) );
        }
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError(
        iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
        
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::PauseTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError(
        remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave id
    User::LeaveIfError(
        remoteRequest->AppendId( SETID( aSlaveId, aTestId ) ) );
    
    // Pause command
    User::LeaveIfError(
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdPause ) );
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::ResumeTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId )
    {    
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError(
        remoteRequest->AppendId( aMasterId ));
    // DstId, i.e.slave id
    User::LeaveIfError(
        remoteRequest->AppendId( SETID( aSlaveId, aTestId ) ) );
    
    // Resume command
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdResume ));
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::CancelTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId )
    {    
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ) );
    // Srcid. i.e. master id
    User::LeaveIfError(
        remoteRequest->AppendId( aMasterId ));
    // DstId, i.e.slave id
    User::LeaveIfError(
        remoteRequest->AppendId( SETID( aSlaveId, aTestId ) ) );
    
    // Cancel command
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdCancel ));
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::RequestEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ));
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    
    // Request event
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdRequest )); 
        
    // Event name
    User::LeaveIfError( remoteRequest->Append( aEventName ) ); 
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::SetEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName, 
        TEventIf::TEventType aEventType )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ));
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
        
    // Set event
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdSetEvent ) ); 
        
    // Event name
    User::LeaveIfError( remoteRequest->Append( aEventName ) ); 
    
    if ( aEventType == TEventIf::EState )
        {
        // State event
        User::LeaveIfError( remoteRequest->Append( remoteRequest->EventType( aEventType ) ) ); 
        }
        
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );    
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::UnsetEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ));
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    
    // Set event
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdUnsetEvent ) ); 
        
    // Event name
    User::LeaveIfError( remoteRequest->Append( aEventName ) ); 
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );    
    
    CleanupStack::PopAndDestroy( remoteRequest );    
    }

void CRemoteCallsProxy::ReleaseEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName )
    {
    CStifTFwIfProt* remoteRequest = CStifTFwIfProt::NewL();
    CleanupStack::PushL( remoteRequest );
    
    remoteRequest->CreateL();
    // Remote message
    User::LeaveIfError( 
        remoteRequest->Append( CStifTFwIfProt::MsgType, CStifTFwIfProt::EMsgRemote ));
    // Srcid. i.e. master id
    User::LeaveIfError( remoteRequest->AppendId( aMasterId ) );
    // DstId, i.e.slave device id
    User::LeaveIfError( remoteRequest->AppendId( SETID( aSlaveId, 0 ) ) );
    
    // Release event
    User::LeaveIfError(
        remoteRequest->Append( CStifTFwIfProt::CmdType, CStifTFwIfProt::ECmdRelease )); 

    // Event name
    User::LeaveIfError( remoteRequest->Append( aEventName ) );
    
    RDebug::Print( _L("[STIF Master sending request] %S"), &remoteRequest->Message() );

    User::LeaveIfError( 
            iTestModuleIf.RemoteSend( remoteRequest->Message() ) );    
    
    CleanupStack::PopAndDestroy( remoteRequest );
    }

void CRemoteCallsProxy::SetSlaveAllocateFreeMonitor( MSlaveAllocateFreeMonitor* aSlaveAllocateFreeMonitor )
    {
    iSlaveAllocateFreeMonitor = aSlaveAllocateFreeMonitor;
    }

void CRemoteCallsProxy::SetRemoteTestCasesMonitor( MRemoteTestCasesMonitor* aRemoteTestCasesMonitor )
    {
    iRemoteTestCasesMonitor = aRemoteTestCasesMonitor;
    }

void CRemoteCallsProxy::SetRemoteEventsMonitor( MRemoteEventsMonitor* aRemoteEventsMonitor )
    {
    iRemoteEventsMonitor = aRemoteEventsMonitor;
    }

void CRemoteCallsProxy::SetRemoteSendReceiveMonitor( MRemoteSendReceiveMonitor* aRemoteSendReceiveMonitor )
    {
    iRemoteSendReceiveMonitor = aRemoteSendReceiveMonitor;
    }

void CRemoteCallsProxy::SetReceiveErrorHandler( MReceiveErrorHandler* aReceiveErrorHandler )
    {
    iReceiveErrorHandler = aReceiveErrorHandler;
    }

void CRemoteCallsProxy::StartReceivingL()
    {
    iTestModuleIf.RemoteReceive( iReceivedRemoteMsg, iStatus );        
    SetActive();
    }

void CRemoteCallsProxy::DispatchReceivedRemoteMsgL()
    {
    _LIT( KErrMsg, "Error during slave respons handling");
    
    iReceiveErrorDescription = KErrMsg;
    
    CStifTFwIfProt* msg = CStifTFwIfProt::NewL();
    CleanupStack::PushL( msg );
    TRAPD( err, msg->SetL( iReceivedRemoteMsg ); );
    if( err != KErrNone )
        {
        __TRACE( KError, (_L("Response parsing failed")));
        User::Leave( err );
        }
    
    // Check protocol identifiers
    if( ( msg->SrcDevId() == 0 ) ||
        ( msg->DstDevId() == 0 ) ||
        ( msg->DstTestId() == 0 ) )
        {
        __TRACE( KError, (_L("Illegal deviceid received")));
        User::Leave( KErrGeneral );
        }
    
    // This is master, cannot receive anything else but responses
    if( msg->iMsgType != CStifTFwIfProt::EMsgResponse )
        {
        __TRACE( KError, (_L("Illegal message received %d"), 
            msg->iMsgType ));
        User::Leave( KErrGeneral );        
        }
        
    switch( msg->iRespType )
        {
        case CStifTFwIfProt::EMsgReserve:
            {
            ReceiveResponseReserveL( msg );
            }
            break;
        case CStifTFwIfProt::EMsgRelease:
            {
            ReceiveResponseReleaseL( msg );
            }            
            break;
        case CStifTFwIfProt::EMsgRemote:
            {         
            __TRACE( KMessage, (_L("ReceiveResponse Remote")));
            switch( msg->iCmdType )
                {
                case CStifTFwIfProt::ECmdRun:
                    ReceiveResponseRunL( msg );
                    break;
                case CStifTFwIfProt::ECmdPause:
                case CStifTFwIfProt::ECmdResume:
                case CStifTFwIfProt::ECmdCancel:
                    ReceiveResponseTestCtlL( msg );
                    break;
                case CStifTFwIfProt::ECmdRequest:
                case CStifTFwIfProt::ECmdRelease:
                case CStifTFwIfProt::ECmdSetEvent:
                case CStifTFwIfProt::ECmdUnsetEvent:
                    ReceiveResponseEventCtlL( msg );                    
                    break;
                case CStifTFwIfProt::ECmdSendReceive:
                    ReceiveResponseSendReceiveL( msg );                    
                    break;
                default:
                    ReceiveResponseSendUnknownL( msg );                    
                    break;
                }
            }            
            break;
        default:
            User::Leave( KErrGeneral );
        } 
               
    CleanupStack::PopAndDestroy( msg );    
    iReceiveErrorDescription = KNullDesC;
    }

void CRemoteCallsProxy::ReceiveResponseReserveL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsg, "Error during slave allocate respons handling");
    __TRACE( KMessage, (_L("ReceiveResponse Reserve")));
    
    // Check protocol Src test id
    if( aMsg->SrcTestId() != 0 )
        {
        __TRACE( KError, (_L("Illegal deviceid received")));
        User::Leave( KErrGeneral );
        }
    
    if( aMsg->iResult != KErrNone )
        {
        __TRACE( KError, (_L("Response with error %d"), aMsg->iResult ));
        User::Leave( aMsg->iResult );
        }
    
    iReceiveErrorDescription = KErrMsg;
    iSlaveAllocateFreeMonitor->NotifySlaveAllocatedL( aMsg->SrcDevId() );
    iReceiveErrorDescription = KNullDesC;
    __TRACE( KMessage, (_L("Slave allocated succesfully, continue execution")));
    }

void CRemoteCallsProxy::ReceiveResponseReleaseL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsg, "Error during slave release respons handling");
    __TRACE( KMessage, (_L("ReceiveResponse Release")));
    
    // Check protocol Src test id
    if( aMsg->SrcTestId() != 0 )
        {
        __TRACE( KError, (_L("Illegal deviceid received")));
        User::Leave( KErrGeneral );
        }
    
    if( aMsg->iResult != KErrNone )
        {
        __TRACE( KError, (_L("Response with error %d"), aMsg->iResult ));
        User::Leave( aMsg->iResult );
        }

    iReceiveErrorDescription = KErrMsg;
    iSlaveAllocateFreeMonitor->NotifySlaveFreedL( aMsg->SrcDevId() );
    iReceiveErrorDescription = KNullDesC;
    __TRACE( KMessage, (_L("Slave freed succesfully, continue execution")));
    }

void CRemoteCallsProxy::ReceiveResponseRunL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsgRun, "Error during remote sub test casse run respons handling");

    
    TPtrC tmp = CStifTFwIfProt::RunStatus( aMsg->iRunStatus ); 
    __TRACE( KMessage, (_L("ReceiveResponse Remote Run %S"), &tmp ));

    
    if ( iRemoteTestCasesMonitor == NULL )
        {
        return;
        }
    
    switch( aMsg->iRunStatus )
        {
        case CStifTFwIfProt::ERunStarted:
            {
            iReceiveErrorDescription = KErrMsgRun;
            iRemoteTestCasesMonitor->NotifyTestCaseStartedL( aMsg->SrcDevId(), aMsg->SrcTestId() );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        case CStifTFwIfProt::ERunError:
        case CStifTFwIfProt::ERunReady:
            {
            TFullTestResult testCaseResult;
            switch( aMsg->iResultCategory )
                {
                case CStifTFwIfProt::EResultNormal:
                    testCaseResult.iCaseExecutionResultType = 
                        TFullTestResult::ECaseExecuted;
                    testCaseResult.iTestResult.iResult = aMsg->iResult;
                    testCaseResult.iCaseExecutionResultCode = 0;
                    break;
                case CStifTFwIfProt::EResultPanic:
                    testCaseResult.iCaseExecutionResultType = 
                        TFullTestResult::ECasePanic;
                    testCaseResult.iTestResult.iResult = KErrGeneral;
                    testCaseResult.iCaseExecutionResultCode = aMsg->iResult;
                    break;
                case CStifTFwIfProt::EResultException:
                    testCaseResult.iCaseExecutionResultType = 
                        TFullTestResult::ECaseException;
                    testCaseResult.iTestResult.iResult = KErrGeneral;
                    testCaseResult.iCaseExecutionResultCode = aMsg->iResult;
                    break;
                case CStifTFwIfProt::EResultTimeout:
                    testCaseResult.iCaseExecutionResultType = 
                        TFullTestResult::ECaseTimeout;                
                    testCaseResult.iTestResult.iResult = KErrGeneral;
                    testCaseResult.iCaseExecutionResultCode = aMsg->iResult;
                    break;
                case CStifTFwIfProt::EResultLeave:
                    testCaseResult.iCaseExecutionResultType = 
                        TFullTestResult::ECaseLeave;                
                    testCaseResult.iTestResult.iResult = KErrGeneral;
                    testCaseResult.iCaseExecutionResultCode = aMsg->iResult;
                    break;
                default:
                    User::Leave( KErrGeneral );
                }
            
            iReceiveErrorDescription = KErrMsgRun;
            if ( aMsg->iRunStatus == CStifTFwIfProt::ERunError )
                {
                iRemoteTestCasesMonitor->NotifyTestCaseRunErrorL( aMsg->SrcDevId(), testCaseResult );
                }
            else
                {
                iRemoteTestCasesMonitor->NotifyTestCaseFinishedL( aMsg->SrcDevId(), aMsg->SrcTestId(), testCaseResult );
                }
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        default:
            // Should never come here
            User::Leave( KErrGeneral );    
        }        
    }

void CRemoteCallsProxy::ReceiveResponseTestCtlL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsgPause, "Error during remote sub test casse pause respons handling");
    _LIT( KErrMsgResume, "Error during remote sub test casse resume respons handling");
    _LIT( KErrMsgCancel, "Error during remote sub test casse cancel respons handling");

    if( aMsg->iResult != KErrNone )
        {
        __TRACE( KError, (_L("Response with error %d"), aMsg->iResult ));
        User::Leave( aMsg->iResult );
        }
        
    switch( aMsg->iCmdType )
        {
        case CStifTFwIfProt::ECmdPause:
            {
            __TRACE( KMessage, (_L("ReceiveResponse Remote Pause")));
            iReceiveErrorDescription = KErrMsgPause;
            iRemoteTestCasesMonitor->NotifyTestCasePausedL( aMsg->SrcDevId(), aMsg->SrcTestId() );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        case CStifTFwIfProt::ECmdResume:
            {
            __TRACE( KMessage, (_L("ReceiveResponse Remote Resume")));
            iReceiveErrorDescription = KErrMsgResume;
            iRemoteTestCasesMonitor->NotifyTestCaseResumedL( aMsg->SrcDevId(), aMsg->SrcTestId() );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        case CStifTFwIfProt::ECmdCancel:
            __TRACE( KMessage, (_L("ReceiveResponse Remote Cancel")));
            iReceiveErrorDescription = KErrMsgCancel;
            iRemoteTestCasesMonitor->NotifyTestCaseCancelledL( aMsg->SrcDevId(), aMsg->SrcTestId() );
            iReceiveErrorDescription = KNullDesC;
            break;
        default:
            // Should never come here
            User::Leave( KErrGeneral );
        }    
    }

void CRemoteCallsProxy::ReceiveResponseEventCtlL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsgRequest, "Error during remote event request respons handling");
    _LIT( KErrMsgStateChanged, "Error during remote event state change respons handling");
    _LIT( KErrMsgSet, "Error during remote event set respons handling");
    _LIT( KErrMsgUnset, "Error during remote event unset respons handling");
    _LIT( KErrMsgRelease, "Error during remote event release respons handling");
    
    if ( iRemoteEventsMonitor == NULL )
        {
        return;
        }
    
    switch( aMsg->iCmdType )
        {
        case CStifTFwIfProt::ECmdRequest:
            {
            if ( ( aMsg->iEventStatus == CStifTFwIfProt::EEventSet ) || 
                    ( aMsg->iEventStatus == CStifTFwIfProt::EEventUnset ) )
                {
                iReceiveErrorDescription = KErrMsgStateChanged;
                iRemoteEventsMonitor->NotifyEventStateChangedL( 
                        aMsg->SrcDevId(),
                        aMsg->iEventName,
                        aMsg->iEventStatus,
                        aMsg->iEventType,
                        aMsg->iResult
                        );
                iReceiveErrorDescription = KNullDesC;
                }
            else
                {
                iReceiveErrorDescription = KErrMsgRequest;
                iRemoteEventsMonitor->NotifyEventRequestedL( 
                        aMsg->SrcDevId(),
                        aMsg->iEventName,
                        aMsg->iEventStatus,
                        aMsg->iEventType,
                        aMsg->iResult
                        );
                iReceiveErrorDescription = KNullDesC;
                }
            }
            break;
        case CStifTFwIfProt::ECmdRelease:
            {
            iReceiveErrorDescription = KErrMsgRelease;
            iRemoteEventsMonitor->NotifyEventReleasedL( 
                    aMsg->SrcDevId(),
                    aMsg->iEventName,
                    aMsg->iResult
                    );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        case CStifTFwIfProt::ECmdSetEvent:
            {
            iReceiveErrorDescription = KErrMsgSet;
            iRemoteEventsMonitor->NotifyEventSetCompletedL( 
                    aMsg->SrcDevId(),
                    aMsg->iEventName,
                    aMsg->iResult
                    );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        case CStifTFwIfProt::ECmdUnsetEvent:
            {
            iReceiveErrorDescription = KErrMsgUnset;
            iRemoteEventsMonitor->NotifyEventUnsetCompletedL( 
                    aMsg->SrcDevId(),
                    aMsg->iEventName,
                    aMsg->iResult
                    );
            iReceiveErrorDescription = KNullDesC;
            }
            break;
        default:
            // Should never come here
            User::Leave( KErrGeneral );
        }
    
    }

void CRemoteCallsProxy::ReceiveResponseSendReceiveL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsgStarted, "Error during remote sendreceive started respons handling");
    _LIT( KErrMsgResult, "Error during remote sendreceive result respons handling");

    if ( iRemoteSendReceiveMonitor == NULL )
        {
        return;
        }
    
    switch( aMsg->iRunStatus )
        {
        case CStifTFwIfProt::ERunStarted:
            {
            iReceiveErrorDescription = KErrMsgStarted;
            iRemoteSendReceiveMonitor->NotifySendReceiveStartedL( aMsg->SrcDevId() );
            iReceiveErrorDescription = KNullDesC;
            break;
            }
        case CStifTFwIfProt::ERunError:
        case CStifTFwIfProt::ERunReady:
            {
            iReceiveErrorDescription = KErrMsgResult;
            iRemoteSendReceiveMonitor->NotifySendReceiveResultL( 
                    aMsg->SrcDevId(),
                    aMsg->iRunStatus,
                    aMsg->iResult );
            iReceiveErrorDescription = KNullDesC;
            break;
            }
        default:
            {
            // Should never come here
            User::Leave( KErrGeneral );    
            }
        }
    }

void CRemoteCallsProxy::ReceiveResponseSendUnknownL( CStifTFwIfProt* aMsg )
    {
    _LIT( KErrMsg, "Error during remote send unknown respons handling");

    if ( iRemoteSendReceiveMonitor == NULL )
        {
        return;
        }
    
    iReceiveErrorDescription = KErrMsg;
    iRemoteSendReceiveMonitor->NotifySendUnknownL( aMsg->SrcDevId(), aMsg->iResult );
    iReceiveErrorDescription = KNullDesC;
    }

CSlave* CSlave::NewL()
    {
    CSlave* self = new(ELeave)CSlave;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CSlave::~CSlave()
    {
    iSubTestCases.Reset(); // CSlave does not own sub test cases objects
    iSubTestCases.Close();
    
    iEvents.ResetAndDestroy();
    iEvents.Close();
    
    delete iNestedASLoop;
    iNestedASLoop = NULL;
    
    delete iName;
    iName = NULL;
    }

CSlave::CSlave()
    {    
    }

void CSlave::ConstructL()
    {
    iName = HBufC::NewL( 0 );
    iNestedASLoop = new(ELeave)CActiveSchedulerWait;
    }

const TDesC& CSlave::GetName() const
    {
    return *iName;
    }

void CSlave::SetNameL( const TDesC& aName )
    {
    HBufC* tmp = aName.AllocL();
    delete iName;
    iName = tmp;
    }

TUint32 CSlave::GetMasterId() const
    {    
    return iMasterId;
    }

void CSlave::SetMasterId( TUint32 aMasterId )
    {
    iMasterId = aMasterId;
    }

TUint16 CSlave::GetSlaveId() const
    {
    return iSlaveId;
    }

TBool CSlave::HasRunningTestCases() const
    {
    if ( iSubTestCases.Count() > 0 )
        {
        return ETrue;
        }
    return EFalse;
    }

void CSlave::RegisterSubTestCaseL( CRemoteSubTestCaseRunner* aSubTestCase )
    {
    iSubTestCases.AppendL( aSubTestCase );
    }
        
void CSlave::UnregisterSubTestCaseL( CRemoteSubTestCaseRunner* aSubTestCase )
    {    
    TInt idx = iSubTestCases.Find( aSubTestCase );
    if ( idx >= 0 )
        {
        iSubTestCases.Remove( idx );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    if ( ( !HasRunningTestCases() ) && iNestedASLoop->IsStarted() )
        {
        iNestedASLoop->AsyncStop();
        }
    }
        
RPointerArray<CRemoteSubTestCaseRunner>& CSlave::GetSubTestCases()
    {
    return iSubTestCases;
    }

void CSlave::WaitForSubTestCasesL()
    {
    if ( iNestedASLoop->IsStarted() )
        {
        User::Leave( KErrInUse );
        }
    
    if ( !HasRunningTestCases() )
        {
        return;
        }
    
    iNestedASLoop->Start();
    }

void CSlave::CancelWaitForSubTestCases()
    {
    iNestedASLoop->AsyncStop();
    }

void CSlave::AddEventL( TEventTS* aEvent )
    {
    iEvents.AppendL( aEvent );
    }

void CSlave::RemoveEventL( TEventTS* aEvent )
    {
    TInt idx = iEvents.Find( aEvent );
    if ( idx >= 0 )
        {
        iEvents.Remove( idx );        
        }
    else
        {
        User::Leave( KErrNotFound );
        }    
    }

TEventTS* CSlave::GetEvent( const TDesC& aName )
    {
    for ( TInt i = 0; i < iEvents.Count(); i++ )
        {
        if ( iEvents[ i ]->Name() == aName )
            {
            return iEvents[ i ];
            }
        }
    
    return NULL;
    }

RPointerArray<TEventTS>& CSlave::GetEvents()
    {
    return iEvents;
    }

#ifdef LOGGER
#undef LOGGER
#endif

#define LOGGER iTestRunner->GetLogger()

void CSlave::SetSlaveId( TUint16 aSlaveId )
    {
    iSlaveId = aSlaveId;
    }

CSlavesManager* CSlavesManager::NewL( CTestRunner* aTestRunner, CTestModuleIf& aTestModuleIf )
    {
    CSlavesManager* self = new(ELeave)CSlavesManager( aTestRunner, aTestModuleIf );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

CSlavesManager::~CSlavesManager()
    {
    Cancel();
    iOperationTimeoutTimer.Close();

    iRemoteCallsProxy->Cancel();
    
    delete iSlaveForAllocate;
    iSlaveForAllocate = NULL;
    
    iSlaves.ResetAndDestroy();
    iSlaves.Close();
    
    delete iNestedASLoop;
    iNestedASLoop = NULL;
    
    delete iRemoteCallsProxy;
    iRemoteCallsProxy = NULL;
    }

CSlavesManager::CSlavesManager( CTestRunner* aTestRunner, CTestModuleIf& aTestModuleIf )
:CActive( EPriorityNormal ), iTestRunner( aTestRunner ), 
 iTestModuleIf( aTestModuleIf ), iOperationTimeout( 30000000 )
    {
    CActiveScheduler::Add( this );
    }

void CSlavesManager::ConstructL()
    {
    iOperationTimeoutTimer.CreateLocal();
    iRemoteCallsProxy = CRemoteCallsProxy::NewL( iTestModuleIf, LOGGER );
    iNestedASLoop = new(ELeave)CActiveSchedulerWait;
    
    iRemoteCallsProxy->SetReceiveErrorHandler( this );
    iRemoteCallsProxy->SetSlaveAllocateFreeMonitor( this );
    iRemoteCallsProxy->SetRemoteTestCasesMonitor( this );
    iRemoteCallsProxy->SetRemoteEventsMonitor( this );
    iRemoteCallsProxy->SetRemoteSendReceiveMonitor( this );
    }

void CSlavesManager::SlaveAllocateL( const TDesC& aName, const TDesC& aType )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    if ( GetSlave( aName ) != NULL )
        {
        User::Leave( KErrAlreadyExists );
        }
    
    delete iSlaveForAllocate;
    iSlaveForAllocate = NULL;
    
    CSlave* slaveForAllocate = CSlave::NewL();
    CleanupStack::PushL( slaveForAllocate );
    slaveForAllocate->SetNameL( aName );
    slaveForAllocate->SetMasterId( KRemoteProtocolMasterId );

    iRemoteCallsProxy->AllocateL( KRemoteProtocolMasterId, aType );    
    CleanupStack::Pop( slaveForAllocate );
    iSlaveForAllocate = slaveForAllocate;    
    iOperation = ESMOSlaveAllocate;
    iLastOperationResult = KErrNone;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::SlaveFreeL( const TDesC& aName )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    CSlave* slave = GetSlave( aName );
    
    if ( slave == NULL )
        {
        User::Leave( KErrNotFound );
        }

    if ( slave->HasRunningTestCases() )
        {
    
        }
    
    iSlaves.Remove( iSlaves.Find( slave ) );
    
    TUint32 masterId = slave->GetMasterId();
    TUint16 slaveId = slave->GetSlaveId();

    delete slave;

    iRemoteCallsProxy->FreeL( masterId, slaveId );        
    iOperation = ESMOSlaveFree;
    iLastOperationResult = KErrNone;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::SendReceiveL( CSlave* aSlave, const TDesC& aMessage )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iRemoteCallsProxy->SendReceiveL( aSlave->GetMasterId(), 
            aSlave->GetSlaveId(), aMessage );    
    iOperation = ESMOSendReceiveWaitForStarted;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );    
    }

void CSlavesManager::SendUnknownL( CSlave* aSlave, const TDesC& aCommand, const TDesC& aMessage )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iRemoteCallsProxy->SendUnknownL( aSlave->GetMasterId(), 
            aSlave->GetSlaveId(), aCommand, aMessage );    
    iOperation = ESMOSendUnknown;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );  
    }

CSlave* CSlavesManager::GetSlave( const TDesC& aName )
    {
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetName() == aName )
            {
            return iSlaves[ i ];
            }
        }
    return NULL;
    }

CSlave* CSlavesManager::GetSlave( TUint16 aSlaveId )
    {
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            return iSlaves[ i ];
            }
        }
    return NULL;
    }

RPointerArray<CSlave>& CSlavesManager::GetSlaves()
    {
    return iSlaves;
    }

CRemoteCallsProxy* CSlavesManager::GetRemoteCallsProxy()
    {
    return iRemoteCallsProxy;
    }

void CSlavesManager::EventRequestL( CSlave* aSlave, TEventTS* aEvent )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iEventForRequest = aEvent;
    
    iRemoteCallsProxy->RequestEventL( aSlave->GetMasterId(),
            aSlave->GetSlaveId(), iEventForRequest->Name() );    
    iOperation = ESMOEventRequest;
    iLastOperationResult = KErrNone;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    iEventForRequest = NULL;
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::EventReleaseL( CSlave* aSlave, const TDesC& aEventName )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iRemoteCallsProxy->ReleaseEventL( aSlave->GetMasterId(),
            aSlave->GetSlaveId(), aEventName );    
    iOperation = ESMOEventRelease;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::EventSetL( CSlave* aSlave, const TDesC& aEventName, TEventIf::TEventType aEventType )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iRemoteCallsProxy->SetEventL( aSlave->GetMasterId(),
            aSlave->GetSlaveId(), aEventName, aEventType );    
    iOperation = ESMOEventSet;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::EventUnsetL( CSlave* aSlave, const TDesC& aEventName )
    {
    if ( iOperation != ESMOIdle )
        {
        User::Leave( KErrInUse );
        }
    
    iRemoteCallsProxy->UnsetEventL( aSlave->GetMasterId(), 
            aSlave->GetSlaveId(), aEventName );    
    iOperation = ESMOEventUnset;
    
    iOperationTimeoutTimer.After( iStatus, iOperationTimeout );
    SetActive();
    iNestedASLoop->Start();
    
    User::LeaveIfError( iLastOperationResult );
    }

void CSlavesManager::NotifySlaveAllocatedL( TUint16 aSlaveId )
    {
    Cancel(); // Stop operation timeout monitor

    if( iOperation != ESMOSlaveAllocate )
        {        
        iLastOperationResult = KErrNotReady;
        delete iSlaveForAllocate;
        iSlaveForAllocate = NULL;
        User::Leave( KErrNotReady );
        }
    else
        {
        iSlaveForAllocate->SetSlaveId( aSlaveId );
        iSlaves.AppendL( iSlaveForAllocate );
        iSlaveForAllocate = NULL;
        iLastOperationResult = KErrNone;
        }

    iOperation = ESMOIdle;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifySlaveFreedL( TUint16 /*aSlaveId*/ )
    {
    Cancel(); // Stop operation timeout monitor

    if( iOperation != ESMOSlaveFree )
        {
        User::Leave( KErrNotReady );
        }

    iLastOperationResult = KErrNone;
    
    iOperation = ESMOIdle;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifyTestCaseStartedL( TUint16 aSlaveId, TUint16 aSlaveTestId )
    {
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->IsRunSubTestCaseRequestOngoing() )
                    {
                    remoteTestCases[ k ]->NotifyTestCaseStartedL( aSlaveTestId );
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyTestCaseRunErrorL( TUint16 aSlaveId, const TFullTestResult& aTestCaseResult )
    {
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->IsRunSubTestCaseRequestOngoing() )
                    {
                    remoteTestCases[ k ]->NotifyTestCaseRunError( aTestCaseResult );
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyTestCaseFinishedL( TUint16 aSlaveId, TUint16 aSlaveTestId, const TFullTestResult& aTestCaseResult )
    {
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->GetTestCaseId() == aSlaveTestId )
                    {
                    remoteTestCases[ k ]->NotifyTestCaseFinishedL( aTestCaseResult );
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyTestCasePausedL( TUint16 aSlaveId, TUint16 aSlaveTestId )
    {    
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->GetTestCaseId() == aSlaveTestId )
                    {
                    remoteTestCases[ k ]->NotifyTestCasePausedL();
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyTestCaseResumedL( TUint16 aSlaveId, TUint16 aSlaveTestId )
    {    
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->GetTestCaseId() == aSlaveTestId )
                    {
                    remoteTestCases[ k ]->NotifyTestCaseResumedL();
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyTestCaseCancelledL( TUint16 aSlaveId, TUint16 aSlaveTestId )
    {    
    for ( TInt i = 0; i < iSlaves.Count(); i++ )
        {
        if ( iSlaves[ i ]->GetSlaveId() == aSlaveId )
            {
            RPointerArray<CRemoteSubTestCaseRunner>& remoteTestCases = 
                    iSlaves[ i ]->GetSubTestCases();
            for ( TInt k = 0; k < remoteTestCases.Count(); k++ )
                {
                if ( remoteTestCases[ k ]->GetTestCaseId() == aSlaveTestId )
                    {
                    remoteTestCases[ k ]->NotifyTestCaseCancelledL();
                    // There should be only one test case which waits for 
                    // run test case request response
                    return;
                    }
                }        
            }
        }
    // There is no test case which waits for run request response
    User::Leave( KErrNotFound );
    }

void CSlavesManager::NotifyEventRequestedL( TUint16 /*aSlaveId*/, 
        const TDesC& aEventName, 
        CStifTFwIfProt::TEventStatus aEventStatus,
        TEventIf::TEventType /*aEventType*/,
        TInt aResult )
    {
    if ( iOperation != ESMOEventRequest )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor    
    
    iLastOperationResult = KErrNone;
    switch( aEventStatus )
        {
        case CStifTFwIfProt::EEventActive:
            __TRACE( KMessage, (_L("Event %S active"), &aEventName ));
            break;
        case CStifTFwIfProt::EEventError:
            __TRACE( KMessage, (_L("Event %S error %d"), &aEventName, aResult ));
            iLastOperationResult = aResult;
            break;
        default:
            User::Leave( KErrGeneral );
        }    
    
    iOperation = ESMOIdle;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifyEventStateChangedL( TUint16 aSlaveId, 
        const TDesC& aEventName, 
        CStifTFwIfProt::TEventStatus aEventStatus,
        TEventIf::TEventType aEventType,
        TInt aResult )
    {
    switch( aEventStatus )
        {
        case CStifTFwIfProt::EEventSet:
            {
            __TRACE( KMessage, (_L("Event %S set"), &aEventName ));
            // Set event            
            CSlave* slave = GetSlave( aSlaveId );
            if ( slave != NULL )
                {
                TEventTS* event = slave->GetEvent( aEventName );
                if ( event != NULL )
                    {
                    event->SetEvent( aEventType );
                    }                
                else
                    {
                    User::Leave( KErrNotFound );
                    }
                }
            else
                {
                User::Leave( KErrNotFound );
                }
            }
            break;
        case CStifTFwIfProt::EEventUnset:
            {
            __TRACE( KMessage, (_L("Event %S set"), &aEventName ));
            // Set event            
            CSlave* slave = GetSlave( aSlaveId );
            if ( slave != NULL )
                {
                TEventTS* event = slave->GetEvent( aEventName );
                if ( event != NULL )
                    {
                    event->SetType( TEventIf::EUnsetEvent );
                    }                
                else
                    {
                    User::Leave( KErrNotFound );
                    }
                }
            else
                {
                User::Leave( KErrNotFound );
                }
            }
            break;
        case CStifTFwIfProt::EEventError:
            {
            __TRACE( KMessage, (_L("Event %S error %d"), &aEventName, aResult ));
            User::Leave( aResult );
            }
            break;
        default:
            {
            User::Leave( KErrGeneral );
            }
        }
    }

void CSlavesManager::NotifyEventReleasedL( TUint16 /*aSlaveId*/, 
        const TDesC& /*aEventName*/, TInt aResult )
    {
    if ( iOperation != ESMOEventRelease )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor
    
    iOperation = ESMOIdle;
    iLastOperationResult = aResult;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifyEventSetCompletedL( TUint16 /*aSlaveId*/, 
        const TDesC& /*aEventName*/, TInt aResult )       
    {
    if ( iOperation != ESMOEventSet )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor
    
    iOperation = ESMOIdle;
    iLastOperationResult = aResult;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifyEventUnsetCompletedL( TUint16 /*aSlaveId*/, 
        const TDesC& /*aEventName*/, TInt aResult )        
    {
    if ( iOperation != ESMOEventUnset )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor
    
    iOperation = ESMOIdle;
    iLastOperationResult = aResult;
    iNestedASLoop->AsyncStop();
    }


void CSlavesManager::NotifySendReceiveStartedL( TUint16 /*aSlaveId*/ )
    {
    if ( iOperation != ESMOSendReceiveWaitForStarted )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor
    
    iOperation = ESMOSendReceiveWaitForReady;
    }

void CSlavesManager::NotifySendReceiveResultL( 
        TUint16 /*aSlaveId*/, 
        CStifTFwIfProt::TRunStatus /*aRunStatus*/,
        TInt aRunResult )
    {
    if ( ( iOperation != ESMOSendReceiveWaitForStarted ) &&
            ( iOperation != ESMOSendReceiveWaitForReady ) )
        {
        User::Leave( KErrNotReady );
        }
    
    iLastOperationResult = aRunResult;
    
    if ( iOperation == ESMOSendReceiveWaitForStarted )
        {
        Cancel(); // Stop operation timeout monitor
        }
    
    iOperation = ESMOIdle;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::NotifySendUnknownL( TUint16 aSlaveId, TInt aResult )
    {
    if ( iOperation != ESMOSendUnknown )
        {
        User::Leave( KErrNotReady );
        }
    
    Cancel(); // Stop operation timeout monitor
    
    CSlave* slave = GetSlave( aSlaveId );
    if( slave == NULL )
        {
        User::Leave( KErrNotFound );
        }
    
    iOperation = ESMOIdle;
    iLastOperationResult = aResult; 
    iNestedASLoop->AsyncStop();    
    }

void CSlavesManager::HandleRemoteReceiveError( TInt aError, const TDesC& aErrorDescription )
    {
    iTestRunner->HandleRemoteReceiveError( aError, aErrorDescription );
    }

void CSlavesManager::RunL()
    {
    // Last operation timeouted
    if ( iOperation ==  ESMOSlaveAllocate )
        {
        delete iSlaveForAllocate;
        iSlaveForAllocate = NULL;
        }
    
    iLastOperationResult = KErrTimedOut;
    iOperation = ESMOIdle;
    iNestedASLoop->AsyncStop();
    }

void CSlavesManager::DoCancel()
    {
    iOperationTimeoutTimer.Cancel();
    }

// EOF
