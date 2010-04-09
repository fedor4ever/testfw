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

#ifndef TESTSCRIPTERUTILS_H_
#define TESTSCRIPTERUTILS_H_

//  INCLUDES
#include <StifTestInterface.h>
#include <StifTFwIfProt.h>

// CONSTANTS
// None

// MACROS
// None

// DATA TYPES
typedef TBuf<0x200> TRemoteProtMsg;

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS
class CTestRunner;
class CSubTestCaseRunner;
class CRemoteCallTimeoutMonitor;
class CStifTFwIfProt;
class CStifLogger;
class CSlave;
class CRemoteSubTestCaseRunner;

// DESCRIPTION
// Testcase starting information
NONSHARABLE_CLASS(CStartInfo): public CBase
    {
    public: // Constructors and destructor
        /**
        * Two-phased constructor.
        */
        static CStartInfo* NewL();
        
        /**
        * Destructor.
        */
        virtual ~CStartInfo();
            
    public: // New functions
        /**
        * Sets module name.
        */
        void SetModuleNameL( const TDesC& aModule, TInt aExtLength = 0 );
            
        /**
        * Sets initialization file name.
        */
        void SetIniFileL( const TDesC& aIni );
            
        /**
        * Sets configuration file name.
        */
        void SetConfigL( const TDesC& aConfig );
            
        /**
        * Sets test identifier.
        */
        void SetTestIdL( const TDesC& aTestId );

        /**
        * Sets test case title.
        */
        void SetTitleL( const TDesC& aTitle);

        /**
         * Sets test case index.
         */
        void SetTestCaseNumber( TInt aTestCaseNumber );
        
        /**
         * Sets test case expected result.
         */
        void SetExpectedResult( TInt aExpectedResult );
        
        /**
         * Sets expected result category.
         */
        void SetExpectedResultCategory( TFullTestResult::TCaseExecutionResult aCategory );

        /**
         * Sets timeout value.
         */
        void SetTimeout( TInt aTimeout );
        
        /**
         * Gets test module name.
         */
        const TDesC& GetModuleName() const;

        /**
         * Gets ini file path.
         */
        const TDesC& GetIniFile() const;

        /**
         * Gets config file path
         */
        const TDesC& GetConfig() const;

        /**
         * Gets test case id.
         */
        const TDesC& GetTestId() const;

        /**
         * Gets test case title.
         */
        const TDesC& GetTitle() const;

        /**
         * Gets test case title.
         */
        TInt GetTestCaseNumber() const;
        
        /**
         * Gets expected result.
         */
        TInt GetExpectedResult() const;

        /**
         * Gets expected result category.
         */
        TFullTestResult::TCaseExecutionResult GetExpectedResultCategory() const;

        /**
         * Gets timeout value.
         */
        TInt GetTimeout() const;

        /**
         * Copy values from other CStartInfo instance.
         */
        void CopyL( const CStartInfo& aStartInfo );
        
    private:
        /**
        * C++ default constructor.
        */
        CStartInfo();
                    
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
                
    private:
        // Module name
        HBufC*  iModule;
        
        // Ini file path
        HBufC*  iIniFile;
        
        // Config file path
        HBufC*  iConfig;
        
        // Test id ( used for complete, pausetest, reasume
        HBufC*  iTestId;
        
        // Title of the test case
        HBufC*  iTitle;

        // Test case index
        TInt    iCaseNum;
        
        // Test case expected result
        TInt    iExpectedResult;
        
        // Test case expected result category
        TFullTestResult::TCaseExecutionResult iCategory;
        
        // Test case timeout value
        TInt    iTimeout;
    };

// TestScripter loop helper
NONSHARABLE_CLASS(CLoopHelper): public CActive
    {
    public:
        /**
        * Two-phased constructor.
        */
        static CLoopHelper* NewL( CTestRunner* aTestRunner );
    public:
        /**
        * Destrucktor.
        */
        ~CLoopHelper();
        
        /**
         * Should be to indicate loop start.
         */
        void LoopStartL();
        
        /**
         * Should be called to indicate end of iteration and begining of next iteration
         */
        void IterationEndStartNextIterationL();
        
        /**
         * Should be called to indicate loop end
         */
        void LoopEndL();

        /**
         * Should be called to register sub test case executed inside loop
         */
        void RegisterInLoopSubTestCaseL( CSubTestCaseRunner* aSubTestCaseRunner );
        
        /**
         * Should be called to unregister sub test case executed inside loop
         */
        void UnregisterInLoopSubTestCaseL( CSubTestCaseRunner* aSubTestCaseRunner );

        /**
         * Should be called to register remote event allocated inside loop
         */
        void RegisterInLoopSlaveL( CSlave* aSlave );

        /**
         * Should be called to unregister remote event allocated inside loop
         */
        void UnregisterInLoopSlaveL( CSlave* aSlave );
        
        /**
         * Returns array of registered in loop slaves.
         */
        RPointerArray<CSlave>& GetRegisteredInLoopSlaves();
        
        /**
         * Should be called to indicate end of sub test case executed inside loop
         */
        void NotifySubTestCaseEndL( CSubTestCaseRunner* aSubTestCaseRunner, TBool aPassed );
        
        /**
         * Indicates whether loop is in progress or not
         */
        TBool LoopInProgress() const;
    private:
        /**
         * Constructor
         */
        CLoopHelper( CTestRunner* aTestRunner );
        
        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL();
        
        /**
         * See CActive::DoCancel
         */
        void DoCancel();
        
        /**
         * See CActive::Runl
         */
        void RunL();

        /**
         * Indicate begining of the loop iteration
         */
        void IterationBeginL( TBool aFirstIteration );

        /**
         * Indicate end of the loop iteration
         */
        void IterationEndL( TBool aLastIteration );        
    private:
        // Pointer to CTestRunner
        CTestRunner* iTestRunner;
        
        // Indicates whether loop is in progress
        TBool iInProgress;
        
        // Indicates whether loop iteration is ongoing
        TBool iIterationOngoing;
        
        // Indicates whether current iteration is last iteration 
        TBool iLoopEnd;
        
        // Counter of passed iterations
        TInt iPassedIterationsCounter;
        
        // Indicates if current iteration should be count as passed or failed
        TBool iPassCurrentIteration;
        
        // Array of sub test cases executed in loop
        RPointerArray<CSubTestCaseRunner> iInLoopSubTestCases;    
        
        // Array of remote events allocated inside loop
        RPointerArray<CSlave> iInLoopSlaves;
    };   

// TestScripter remote event wrapper
class TEventTS: public TEventIf
    {
    public:  // Constructors and destructor
        /**
        * C++ default constructor.
        */
        TEventTS();
        
        /**
        * C++ parametric constructor.
        */
        TEventTS( TName& aEventName );
  
        /**
        * C++ destructor.
        */
        ~TEventTS();

    public: // New functions
        
        /**
        * Set request status member.
        */
        void SetRequestStatus( TRequestStatus* aStatus );
        
        /**
        * Set event pending.
        */
        void SetEvent( TEventType aEventType );
        
        /**
        * Wait event.
        */
        void WaitEvent( TRequestStatus& aStatus );
        
    private: // New functions
        /**
        * Complete request status member.
        */
        void Complete( TInt aError );

    private:    // Data
        /**
        * Request status.
        */
        TRequestStatus* iClientReqStatus; 
    };

// Interface for classes which can handle errors which may occure during remote
// message receive
NONSHARABLE_CLASS( MReceiveErrorHandler )
    {
    public:
		/**
		 * Handle error which occured during remote message receive
		 */
        virtual void HandleRemoteReceiveError( 
                TInt aError, const TDesC& aErrorDescription ) = 0;        
    };

// Interface for classes which should be notified about remote allocate 
// and free requests results
NONSHARABLE_CLASS( MSlaveAllocateFreeMonitor )
    {
    public:
		/**
		 * Notify slave is allocated
		 */
        virtual void NotifySlaveAllocatedL( TUint16 aSlaveId ) = 0;
        
        /**
         * Notify slave is freed
         */
        virtual void NotifySlaveFreedL( TUint16 aSlaveId ) = 0;
    };

// Interface for classes which should be notified about results of remote test 
// control requests 
NONSHARABLE_CLASS( MRemoteTestCasesMonitor )
    {
    public:
		/**
		 * Notify test case has been started.
		 */
        virtual void NotifyTestCaseStartedL( TUint16 aSlaveId, TUint16 aSlaveTestId ) = 0;
        
        /**
         * Notify test case can't be started.
         */
        virtual void NotifyTestCaseRunErrorL( TUint16 aSlaveId, const TFullTestResult& aTestCaseResult ) = 0;
        
        /**
         * Notify execution test case has finished
         */
        virtual void NotifyTestCaseFinishedL( TUint16 aSlaveId, TUint16 aSlaveTestId, const TFullTestResult& aTestCaseResult ) = 0;
        
        /**
         * Notify test case has been paused.
         */
        virtual void NotifyTestCasePausedL( TUint16 aSlaveId, TUint16 aSlaveTestId ) = 0;
        
        /**
         * Notify test case has been resumed.
         */
        virtual void NotifyTestCaseResumedL( TUint16 aSlaveId, TUint16 aSlaveTestId ) = 0;
        
        /**
         * Notify test case has been cancelled.
         */
        virtual void NotifyTestCaseCancelledL( TUint16 aSlaveId, TUint16 aSlaveTestId ) = 0;
    };

// Interface for classes which should be notified about results of remote events
// control requests.
NONSHARABLE_CLASS( MRemoteEventsMonitor )
    {
    public:
		/**
		 * Notify remote event has been requested
		 */
        virtual void NotifyEventRequestedL( TUint16 aSlaveId, 
                const TDesC& aEventName, 
                CStifTFwIfProt::TEventStatus aEventStatus,
                TEventIf::TEventType aEventType,
                TInt aResult ) = 0;

        /**
         * Notify remote event state has changed
         */
        virtual void NotifyEventStateChangedL( TUint16 aSlaveId, 
                const TDesC& aEventName, 
                CStifTFwIfProt::TEventStatus aEventStatus,
                TEventIf::TEventType aEventType,
                TInt aResult ) = 0;
        
        /**
         * Notify remote event has been released
         */
        virtual void NotifyEventReleasedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult ) = 0;
        
        /**
         * Notify remote event has been sucessfully set
         */
        virtual void NotifyEventSetCompletedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult ) = 0;
        
        /**
         * Notify remote event has been sucessfully unset
         */
        virtual void NotifyEventUnsetCompletedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult ) = 0;        
    };

// Interface for classes which should be notified about results of sendreceive 
// and send unknown requests 
NONSHARABLE_CLASS( MRemoteSendReceiveMonitor )
    {
    public:
		/**
		 * Notify sendreceive request has been started
		 */
        virtual void NotifySendReceiveStartedL( TUint16 aSlaveId ) = 0;

        /**
         * Notify sendreceive request has finished
         */
        virtual void NotifySendReceiveResultL( 
                TUint16 aSlaveId, 
                CStifTFwIfProt::TRunStatus aRunStatus,
                TInt aRunResult ) = 0;
        
        /**
         * Notify send unknown request has finished
         */
        virtual void NotifySendUnknownL( TUint16 aSlaveId, TInt aResult ) = 0;        
    };

// This class creates, execute and recive respons for master-slave requests
NONSHARABLE_CLASS( CRemoteCallsProxy ): public CActive
    {
    public:
		/**
		 * Two phase constructor.
		 */
        static CRemoteCallsProxy* NewL( CTestModuleIf& aTestModuleIf, CStifLogger* aLogger );
    public:
        /**
         * Destructor.
         */
        ~CRemoteCallsProxy();

        /**
         * Sends slave allocate request.
         */
        void AllocateL( TUint32 aMasterId, const TDesC& aType );
        
        /**
         * Sends slave free request.
         */
        void FreeL( TUint32 aMasterId, TUint16 aSlaveId );
        
        /**
         * Sends sendreceive request.
         */
        void SendReceiveL(  TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aMessage );
        
        /**
         * Sends unknown request
         */        
        void SendUnknownL(  TUint32 aMasterId, TUint16 aSlaveId,
                const TDesC& aCommand, const TDesC& aMessage );
        
        /**
         * Sends start test case request.
         */
        void RunTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, CStartInfo* aStartInfo );
        
        /**
         * Sends pause test case request.
         */
        void PauseTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId );
        
        /**
         * Sends resume test case request.
         */
        void ResumeTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId );
        
        /**
         * Sends cancel test case request.
         */
        void CancelTestCaseL( TUint32 aMasterId, TUint16 aSlaveId, TUint16 aTestId );
        
        /**
         * Sends request event request.
         */
        void RequestEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName );
        
        /**
         * Sends set event request.
         */
        void SetEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName, 
                TEventIf::TEventType aEventType = TEventIf::EIndication );
        
        /**
         * Sends unset event request.
         */
        void UnsetEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName );
        
        /**
         * Sends release event request.
         */
        void ReleaseEventL( TUint32 aMasterId, TUint16 aSlaveId, const TDesC& aEventName );
        
        /**
         * Sets slave allocate/free requests notyfications handler.
         */
        void SetSlaveAllocateFreeMonitor( MSlaveAllocateFreeMonitor* aSlaveAllocateFreeMonitor );

        /**
         * Sets remote test case control requests notyfications handler.
         */
        void SetRemoteTestCasesMonitor( MRemoteTestCasesMonitor* aRemoteTestCasesMonitor );

        /**
         * Sets remote events control requests notyfications handler.
         */
        void SetRemoteEventsMonitor( MRemoteEventsMonitor* aRemoteEventssMonitor );

        /**
         * Sets remote sendreceive request notyfications handler.
         */
        void SetRemoteSendReceiveMonitor( MRemoteSendReceiveMonitor* aRemoteSendReceiveMonitor );

        /**
         * Sets remote respones receive error handler.
         */
        void SetReceiveErrorHandler( MReceiveErrorHandler* aReceiveErrorHandler );
        
    private:
        /**
         * Constructor.
         */
        CRemoteCallsProxy( CTestModuleIf& aTestModuleIf, CStifLogger* aLogger  );
        
        /**
         * Second phase of two phase construction.
         */
        void ConstructL();
        
        /**
         * Cancels remote responses receiving.
         */
        void DoCancel();
        
        /**
         * Handlers remote request respons
         */
        void RunL();
        
        /**
         * Handles errors during remote request response dispatching
         */
        TInt RunError( TInt aError );
        
        /**
         * Starts remote requests respons receiving.
         */
        void StartReceivingL();
        
        /**
         * Dispatches received remote request response.
         */
        void DispatchReceivedRemoteMsgL();
        
        /**
         * Handles slave reserve request response.
         */
        void ReceiveResponseReserveL( CStifTFwIfProt* aMsg );
        
        /**
         * Handles slave release request response.
         */
        void ReceiveResponseReleaseL( CStifTFwIfProt* aMsg );
        
        /**
         * Handles remote test case run request response.
         */
        void ReceiveResponseRunL( CStifTFwIfProt* aMsg );
        
        /**
         * Handles remote test case control request response.
         */
        void ReceiveResponseTestCtlL( CStifTFwIfProt* aMsg );

        /**
         * Handles remote event control request response.
         */
        void ReceiveResponseEventCtlL( CStifTFwIfProt* aMsg );

        /**
         * Handles remote sendreceive request response.
         */
        void ReceiveResponseSendReceiveL( CStifTFwIfProt* aMsg );
        
        /**
         * Handles remote send unknown request response.
         */
        void ReceiveResponseSendUnknownL( CStifTFwIfProt* aMsg );
    private:
        // Pointer to STF looger
        CStifLogger* iLogger;
        
        // TestModuleIf used to send master-slave requests
        CTestModuleIf& iTestModuleIf;
        
        // Buffer for remote request response
        TRemoteProtMsg iReceivedRemoteMsg;

        // Buffer for error  description
        TBuf<256> iReceiveErrorDescription;
        
        // Pointer to slave allocate/free monitor
        MSlaveAllocateFreeMonitor* iSlaveAllocateFreeMonitor;
        
        // Pointer to test case control requests monitor
        MRemoteTestCasesMonitor* iRemoteTestCasesMonitor;
        
        // Pointer to events control requests monitor
        MRemoteEventsMonitor* iRemoteEventsMonitor;
        
        // Pointer to send/receive requests monitor
        MRemoteSendReceiveMonitor* iRemoteSendReceiveMonitor;
        
        // Pointer to receive error handler
        MReceiveErrorHandler* iReceiveErrorHandler;
    };

// This class contains most important informations about allocated slave device
NONSHARABLE_CLASS( CSlave ): public CBase
    {
    public:
		/**
		 * Two phase constructor.
		 */
        static CSlave* NewL();
        
        /**
         * Destructor.
         */
        ~CSlave();
        
        /**
         * Returns slave name.
         */
        const TDesC& GetName() const;
        
        /**
         * Sets slave name.
         */
        void SetNameL( const TDesC& aName );
        
        /**
         * Returns slave master's id.
         */
        TUint32 GetMasterId() const;
        
        /**
         * Sets slave master's id
         */
        void SetMasterId( TUint32 aMasterId );
        
        /**
         * Returns slave's id.
         */
        TUint16 GetSlaveId() const;
        
        /**
         * Sets slave's id.
         */
        void SetSlaveId( TUint16 aSlaveId );

        /**
         * Returns true if slave has running remote sub test cases.
         */
        TBool HasRunningTestCases() const;
        
        /**
         * Register slave remote sub test case.
         */
        void RegisterSubTestCaseL( CRemoteSubTestCaseRunner* aSubTestCase );
        
        /**
         * Unregister slave remote sub test case.
         */
        void UnregisterSubTestCaseL( CRemoteSubTestCaseRunner* aSubTestCase );
        
        /**
         * Returns list of slave remote sub test cases. 
         */
        RPointerArray<CRemoteSubTestCaseRunner>& GetSubTestCases();
        
        /**
         * Waits until all running slave remote sub test cases finis.
         */
        void WaitForSubTestCasesL();
        
        /**
         * Cancel WaitForSubTestCasesL call.
         */
        void CancelWaitForSubTestCases();
        
        /**
         * Register slave remote event.
         */
        void AddEventL( TEventTS* aEvent );

        /**
         * Unregister slave remote event.
         */
        void RemoveEventL( TEventTS* aEvent );
        
        /**
         * Returns selected slave remote event.
         */
        TEventTS* GetEvent( const TDesC& aName );
        
        RPointerArray<TEventTS>& GetEvents();
    private:
        /**
         * Constructor.
         */
        CSlave();
        
        /**
         * Second phase of two phase construction.
         */
        void ConstructL();
    private:
        /**
         * Slave's name.
         */
        HBufC* iName;
        
        /**
         * Slave's master id.
         */
        TUint32 iMasterId;
        
        /**
         * Slave's id.
         */
        TUint16 iSlaveId;
        
        /**
         * List of slave's remote sub test cases.
         */
        RPointerArray<CRemoteSubTestCaseRunner> iSubTestCases;
        
        /**
         * List of slave's remote events.
         */
        RPointerArray<TEventTS> iEvents;
        
        /**
         * nested active scheduler loop used to wait for running remote sub
         * test cases finis.
         */
        CActiveSchedulerWait* iNestedASLoop;
    };

// Manages slaves and wraps most of the master-slave communication
NONSHARABLE_CLASS( CSlavesManager ): 
    protected CActive, 
    private MSlaveAllocateFreeMonitor, 
    private MRemoteTestCasesMonitor,
    private MRemoteEventsMonitor,
    private MRemoteSendReceiveMonitor,
    private MReceiveErrorHandler
    {
    private:
		/**
		 * Types of internal CSlavesManager states.
		 */
        enum TSlavesManagerOperation { 
			ESMOIdle, 
			ESMOSlaveAllocate, 
			ESMOSlaveFree,
			ESMOEventRequest, 
			ESMOEventRelease,
			ESMOEventSet, 
			ESMOEventUnset,
			ESMOSendReceiveWaitForStarted,
			ESMOSendReceiveWaitForReady,
			ESMOSendUnknown,
			};
    public:
        /**
         * Two phase constructor.
         */
        static CSlavesManager* NewL( CTestRunner* aTestRunner, CTestModuleIf& aTestModuleIf );
        
        /**
         * Destructor.
         */
        ~CSlavesManager();
        
        /**
         * Allocates slave device.
         */
        void SlaveAllocateL( const TDesC& aName, const TDesC& aType );
        
        /**
         * Frees slave device.
         */
        void SlaveFreeL( const TDesC& aName );

        /**
         * Send remote command.
         */
        void SendReceiveL( CSlave* aSlave, const TDesC& aMessage );
        
        /**
         * Send command not specified in STF master-slave protocol
         */
        void SendUnknownL( CSlave* aSlave, const TDesC& aCommand, const TDesC& aMessage );
        
        /**
         * Returns selected slave device wrapper object.
         */
        CSlave* GetSlave( const TDesC& aName );
        
        /**
         * Returns selected slave device wrapper object.
         */
        CSlave* GetSlave( TUint16 aSlaveId );

        RPointerArray<CSlave>& GetSlaves();

        /**
         * Requests specified event from slave device
         */
        void EventRequestL( CSlave* aSlave, TEventTS* aEvent );
        
        /**
         * Releases selected remote event.
         */
        void EventReleaseL( CSlave* aSlave, const TDesC& aEventName );        

        /**
         * Sets remote event.
         */
        void EventSetL( CSlave* aSlave, const TDesC& aEventName, TEventIf::TEventType aEventType );
        
        /**
         * Unsets remote event
         */
        void EventUnsetL( CSlave* aSlave, const TDesC& aEventName );        
        
        /**
         * Returns CRemoteCallsProxy used in master-slave communication.
         */
        CRemoteCallsProxy* GetRemoteCallsProxy();
        
    private:
        /**
         * Constructor.
         */
        CSlavesManager( CTestRunner* aTestRunner, CTestModuleIf& aTestModuleIf );
        
        /**
         * Second phase of two phase construction.
         */
        void ConstructL();

        /**
         * Handles operation timeout event.
         */
        void RunL();
        
        /**
         * Cancels operation timeout monitor.
         */
        void DoCancel();
        
		/**
		 * Notify slave is allocated
		 */
        void NotifySlaveAllocatedL( TUint16 aSlaveId );
        
        /**
         * Notify slave is freed
         */
        void NotifySlaveFreedL( TUint16 aSlaveId );

		/**
		 * Notify test case has been started.
		 */
        void NotifyTestCaseStartedL( TUint16 aSlaveId, TUint16 aSlaveTestId );
        
        /**
         * Notify test case can't be started.
         */        
        void NotifyTestCaseRunErrorL( TUint16 aSlaveId, 
        		const TFullTestResult& aTestCaseResult );
        
        /**
         * Notify execution test case has finished
         */
        void NotifyTestCaseFinishedL( TUint16 aSlaveId, TUint16 aSlaveTestId, 
        		const TFullTestResult& aTestCaseResult );
        
        /**
         * Notify test case has been paused.
         */
        void NotifyTestCasePausedL( TUint16 aSlaveId, TUint16 aSlaveTestId );
        
        /**
         * Notify test case has been resumed.
         */
        void NotifyTestCaseResumedL( TUint16 aSlaveId, TUint16 aSlaveTestId );
        
        /**
         * Notify test case has been cancelled.
         */
        void NotifyTestCaseCancelledL( TUint16 aSlaveId, TUint16 aSlaveTestId );
        
		/**
		 * Notify remote event has been requested
		 */
        void NotifyEventRequestedL( TUint16 aSlaveId, 
                const TDesC& aEventName, 
                CStifTFwIfProt::TEventStatus aEventStatus,
                TEventIf::TEventType aEventType,
                TInt aResult );
        
        /**
         * Notify remote event state has changed
         */
        void NotifyEventStateChangedL( TUint16 aSlaveId, 
                const TDesC& aEventName, 
                CStifTFwIfProt::TEventStatus aEventStatus,
                TEventIf::TEventType aEventType,
                TInt aResult );
        
        /**
         * Notify remote event has been released
         */
        void NotifyEventReleasedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult  );

        /**
         * Notify remote event has been sucessfully set
         */
        void NotifyEventSetCompletedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult );
        
        /**
         * Notify remote event has been sucessfully unset
         */
        void NotifyEventUnsetCompletedL( TUint16 aSlaveId, 
                const TDesC& aEventName, TInt aResult );        
        
		/**
		 * Notify sendreceive request has been started
		 */
        void NotifySendReceiveStartedL( TUint16 aSlaveId );
        
        /**
         * Notify sendreceive request has finished
         */
        void NotifySendReceiveResultL( 
                TUint16 aSlaveId, 
                CStifTFwIfProt::TRunStatus aRunStatus,
                TInt aRunResult );
        
        /**
         * Notify send unknown request has finished
         */
        void NotifySendUnknownL( TUint16 aSlaveId, TInt aResult );
        
		/**
		 * Handle error which occured during remote message receive
		 */
        void HandleRemoteReceiveError( TInt aError, const TDesC& aErrorDescription );
        
    private:
        // Indicates currently performed operation
        TSlavesManagerOperation iOperation;
        
        // Contains last operation result
        TInt iLastOperationResult;
        
        // Pointer to CTestRunner object
        CTestRunner* iTestRunner;
        
        // TestModuleIf used by CRemoteCallsProxy in master-slave communication
        CTestModuleIf& iTestModuleIf;
        
        // CRemoteCallsProxy used in master-slave communication
        CRemoteCallsProxy* iRemoteCallsProxy;
        
        // Array of allocated slave devices
        RPointerArray<CSlave> iSlaves;
        
        // Temporary field used during slave device allocation
        CSlave* iSlaveForAllocate;
        
        // Temporary field used during event request
        TEventTS* iEventForRequest;
        
        // Operation timeout
        TTimeIntervalMicroSeconds32 iOperationTimeout;

        // Operation timeout timer
        RTimer iOperationTimeoutTimer;
        
        // Nested active scheduler loop used to block CSlavesManager methods 
        // execution until respons for selected request is received
        CActiveSchedulerWait* iNestedASLoop;
    };

#endif /* TESTSCRIPTERUTILS_H_ */
