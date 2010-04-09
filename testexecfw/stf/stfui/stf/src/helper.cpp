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
#include "helper.h"

_LIT(KVersion, "STF console based executor, version: 1.0.0");
_LIT(KRelDate, "2009-12-03");

CHelper::CHelper(CConsoleBase* aConsole) : iConsole(aConsole)
    {}

CHelper::~CHelper()
    {
    }

CHelper* CHelper::NewL(CConsoleBase* aConsole)
    {
    CHelper* self = new(ELeave) CHelper(aConsole);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

void CHelper::ConstructL()
    {
    //Nothing to do.
    }

void CHelper::ShowVersion()
    {
        
    iConsole->Printf(_L("* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).\n"));
    iConsole->Printf(_L("* All rights reserved.\n"));
    iConsole->Printf(_L("* This component and the accompanying materials are made available\n"));
    iConsole->Printf(_L("* under the terms of \"Eclipse Public License v1.0\"\n"));
    iConsole->Printf(_L("* which accompanies this distribution, and is available\n"));
    iConsole->Printf(_L("* at the URL \"http://www.eclipse.org/legal/epl-v10.html\".\n"));
    iConsole->Printf(_L("*\n"));
    iConsole->Printf(_L("* Nokia Corporation\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("\n"));
        
    //variables used to get version of STIF
    TInt majorV;
    TInt minorV;
    TInt buildV;
    TBuf<30> relDate;
    TStifUtil::STIFVersion(majorV, minorV, buildV, relDate);
    
    TBuf<50> version;
    version.Format(_L("STF v%d.%d.%d - "), majorV, minorV, buildV);
    version.Append(relDate);
    version.Append(_L("\n"));
    
    iConsole->Printf(version);  //printing STIF version information (version and release date)
    
    TBuf<100> toolversion;
    toolversion.Append(KVersion);
    toolversion.Append(_L(" - Release at:"));
    toolversion.Append(KRelDate);
    toolversion.Append(_L("\n"));
    iConsole->Printf(toolversion);
    iConsole->Printf(_L("\n"));
    }


void CHelper::ShowHelp()
    {
    TSize screenSize;
    screenSize = iConsole->ScreenSize();
    
    iConsole->Printf(_L("Usage: stf [-option value] [--parameter value] [case name/case index]\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("where options include:\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("-m <modulename>\n"));
    iConsole->Printf(_L("-s <script file path>\n"));
    iConsole->Printf(_L("-i <engine initialization file path>\n"));
    iConsole->Printf(_L("-c <module initialization file path>\n"));
    iConsole->Printf(_L("-engine <configuration item> <value>\n"));
    iConsole->Printf(_L("-log <configuration item> <value>\n"));
    iConsole->Printf(_L("-p <item> <value>\n"));
    iConsole->Printf(_L("-v  display version information\n"));
    iConsole->Printf(_L("-NOPROMPT exit without prompt when the execution finished.\n"));
    iConsole->Printf(_L("-?/-h  display this help information.\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("For example:\n"));
    iConsole->Printf(_L("stf -m demomodule 3\n"));
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("\n"));
    }


