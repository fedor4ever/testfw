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
* Description: This module contains implementation of CStifLoggerBase 
* class member functions.
*
*/

// INCLUDE FILES
#include <e32std.h>
#include "StifLoggerBase.h"
#include "TxtLogger.h"
#include "HtmlLogger.h"
#include "DataLogger.h"
#include "LoggerTracing.h"
#include "LoggerOverFlow.h"

/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: CStifLoggerBase

    Description: Default constructor

    C++ default constructor can NOT contain any code, that
    might leave.

    Parameters: None
    
    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
CStifLoggerBase::CStifLoggerBase()
    {

    }

/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

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

    Return Values: CStifLoggerBase* logger: pointer to CStifLoggerBase object

    Errors/Exceptions: Leaves if aTestPath or aTestFile length is over KMaxName
                       Leaves if called serv.Connect() method fails
                       Leaves if called CHtmlLogger::NewL method fails
                       Leaves if called CDataLogger::NewL method fails
                       Leaves if called CTxtLogger::NewL method fails

    Status: Proposal

-------------------------------------------------------------------------------
*/
EXPORT_C CStifLoggerBase* CStifLoggerBase::NewL( const TDesC& aTestPath,
                                            const TDesC& aTestFile,
                                            CStifLogger::TLoggerType aLoggerType,
                                            CStifLogger::TOutput aOutput,
                                            TBool aOverWrite,
                                            TBool aWithTimeStamp,
                                            TBool aWithLineBreak,
                                            TBool aWithEventRanking,
                                            TBool aThreadIdToLogFile,
                                            TBool aCreateLogDir,
                                            TInt  aStaticBufferSize,
                                            TBool aUnicode,
                                            TInt aThreadId)
    {
    if( KMaxName < aTestPath.Length() || KMaxName < aTestFile.Length() )
        {
        User::Leave( KErrArgument );
        }



    // Html file logging
    if ( aLoggerType == CStifLogger::EHtml )
        {
        // Create CHtmlLogger object logger
        CHtmlLogger* logger = CHtmlLogger::NewL( aTestPath,
                                                    aTestFile,
                                                    aLoggerType,
                                                    aOutput,
                                                    aOverWrite,
                                                    aWithTimeStamp,
                                                    aWithLineBreak,
                                                    aWithEventRanking,
                                                    aThreadIdToLogFile,
                                                    aCreateLogDir,
                                                    aStaticBufferSize,
                                                    aUnicode,
                                                    aThreadId);
        return (CStifLoggerBase*)logger;
        }
    // Data file
    else if ( aLoggerType == CStifLogger::EData )
        {
        // Create CDataLogger object logger
        CDataLogger* logger = CDataLogger::NewL( aTestPath,
                                                    aTestFile,
                                                    aLoggerType,
                                                    aOutput,
                                                    aOverWrite,
                                                    aWithTimeStamp,
                                                    aWithLineBreak,
                                                    aWithEventRanking,
                                                    aThreadIdToLogFile,
                                                    aCreateLogDir,
                                                    aStaticBufferSize,
                                                    aUnicode,
                                                    aThreadId);
        return (CStifLoggerBase*)logger;
        }
    // For default: text file logging
    else
        {
        // Create CTxtLogger object logger
        CTxtLogger* logger = CTxtLogger::NewL( aTestPath,
                                                aTestFile,
                                                aLoggerType,
                                                aOutput,
                                                aOverWrite,
                                                aWithTimeStamp,
                                                aWithLineBreak,
                                                aWithEventRanking,
                                                aThreadIdToLogFile,
                                                aCreateLogDir,
                                                aStaticBufferSize,
                                                aUnicode,
                                                aThreadId);
        return (CStifLoggerBase*)logger;
        }

    }

/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: ~CStifLoggerBase

    Description: Destructor

    Parameters: None

    Return Values: None

    Errors/Exceptions: None

    Status: Approved

-------------------------------------------------------------------------------
*/
CStifLoggerBase::~CStifLoggerBase()
    {

    if(iOutput)
        {
        delete iOutput;
        iOutput = 0;
        }
    
    }


/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: Log

    Description: Log a 16 bit information.

    There is also parameter to styling text information e.g. text color.

    Parameters: TInt aStyle: in: Logged text forming parameter
                const TDesC aLogInfo: in: 16 bit data to be logged

    Return Values: TInt: Symbian error code.

    Errors/Exceptions:  None

    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C TInt CStifLoggerBase::Log( TInt aStyle, const TDesC& aLogInfo )
    {
    return Send( aStyle, aLogInfo );

    }

/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: Log

    Description: Log a 8 bit information.

    There is also parameter to styling text information e.g. text color.

    Parameters: TInt aStyle: in: Logged text forming parameter
                const TDesC8 aLogInfo: in: 8 bit data to be logged

    Return Values: TInt: Symbian error code.

    Errors/Exceptions:  None

    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C TInt CStifLoggerBase::Log( TInt aStyle, const TDesC8& aLogInfo )
    {
    return Send( aStyle, aLogInfo );

    }

/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: CreationResult

    Description: Return StifLoggerBase creation result.

    Parameters: None
    
    Return Values: StifLoggerBase creation result

    Errors/Exceptions:  None

    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C TInt CStifLoggerBase::CreationResult()
    {
     
    CStifLogger::TOutput outputType;
    return iOutput->OutputCreationResult( outputType );
    
    }
    
/*
-------------------------------------------------------------------------------

    Class: CStifLoggerBase

    Method: OutputType

    Description: Get output type. Valid only if CreationResult returns KErrNone.

    Parameters: TOutput& aOutput
    
    Return Values: StifLoggerBase creation result

    Errors/Exceptions:  None

    Status: Approved

-------------------------------------------------------------------------------
*/
EXPORT_C CStifLogger::TOutput CStifLoggerBase::OutputType()
    {
    
    CStifLogger::TOutput outputType;
    iOutput->OutputCreationResult( outputType );
    return outputType;
    
    }

// End of File
