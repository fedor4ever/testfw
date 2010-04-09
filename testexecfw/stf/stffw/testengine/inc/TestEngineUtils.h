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
#ifndef TESTENGINEUTILS_H_
#define TESTENGINEUTILS_H_

#include <e32base.h>
#include <StifLogger.h>
class CStifParser;

class TTestEngineUtils
    {
public:
    static TInt ParseLoggerDefaults( CStifParser* aParser, TLoggerSettings& aLoggerSettings, CStifLogger* aLogger  );
    };

#endif /* TESTENGINEUTILS_H_ */
