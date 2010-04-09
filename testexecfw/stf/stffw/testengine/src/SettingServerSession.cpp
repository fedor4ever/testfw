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

#include <StifParser.h>
#include "STIFTestFrameworkSettings.h"

#include "Logging.h"
#include "TestEngineUtils.h"

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
#define LOGGER iSettingServer->Logger()

// ================= MEMBER FUNCTIONS =========================================

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: NewL

    Description: Create new CLoggerSetting (Setting server session)

    Parameters: RThread& aClient: in: Handle to thread client
                CSettingServer* aServer: in: Pointer to CSettingServer

    Return Values: CLoggerSetting* Pointer to new CLoggerSetting

    Errors/Exceptions: Leaves if memory allocation fails or ConstructL leaves.

    Status: Proposal

-------------------------------------------------------------------------------
*/
CLoggerSetting* CLoggerSetting::NewL( CSettingServer* aServer )
    {
    CLoggerSetting* self=new( ELeave ) CLoggerSetting();
    CleanupStack::PushL( self );
    self->ConstructL( aServer );
    CleanupStack::Pop();
    return self;

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: CLoggerSetting

    Description: Constructor. Initialise base class.

    Parameters: RThread& aClient: in: Handle to client

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
CLoggerSetting::CLoggerSetting() :
        CSession2()
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: ~CLoggerSetting

    Description: Destructor.

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
CLoggerSetting::~CLoggerSetting()
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: ConstructL

    Description: Second level constructor.
    
    Parameters: CSettingServer* aServer: in: Server

    Return Values: None

    Errors/Exceptions: Leaves if base class CreateL leaves

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CLoggerSetting::ConstructL( CSettingServer* aServer )
    {
    // @spe __TRACE( KInit,( _L( "CLoggerSetting::ConstructL - constructing server session" ) ) );
    iSettingServer = aServer;

    // second-phase construct base class
    //CSession2::CreateL();

//    iSettingServer = aServer;

     // Inform server class that session is opening. 
    iSettingServer->OpenSession();    

    __TRACE( KInit,( _L( "CLoggerSetting::ConstructL - constructing server session done" ) ) );

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: PanicClient

    Description: Panic clients.

    Parameters: TInt aPanic: in: Panic code

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CLoggerSetting::PanicClient( const TInt aPanic, const RMessage2& aMessage ) const
    {
    __TRACE( KError,( _L( "CLoggerSetting::PanicClient code = %d" ), aPanic ) );

    _LIT( KTxtModule,"CLoggerSetting" );

    aMessage.Panic( KTxtModule,aPanic );
    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: CloseSession

    Description: Close session

    Parameters: const RMessage&: out: Message to be completed.

    Return Values: TInt: Always KErrNone

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::CloseSession( /*const RMessage& aMessage*/ )
    {
    __TRACE( KVerbose,( _L( "CLoggerSetting::CloseSession in" ) ) );
             
    // Close session from server (and stop it if required)
    iSettingServer->CloseSession();

    // NOTE: This complete will make after DispatchMessageL in ServiceL method.
    // Complete message. This must be done after possible closing
    // of the server to make sure that server is first closed before
    // client can send new messages.
    // aMessage.Complete( KErrNone );

    __TRACE( KVerbose,( _L( "CLoggerSetting::CloseSession out" ) ) );

    return KErrNone;

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: ServiceL

    Description: Trap harness for dispatcher

    Parameters: const RMessage& aMessage: inout: Message

    Return Values: None

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
void CLoggerSetting::ServiceL( const RMessage2& aMessage )
    {
    __TRACE( KVerbose,( _L( "CLoggerSetting::ServiceL in" ) ) );

    TInt r( KErrNone );
    TBool isMessageSync( EFalse );
    
    TRAPD( ret, r = DispatchMessageL( aMessage, isMessageSync ) );

    if( ret != KErrNone )
        {
        // Complete message on leaving cases with leave code allways.
        __TRACE( KError,( _L( "CLoggerSetting::DispatchMessageL leaved" ) ) );
        aMessage.Complete( ret );
        }
    else if( isMessageSync )
        {
        // Message is syncronous, Complete message with error code 
        // originating from message handling
         __TRACE( KVerbose,( _L( "CLoggerSetting::DispatchMessageL completed with: %d" ), r ) );
        aMessage.Complete( r );
        }
    __TRACE( KVerbose,( _L( "CLoggerSetting::ServiceL out" ) ) );

    }

/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: DispatchMessageL

    Description: Dispatch message, calls corresponding function to do it.

    Parameters: const RMessage& aMessage: inout: Message to be handled
                TBool& aIsMessageSync: inout: Indication of the message type

    Return Values: TInt: Error code

    Errors/Exceptions: Leaves if operation handling function leaves

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::DispatchMessageL( const RMessage2& aMessage,
                                        TBool& aIsMessageSync )
    {
    __TRACE( KInit ,( _L( "CLoggerSetting::DispatchMessageL in" ) ) );
    switch( aMessage.Function() )
        {
        case ESettingServerCloseSession: 
            {
            __TRACE( KInit ,( _L( "Closing Setting server session" ) ) );
            aIsMessageSync = ETrue;
            return CloseSession( /*aMessage*/ );
            }
        case ELoadLoggerSettingsFromIniFile:
            {
            __TRACE( KInit ,( _L( "Read Logger setting(s) from initialization file" ) ) );
            aIsMessageSync = ETrue;
            return LoadLoggerSettingsFromIniFile( aMessage );
            }
        case ELoadLoggerSettingsFromCommandLine:
           {
           __TRACE( KInit ,( _L( "Read Logger setting(s) from command line" ) ) );
           aIsMessageSync = ETrue;
           return LoadLoggerSettingsFromCommandLine( aMessage );
           }
        case EGetLoggerSettings:
            {
            __TRACE( KInit ,( _L( "Get Logger setting(s)" ) ) );
            aIsMessageSync = ETrue;
            return GetLoggerSettings( aMessage );
            }
        case ESetLoggerOutputPath:
            {
            __TRACE( KInit ,( _L( "Set new initialization file setting" ) ) );
            aIsMessageSync = ETrue;
            return SetLoggerOutputPath( aMessage );
            }
        case EResetLoggerSettings:
            {
            __TRACE( KInit ,( _L( "Reset logger settings" ) ) );
            aIsMessageSync = ETrue;
            return ResetLoggerSettings( aMessage );
            }
         // Invalid request
        default:
            {
            PanicClient( EBadRequest, aMessage );
            return KErrNotSupported;
            }
        }

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: ReadLoggerSettingsFromIniFile

    Description: Read Logger setting from initialization file. Mainly use from
                 TestEngine side.

    Parameters: const RMessage& aMessage: inout: Message to be handled

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::LoadLoggerSettingsFromIniFile( const RMessage2& aMessage )
    {
    __TRACE( KInit, ( _L( "CLoggerSetting::ReadLoggerSettingsFromIniFile" ) ) );

    TInt overwritePreviousSettings = aMessage.Int2();
    
    if ( overwritePreviousSettings || ( iSettingServer->iDefaultSettings ) )
        {
        // Get data from message
        TFileName iniFile;
        
        // Get length from message param 1
        // Read data from client descriptor (param 0)
        TRAPD( ret, aMessage.ReadL( 0, iniFile ) );
    
        if ( ret != KErrNone )
            {
            PanicClient( EBadDescriptor, aMessage );
            return KErrBadDescriptor;
            }
    
        // Inifile name and path information
        iSettingServer->iIniFile = iniFile;    
        
        ret = ReadLoggerDefaults();
        if( ret != KErrNone )
            {
            return ret;
            }
        }
    
    // Copies logger settings to the package
    TPckg<TLoggerSettings> loggerSettingsPckg( iSettingServer->iLoggerSettings );

    // Writes a packege that includes the logger overwrite settings to aMessage
    TRAPD( err, aMessage.WriteL( 1, loggerSettingsPckg ) );

    if ( err != KErrNone )
        {
        PanicClient( EBadDescriptor, aMessage );
        }

    return KErrNone;
    }
/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: ReadLoggerSettingsFromIniFile

    Description: Read Logger setting from initialization file. Mainly use from
                 TestEngine side.

    Parameters: const RMessage& aMessage: inout: Message to be handled

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::LoadLoggerSettingsFromCommandLine( const RMessage2& aMessage  )
    {
    TBuf<256> logconfiguration;
    aMessage.ReadL( 0, logconfiguration );
    //iSettingServer->iLoggerSettings;
    logconfiguration.TrimAll();
    TInt i=0;
    TLex lex(logconfiguration);
    while ( !lex.Eos() )
    {
    TPtrC key=lex.NextToken();
    i++;
    TPtrC value=lex.NextToken();
    i++;
    if(key !=KNullDesC && value !=KNullDesC)
    {
    UpdateLoggerSettings(key,value);
        
    }
    
    }
    return KErrNone;
    }
TInt CLoggerSetting::UpdateLoggerSettings(TDesC& aKey,TDesC& aValue)
    {
    _LIT(KCreateLogDirectories,"CreateLogDirectories");
    _LIT(KEmulatorBasePath,"EmulatorBasePath");
    _LIT(KEmulatorFormat,"EmulatorFormat");
    _LIT(KEmulatorOutput,"EmulatorOutput");
    _LIT(KHardwareBasePath,"HardwareBasePath");
    _LIT(KHardwareFormat,"HardwareFormat");
    _LIT(KHardwareOutput,"HardwareOutput");
    _LIT(KFileCreationMode,"FileCreationMode");
    _LIT(KWithTimeStamp,"WithTimeStamp");
    _LIT(KWithLineBreak,"WithLineBreak");
    _LIT(KWithEventRanking,"WithEventRanking");
    _LIT(KThreadIdToLogFile,"ThreadIdToLogFile");
    _LIT(KFileUnicode,"FileUnicode");
    _LIT(KAddTestCaseTitle,"AddTestCaseTitle");
    if(aKey==KCreateLogDirectories)
    {
        if( aValue == _L( "NO" ) )
        {
        iSettingServer->iLoggerSettings.iCreateLogDirectories=EFalse;
        }
        else
        {
        iSettingServer->iLoggerSettings.iCreateLogDirectories=ETrue;
        }
        iSettingServer->iLoggerSettings.iIsDefined.iCreateLogDir = ETrue;
    }
    else if(aKey==KEmulatorBasePath)
    {
        iSettingServer->iLoggerSettings.iEmulatorPath = aValue;
        iSettingServer->iLoggerSettings.iIsDefined.iPath = ETrue;
    }
    else if(aKey==KEmulatorFormat)
    {
        if( aValue == _L( "TXT" ) )
           {
           iSettingServer->iLoggerSettings.iEmulatorFormat = CStifLogger::ETxt;
           }
        else if( aValue == _L( "HTML" ) )
           {
           iSettingServer->iLoggerSettings.iEmulatorFormat  = CStifLogger::EHtml;
           }
        else if( aValue == _L( "DATA" ) )
           {
           iSettingServer->iLoggerSettings.iEmulatorFormat  = CStifLogger::EData;
           }
        else if( aValue == _L( "XML" ) )
           {
           iSettingServer->iLoggerSettings.iEmulatorFormat  = CStifLogger::ETxt;
          
           }
       
        iSettingServer->iLoggerSettings.iIsDefined.iFormat = ETrue;
    }
    else if(aKey==KEmulatorOutput)
    {
        
        if( aValue == _L( "FILE" ) )
            {
            iSettingServer->iLoggerSettings.iEmulatorOutput = CStifLogger::EFile;
            }
        else if( aValue == _L( "RDEBUG" ) )
            {
            iSettingServer->iLoggerSettings.iEmulatorOutput = CStifLogger::ERDebug;
            }
        iSettingServer->iLoggerSettings.iIsDefined.iOutput = ETrue;
    }
    else if(aKey==KHardwareBasePath)
    {
        iSettingServer->iLoggerSettings.iHardwarePath = aValue;
        iSettingServer->iLoggerSettings.iIsDefined.iOutput = ETrue;
    }
    else if(aKey==KHardwareFormat)
    {
        
        if( aValue == _L( "TXT" ) )
           {
           iSettingServer->iLoggerSettings.iHardwareFormat = CStifLogger::ETxt;
           }
        else if( aValue == _L( "HTML" ) )
           {
           iSettingServer->iLoggerSettings.iHardwareFormat  = CStifLogger::EHtml;
           }
        else if( aValue == _L( "DATA" ) )
           {
           iSettingServer->iLoggerSettings.iHardwareFormat  = CStifLogger::EData;
           }
        else if( aValue == _L( "XML" ) )
           {
           iSettingServer->iLoggerSettings.iHardwareFormat  = CStifLogger::ETxt;
          
           }
        iSettingServer->iLoggerSettings.iIsDefined.iHwFormat = ETrue;
    }
    else if(aKey==KHardwareOutput)
    {
        if( aValue == _L( "FILE" ) )
            {
            iSettingServer->iLoggerSettings.iHardwareOutput = CStifLogger::EFile;
            }
        else if( aValue == _L( "RDEBUG" ) )
            {
            iSettingServer->iLoggerSettings.iHardwareOutput = CStifLogger::ERDebug;
            }
         iSettingServer->iLoggerSettings.iIsDefined.iHwOutput = ETrue;
    }
    else if(aKey==KFileCreationMode)
    {
        if( aValue == _L( "APPEND" ) )
            {
            iSettingServer->iLoggerSettings.iOverwrite = EFalse;
            }
        else if( aValue == _L( "OVERWRITE") )
            {
            iSettingServer->iLoggerSettings.iOverwrite = ETrue;
            }
        iSettingServer->iLoggerSettings.iIsDefined.iOverwrite = ETrue;
    }
    else if(aKey==KWithTimeStamp)
    {
    
        if( aValue == _L( "NO" ) )
           {
           iSettingServer->iLoggerSettings.iTimeStamp = EFalse;
           }
      
        else if( aValue == _L( "YES" ) )
           {
           iSettingServer->iLoggerSettings.iTimeStamp = ETrue;
           }
        iSettingServer->iLoggerSettings.iIsDefined.iTimeStamp = ETrue;
    }
    else if(aKey==KWithLineBreak)
    {
        if( aValue == _L( "NO" ) )
           {
           iSettingServer->iLoggerSettings.iLineBreak = EFalse;
           }
      
        else if( aValue == _L( "YES" ) )
           {
           iSettingServer->iLoggerSettings.iLineBreak = ETrue;
           }
        iSettingServer->iLoggerSettings.iIsDefined.iLineBreak = ETrue;
    }
    else if(aKey==KWithEventRanking)
    {
        if( aValue == _L( "NO" ) )
          {
          iSettingServer->iLoggerSettings.iEventRanking = EFalse;
          }
     
       else if( aValue == _L( "YES" ) )
          {
          iSettingServer->iLoggerSettings.iEventRanking = ETrue;
          }
       iSettingServer->iLoggerSettings.iIsDefined.iEventRanking = ETrue;
    }
    else if(aKey==KThreadIdToLogFile)
    {
        if( aValue == _L( "NO" ) )
          {
          iSettingServer->iLoggerSettings.iThreadId = EFalse;
          }
     
       else if( aValue == _L( "YES" ) )
          {
          iSettingServer->iLoggerSettings.iThreadId = ETrue;
          }
        iSettingServer->iLoggerSettings.iIsDefined.iThreadId = ETrue;
    }
    else if(aKey==KFileUnicode)
    {
        if( aValue == _L( "NO" ) )
          {
          iSettingServer->iLoggerSettings.iUnicode = EFalse;
          }
     
       else if( aValue == _L( "YES" ) )
          {
          iSettingServer->iLoggerSettings.iUnicode = ETrue;
          }
        iSettingServer->iLoggerSettings.iIsDefined.iUnicode = ETrue;
    }
    else if(aKey==KAddTestCaseTitle)
    {
       if( aValue == _L( "NO" ) )
         {
         iSettingServer->iLoggerSettings.iAddTestCaseTitle = EFalse;
         }
    
      else if( aValue == _L( "YES" ) )
         {
         iSettingServer->iLoggerSettings.iAddTestCaseTitle = ETrue;
         }
       iSettingServer->iLoggerSettings.iIsDefined.iAddTestCaseTitle = ETrue;
    }
    
    
    return KErrNone;
    }
/*
-------------------------------------------------------------------------------

    Class: CLoggerSetting

    Method: ReadLoggerDefaults

    Description: Parse Logger defaults from STIF initialization
                 file.

    Parameters: None

    Return Values: TInt: Return Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::ReadLoggerDefaults()
    {
    __TRACE( KInit,( _L( "" ) ) );
    __TRACE( KInit,( _L( "CLoggerSetting::ReadLoggerDefaults" ) ) );
    __TRACE( KInit,( _L( "Start reading and parsing Logger defaults" ) ) );

    TInt ret = KErrNone;
    
    // Create parser for parsing ini file
    CStifParser* parser = NULL;
    TRAPD( r, parser = CStifParser::NewL( _L(""), iSettingServer->iIniFile ) );
    if ( r != KErrNone )
        {
        __TRACE( KError,( CStifLogger::ERed, _L( "Can't open ini-file [%S], code %d" ), &iSettingServer->iIniFile, r ) );
        return r;
        }

    TTestEngineUtils::ParseLoggerDefaults( parser, iSettingServer->iLoggerSettings, LOGGER );    
    
    delete parser;

    return ret;
    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: GetLoggerSettings

    Description: Get Logger settings. Mainly use from Logger side.

    If there are not any active sessions, then stop active scheduler and
    close whole server.

    Parameters: None

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::GetLoggerSettings( const RMessage2& aMessage )
    {
    __TRACE( KInit, ( _L( "CLoggerSetting::GetLoggerSettings in" ) ) );
    
    // Copies logger settings to the package
    TPckg<TLoggerSettings> loggerSettingsPckg( iSettingServer->iLoggerSettings );

    // Writes a packege that includes the logger overwrite settings to aMessage
    TRAPD( err, aMessage.WriteL( 0, loggerSettingsPckg ) );

    if ( err != KErrNone )
        {
        PanicClient( EBadDescriptor, aMessage );
        }

    __TRACE( KInit, ( _L( "CLoggerSetting::GetLoggerSettings out" ) ) );

    return KErrNone;

    }

/*
-------------------------------------------------------------------------------

    Class: CSettingServer

    Method: SetNewIniFileSetting

    Description: Set new initialization file settings to WINS and HW platforms.

    Parameters: None

    Return Values: TInt: Symbian error code

    Errors/Exceptions: None

    Status: Proposal

-------------------------------------------------------------------------------
*/
TInt CLoggerSetting::SetLoggerOutputPath( const RMessage2& aMessage )
    {
    __TRACE( KInit, ( _L( "CLoggerSetting::SetNewIniFileSetting" ) ) );
    
    TInt ret( 0 );

    TName loggerOutputPath;
    // Get data from message
    // NOTE! If message length is over TName, ReadL will cut the message
    // to allowed size and won't return any error code or leave code.
    TRAP( ret, aMessage.ReadL( 0, loggerOutputPath ) );

    if ( ret != KErrNone )
        {
        PanicClient( EBadDescriptor, aMessage );
        return KErrBadDescriptor;
        }

    iSettingServer->iLoggerSettings.iEmulatorPath = loggerOutputPath;
    iSettingServer->iLoggerSettings.iIsDefined.iPath = ETrue;
    iSettingServer->iLoggerSettings.iHardwarePath = loggerOutputPath;
    iSettingServer->iLoggerSettings.iIsDefined.iHwPath = ETrue;

    __TRACE( KInit, ( _L( "New emulator and hardware path: [%S]" ), &loggerOutputPath ) );

    return KErrNone;
    }

TInt CLoggerSetting::ResetLoggerSettings( const RMessage2& aMessage )
    {
    __TRACE( KInit, ( _L( "CLoggerSetting::ResetLoggerSettings" ) ) );

    iSettingServer->ResetLoggerSettings();
    return KErrNone;
    }

//  End of File
