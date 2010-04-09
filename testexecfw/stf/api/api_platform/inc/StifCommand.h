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
* Description: This file contains the type definitions used by 
* command feature (i.e. StopExecution).
*
*/

#ifndef STIF_COMMAND_H
#define STIF_COMMAND_H


//  INCLUDES
#include <E32Base.h>

// CONSTANTS 
const TInt KMaxCommandParamsLength = 50;	// DEPRECATED use KMaxCommandParamsLength2 instead
const TInt KMaxCommandParamsLength2 = 600;

// MACROS

// FUNCTION PROTOTYPES

// DATA TYPES

// This enum type lists all possible commands sent from test case to test engine
enum TCommand
    {
    EStopExecution,
    ESendTestModuleVersion,
    };

typedef TPckg<TCommand> TCommandPckg;

// This enum type lists all possible ways of how to run StopExecution command
enum TStopExecutionType
    {
    EOk,
#ifndef __TEF_EXPORT_CONST_H__   
    //EFail & EAbort are overlaped with enum in tefexportconst.h
    //TEFTestModule.dll migrates TEF into STIF module, If build TEFTestModule.dll, EFail and EAbort definition will be deleted from STIF. 
    EFail,
    EAbort,
#endif
    };

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
struct TStopExecutionCommandParams
    {
    TInt               iTestCaseHandle;
    TStopExecutionType iType;
    TInt               iCode;
    };

typedef TPckg<TStopExecutionCommandParams> TStopExecutionCommandParamsPckg;


struct TSendTestModuleVesionCommandParams
	{
	TFileName iTestModuleName;
	TInt iMajor;
	TInt iMinor;
	TInt iBuild;
	};

typedef TPckg<TSendTestModuleVesionCommandParams> TSendTestModuleVesionCommandParamsPckg;

#endif //STIF_COMMAND_H

// End of File
