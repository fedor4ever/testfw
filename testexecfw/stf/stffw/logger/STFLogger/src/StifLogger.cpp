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
 * Description: This module contains implementation of CStifLogger 
 * class member functions.
 *
 */

// INCLUDE FILES
#include <e32std.h>
#include "StifLogger.h"
#include "STFLogger.h"
#include "STFLoggerOverFlow.h"
#include "SettingServerClient.h"

CStifLogger::CStifLogger() :
    iLogger(NULL)
    {
    }

void CStifLogger::ConstructL(const TDesC& aTestPath, const TDesC& aTestFile,
        TLoggerType aLoggerType, TOutput aOutput, TBool aOverWrite,
        TBool aWithTimeStamp, TBool aWithLineBreak, TBool aWithEventRanking,
        TBool aThreadIdToLogFile, TBool aCreateLogDir,
        TInt aStaticBufferSize, TBool aUnicode)
    {
    TInt ret;
    if(!iLogger)
        {
        iLogger = new (ELeave) RSTFLogger();
        ret = iLogger->Connect();
        if (ret)
            {
            User::Leave(ret);
            }
        }
    ret = iLogger->CreateL(aTestPath, aTestFile, aLoggerType, aOutput,
            aOverWrite, aWithTimeStamp, aWithLineBreak, aWithEventRanking,
            aThreadIdToLogFile, aCreateLogDir, aStaticBufferSize, aUnicode);
    if (ret)
        {
        User::Leave(ret);
        }
    }

void CStifLogger::ConstructL(const TDesC& aTestPath, const TDesC& aTestFile,
        TLoggerSettings& aLoggerSettings)
    {

    TInt ret;
    if(!iLogger)
        {
        iLogger = new (ELeave) RSTFLogger();
        ret = iLogger->Connect();
        if (ret)
            {
            User::Leave(ret);
            }
        }
    ret = iLogger->CreateL(aTestPath, aTestFile, aLoggerSettings);
    if (ret)
        {
        User::Leave(ret);
        }
    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: NewL

 Description: Two-phased constructor.

 Parameters: const TDesC& aTestPath: in: Path to logged information
 const TDesC& aTestFile: in: Log name for information
 TLoggerType aLoggerType: in: Log file type(txt, html,
 data)
 TOutput aOutput: in: Output source(File)
 TBool aOverWrite: in: Indicator to file overwrite
 TBool aWithTimeStamp: in: Indicator to time stamp
 TBool aWithLineBreak: in: Indicator to line break
 TBool aWithEventRanking: in: Indicator to event ranking
 TBool aThreadIdToLogFile: in: Indicator to thread id adding to
 end of the log file
 TBool aCreateLogDir: in: Indicator to directory creation
 TInt  aStaticBufferSize
 TBool aUnicode: in: Indicator if file has to be in unicode format

 Return Values: CStifLogger* logger: pointer to CStifLogger object

 Errors/Exceptions: Leaves if aTestPath or aTestFile length is over KMaxName
 Leaves if called serv.Connect() method fails
 Leaves if called CHtmlLogger::NewL method fails
 Leaves if called CDataLogger::NewL method fails
 Leaves if called CTxtLogger::NewL method fails

 Status: Proposal

 -------------------------------------------------------------------------------
 */
EXPORT_C CStifLogger* CStifLogger::NewL(const TDesC& aTestPath,
        const TDesC& aTestFile, TLoggerType aLoggerType, TOutput aOutput,
        TBool aOverWrite, TBool aWithTimeStamp, TBool aWithLineBreak,
        TBool aWithEventRanking, TBool aThreadIdToLogFile,
        TBool aCreateLogDir, TInt aStaticBufferSize, TBool aUnicode)
    {
    if( KMaxName < aTestPath.Length() || KMaxName < aTestFile.Length() )
        {
        User::Leave( KErrArgument );
        }
    
    CStifLogger* self = new (ELeave) CStifLogger();
    CleanupStack::PushL(self);

    self->ConstructL(aTestPath, aTestFile, aLoggerType, aOutput, aOverWrite,
            aWithTimeStamp, aWithLineBreak, aWithEventRanking,
            aThreadIdToLogFile, aCreateLogDir, aStaticBufferSize, aUnicode);
    CleanupStack::Pop(self);
    return self;
    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: NewL

 Description: Two-phased constructor.

 TestEngine's and TestServer's are friend. For TestEngine's and
 TestServer's StifLogger creation.

 Parameters: const TDesC& aTestPath: in: Path to logged information
 const TDesC& aTestFile: in: Log name for information
 TLoggerSettings& aLoggerSettings: in: Struct for StifLogger
 settigs

 Return Values: CStifLogger* logger: pointer to CStifLogger object

 Errors/Exceptions:  Leaves if called CHtmlLogger::NewL method fails
 Leaves if called CDataLogger::NewL method fails
 Leaves if called CTxtLogger::NewL method fails

 Status: Proposal

 -------------------------------------------------------------------------------
 */
EXPORT_C CStifLogger* CStifLogger::NewL(const TDesC& aTestPath,
        const TDesC& aTestFile, TLoggerSettings& aLoggerSettings)
    {
    CStifLogger* self = new (ELeave) CStifLogger();
    CleanupStack::PushL(self);
    self->ConstructL(aTestPath, aTestFile, aLoggerSettings);
    CleanupStack::Pop(self);
    return self;

    }

CStifLogger::~CStifLogger()
    {
    if (iLogger)
        {
        iLogger->Close();
        delete iLogger;
        }
    }

void CStifLogger::OverwriteLoggerSettings(
        TLoggerSettings& /*aLoggerSettings*/, TName& /*aTestPath*/,
        TName& /*aTestFile*/, TLoggerType& /*aLoggerType*/,
        TOutput& /*aOutput*/, TBool& /*aOverWrite*/,
        TBool& /*aWithTimeStamp*/, TBool& /*aWithLineBreak*/,
        TBool& /*aWithEventRanking*/, TBool& /*aThreadIdToLogFile*/,
        TBool& /*aCreateLogDir*/, TBool& /*aUnicode*/)
    {
    //Keep nothing to do.
    }

EXPORT_C TInt CStifLogger::Log(const TDesC& aLogInfo)
    {
    // No text style info
    return iLogger->Log(ENoStyle, aLogInfo);

    }

EXPORT_C TInt CStifLogger::Log(const TDesC8& aLogInfo)
    {
    // No text style info
    return iLogger->Log(ENoStyle, aLogInfo);

    }

EXPORT_C TInt CStifLogger::Log(TInt aStyle, const TDesC& aLogInfo)
    {
    return iLogger->Log(aStyle, aLogInfo);

    }

EXPORT_C TInt CStifLogger::Log(TInt aStyle, const TDesC8& aLogInfo)
    {
    return iLogger->Log(aStyle, aLogInfo);

    }

EXPORT_C TInt CStifLogger::Log(TRefByValue<const TDesC> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo logInfo;

    TDesSTFLoggerOverflowHandler overFlowHandler(iLogger, 1);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    // No text style info
    return iLogger->Log(ENoStyle, logInfo);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: Log

 Description: Log a 8 bit information.

 This log method accepts several parameters.

 Parameters: TRefByValue<const TDesC8> aLogInfo: in: A templated class which 
 encapsulates a reference to an object within a wrapper

 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  TDes8LoggerOverflowHandler called if logged information is 
 over KMaxLogData

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::Log(TRefByValue<const TDesC8> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo8 logInfo;
    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDes8STFLoggerOverflowHandler overFlowHandler(iLogger, 1);
    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);
    // No text style info
    return iLogger->Log(ENoStyle, logInfo);
    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: Log

 Description: Log a 16 bit information.

 This log method accepts several parameters. There is also parameter to
 styling text information e.g. text color.

 Parameters: TInt aStyle: in: Logged text forming parameter
 TRefByValue<const TDesC> aLogInfo: in: A templated class 
 which encapsulates a reference to an object
 within a wrapper

 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  TDesOverflowHandler called if logged information is
 over KMaxLogData

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::Log(TInt aStyle,
        TRefByValue<const TDesC> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDesSTFLoggerOverflowHandler overFlowHandler(iLogger, 2);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    return iLogger->Log(aStyle, logInfo);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: Log

 Description: Log a 8 bit information.

 This log method accepts several parameters. There is also parameter to
 styling text information e.g. text color.

 Parameters: TInt aStyle: in: Logged text forming parameter
 TRefByValue<const TDesC8> aLogInfo: in: A templated class 
 which encapsulates a reference to an object
 within a wrapper

 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  TDes8LoggerOverflowHandler called if logged information is
 over KMaxLogData

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::Log(TInt aStyle,
        TRefByValue<const TDesC8> aLogInfo, ...)
    {
    VA_LIST list;
    VA_START( list, aLogInfo );
    TLogInfo8 logInfo;

    // Create overflow handler. If the log information size is over the
    // KMaxLogData rest of the information will cut.
    TDes8STFLoggerOverflowHandler overFlowHandler(iLogger, 2);

    // Parse parameters
    logInfo.AppendFormatList(aLogInfo, list, &overFlowHandler);

    return iLogger->Log(aStyle, logInfo);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: WriteDelimiter

 Description: Log a 16 bit delimiter.

 Log a delimiters required locations to the log information.
 This will be used if parameters are not given when calling this method.

 Parameters: const TDesC& aDelimiter: in: Logged delimiter(e.g. '#' or 'XO')
 TInt aCount: in: Repeated count for delimiter

 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  TDesLoggerOverflowHandler called if logged information
 is over KMaxLogData.

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::WriteDelimiter(const TDesC& aDelimiter,
        TInt aCount)
    {
    TLogInfo delimiter;

    // Create overflow handler. If the delimiter size expands over the
    // KMaxLogData the TDesLoggerOverflowHandler will call.
    TDesSTFLoggerOverflowHandler overFlowHandler(iLogger, 3);

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
    return iLogger->Log(ENoStyle, delimiter);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: WriteDelimiter

 Description: Log a 8 bit delimiter.

 Log a delimiters required locations to the log information.

 Parameters: const TDesC8& aDelimiter: in: Logged delimiter
 (e.g. '#' or 'XO')
 TInt aCount: in: Repeated count for delimiter

 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  TDes8LoggerOverflowHandler called if logged information is
 over KMaxLogData.

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::WriteDelimiter(const TDesC8& aDelimiter,
        TInt aCount)
    {
    TLogInfo8 delimiter;

    // Create overflow handler. If the delimiter size expands over the
    // KMaxLogData the TDesLoggerOverflowHandler will call.
    TDes8STFLoggerOverflowHandler overFlowHandler(iLogger, 3);

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
    return iLogger->Log(ENoStyle, delimiter);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: SaveData

 Description: Save file or data( 16 bit ).

 Used when is need to save file or data to storage e.g. web page.

 Parameters: TDesC& aData: in: Data to be saved
 
 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  None

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::SaveData(TDesC& aData)
    {
    // No text style info
    return iLogger->Log(ENoStyle, aData);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: SaveData

 Description: Save file or data( 8 bit ).

 Used when is need to save file or data to storage e.g. web page.

 Parameters: TDesC8& aData: in: Data to be saved
 
 Return Values: TInt: Symbian error code.

 Errors/Exceptions:  None

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::SaveData(TDesC8& aData)
    {
    // No text style info
    return iLogger->Log(ENoStyle, aData);

    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: CreationResult

 Description: Return StifLogger creation result.

 Parameters: None
 
 Return Values: StifLogger creation result

 Errors/Exceptions:  None

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C TInt CStifLogger::CreationResult()
    {
    TInt outputType;
    iLogger->CreationResult(outputType);
    return outputType;
    }

/*
 -------------------------------------------------------------------------------

 Class: CStifLogger

 Method: OutputType

 Description: Get output type. Valid only if CreationResult returns KErrNone.

 Parameters: TOutput& aOutput
 
 Return Values: StifLogger creation result

 Errors/Exceptions:  None

 Status: Approved

 -------------------------------------------------------------------------------
 */
EXPORT_C CStifLogger::TOutput CStifLogger::OutputType()
    {

    TOutput outputType;
    iLogger->OutputType(outputType);
    return outputType;

    }

EXPORT_C TInt CStifLogger::Send(TInt aStyle, const TDesC& aData)
    {
    return iLogger->Log(aStyle, aData);
    }

EXPORT_C TInt CStifLogger::Send(TInt aStyle, const TDesC8& aData)
    {
    return iLogger->Log(aStyle, aData);
    }

// End of File

