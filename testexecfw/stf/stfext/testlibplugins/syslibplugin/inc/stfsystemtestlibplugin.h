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
* Description: This file contains the header file of the CAtsLogger 
* class.
*
*/

// This file defines the API for stfsystemtestlibplugin.dll

#ifndef __STFSYSTEMTESTLIBPLUGIN_H__
#define __STFSYSTEMTESTLIBPLUGIN_H__

//  Include Files

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include "StfTestLibPluginInterface.h"
//  Constants

const TInt KstfsystemtestlibpluginBufferLength = 15;
typedef TBuf<KstfsystemtestlibpluginBufferLength>
        TstfsystemtestlibpluginExampleString;

//  Class Definitions

class Cstfsystemtestlibplugin : public CStfTestLibPlugin
    {
public:
    // new functions
    static Cstfsystemtestlibplugin* NewL();
    ~Cstfsystemtestlibplugin();

public:
    // new functions, example API
    virtual TBool IsCommandSupported(const TDesC& aCommand) ;
    virtual TInt  ExecuteCommandL(const TDesC& aCommandLine);

private:
    // new functions
    Cstfsystemtestlibplugin();
    void ConstructL();

private:
    
    };

#endif  // __STFSYSTEMTESTLIBPLUGIN_H__

