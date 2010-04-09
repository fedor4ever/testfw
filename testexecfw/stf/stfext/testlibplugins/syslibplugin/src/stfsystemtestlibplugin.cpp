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

//  Include Files  

#include "stfsystemtestlibplugin.h"	// Cstfsystemtestlibplugin
#include "stfsystemtestlibplugin.pan"	  	// panic codes
#include "AppInstall.h"
_LIT(KSTFRunProgram,"RUN_PROGRAM");
_LIT(KSTFKillProgram,"KILL_PROGRAM");
_LIT(KSTFInstallSIS,"InstallSis");
_LIT(KSTFUnInstallSIS,"UnInstallSis");
//  Member Functions

EXPORT_C  Cstfsystemtestlibplugin* LibEntryL()
    {
    Cstfsystemtestlibplugin* self=Cstfsystemtestlibplugin::NewL();
    return  self;
    }

Cstfsystemtestlibplugin* Cstfsystemtestlibplugin::NewL()
    {
    Cstfsystemtestlibplugin* self = new (ELeave) Cstfsystemtestlibplugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

Cstfsystemtestlibplugin::Cstfsystemtestlibplugin()
// note, CBase initialises all member variables to zero
    {
    }

void Cstfsystemtestlibplugin::ConstructL()
    {
    }

Cstfsystemtestlibplugin::~Cstfsystemtestlibplugin()
    {
   
    }

 

EXPORT_C TBool Cstfsystemtestlibplugin::IsCommandSupported(const TDesC& aCommand)
    {
    TBool result=EFalse;
        if(aCommand==KSTFRunProgram)
            {
            result=ETrue;
            }
        else if(aCommand==KSTFKillProgram)
            {
            result=ETrue;
            }
      
        else if(aCommand==KSTFInstallSIS)
            {
            result=ETrue;
            }
        else if(aCommand==KSTFUnInstallSIS)
            {
            result=ETrue;
            }
        
        
    return result;
    }
EXPORT_C TInt  Cstfsystemtestlibplugin::ExecuteCommandL(const TDesC& aCommandLine)
    {
    TInt result=KErrNone;
    TLex lex(aCommandLine);
    TPtrC token(lex.NextToken());
    if(token.CompareF(KSTFRunProgram) == 0)
        {
        RProcess process;
        TPtrC filename(lex.NextToken());
        TPtrC para(lex.NextToken());
        TInt err=process.Create(filename, para);
        if (err == KErrNone)
        {
        CleanupClosePushL(process);
        TRequestStatus status;
        process.Rendezvous(status);
       if (status == KRequestPending)
           {
           process.Resume();
           User::WaitForRequest(status);
           }
       else
           {
           process.RendezvousCancel(status);
           process.Kill(0);
          
           }
       CleanupStack::PopAndDestroy();
       }
       else
       {
          
       }
        }
    else if(token.CompareF(KSTFKillProgram) == 0)
        {
        
        
            TBuf<100> execute;    
            execute.Append(lex.NextToken());
            execute.TrimAll();
            execute.Append(_L("*"));
           
            TFindProcess processFinder(execute);
           
            TFullName result;
            TInt err=processFinder.Next(result);
             if( err == KErrNone)
             {
            
             RProcess processHandle;
             User::LeaveIfError(processHandle.Open ( processFinder, EOwnerThread));   
             processHandle.Kill(KErrNone); 
             processHandle.Close();
             }
        
        
        
        
        }
    else if(token.CompareF(KSTFInstallSIS) == 0)
        {
        TPtrC sisname(lex.NextToken());
        result=CAppInstall::Install(sisname);
        }
    else if(token.CompareF(KSTFUnInstallSIS) == 0)
        {
        TPtrC uid(lex.NextToken());
        result=CAppInstall::Uninstall(uid);
        }
    return result;
    }
