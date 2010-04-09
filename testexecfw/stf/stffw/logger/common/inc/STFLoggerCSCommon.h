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
#ifndef STFLOGGERCSCOMMON_H__
#define STFLOGGERCSCOMMON_H__

#include <stiflogger.h>

_LIT(KSTFLoggerServerName,"STFLoggingServer");// The server's identity within the client-server framework
_LIT(KSTFLoggerServerBinaryName,"STFLoggingServer"); // The name of the server binary (dll or exe) 



enum TSTFLoggerFunction
	{
	ESTFLogger_CreateLogger,	
	ESTFLogger_Log_TInt_TDesC,
	ESTFLogger_Log_TInt_TDesC8,
	ESTFLogger_CreationResult,
	ESTFLogger_OutputType
	};


class TSTFLoggerCreateData
	{
public:
	TSTFLoggerCreateData();
	TSTFLoggerCreateData(const TDesC& aTestPath,
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
			            TInt  aThreadId);

public:
	TPath iTestPath;
	TFileName iTestFile;
    CStifLogger::TLoggerType iLoggerType;
    CStifLogger::TOutput iOutput;
    TBool iOverWrite;
    TBool iWithTimeStamp;
    TBool iWithLineBreak;
    TBool iWithEventRanking;
    TBool iThreadIdToLogFile;
    TBool iCreateLogDir;
    TInt  iStaticBufferSize;
    TBool iUnicode;
    TInt  iThreadId;
	};

inline TSTFLoggerCreateData::TSTFLoggerCreateData():
                                                    iLoggerType(CStifLogger::ETxt),
                                                    iOutput(CStifLogger::EFile),
                                                    iOverWrite(ETrue),
                                                    iWithTimeStamp(ETrue),
                                                    iWithLineBreak(ETrue),
                                                    iWithEventRanking(EFalse),
                                                    iThreadIdToLogFile(ETrue),
                                                    iCreateLogDir(EFalse),
                                                    iStaticBufferSize(0),
                                                    iUnicode(EFalse),
                                                    iThreadId(0)
	{
	};

inline TSTFLoggerCreateData::TSTFLoggerCreateData(const TDesC& aTestPath,
										        const TDesC& aTestFile,
										        CStifLogger::TLoggerType aLoggerType,
										        CStifLogger::TOutput aOutput,
										        TBool aOverWrite,
										        TBool aWithTimeStamp,
										        TBool aWithLineBreak,
										        TBool aWithEventRanking,
										        TBool aThreadIdToLogFile,
										        TBool aCreateLogDir,
										        TInt aStaticBufferSize,
										        TBool aUnicode,
										        TInt aThreadId):
                                                                  iTestPath(aTestPath),
                                                                  iTestFile (aTestFile),
                                                                  iLoggerType (aLoggerType),
                                                                  iOutput (aOutput),
                                                                  iOverWrite (aOverWrite),
                                                                  iWithTimeStamp (aWithTimeStamp),
                                                                  iWithLineBreak (aWithLineBreak),
                                                                  iWithEventRanking (aWithEventRanking),
                                                                  iThreadIdToLogFile (aThreadIdToLogFile),
                                                                  iCreateLogDir (aCreateLogDir),
                                                                  iStaticBufferSize (aStaticBufferSize),
                                                                  iUnicode (aUnicode),
                                                                  iThreadId (aThreadId)
										        
	{
	}


#endif // STFLOGGERCSCOMMON_H__
