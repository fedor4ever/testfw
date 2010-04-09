/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include "TestEngineUtils.h"
#include "Logging.h"
#include <StifLogger.h>
#include <StifParser.h>
#include "STIFTestFrameworkSettings.h"

#define LOGGER aLogger

TInt TTestEngineUtils::ParseLoggerDefaults( CStifParser* aParser, TLoggerSettings& aLoggerSettings, CStifLogger* aLogger )
    {
    __TRACE( KInit,( _L( "" ) ) );
    __TRACE( KInit,( _L( "CLoggerSetting::ReadLoggerDefaults" ) ) );
    __TRACE( KInit,( _L( "Start reading and parsing Logger defaults" ) ) );

    TInt ret = KErrNone;

    CSTIFTestFrameworkSettings* settings = NULL;
    TRAPD( settings_ret, settings = CSTIFTestFrameworkSettings::NewL() );
    if ( settings_ret != KErrNone )
        {
        __TRACE( KError,( CStifLogger::ERed, _L( "CSTIFTestFrameworkSettings class object creation fails") ) );
        return settings_ret;
        }

    TInt get_ret( KErrNone );

    CStifSectionParser* sectionParser = NULL;
    // Parse Logger's overwrite values
    _LIT( KDefaultsStart, "[Logger_Defaults]" );
    _LIT( KDefaultsEnd, "[End_Logger_Defaults]" );
    __TRACE( KInit,( _L( "Starting to search sections" ) ) );
    
    TRAPD( r,
        sectionParser = aParser->SectionL( KDefaultsStart, KDefaultsEnd );
        );
    // Get directory settings
    if ( ( r == KErrNone ) && sectionParser )
        {
        __TRACE( KInit,( _L( "Found '%S' and '%S' sections" ), &KDefaultsStart, &KDefaultsEnd ) );

        __TRACE( KInit,( _L( "Parsing Logger directory indicator" ) ) );
        TBool createDir( EFalse );
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                                _L( "CreateLogDirectories=" ), createDir );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Logger directory indicator: %d"), createDir ) );
            aLoggerSettings.iCreateLogDirectories = createDir;
            aLoggerSettings.iIsDefined.iCreateLogDir = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Indicator not found or not given" ) ) );
            }

        // Get Logger path settings
        __TRACE( KInit,( _L( "Parsing Logger path setting (Emulator)" ) ) );
        TPtrC emulatorPath;
        get_ret = settings->GetFileSetting( sectionParser,
                                               _L( "EmulatorBasePath=" ),
                                               emulatorPath );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Logger base path setting: %S"), &emulatorPath ) );
            aLoggerSettings.iEmulatorPath = emulatorPath;
            aLoggerSettings.iIsDefined.iPath = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Path not found or not given (Emulator)" ) ) );
            }

        // Get Logger format settings
        __TRACE( KInit,( _L( "Parsing Logger file type setting (Emulator)" ) ) );
        CStifLogger::TLoggerType emulatorType = CStifLogger::ETxt;
        TRAP( get_ret, 
            get_ret = settings->GetFormatL( sectionParser,
                                    _L( "EmulatorFormat=" ), emulatorType );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "File type setting: %d"), emulatorType ) );
            aLoggerSettings.iEmulatorFormat = emulatorType;
            aLoggerSettings.iIsDefined.iFormat = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "File type not found or not given (Emulator)" ) ) );
            }

        // Get Logger output settings
        __TRACE( KInit,( _L( "Parsing Logger output setting (Emulator)" ) ) );
        CStifLogger::TOutput emulatorOutput = CStifLogger::EFile;
        TRAP( get_ret,
            get_ret = settings->GetOutputL( sectionParser,
                                    _L( "EmulatorOutput=" ), emulatorOutput );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Output setting: %d"), emulatorOutput ) );
            aLoggerSettings.iEmulatorOutput = emulatorOutput;
            aLoggerSettings.iIsDefined.iOutput = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Output not found or not given (Emulator)" ) ) );
            }

        // Get Logger HW path settings
        __TRACE( KInit,( _L( "Parsing Logger path setting (Hardware)" ) ) );
        TPtrC hwPath;
        get_ret = settings->GetFileSetting( sectionParser,
                                               _L( "HardwareBasePath=" ),
                                               hwPath );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Base path setting: %S"), &hwPath ) );
            aLoggerSettings.iHardwarePath = hwPath;
            aLoggerSettings.iIsDefined.iHwPath = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Path not found or not given (Hardware)" ) ) );
            }

        // Get Logger HW format settings
        __TRACE( KInit,( _L( "Parsing Logger file type setting (Hardware)" ) ) );
        CStifLogger::TLoggerType hwType = CStifLogger::ETxt;
        TRAP( get_ret,
            get_ret = settings->GetFormatL( sectionParser,
                                        _L( "HardwareFormat=" ), hwType );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "File type setting: %d"), hwType ) );
            aLoggerSettings.iHardwareFormat = hwType;
            aLoggerSettings.iIsDefined.iHwFormat = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "File type not found or not given (Hardware)" ) ) );
            }

        // Get Logger HW output settings
        __TRACE( KInit,( _L( "Parsing Logger output setting (Hardware)" ) ) );
        CStifLogger::TOutput hwOutput = CStifLogger::EFile;
        TRAP( get_ret, 
        get_ret = settings->GetOutputL( sectionParser,
                                    _L( "HardwareOutput=" ), hwOutput );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Output setting: %d"), hwOutput ) );
            aLoggerSettings.iHardwareOutput = hwOutput;
            aLoggerSettings.iIsDefined.iHwOutput = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Output not found or not given (Hardware)" ) ) );
            }

        // Get Logger file creation mode (overwrite settings)
        __TRACE( KInit,( _L( "Parsing file creation setting" ) ) );
        TBool overwrite( ETrue );
        TRAP( get_ret,
            get_ret = settings->GetOverwriteL( sectionParser,
                                _L( "FileCreationMode=" ), overwrite );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "File creation mode setting: %d"), overwrite ) );
            aLoggerSettings.iOverwrite = overwrite;
            aLoggerSettings.iIsDefined.iOverwrite = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "File creation not found or not given" ) ) );
            }

        // Get Time stamp settings
        __TRACE( KInit,( _L( "Parsing time stamp setting" ) ) );
        TBool timeStamp( ETrue );
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                                _L( "WithTimeStamp=" ), timeStamp );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Time stamp setting: %d"), timeStamp ) );
            aLoggerSettings.iTimeStamp = timeStamp;
            aLoggerSettings.iIsDefined.iTimeStamp = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Time stamp not found or not given" ) ) );
            }

        // Get Test report line break settings
        __TRACE( KInit,( _L( "Parsing line break setting" ) ) );
        TBool lineBreak( ETrue );
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                                _L( "WithLineBreak=" ), lineBreak );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Line break setting: %d"), lineBreak ) );
            aLoggerSettings.iLineBreak = lineBreak;
            aLoggerSettings.iIsDefined.iLineBreak = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Line break not found or not given" ) ) );
            }

        // Get Test report event ranking settings
        __TRACE( KInit,( _L( "Parsing event ranking setting" ) ) );
        TBool eventRanking( EFalse );
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                            _L( "WithEventRanking=" ), eventRanking );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Event ranking setting: %d"), eventRanking ) );
            aLoggerSettings.iEventRanking = eventRanking;
            aLoggerSettings.iIsDefined.iEventRanking = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Event ranking not found or not given" ) ) );
            }

        // Get Test report thread id settings
        __TRACE( KInit,( _L( "Parsing thread id setting" ) ) );
        TBool threadId( EFalse );
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                                    _L( "ThreadIdToLogFile=" ), threadId );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Thread id setting: %d"), threadId ) );
            aLoggerSettings.iThreadId = threadId;
            aLoggerSettings.iIsDefined.iThreadId = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Thread id not found or not given" ) ) );
            }
        // Get unicode setting
        __TRACE( KInit,( _L( "Parsing unicode setting" ) ) );
        TBool unicode(EFalse);
        TRAP( get_ret,
            get_ret = settings->GetBooleanSettingsL( sectionParser,
                                    _L( "FileUnicode=" ), unicode );
            );
        if ( get_ret == KErrNone )
            {
            __TRACE( KInit,( _L( "Unicode setting: %d"), unicode ) );
            aLoggerSettings.iUnicode = unicode;
            aLoggerSettings.iIsDefined.iUnicode = ETrue;
            }
        else
            {
            __TRACE( KInit,( _L( "Unicode setting not found or not given" ) ) );
            }

        // Check if test case title should be added to log file name
        __TRACE( KInit,( _L( "Parsing AddTestCaseTitle setting" ) ) );
        TBool addTestCaseTitle(EFalse);
        TRAP(get_ret,
            get_ret = settings->GetBooleanSettingsL(sectionParser,
                                    _L( "AddTestCaseTitle=" ), addTestCaseTitle);
            );
        if(get_ret == KErrNone)
            {
            __TRACE( KInit,( _L( "AddTestCaseTitle setting: %d"), addTestCaseTitle));
            aLoggerSettings.iAddTestCaseTitle = addTestCaseTitle;
            aLoggerSettings.iIsDefined.iAddTestCaseTitle = ETrue;
            }
        else
            {
            __TRACE(KInit, (_L("AddTestCaseTitle setting not found or not given")));
            }
        }
    else
        {
        __TRACE( KInit,( _L( "Not found '%S' and '%S' sections" ), &KDefaultsStart, &KDefaultsEnd ) );
        ret = KErrNotFound;
        }
    __TRACE( KInit,( _L( "End reading and parsing Logger defaults" ) ) );
    __TRACE( KInit,( _L( "" ) ) );
    delete sectionParser;
    delete settings;

    return ret;
    }

// End of file
