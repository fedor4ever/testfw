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
* Description: This module contains implementation of 
* CTestEngineServer class member functions.
*
*/

// INCLUDE FILES
#include "TestEngineServer.h"
#include "TestEngineClientServer.h"
#include "Logging.h"
#include "TestEngine.h"
#include "TestReport.h"

#define LOGGER iLogger

const TInt KTestEngineServerShutdownDelay = 5000000; // 10s

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: CTestEngineServer

    Description: Default constructor

    C++ default constructor can NOT contain any code, that
    might leave.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
CTestEngineServer::CTestEngineServer() :
    CServer2( CTestEngineServer::ETestEngineServerPriority ),
    iSessionCount( 0 )
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: ConstructL

    Description: Symbian OS second phase constructor

    Symbian OS default constructor can leave.

    Parameters: None

    Return Values: None

    Errors/Exceptions: Leaves if CObjectConIx::NewL leaves
                       Leaves if CStifLogger::NewL leaves
                       Leaves if StartL leaves

    Status: Approved

-------------------------------------------------------------------------------
*/
void CTestEngineServer::ConstructL()
    {
    // Create container
    iContainerIndex = CObjectConIx::NewL();

    InitLoggerSettings();
    InitLoggerL();

    iAutoShutdownTimer = CTestEngineServerAutoShutdown::NewL();
    iAutoShutdownTimer->Start();
    
    __TRACE(KVerbose, (_L( "CTestEngineServer::ConstructL: Server Created" ) ) );
    }

void CTestEngineServer::InitLoggerSettings()
    {
    // Initializations to indicator is setting in use
    iLoggerSettings.iIsDefined.iCreateLogDir = EFalse;
    iLoggerSettings.iIsDefined.iPath = EFalse;
    iLoggerSettings.iIsDefined.iHwPath = EFalse;
    iLoggerSettings.iIsDefined.iFormat = EFalse;
    iLoggerSettings.iIsDefined.iHwFormat = EFalse;
    iLoggerSettings.iIsDefined.iOutput = EFalse;
    iLoggerSettings.iIsDefined.iHwOutput = EFalse;
    iLoggerSettings.iIsDefined.iOverwrite = EFalse;
    iLoggerSettings.iIsDefined.iLineBreak = EFalse;
    iLoggerSettings.iIsDefined.iTimeStamp = EFalse;
    iLoggerSettings.iIsDefined.iEventRanking = EFalse;
    iLoggerSettings.iIsDefined.iThreadId = EFalse;    
    }

void CTestEngineServer::InitLoggerL()
    {
    // Create logger, in Wins use HTML in HW default logger
    TLoggerSettings loggerSettings;

    // Directory must create by hand if test engine log wanted
    loggerSettings.iCreateLogDirectories = EFalse;

    loggerSettings.iOverwrite = ETrue;
    loggerSettings.iTimeStamp = ETrue;
    loggerSettings.iLineBreak = ETrue;
    loggerSettings.iEventRanking = EFalse;
    loggerSettings.iThreadId = EFalse;
    loggerSettings.iHardwareFormat = CStifLogger::ETxt;
#ifndef FORCE_STIF_INTERNAL_LOGGING_TO_RDEBUG
    loggerSettings.iEmulatorFormat = CStifLogger::EHtml;
    loggerSettings.iEmulatorOutput = CStifLogger::EFile;
    loggerSettings.iHardwareOutput = CStifLogger::EFile;
#else
    RDebug::Print( _L( "STIF Test Engine logging forced to RDebug" ) );
    loggerSettings.iEmulatorFormat = CStifLogger::ETxt;
    loggerSettings.iEmulatorOutput = CStifLogger::ERDebug;
    loggerSettings.iHardwareOutput = CStifLogger::ERDebug;
#endif
    loggerSettings.iUnicode = EFalse;
    loggerSettings.iAddTestCaseTitle = EFalse;

    iLogger = CStifLogger::NewL( _L( "C:\\logs\\testframework\\testengine\\"),
                                    _L( "testengine" ),
                                    loggerSettings );    
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: NewL

    Description: Two-phased constructor.

    Parameters: None

    Return Values: CTestEngineServer* : pointer to CTestEngineServer object

    Errors/Exceptions: Leaves if construction of CBufferArray fails

    Status: Approved

-------------------------------------------------------------------------------
*/
CTestEngineServer* CTestEngineServer::NewL()
    {
    CTestEngineServer* self = new ( ELeave ) CTestEngineServer();
    CleanupStack::PushL( self );      
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: ~CTestEngineServer

    Description: Destructor

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
CTestEngineServer::~CTestEngineServer()
    {
    delete iAutoShutdownTimer;
    
    delete iContainerIndex; 

    __TRACE(KAlways, ( _L( "---------------- Log Ended ----------------" ) ) );
    delete iLogger;
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: NewContainerL

    Description: Create new container

    Parameters: None

    Return Values: None

    Errors/Exceptions: Leaves if called CreateL method leaves

    Status: Approved

-------------------------------------------------------------------------------
*/
CObjectCon* CTestEngineServer::NewContainerL()
    {
    CObjectCon* container = iContainerIndex->CreateL();
    
    if ( iSessionCount == 0 )
        {
        iAutoShutdownTimer->Cancel();
        }
    iSessionCount++;

    return container;
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: DeleteContainer

    Description: Deletes a container

    Parameters: CObjectCon* 

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
void CTestEngineServer::DeleteContainer( CObjectCon* aContainer )
    {
    iContainerIndex->Remove( aContainer );

    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: SessionClosed

    Description: Inform Server that session is closed.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
void CTestEngineServer::SessionClosed()
    {
    // Decrease session count
    iSessionCount--;

    // Check if last session is closed
    if ( iSessionCount <= 0 )
        {
        // Stop the active scheduler
        // Execution will continue in ThreadFunction()
        iAutoShutdownTimer->Start();
        
        RSettingServer settingServer;
        TRAPD( ret, settingServer.Connect() );
        if ( ret == KErrNone )
            {
            settingServer.ResetLoggerSettings();
            settingServer.Close();
            }
        }

    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: NewSessionL

    Description: Create a new client session for this server.

    Parameters: const TVersion& aVersion: in: Client side version number

    Return Values: CSharableSession* : pointer to CSharableSession object

    Errors/Exceptions: Leaves if given version is incorrect
                       Leaves if CTestEngine::NewL leaves

    Status: Approved

-------------------------------------------------------------------------------
*/
CSession2* CTestEngineServer::NewSessionL( const TVersion &aVersion,
                                         const RMessage2& /*aMessage*/ ) const
    {
    // Check that version is ok
    TVersion v( KTestEngineMajorVersionNumber,
                KTestEngineMinorVersionNumber,
                KTestEngineBuildVersionNumber );
    if ( !User::QueryVersionSupported( v, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    return CTestEngine::NewL( ( CTestEngineServer* ) this );
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: PanicServer

    Description: Panic the server

    Parameters: TTestEnginePanic aPanic: in: Panic number

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
void CTestEngineServer::PanicServer( TTestEnginePanic aPanic )
    {
    _LIT( KTestServer, "CTestEngineServer" );
    User::Panic( KTestServer, aPanic );

    }

void CTestEngineServer::SetLoggerSettings( const TLoggerSettings& aLoggerSettings )
    {
    iLoggerSettings = aLoggerSettings;
    }

const TLoggerSettings& CTestEngineServer::GetLoggerSettings() const
    {
    return iLoggerSettings;
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServer

    Method: Logger

    Description: Return the pointer to iLogger.

    Parameters: None

    Return Values: CStifLogger*: Pointer to StifLogger

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
CStifLogger* CTestEngineServer::Logger()
    {
    return iLogger;

    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServerAutoShutdown

    Method: NewL

    Description: NewL is two-phased constructor.

    Parameters: None

    Return Values: CTestEngineServerAutoShutdown*: Pointer to new CTestEngineServerAutoShutdown object

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
CTestEngineServerAutoShutdown* CTestEngineServerAutoShutdown::NewL()
    {
    CTestEngineServerAutoShutdown* self = new(ELeave)CTestEngineServerAutoShutdown;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServerAutoShutdown

    Method: CTestEngineServerAutoShutdown

    Description: Default constructor.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
CTestEngineServerAutoShutdown::CTestEngineServerAutoShutdown()
:CTimer(-1)
    {
    CActiveScheduler::Add( this );
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServerAutoShutdown

    Method: ConstructL

    Description: Second phase of two-pahes constructor.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
void CTestEngineServerAutoShutdown::ConstructL()
    {
    CTimer::ConstructL();
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServerAutoShutdown

    Method: Start

    Description: Starts timer countdown.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
void CTestEngineServerAutoShutdown::Start()
    {
    After( KTestEngineServerShutdownDelay );
    }

/*
-------------------------------------------------------------------------------

    Class: CTestEngineServerAutoShutdown

    Method: RunL

    Description: Stops active scheduler.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
void CTestEngineServerAutoShutdown::RunL()
    {
    RDebug::Print( _L("TestEngine server shutdown") );
    CActiveScheduler::Stop();
    }

/*
-------------------------------------------------------------------------------

    Function: RunTestEngineServerL

    Description: Creates and starts TestEngine server

    Parameters: None

    Return Values: None

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
static void RunTestEngineServerL()
    {
    User::LeaveIfError( RThread::RenameMe( KTestEngineServerName ) );

    CActiveScheduler* activeScheduler = new(ELeave) CActiveScheduler;
    CleanupStack::PushL( activeScheduler );
    CActiveScheduler::Install( activeScheduler );

    // create the server (leave it on the cleanup stack)
    CTestEngineServer* testEngineServer = CTestEngineServer::NewL();
    CleanupStack::PushL( testEngineServer );

    testEngineServer->StartL( KTestEngineServerName );
    
    // Initialisation complete, now signal the client
    RProcess::Rendezvous( KErrNone );

    // Ready to run
    CActiveScheduler::Start();

    // Cleanup the server and scheduler
    CleanupStack::PopAndDestroy( testEngineServer );    
    CleanupStack::PopAndDestroy( activeScheduler );
    }

/*
-------------------------------------------------------------------------------

    Function: E32Main

    Description: TestEngine process main function

    Parameters: None

    Return Values: TInt: Process exit code.

    Errors/Exceptions: None
-------------------------------------------------------------------------------
*/
TInt E32Main()
    {
    __UHEAP_MARK;
    RDebug::Print( _L("TestEngine server start") );
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TInt ret = KErrNoMemory;
    if ( cleanup )
        {
        TRAP( ret, RunTestEngineServerL() );
        delete cleanup;
        }
    RDebug::Print( _L("TestEngine server closed") );
    __UHEAP_MARKEND;
    return ret;
    }

// End of file
