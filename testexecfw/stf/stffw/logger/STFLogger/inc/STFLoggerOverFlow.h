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
* TDesLoggerOverflowHandler and TDes8LoggerOverflowHandler.
*
*/

#ifndef LOGGEROVERFLOW_H
#define LOGGEROVERFLOW_H

#include "STFLogger.h"


// DESCRIPTION
// TDesLoggerOverflowHandler is a STIF Test Framework StifLoggerBase class.
// Class contains an over flow operations.
class TDesSTFLoggerOverflowHandler 
        :public TDes16Overflow
    {
    public:     // New functions
        TDesSTFLoggerOverflowHandler( RSTFLogger* aLogger, TInt aOverFlowSource );
        void Overflow( TDes16& aDes );

    private:     // Data
    	RSTFLogger* iLogger;
        TInt iOverFlowSource;
    };

// CLASS DECLARATION

// DESCRIPTION
// TDes8LoggerOverflowHandler is a STIF Test Framework StifLoggerBase class.
// Class contains an over flow operations.
class TDes8STFLoggerOverflowHandler 
        :public TDes8Overflow
    {
    public:     // New functions

        TDes8STFLoggerOverflowHandler( RSTFLogger* aLogger, TInt aOverFlowSource );
        void Overflow( TDes8& aDes );

    private:     // Data
    	RSTFLogger* iLogger;
        TInt iOverFlowSource;
    };

#endif      // LOGGEROVERFLOW_H

// End of File
