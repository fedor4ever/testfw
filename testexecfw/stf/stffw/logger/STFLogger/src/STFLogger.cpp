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
 *
 */

//  Include Files  

#include "STFLogger.h"	// RSTFLogger
#include "STFLoggerCSCommon.h"
#include "STFLoggerOverFlow.h"
#include <hal.h> 
#include "SettingServerClient.h"

// Runs client-side and starts the separate server process
static TInt StartTheServer()
    {

    //Ccheck server not already started
    TFindServer findServer(KSTFLoggerServerBinaryName);
    TFullName name;
    if (findServer.Next(name) == KErrNone)
        {
        return KErrAlreadyExists;
        }
    RProcess server;
    TInt r = server.Create(KSTFLoggerServerBinaryName, KNullDesC);
    if (r != KErrNone)
        return r;

    TRequestStatus stat;
    server.Rendezvous(stat);
    if (stat != KRequestPending)
        server.Kill(0); // abort startup
    else
        server.Resume(); // logon OK - start the server

    User::WaitForRequest(stat); // wait for start or death

    // Check the exit type.
    // We can't use the 'exit reason' because if the server panicked this
    // is set to the panic 'reason' (which may be '0' and cannot thus be distinguished
    // from KErrNone)
    r = server.ExitType();
    if (EExitPanic == r)
        r = KErrGeneral;
    else
        r = stat.Int();

    server.Close(); // This is no longer needed
    return r;
    }

//  Member Functions
EXPORT_C TInt RSTFLogger::Connect()
    {
    TInt retry = 2;
    for (;;)
        {// Uses system-pool message slots
        TInt r = CreateSession(KSTFLoggerServerName, TVersion(1, 0, 0));
        if ((KErrNotFound != r) && (KErrServerTerminated != r))
            return (r);
        if (--retry == 0)
            return (r);
        r = StartTheServer();
        if ((KErrNone != r) && (KErrAlreadyExists != r))
            return (r);
        }
    }

EXPORT_C TInt RSTFLogger::CreateL(const TDesC& aTestPath,
        const TDesC& aTestFile, CStifLogger::TLoggerType aLoggerType,
        CStifLogger::TOutput aOutput, TBool aOverWrite, TBool aWithTimeStamp,
        TBool aWithLineBreak, TBool aWithEventRanking,
        TBool aThreadIdToLogFile, TBool aCreateLogDir,
        TInt aStaticBufferSize, TBool aUnicode)
    {
    if( KMaxName < aTestPath.Length() || KMaxName < aTestFile.Length() )
        {
        User::Leave( KErrArgument );
        }
    
    // Handle to Setting server.
    RSettingServer settingServer;
    // Connect to the Setting server and create session
    TInt ret = settingServer.Connect();
    if (ret != KErrNone)
        {
        settingServer.Close();
        return ret;
        }
    // Struct to StifLoggerBase settigs.
    TLoggerSettings loggerSettings;
    // Parse StifLoggerBase defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    settingServer.Close();
    if (ret != KErrNone)
        {
        return ret;
        }

    TName testPath = aTestPath;
    TName testFile = aTestFile;

    // Take in use the overwrite parameters
    OverwriteLoggerSettings(loggerSettings, testPath, testFile, aLoggerType,
            aOutput, aOverWrite, aWithTimeStamp, aWithLineBreak,
            aWithEventRanking, aThreadIdToLogFile, aCreateLogDir, aUnicode);
    
    RThread threadHandle;                   // A handle to a thread
    TThreadId id = threadHandle.Id();       // Encapsulates the Id of a thread

    TSTFLoggerCreateData loggerData(testPath, testFile, aLoggerType, aOutput,
            aOverWrite, aWithTimeStamp, aWithLineBreak, aWithEventRanking,
            aThreadIdToLogFile, aCreateLogDir, aStaticBufferSize, aUnicode, (TInt)id);
    TPckg<TSTFLoggerCreateData> data(loggerData);
    TIpcArgs args(&data);
    return (SendReceive(ESTFLogger_CreateLogger, args));
    }



EXPORT_C TInt RSTFLogger::CreateL(const TDesC& aTestPath,
        const TDesC& aTestFile, TLoggerSettings& aLoggerSettings)
    {
    TSTFLoggerCreateData loggerData;
    loggerData.iTestPath = aTestPath;
    loggerData.iTestFile = aTestFile;
    loggerData.iOverWrite = aLoggerSettings.iOverwrite;
    loggerData.iWithTimeStamp = aLoggerSettings.iTimeStamp;
    loggerData.iWithLineBreak = aLoggerSettings.iLineBreak;
    loggerData.iWithEventRanking = aLoggerSettings.iEventRanking;
    loggerData.iThreadIdToLogFile = aLoggerSettings.iThreadId;
    loggerData.iCreateLogDir = aLoggerSettings.iCreateLogDirectories;
    loggerData.iStaticBufferSize = 0;
    loggerData.iUnicode = aLoggerSettings.iUnicode;
    TInt cpu(0);
    HAL::Get(HALData::ECPU, cpu);

    // HW environment
    if (cpu == HALData::ECPU_ARM)
        {
        loggerData.iOutput = aLoggerSettings.iHardwareOutput;
        loggerData.iLoggerType = aLoggerSettings.iHardwareFormat;
        } // End of HW environment branch

    // Wins environment ( cpu == HALData::ECPU_X86 )
    else
        {
        loggerData.iOutput = aLoggerSettings.iEmulatorOutput;
        loggerData.iLoggerType = aLoggerSettings.iEmulatorFormat;
        } // End of WINS environment branch
    
    RThread threadHandle;                   // A handle to a thread
    TThreadId id = threadHandle.Id();       // Encapsulates the Id of a thread
    
    loggerData.iThreadId = (TInt)id;

    TPckg<TSTFLoggerCreateData> data(loggerData);
    TIpcArgs args(&data);
    return (SendReceive(ESTFLogger_CreateLogger, args));
    }

EXPORT_C TInt RSTFLogger::Log(const TDesC& aLogInfo)
    {
    return (Log(CStifLogger::ENoStyle, aLogInfo));
    }

EXPORT_C TInt RSTFLogger::Log(const TDesC8& aLogInfo)
    {
    return (Log(CStifLogger::ENoStyle, aLogInfo));
    }

EXPORT_C TInt RSTFLogger::Log(TInt aStyle, const TDesC& aLogInfo)
    {
    TIpcArgs args(aLogInfo.Length(), aStyle, &aLogInfo);
    return (SendReceive(ESTFLogger_Log_TInt_TDesC, args));
    }

EXPORT_C TInt RSTFLogger::Log(TInt aStyle, const TDesC8& aLogInfo)
    {
    TIpcArgs args(aLogInfo.Length(), aStyle, &aLogInfo);
    return (SendReceive(ESTFLogger_Log_TInt_TDesC8, args));
    }

EXPORT_C TInt RSTFLogger::Log(TRefByValue<const TDesC> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDesSTFLoggerOverflowHandler overFlowHandler(this, 1);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    // No text style info

    return Log(CStifLogger::ENoStyle, logInfo);
    }

EXPORT_C TInt RSTFLogger::Log(TRefByValue<const TDesC8> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo8 logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDes8STFLoggerOverflowHandler overFlowHandler(this, 1);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    // No text style info
    return Log(CStifLogger::ENoStyle, logInfo);
    }

EXPORT_C TInt RSTFLogger::Log(TInt aStyle, TRefByValue<const TDesC> aLogInfo,
        ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDesSTFLoggerOverflowHandler overFlowHandler(this, 2);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    return Log(aStyle, logInfo);
    }

EXPORT_C TInt RSTFLogger::Log(TInt aStyle,
        TRefByValue<const TDesC8> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo8 logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDes8STFLoggerOverflowHandler overFlowHandler(this, 2);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    return Log(aStyle, logInfo);
    }

EXPORT_C TInt RSTFLogger::WriteDelimiter(const TDesC& aDelimiter, TInt aCount)
    {
    TLogInfo delimiter;

    // Create overflow handler. If the delimiter size expands over the
    // KMaxLogData the TDesLoggerOverflowHandler will call.
    TDesSTFLoggerOverflowHandler overFlowHandler(this, 3);

    // Create a delimiter
    for (TInt a = 0; a < aCount; a++)
        {
        // If delimiter creation keeps under the KMaxLogData.
        // If not we use TDesLoggerOverflowHandler.
        if ((a * aDelimiter.Length()) < KMaxLogData)
            {
            delimiter.Append(aDelimiter);
            }
        // KMaxLogData is exceeded
        else
            {
            // If the title size is over the KMaxLogData default delimiter will
            // use. Use normal overflowhandler to print overflow information.
            TBuf<4> empty; // Not really used.
            overFlowHandler.Overflow(empty);
            delimiter.Copy(
                    _L( "##################################################" ));
            break;
            }
        }

    // No text style info
    return Log(CStifLogger::ENoStyle, delimiter);
    }

EXPORT_C TInt RSTFLogger::WriteDelimiter(const TDesC8& aDelimiter,
        TInt aCount)
    {
    TLogInfo8 delimiter;

    // Create overflow handler. If the delimiter size expands over the
    // KMaxLogData the TDesLoggerOverflowHandler will call.
    TDes8STFLoggerOverflowHandler overFlowHandler(this, 3);

    // Create a delimiter
    for (TInt a = 0; a < aCount; a++)
        {
        // If delimiter creation keeps under the KMaxLogData.
        // If not we use TDesLoggerOverflowHandler.
        if ((a * aDelimiter.Length()) < KMaxLogData)
            {
            delimiter.Append(aDelimiter);
            }
        // KMaxLogData is exceeded
        else
            {
            // If the title size is over the KMaxLogData default delimiter will
            // use. Use normal overflowhandler to print overflow information.
            TBuf8<4> empty; // Not really used.
            overFlowHandler.Overflow(empty);
            delimiter.Copy(
                    _L8( "##################################################" ));
            break;
            }
        }

    // No text style info
    return Log(CStifLogger::ENoStyle, delimiter);
    }

EXPORT_C TInt RSTFLogger::SaveData(const TDesC& aData)
    {
    return Log(CStifLogger::ENoStyle, aData);
    }

EXPORT_C TInt RSTFLogger::SaveData(const TDesC8& aData)
    {
    return Log(CStifLogger::ENoStyle, aData);
    }

EXPORT_C TInt RSTFLogger::CreationResult(TInt& aResult)
    {
    TPckg<TInt> data(aResult);
    TIpcArgs args(&data);
    return (SendReceive(ESTFLogger_CreationResult, args));
    }

EXPORT_C TInt RSTFLogger::OutputType(CStifLogger::TOutput& aOutputType)
    {
    TPckg<CStifLogger::TOutput> data(aOutputType);
    TIpcArgs args(&data);
    return (SendReceive(ESTFLogger_OutputType, args));
    }

void RSTFLogger::OverwriteLoggerSettings(TLoggerSettings& aLoggerSettings,
        TName& aTestPath, TName& aTestFile,
        CStifLogger::TLoggerType& aLoggerType, CStifLogger::TOutput& aOutput,
        TBool& aOverWrite, TBool& aWithTimeStamp, TBool& aWithLineBreak,
        TBool& aWithEventRanking, TBool& aThreadIdToLogFile,
        TBool& aCreateLogDir, TBool& aUnicode)
    {
    // Which environment is in use
    TInt cpu(0);

    // Indicator is file type remove acceptable operation
    TBool removeFileType(EFalse);

    // Get environment
    HAL::Get(HALData::ECPU, cpu);

    // HW environment
    if (cpu == HALData::ECPU_ARM)
        {
        TInt pathLen = aTestPath.Length()
                + aLoggerSettings.iHardwarePath.Length();

        // Check that path overwrite setting is defined and length is legal
        if (aLoggerSettings.iIsDefined.iHwPath && pathLen < KMaxName)
            {
            TName newPath;
            newPath = aLoggerSettings.iHardwarePath;
            // Check is '\' the last character
            TInt ret(0);
            ret = newPath.LocateReverse(92);
            // Is '\' character founded
            if (ret != KErrNotFound)
                {
                // Is '\' last character
                if (ret == (newPath.Length() - 1))
                    {
                    // delete last '\'
                    newPath.Delete((aLoggerSettings.iHardwarePath.Length()
                            - 1), 1);
                    }
                }
            // Removes drive letter if given and appends implemented path 
            TParse parse;
            parse.Set(aTestPath, NULL, NULL);
            // Path() return value starts with '\'
            newPath.Append(parse.Path());
            aTestPath = newPath;
            }
        if (aLoggerSettings.iIsDefined.iHwFormat)
            {
            aLoggerType = aLoggerSettings.iHardwareFormat;
            // Remove file type if it is set
            removeFileType = ETrue;
            }
        if (aLoggerSettings.iIsDefined.iHwOutput)
            {
            aOutput = aLoggerSettings.iHardwareOutput;
            }
        }

    // Wins environment ( cpu == HALData::ECPU_X86 )
    else
        {
        TInt pathLen = aTestPath.Length()
                + aLoggerSettings.iEmulatorPath.Length();

        // Check that path overwrite setting is defined and length is legal
        if (aLoggerSettings.iIsDefined.iPath && pathLen < KMaxName)
            {
            TName newPath;
            newPath = aLoggerSettings.iEmulatorPath;
            // Check is '\' the last character
            TInt ret(0);
            ret = newPath.LocateReverse(92);
            // Is '\' character founded
            if (ret != KErrNotFound)
                {
                // Is '\' last character
                if (ret == (newPath.Length() - 1))
                    {
                    // delete last '\'
                    newPath.Delete((aLoggerSettings.iEmulatorPath.Length()
                            - 1), 1);
                    }
                }
            // Removes drive letter if given and appends implemented path
            TParse parse;
            parse.Set(aTestPath, NULL, NULL);
            // Path() return value starts with '\'
            newPath.Append(parse.Path());
            aTestPath = newPath;
            }
        if (aLoggerSettings.iIsDefined.iFormat)
            {
            aLoggerType = aLoggerSettings.iEmulatorFormat;
            // Remove file type if it is set
            removeFileType = ETrue;
            }
        if (aLoggerSettings.iIsDefined.iOutput)
            {
            aOutput = aLoggerSettings.iEmulatorOutput;
            }
        }

    // Rest of the overwrite values
    if (aLoggerSettings.iIsDefined.iOverwrite)
        {
        aOverWrite = aLoggerSettings.iOverwrite;
        }
    if (aLoggerSettings.iIsDefined.iTimeStamp)
        {
        aWithTimeStamp = aLoggerSettings.iTimeStamp;
        }
    if (aLoggerSettings.iIsDefined.iLineBreak)
        {
        aWithLineBreak = aLoggerSettings.iLineBreak;
        }
    if (aLoggerSettings.iIsDefined.iEventRanking)
        {
        aWithEventRanking = aLoggerSettings.iEventRanking;
        }
    if (aLoggerSettings.iIsDefined.iThreadId)
        {
        aThreadIdToLogFile = aLoggerSettings.iThreadId;
        }
    if (aLoggerSettings.iIsDefined.iCreateLogDir)
        {
        aCreateLogDir = aLoggerSettings.iCreateLogDirectories;
        }
    if (aLoggerSettings.iIsDefined.iUnicode)
        {
        aUnicode = aLoggerSettings.iUnicode;
        }

    // Remove file type if allowed and if file type is set to filename
    if (removeFileType)
        {
        // Remove file type '.XXX' from file name if exist
        if (aTestFile.Find(_L( "." )))
            {
            TParse parse;
            parse.Set(aTestFile, NULL, NULL);
            // Type length
            TInt length(0);
            // '.XXX'
            length = parse.Ext().Length();
            // Delete type
            aTestFile.Delete(aTestFile.Length() - length, length);
            }
        }

    }
