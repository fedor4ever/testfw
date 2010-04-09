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

#ifndef __STFFILETESTLIB_H__
#define __STFFILETESTLIB_H__

//  Include Files

#include <e32base.h>	// CBase
#include <e32std.h>	 // TBuf
#include "StfTestLibPluginInterface.h"





//  Class Definitions
    
class CStfFileTestLibPlugin : public CStfTestLibPlugin
    {
public:    // new functions
    
    ~CStfFileTestLibPlugin();
    
    static CStfFileTestLibPlugin* NewL();
   

public:
    // new functions, example API
    virtual TBool IsCommandSupported(const TDesC& aCommand) ;
    virtual TInt  ExecuteCommandL(const TDesC& aCommandLine);
private:
    // new functions
    CStfFileTestLibPlugin();
    void ConstructL();

private:
    // data
   
    };



#endif  // __STFFILETESTLIB_H__

