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
//  Include Files  
#include "stf.h"
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>			// Console
#include "executor.h"
#include "cmdparser.h"
#include "helper.h"

_LIT(KTextFailed, " failed, leave code = %d");
_LIT(KTextPressAnyKey, " [press any key to exit.]\n");

LOCAL_D CConsoleBase* console;
LOCAL_D TBool isPrompt = ETrue;


LOCAL_C TInt Execute()
    {
      
      //parse the command line
      CCmdParser* parser = NULL;    
      TRAPD(err, parser = CCmdParser::NewL());
      if (err != KErrNone)
          {
#ifdef _DEBUG
         RDebug::Print(_L("Command parser construction failed %d: "), err);
 #endif
         return err;
          }

      const TInt length = User().CommandLineLength();
      HBufC* cmdLine = NULL;
      TRAPD(err1 , cmdLine = HBufC::NewL(length));
      if (err1 != KErrNone)
           {
   #ifdef _DEBUG
           RDebug::Print(_L("Command line buffer construction failed %d: "), err1);
   #endif
           delete parser;
           return err1;
           }
      
      TPtr ptr = cmdLine->Des();
      User().CommandLine(ptr);
      ptr.TrimAll();
      TBool rst = parser->Parse(ptr);
      delete cmdLine;
      //end parse the command line.
      
      isPrompt = parser->IsPrompt();
      if (parser->IsHelp() || parser->IsVersion())
          {
          //only show help or version info.
          isPrompt = ETrue;
          CHelper* helper = NULL;
          TRAPD( err, helper = CHelper::NewL(console));
          if (err != KErrNone)
              {
  #ifdef _DEBUG
              RDebug::Print(_L("helper construction failed %d: "), err);
  #endif
              delete parser;
              return err;
              }
          if(parser->IsHelp())
              {
              helper->ShowHelp();            
              }
          else
              {
              helper->ShowVersion();
              }
          delete helper;
          }
      else
          {
          if (!rst)
              {
      #ifdef _DEBUG
              RDebug::Print(_L("arguments error."));
      #endif
              delete parser;
              return KErrArgument;
              } 
          
          // Construct the test client
           CExecutor* executor = NULL;
           TRAPD( err, executor = CExecutor::NewL(parser,console) );
           if (err != KErrNone)
               {
       #ifdef _DEBUG
               RDebug::Print(_L("STF executor construction failed %d: \n"), err);
       #endif
               delete parser;
               return err;
               }

               // Run tests
               TRAP( err, executor->RunTestsL() );
           if (err != KErrNone)
               {
               console->Printf(_L("STF execution failed with: %d \n"),err);
               
       #ifdef _DEBUG
               RDebug::Print(_L("RunTestsL left with %d: "), err);
       #endif
               }
           
           // Deallocate resources
           delete executor;
          }
   
      delete parser;
      return KErrNone;
    }

LOCAL_C void DoStartL()
    {
    // Create active scheduler (to run active objects)
    CActiveScheduler* scheduler = new (ELeave) CActiveScheduler();
    CleanupStack::PushL(scheduler);
    CActiveScheduler::Install(scheduler);
    TInt err = Execute();
    if(err != KErrNone)
        {
        console->Printf(_L("STF Get Error: %d."), err);
        if(err == KErrArgument)
                {
                console->Printf(_L("Arguments error.\n"));
                }
            else if(err == KErrNoMemory)
                {
                console->Printf(_L("No enough memory.\n"));    
                }
            else if(err == KErrNotFound)
                {
                console->Printf(_L("Required resouce not found.\n"));    
                }
            else if(err == KErrNotSupported)
                {
                console->Printf(_L("Operation not supported in this version.\n"));   
                }
            else
                {
                console->Printf(_L("Other unknow error.\n"));   
                }
        }
        
    
    // Delete active scheduler
    CleanupStack::PopAndDestroy(scheduler); 
    }

//  Global Variables
GLDEF_C TInt E32Main()
    {

    TInt processHandleCountBefore;
    TInt threadHandleCountBefore;
    RThread().HandleCount(processHandleCountBefore, threadHandleCountBefore);
    TInt reqsBefore = RThread().RequestCount();

    TInt processHandleCountAfter;
    TInt threadHandleCountAfter;
    TInt reqsAfter;
    
    __UHEAP_MARK;
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TRAPD(createError, console = Console::NewL(_L("STF"), TSize(
               KConsFullScreen, KConsFullScreen)));
    if(createError)
        {
        return createError;
        }
    
    TRAPD(mainError, DoStartL());
    if(mainError)
        {
        console->Printf(KTextFailed, mainError);    
        }
    if(isPrompt)
        {
        console->Printf(KTextPressAnyKey);
        console->Getch();
        }
    reqsAfter = RThread().RequestCount();
    RThread().HandleCount(processHandleCountAfter, threadHandleCountAfter);

    if (reqsAfter != reqsBefore)
        {
#ifdef _DEBUG
        RDebug::Print(_L("Request count not matching! %d vs. %d: "),
        reqsBefore, reqsAfter);
#endif
        }
    if (threadHandleCountAfter != threadHandleCountBefore)
        {
#ifdef _DEBUG
        RDebug::Print(_L("Handle count not matching! %d vs. %d: "),
        threadHandleCountBefore, threadHandleCountAfter);
#endif
        }
    delete console;
    delete cleanup;
    __UHEAP_MARKEND;
    return KErrNone;
    }
