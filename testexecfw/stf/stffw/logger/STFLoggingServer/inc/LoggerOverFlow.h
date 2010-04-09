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

//  INCLUDES
#include "StifLoggerBase.h"
#include "Output.h"
#include "RDebugOutput.h"

// DESCRIPTION
// TDesLoggerOverflowHandler is a STIF Test Framework StifLoggerBase class.
// Class contains an over flow operations.

class TDesLoggerOverflowHandler 
        :public TDes16Overflow
    {
    public:     // New functions

        /**
        * C++ default constructor.
        */
        TDesLoggerOverflowHandler( CStifLoggerBase* aLogger, TInt aOverFlowSource );

        /**
        * Simple overflow handling(16 bit)
        */
        void Overflow( TDes16& aDes );

    public:     // Data

        /**
        * Pointer to CStifLoggerBase class object
        */
        CStifLoggerBase* iLogger;

        /**
        * Over flow source
        */
        TInt iOverFlowSource;
    };

// CLASS DECLARATION

// DESCRIPTION
// TDes8LoggerOverflowHandler is a STIF Test Framework StifLoggerBase class.
// Class contains an over flow operations.

class TDes8LoggerOverflowHandler 
        :public TDes8Overflow
    {
    public:     // New functions

        /**
        * C++ default constructor.
        */
        TDes8LoggerOverflowHandler( CStifLoggerBase* aLogger, TInt aOverFlowSource );


        /**
        * Simple overflow handling(8 bit)
        */
        void Overflow( TDes8& aDes );

    public:     // Data

        /**
        * Pointer to CStifLoggerBase class object
        */
        CStifLoggerBase* iLogger;

        /**
        * Over flow source
        */
        TInt iOverFlowSource;
    };

#endif      // LOGGEROVERFLOW_H

// End of File
