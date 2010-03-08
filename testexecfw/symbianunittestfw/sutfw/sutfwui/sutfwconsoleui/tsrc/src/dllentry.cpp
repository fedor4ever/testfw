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
*/

#include "ut_symbianunittestcommandlineparser.h"


// Exactly one exported function returning 
// the pointer to the suite of tests for the SymbianUnit framework.
//
EXPORT_C MSymbianUnitTestInterface* CreateTestL()
    {
    return UT_CSymbianUnitTestCommandLineParser::NewL();
    }


