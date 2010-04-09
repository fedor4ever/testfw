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
* Description: This file contains the header file of the 
* CStifLoggerBase.
*
*/

#ifndef STIFLOGGERBASE_H
#define STIFLOGGERBASE_H

//  INCLUDES
#include <e32base.h>
#include <StifLogger.h>

// CLASS DECLARATION

// DESCRIPTION
// CStifLoggerBase is a STIF Test Framework StifLogger class.
// Class contains a file logging operations.

class CStifLoggerBase 
        :public CBase
    {
 
    public:     // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CStifLoggerBase* NewL( const TDesC& aTestPath,
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
                                            TBool aUnicode = EFalse,
                                            TInt aThreadId =0);
        /**
        * Destructor.
        */
        virtual ~CStifLoggerBase();

    public:     // New functions

        /**
        * C++ default constructor.
        */
        CStifLoggerBase();

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
        * Get creation result.
        */
        IMPORT_C TInt CreationResult();

        /**
        * Get output type. Valid only if CreationResult returns KErrNone.
        */
        IMPORT_C CStifLogger::TOutput OutputType();

        /**
        * Pure virtual. Send style information and data to the output module.
        * 16 bit.
        */
        virtual TInt Send( TInt /*aStyle*/, const TDesC& /*aData*/ ) = 0;

        /**
        * Pure virtual. Send style information and data to the output module.
        * 8 bit.
        */
        virtual TInt Send( TInt /*aStyle*/, const TDesC8& /*aData*/ ) = 0;

        /**
        * Log 16 bit _LIT information to some storage e.g. text file.
        */
        template<TInt S> TInt Log( const TLitC<S>& aLogInfo )
            { 
            return Send( CStifLogger::ENoStyle, (TDesC&)aLogInfo );
            };

        /**
        * Log 8 bit _LIT information to some storage e.g. text file.
        */
        template<TInt S> TInt Log( const TLitC8<S>& aLogInfo )
            { 
            return Send( CStifLogger::ENoStyle, (TDesC8&)aLogInfo );
            };

        /**
        * Log 16 bit _LIT information to some storage e.g. text file. There is
        * also parameter to styling text information e.g. text color.
        */
        template<TInt S> TInt Log( TInt aStyle, const TLitC<S>& aLogInfo )
            { 
            return Send( aStyle, (TDesC&)aLogInfo );
            };

        /**
        * Log 8 bit _LIT information to some storage e.g. text file.  There is
        * also parameter to styling text information e.g. text color.
        */
        template<TInt S> TInt Log( TInt aStyle, const TLitC8<S>& aLogInfo )
            { 
            return Send( aStyle, (TDesC8&)aLogInfo );
            };

    protected:  // Data
        /**
        * Output object.
        */
        COutput*    iOutput;        
    };

#endif      // STIFLOGGERBASE_H

// End of File
