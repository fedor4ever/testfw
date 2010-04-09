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
* Description: This module contain implementation of RSettingServer 
* class member functions.
*
*/

// INCLUDE FILES
#include <e32svr.h>
#include <SettingServerClient.h>
#include "SettingServerClientServer.h"
#include "SettingServer.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES

// CONSTANTS
_LIT( KSettingServerName, "StifSettingServer" );

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ==================== LOCAL FUNCTIONS =======================================

/**
 * Panics settings server thread
 */
void PanicSettingServer( const TSettingServerPanic aPanic )
    {
    RDebug::Print( _L( "CSettingServer::PanicServer" ) );
    _LIT( KTxtServer,"CSettingServer" );
    User::Panic( KTxtServer,aPanic );
    }

/*
-------------------------------------------------------------------------------

    Class: None

    Method: SettingServerThreadFunction

    Description: The thread function, where Setting Server lives in
    
    Parameters: TAny* aStartInfo aName: in: Start-up information
    
    Return Values: TInt: Result from server start

    Errors/Exceptions: Panics current thread if:
                       Invalid start-up information
                       Clean-up stack can't be created
                       Setting Server can't be started

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt SettingServerThreadFunction( TAny* aStarted )
    {
    __UHEAP_MARK;

    // Get start-up information
    TThreadStartSetting* startInfo = ( TThreadStartSetting* ) aStarted;
    __ASSERT_ALWAYS( startInfo,PanicSettingServer( ENoStartupInformation ) );

    // Create clean-up stack
    CTrapCleanup* tc = CTrapCleanup::New();
    __ASSERT_ALWAYS( tc, PanicSettingServer(ECreateTrapCleanup));

    // Construct and install active scheduler
    CActiveScheduler* scheduler = new CActiveScheduler;
    __ASSERT_ALWAYS( scheduler, PanicSettingServer( EMainSchedulerError ) );
    CActiveScheduler::Install( scheduler );

    // Construct server
    CSettingServer* server = NULL;
    RDebug::Print( startInfo->iName );
    TRAPD( err, server = CSettingServer::NewL( startInfo->iName ) );
    __ASSERT_ALWAYS( !err, PanicSettingServer( ESvrCreateServer ) );

    // Inform that we are up and running
    startInfo->iStartupResult = KErrNone;
    startInfo->iStarted.Signal();

    // Start handling requests
    CActiveScheduler::Start();

   // Execution continues from here after CActiveScheduler::Stop

    // Delete the server. This should be deleted before scheduler,
    // if server still has any active objects
    delete server;
    server = NULL;
    
    // Delete scheduler. 
    delete scheduler;
    scheduler = NULL;

    // Delete clean-up stack
    delete tc;
    tc = NULL;

    __UHEAP_MARKEND;

    return KErrNone;

    }

/*
-------------------------------------------------------------------------------

    Class: -

    Method: StartNewServer

    Description: Starts a new server. Server will be running its own
    thread and this functions returns when server is up and running or
    server start-up fails.

    Parameters:  TName& aServerName: inout: The name of the server
    
    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt StartNewSettingServer()
    {
    __UHEAP_MARK;

    //Ccheck server not already started
    TFindServer findServer( KSettingServerName );
    TFullName name;
    if( findServer.Next( name ) == KErrNone )
        {   
        // Server already started, nothing to do
        __UHEAP_MARKEND;
        return KErrAlreadyExists;
        }

    // Construct start-up information object
    TThreadStartSetting* startInfo = new TThreadStartSetting();
    if( startInfo == NULL )
        {
        __UHEAP_MARKEND;
        return KErrNoMemory;
        }

    // Fill the start-up information
    startInfo->iName = KSettingServerName;
    startInfo->iStartupResult = KErrNone;
    startInfo->iStarted.CreateLocal( 0 );

    // Create thread    
    TInt res = startInfo->iServerThread.Create( 
        startInfo->iName ,                       // Name of thread
        SettingServerThreadFunction,          // Thread function
        KDefaultStackSize,                       // Stack size
        KDefaultHeapSize,                        // Heap initial size
        KMaxHeapSize,                            // Heap start max size
        startInfo                                // Parameter to thread function
        );

    // If thread creation failed
    if( res != KErrNone )        
        {
        startInfo->iStarted.Close();             // Close semaphore
        delete startInfo;
        startInfo = NULL;
        __UHEAP_MARKEND;
        return res;
        }

    // Now start thread
    startInfo->iServerThread.SetPriority( EPriorityMuchMore ); 
    startInfo->iServerThread.Resume();
    
    // Wait until the thread is started
    startInfo->iStarted.Wait();

    // Free memory
    TInt r = startInfo->iStartupResult;
    startInfo->iServerThread.Close();
    startInfo->iStarted.Close();
    delete startInfo;
    startInfo = NULL;

    __UHEAP_MARKEND;

    // Return start-up result.
    return r;

    }

// ================= MEMBER FUNCTIONS =========================================

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: RSettingServer

    Description: Default constructor

    C++ default constructor can NOT contain any code, that
    might leave.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C RSettingServer::RSettingServer()
    {

    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: Connect

    Description: Connect method creates new RSettingServer session.
    First the server is tried to start. If start is successfull or server is
    already started, then a new session is created.  

    Parameters: None

    Return Values: TInt: Error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt RSettingServer::Connect()
    {
    TInt ret = StartNewSettingServer();

    if( ret == KErrNone || ret == KErrAlreadyExists )
        {
        ret = CreateSession( KSettingServerName, Version() );
        }

    return ret;

    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: Version

    Description: Return client side version.

    Parameters: None

    Return Values: TVersion: Version number

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TVersion RSettingServer::Version() const
    {
    return( TVersion( KSettingServerMajorVersionNumber,
                      KSettingServerMinorVersionNumber, 
                      KSettingServerVersionNumber 
                     ) );

    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: Close

    Description: Closes the RSettingServer session.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C void RSettingServer::Close()
    {
    // Check that server is connected and send close message to it if possible.
    if( Handle() != 0 )
        {
        TIpcArgs args( TIpcArgs::ENothing, TIpcArgs::ENothing, TIpcArgs::ENothing );
        // Goes to CSettingServer's DispatchMessageL() method
        SendReceive( ESettingServerCloseSession, args );
        }

    RSessionBase::Close();

    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: ReadLoggerSettingsFromIniFile

    Description: Read Logger setting from initialization file. Mainly use from
                 TestEngine side.

    Parameters: TLoggerSettings& aLoggerSettings: inout: Logger's overwrite
                struct

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt RSettingServer::LoadLoggerSettingsFromIniFile( TLoggerSettings& aLoggerSettings, 
        const TDesC& aIniFile,
        TBool aOverwritePreviousSettings )
    {
    // Package
    TPckg<TLoggerSettings> loggerSettingsPckg( aLoggerSettings );

	TIpcArgs args( &aIniFile, &loggerSettingsPckg, aOverwritePreviousSettings );
    // Goes to CSettingServer's DispatchMessageL() method
    SendReceive( ELoadLoggerSettingsFromIniFile, args );
   
    return KErrNone;

    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: LoadLoggerSettingsFromCommandLine

    Description: Read Logger setting from command line. Mainly use from
                 TestEngine side.

    Parameters: TDesC& aLoggerSettings: inout: Logger's overwrite
                struct

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt RSettingServer::LoadLoggerSettingsFromCommandLine(TDesC& aLoggerSettings)
    {
    TIpcArgs args( &aLoggerSettings, TIpcArgs::ENothing, TIpcArgs::ENothing );
    return SendReceive( ELoadLoggerSettingsFromCommandLine, args );
    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: GetLoggerSettings

    Description: Get Logger settings. Mainly use from Logger side.

    Parameters: TLoggerSettings& aLoggerSettings: in: Logger's overwrite struct

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt RSettingServer::GetLoggerSettings( 
                                            TLoggerSettings& aLoggerSettings )
    {
    // Package
    TPckg<TLoggerSettings> loggerSettingsPckg( aLoggerSettings );

	TIpcArgs args( &loggerSettingsPckg, TIpcArgs::ENothing, TIpcArgs::ENothing );
    // Goes to CSettingServer's DispatchMessageL() method
	return SendReceive( EGetLoggerSettings, args );
    }

/*
-------------------------------------------------------------------------------

    Class: RSettingServer

    Method: SetIniFileSetting

    Description: Set new initialization file setting(e.g. SetAttribute).

    Parameters: TName& aNewIniFileSetting: in: New setting

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C TInt RSettingServer::SetLoggerOutputPath( const TDesC& aLoggerOutputPath )
    {
	TIpcArgs args( &aLoggerOutputPath, TIpcArgs::ENothing, TIpcArgs::ENothing );
    // Goes to CSettingServer's DispatchMessageL() method
	return SendReceive( ESetLoggerOutputPath, args );
    }

EXPORT_C TInt RSettingServer::ResetLoggerSettings()
    {
    return SendReceive( EResetLoggerSettings, TIpcArgs() );
    }

// End of File
