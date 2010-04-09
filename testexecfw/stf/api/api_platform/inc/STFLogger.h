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

// This file defines the API for STFLogger.dll

#ifndef __STFLOGGER_H__
#define __STFLOGGER_H__

//  Include Files

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include <f32file.h>
#include <stiflogger.h>


#define __DECLARE_LOG               RSTFLogger stf_Logger;
#define __OPENLOGL(D, L)            User::LeaveIfError(stf_Logger.Connect()); \
                                    TParse p; \
                                    _LIT(KLogFile, L); \
                                    p.Set(KLogFile(), NULL, NULL); \
                                    _LIT(KLogDir, D); \
                                    stf_Logger.CreateL(KLogDir(), p.NameAndExt());
#define __CLOSELOG                  stf_Logger.Close();
extern RSTFLogger stf_Logger;

//These type of log should be used as: __LOG(_L("test"));
#define __LOG(C)                    stf_Logger.Log(C);
#define __LOG1(C, Z)                stf_Logger.Log(C, Z);
#define __LOG2(C, Z, Y)             stf_Logger.Log(C, Z, Y);
#define __LOG3(C, Z, Y, X)          stf_Logger.Log(C, Z, Y, X);
#define __LOG4(C, Z, Y, X, W)       stf_Logger.Log(C, Z, Y, X, W);
#define __LOG5(C, Z, Y, X, W, V)    stf_Logger.Log(C, Z, Y, X, W, V);
#define __LOG6(C, Z, Y, X, W, V, U) stf_Logger.Log(C, Z, Y, X, W, V, U);




class RSTFLogger : public RSessionBase
	{
public:
    
	// new functions
	IMPORT_C TInt Connect();

public: // new functions, example API
		
	IMPORT_C TInt CreateL(	const TDesC& aTestPath,
				            const TDesC& aTestFile,
				            // Default logger type is .txt
				            CStifLogger::TLoggerType aLoggerType = CStifLogger::ETxt,
				            // By default output is file
				            CStifLogger::TOutput aOutput = CStifLogger::EFile,
				            // By default overwrite is used
				            TBool aOverWrite = ETrue,
				            // By default time stamp is used
				            TBool aWithTimeStamp = ETrue,
				            // By default line break is used
				            TBool aWithLineBreak = ETrue,
				            // By default even ranking is not
				            // used
				            TBool aWithEventRanking = EFalse,
				            // By default thread id is added
				            // after the log file
				            TBool aThreadIdToLogFile = ETrue,
				            // By default log directory will
				            // not create
				            TBool aCreateLogDir = EFalse,
				            // Static logger implementation is
				            // not yet permanent, may change 
				            TInt aStaticBufferSize = 0,
				            // By default files are not written
				            // in unicode format
				            TBool aUnicode = EFalse );
	
	   IMPORT_C TInt CreateL(   const TDesC& aTestPath,
	                           const TDesC& aTestFile,
                               TLoggerSettings& aLoggerSettings );
	
    /**
    * Log a 16 bit information to some storage e.g. text file.
    */
    IMPORT_C TInt Log( const TDesC& aLogInfo );

    /**
    * Log a 8 bit information to some storage e.g. text file.
    */
    IMPORT_C TInt Log( const TDesC8& aLogInfo );

    /**
    * Log a 16 bit information to some storage e.g. text file. There is
    * also parameter to styling text information e.g. text color.
    */
    IMPORT_C TInt Log( TInt aStyle, const TDesC& aLogInfo );

    /**
    * Log a 8 bit information to some storage e.g. text file. There is
    * also parameter to styling text information e.g. text color.
    */
    IMPORT_C TInt Log( TInt aStyle, const TDesC8& aLogInfo );

    /**
    * Log a 16 bit information to some storage e.g. text file. In this log
    * method may set several parameters.
    */
    IMPORT_C TInt Log( TRefByValue<const TDesC> aLogInfo,... );

    /**
    * Log a 8 bit information to some storage e.g. text file. In this log
    * method may set several parameters.
    */
    IMPORT_C TInt Log( TRefByValue<const TDesC8> aLogInfo,... );

    /**
    * Log a 16 bit information to some storage e.g. text file. There is
    * also parameter to styling text information e.g. text color.
    */
    IMPORT_C TInt Log( TInt aStyle,
        TRefByValue<const TDesC> aLogInfo,... );

    /**
    * Log a 8 bit information to some storage e.g. text file. There is
    * also parameter to styling text information e.g. text color.
    */
    IMPORT_C TInt Log( TInt aStyle,
        TRefByValue<const TDesC8> aLogInfo,... );

    /**
    * Log a 16 bit delimiter. Default char is '#' and default
    * repeat count is 60.
    * This will be used if parameters are not given when calling
    * this method.
    */
    IMPORT_C TInt WriteDelimiter( const TDesC& aDelimiter = _L( "#"),
                                    TInt aCount = 60 );

    /**
    * Log a 8 bit delimiter. 8 bit delimiter includes no default values.
    * Delimiter may be '#' or 'XO' etc.
    */
    IMPORT_C TInt WriteDelimiter( const TDesC8& aDelimiter, TInt aCount );

    /**
    * Used when is need to save file or data to storage e.g. web page.
    * 16 bit.
    */
    IMPORT_C TInt SaveData(const TDesC& aData );

    /**
    * Used when is need to save file or data to storage e.g. web page.
    * 8 bit.
    */
    IMPORT_C TInt SaveData(const TDesC8& aData );
    
    /**
    * Get creation result.
    */
    IMPORT_C TInt CreationResult(TInt& aResult);

    /**
    * Get output type. Valid only if CreationResult returns KErrNone.
    */
    IMPORT_C TInt OutputType(CStifLogger::TOutput& aOutputType);
    
    /**
    * Log 16 bit _LIT information to some storage e.g. text file.
    */
    template<TInt S> TInt Log( const TLitC<S>& aLogInfo )
        { 
        return Log( CStifLogger::ENoStyle, (TDesC&)aLogInfo );
        };

    /**
    * Log 8 bit _LIT information to some storage e.g. text file.
    */
    template<TInt S> TInt Log( const TLitC8<S>& aLogInfo )
        { 
        return Log( CStifLogger::ENoStyle, (TDesC8&)aLogInfo );
        };

    /**
    * Log 16 bit _LIT information to some storage e.g. text file. There is
    * also parameter to styling text information e.g. text color.
    */
    template<TInt S> TInt Log( TInt aStyle, const TLitC<S>& aLogInfo )
        { 
        return Log( aStyle, (TDesC&)aLogInfo );
        };

    /**
    * Log 8 bit _LIT information to some storage e.g. text file.  There is
    * also parameter to styling text information e.g. text color.
    */
    template<TInt S> TInt Log( TInt aStyle, const TLitC8<S>& aLogInfo )
        { 
        return Log( aStyle, (TDesC8&)aLogInfo );
        };
    
private:
    void OverwriteLoggerSettings( TLoggerSettings& aLoggerSettings,
                                                TName& aTestPath,
                                                TName& aTestFile,
                                                CStifLogger::TLoggerType& aLoggerType,
                                                CStifLogger::TOutput& aOutput,
                                                TBool& aOverWrite,
                                                TBool& aWithTimeStamp,
                                                TBool& aWithLineBreak,
                                                TBool& aWithEventRanking,
                                                TBool& aThreadIdToLogFile,
                                                TBool& aCreateLogDir,
                                                TBool& aUnicode );

	};

	

#endif  // __STFLOGGER_H__

