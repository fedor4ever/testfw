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

#ifndef __STFSYSTEMTESTLIBPLUGIN_PAN__
#define __STFSYSTEMTESTLIBPLUGIN_PAN__

//  Data Types

enum TstfsystemtestlibpluginPanic
    {
    EstfsystemtestlibpluginNullPointer
    };

//  Function Prototypes

GLREF_C void Panic(TstfsystemtestlibpluginPanic aPanic);

#endif  // __STFSYSTEMTESTLIBPLUGIN_PAN__

