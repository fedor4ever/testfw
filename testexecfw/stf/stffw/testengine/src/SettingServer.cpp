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
* Description: This module contains implementation of CSettingServer 
* class member functions.
*
*/

// INCLUDE FILES
#include <e32std.h>
#include <e32svr.h>

#include "SettingServerClient.h"
#include "SettingServer.h"

#include <StifLogger.h>
#include "Logging.h"

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ==================== LOCAL FUNCTIONS =======================================
// None

#undef LOGGER
#define LOGGER iSettingServerLogger

// ================= MEMBER FUNCTIONS =========================================

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: PanicServer

    Description: Panics the server. 

    Parameters: const TSettingServerPanic aPanic :in:  Panic code

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CSettingServer::PanicServer( const TSettingServerPanic aPanic )
    {
    RDebug::Print( _L( "CSettingServer::PanicServer" ) );
    _LIT( KTxtServer,"CSettingServer" );
    User::Panic( KTxtServer,aPanic );
    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: NewL

    Description: Returns new CSettingServer object

    Parameters: const TName& aName: in: Server name

    Return Values: None

    Errors/Exceptions: Leaves if memory allocation, ConstructL or StartL
                       leaves.

    Status: Proposal

-------------------------------------------------------------------------------
*/
CSettingServer* CSettingServer::NewL( const TName& aName )

    {
    CSettingServer* self = new( ELeave ) CSettingServer();
    CleanupStack::PushL( self );

    // Construct the server
    self->ConstructL();

    // Start the server
    self->StartL( aName );

    CleanupStack::Pop( self );

    return self;

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: ConstructL

    Description: Second level constructor.
 
    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CSettingServer::ConstructL()
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
    loggerSettings.iHardwareOutput = CStifLogger::EFile;
    loggerSettings.iEmulatorOutput = CStifLogger::EFile;
#else
    RDebug::Print( _L( "STIF SettingServer logging forced to RDebug" ) );
    loggerSettings.iEmulatorFormat = CStifLogger::ETxt;
    loggerSettings.iHardwareOutput = CStifLogger::ERDebug;
    loggerSettings.iEmulatorOutput = CStifLogger::ERDebug;
#endif
    loggerSettings.iUnicode = EFalse;
    loggerSettings.iAddTestCaseTitle = EFalse;
    iSettingServerLogger = CStifLogger::NewL( 
                        _L( "C:\\logs\\testframework\\testengine\\"),
                        _L( "SettingServer" ),
                        loggerSettings );

    ResetLoggerSettings();
    
    __TRACE( KInit,( _L( "-.-.-.-.-.-.-.-.-.- SettingServer log starts -.-.-.-.-.-.-.-.-.-" ) ) );
    __TRACE( KInit,( _L( "CSettingServer::ConstructL()" ) ) );
    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: CSettingServer

    Description: Constructor.

    Initialises non-zero member variables and base class with correct
    priority.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
CSettingServer::CSettingServer() :
        CServer2( CSettingServer::ESettingServerSchedulerPriority )
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: ~CSettingServer

    Description: Destructor.
   
    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
CSettingServer::~CSettingServer()
    {
    delete iSettingServerLogger;

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: NewSessionL

    Description: Returns new session.
    
    Parameters: const TVersion &aVersion:in :Version required

    Return Values: CSharableSession* New session

    Errors/Exceptions: Leaves if invalid version
                       Leaves if CSettingServer construction leaves

    Status: Proposal

-------------------------------------------------------------------------------
*/
CSession2* CSettingServer::NewSessionL( const TVersion& aVersion,
                                        const RMessage2& /*aMessage*/ ) const

    {
    // check version is ok
    TVersion v( KSettingServerMajorVersionNumber,
                KSettingServerMinorVersionNumber,
                KSettingServerVersionNumber
               );

    if( !User::QueryVersionSupported( v, aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }

    return CLoggerSetting::NewL( ( CSettingServer* ) this );
    }
/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: OpenSession

    Description: Opens session. Session calls calls this when session is
    properly constructed.
    
    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CSettingServer::OpenSession()
    {
    iSessions++;

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: Logger

    Description: Return pointer to the Logger(iSettingServerLogger)

    Parameters: None

    Return Values: CStifLogger*: Pointer to Logger

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
CStifLogger* CSettingServer::Logger()
    {
    return iSettingServerLogger;

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: CloseSession

    Description: Closes session. CLoggerSetting::CloseSession calls this
    to inform server class. 

    If there are not any active sessions, then stop active scheduler and
    close whole server.

    Parameters: None

    Return Values: None

    Errors/Exceptions: Function panics if OpenSessions() and CloseSessions()
                       does not match.

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CSettingServer::CloseSession()
    {
    __ASSERT_ALWAYS ( iSessions > 0, CSettingServer::PanicServer(ETooManyCloseSessions));
    iSessions--;  

    if ( iSessions == 0 )
        {
        // Stop the active scheduler if no-one is using server anymore
        CActiveScheduler::Stop();
        }

    }

void CSettingServer::ResetLoggerSettings()
    {
    // iLoggerSettings initializations to indicate is setting in use
    iLoggerSettings.iIsDefined.iCreateLogDir = EFalse;
    iLoggerSettings.iEmulatorPath.Copy( _L( "" ) );
    iLoggerSettings.iEmulatorFormat = CStifLogger::ETxt;
    iLoggerSettings.iEmulatorOutput = CStifLogger::EFile;
    iLoggerSettings.iHardwarePath.Copy( _L( "" ) );
    iLoggerSettings.iHardwareFormat = CStifLogger::ETxt;
    iLoggerSettings.iHardwareOutput = CStifLogger::EFile;
    iLoggerSettings.iOverwrite = ETrue;
    iLoggerSettings.iTimeStamp = ETrue;
    iLoggerSettings.iLineBreak = ETrue;
    iLoggerSettings.iEventRanking = EFalse;
    iLoggerSettings.iThreadId = EFalse;
    iLoggerSettings.iUnicode = EFalse;
    iLoggerSettings.iAddTestCaseTitle = EFalse;

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
    iLoggerSettings.iIsDefined.iUnicode = EFalse;
    iLoggerSettings.iIsDefined.iAddTestCaseTitle = EFalse;

    iIniFile = KNullDesC;
    iDefaultSettings = ETrue;
    }


//  End of File
