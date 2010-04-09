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
#include "caserunner.h"
#include "executor.h"

CExecutor::CExecutor(CCmdParser* aParser,CConsoleBase* aConsole) :
    iConsole(aConsole),iParser(aParser)
    {
    }

void CExecutor::ConstructL()
    {
    // Create console screen
   
    RDebug::Print(_L("Creating module list object"));
    TRAPD(err, iModuleList = CTestModuleList::NewL(NULL));
    if (err != KErrNone)
        {
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("CTestModuleList::NewL"), err);
        return;
        }
    if (!iModuleList)
        {
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("CTestModuleList::NewL - iModuleList is NULL"),
                KErrGeneral);
        return;
        }

    // Add to module list info about module taken from command line
    RDebug::Print(_L("Adding command line module to list"));
    TName moduleName;
    moduleName.Copy(iParser->TestModule());
    moduleName.LowerCase();
    RDebug::Print(_L("AddTestModule : '%S'"), &moduleName);
    err = iModuleList->AddTestModule(moduleName);
    
    if (err != KErrNone && err != KErrAlreadyExists)
        {
        LogErrorAndLeaveL(
                _L("CExecutor::ConstructL"),
                _L("CTestModuleList::AddTestModule - Could not add module to list of modules"),
                err);
        return;
        }

    //Get added module
    CTestModuleInfo* moduleInfo = iModuleList->GetModule(moduleName);
    if (!moduleInfo)
        {
        LogErrorAndLeaveL(
                _L("CExecutor::ConstructL"),
                _L("CTestModuleList::GetModule - Could not add get module info from list"),
                KErrGeneral);
        return;
        }

    //Add ini file if given
    if (iParser->ModuleIniFile().Length() > 0)
        {
        TFileName filename;
        filename.Copy(iParser->ModuleIniFile());
        filename.LowerCase();
        moduleInfo->SetIniFile(filename);
        }

    //Add config file if given
    if (iParser->ConfigFile().Length() > 0)
        {
        TFileName filename;
        filename.Copy(iParser->ConfigFile());
        filename.LowerCase();
        moduleInfo->AddCfgFile(filename);
        }
    //Now check all config files if there are included modules
    _LIT(KIncludeModuleStart, "[New_Include_Module]");
    _LIT(KIncludeModuleEnd, "[End_Include_Module]");

    RDebug::Print(_L("Start parsing included modules"));
    CTestCaseFileInfo* finfo = iModuleList->GetUncheckedCfgFile();
    while (finfo)
        {
        TFileName fname;
        finfo->GetCfgFileName(fname);

        RDebug::Print(_L("Checking file: '%S'"), &fname);
        finfo->SetChecked();

        CStifParser* parser = NULL;

        TRAP(err, parser = CStifParser::NewL(_L(""), fname));
        if (err != KErrNone)
            {
            iConsole->Printf(_L("\n ErrNo: [%d],can't find cfg \n file: [%S] \n in %S module. \n"), err,&fname,&moduleName);
            LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                    _L("CStifParser::NewL - Could not create parser"),
                    err);
            return;
            }
        CleanupStack::PushL(parser);

        ParseTestModulesL(parser, iModuleList, KIncludeModuleStart,
                KIncludeModuleEnd);

        CleanupStack::PopAndDestroy(parser);
        finfo = iModuleList->GetUncheckedCfgFile();
        }
    RDebug::Print(_L("End parsing included modules"));

    // Create Test Engine
    RDebug::Print(_L("Creating test engine"));
    TInt ret = iTestEngine.Connect();
    if (ret != KErrNone)
        {
        // Log error
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("iTestEngine.Connect"), ret);
        return;
        }
    
    
    ret = iTestEngine.LoadConfiguration(iParser->EngineIniFile());
    
    if (ret != KErrNone)
        {
        iConsole->Printf(_L("\n ErrNo: [%d],can't load  engine ini :[%S]. \n"), err,&iParser->EngineIniFile());
              
        // Log error
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("iTestEngine.LoadConfiguration"), ret);
        return;
        }
  
      //update Log Configuration
      ret = iTestEngine.UpdateLogConfiguration(iParser->LogConfigList());
      if (ret != KErrNone)
          {
          // Log error
          LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                 _L("iTestEngine.UpdateLogConfiguration"), ret);
          return;
          } 
    
      //update Engine Configuration        
      ret = iTestEngine.UpdateEngineConfiguration(iParser->EngineConfigList());
      if (ret != KErrNone)
          {
          // Log error
          LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                 _L("iTestEngine.UpdateEngineConfiguration"), ret);
          return;
          }  
    RDebug::Print(_L("End creating test modules"));
    
    //Get all test modules loaded via TestFramework.ini
    //Remove them before load user assiged module.
    // Enumerate test cases
    TCaseCount caseCount;
    TRequestStatus status;
    iTestEngine.EnumerateTestCases(caseCount, status);
    User::WaitForRequest(status);

    // Check that enumerate succeeded
    if (status != KErrNone)
        {
        // Log error 
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("iTestEngine.EnumerateTestCases"), status.Int());
        return;
        }

    // Get test cases to buffer
    CFixedFlatArray<TTestInfo>* casesList = CFixedFlatArray<TTestInfo>::NewL(
            caseCount());
    
    CleanupStack::PushL(casesList);
    
    ret = iTestEngine.GetTestCases(*casesList);
    if (ret != KErrNone)
        {
        // Log error 
        LogErrorAndLeaveL(_L("CExecutor::RunAllTestCasesL"),
                _L("iTestEngine.GetTestCases"), status.Int());
        return;
        }
  
    //remove all exist cases.
    for(TInt i=0;i<(*casesList).Count();i++)
        {
        RDebug::Print((*casesList)[i].iModuleName);
        iTestEngine.RemoveTestModule((*casesList)[i].iModuleName); 
        iTestEngine.RemoveConfigFile((*casesList)[i].iModuleName, (*casesList)[i].iConfig);
        }
    CleanupStack::PopAndDestroy(casesList);
        
    
    // Add all test modules and config files
    RDebug::Print(_L("Start creating test modules"));
    moduleInfo = NULL;
    TInt i;
    TInt modCnt = iModuleList->Count();

    for (i = 0; i < modCnt; i++)
        {
        RDebug::Print(_L("Processing module"));
        // Get module
        moduleInfo = iModuleList->GetModule(i);
        if (!moduleInfo)
            {
            RDebug::Print(_L("Could not get module info at index %d"), i);
            continue;
            }

        // Get module name
        TName moduleName;
        moduleInfo->GetModuleName(moduleName);
        RDebug::Print(_L("module name: '%S'"), &moduleName);

        // Get ini file, if exists
        TFileName ini;
        moduleInfo->GetIniFileName(ini);
        if (ini.Length() == 0)
            {
            RDebug::Print(_L("ini file not found"));
            }
        else
            {
            RDebug::Print(_L("ini file: '%S'"), &ini);
            }

        // Create test module
        RDebug::Print(_L("Adding module to test engine"));
        ret = iTestEngine.AddTestModule(moduleName, ini);
        if (ret != KErrNone && ret != KErrAlreadyExists)
            {
            iConsole->Printf(_L("\n ErrNo: [%d],can't find module: [%S] \n"), ret,&moduleName);
            LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                    _L("iTestEngine.AddTestModule"), ret);
            
            return;
            }

        //Add test case files
        TInt cfgCnt = moduleInfo->CountCfgFiles();
        TInt j;
        TFileName cfgFile;
        for (j = 0; j < cfgCnt; j++)
            {
            moduleInfo->GetCfgFileName(j, cfgFile);
            if (cfgFile.Length() > 0)
                {
                RDebug::Print(_L("config file: '%S'"), &cfgFile);

                ret = iTestEngine.AddConfigFile(moduleName, cfgFile);
                if (ret != KErrNone && ret != KErrAlreadyExists)
                    {
                   
                    // Log error
                    LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                            _L("RTestEngine::AddConfigFile"), ret);
                    return;
                    }
                }
            else
                {
                RDebug::Print(_L("Got empty cfg file"));
                }
            }
        if (cfgCnt == 0)
            {
            RDebug::Print(_L("cfg file not found"));
            }

        RDebug::Print(_L("Module '%S' processed correctly"), &moduleName);
        }
    
    // Enumerate test cases in added module
    iTestEngine.EnumerateTestCases(caseCount, status);
    User::WaitForRequest(status);

    // Check that enumerate succeeded
    if (status != KErrNone)
        {
        // Log error 
        LogErrorAndLeaveL(_L("CExecutor::ConstructL"),
                _L("iTestEngine.EnumerateTestCases"), status.Int());
        return;
        }

    iTestCasesList = CFixedFlatArray<TTestInfo>::NewL(caseCount());
    ret = iTestEngine.GetTestCases(*iTestCasesList);
    if (ret != KErrNone)
        {
        // Log error 
        LogErrorAndLeaveL(_L("CExecutor::RunAllTestCasesL"),
                _L("iTestEngine.GetTestCases"), status.Int());
        return;
        }
    }

CExecutor* CExecutor::NewL(CCmdParser* aParser,CConsoleBase* aConsole)
    {
    CExecutor* self = new (ELeave) CExecutor(aParser,aConsole);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

CExecutor::~CExecutor()
    {
    iTestEngine.Close();
    delete iTestCasesList;
    delete iModuleList;
    
    }

void CExecutor::ParseTestModulesL(CStifParser* aParser,
        CTestModuleList* aModuleList, const TDesC& aSectionStart,
        const TDesC& aSectionEnd)
    {
    //First let's find all modules given in Stif's ini file and store that info in CTestModuleList object
    CStifSectionParser* sectionParser = NULL;
    CStifItemParser* item = NULL;

    sectionParser = aParser->SectionL(aSectionStart, aSectionEnd);

    while (sectionParser)
        {
        RDebug::Print(_L("Found '%S' and '%S' sections"), &aSectionStart,
                &aSectionEnd);
        CleanupStack::PushL(sectionParser);
        RDebug::Print(_L("Starting to read module information"));
        // Get name of module
        _LIT(KModuleName, "ModuleName=");
        item = sectionParser->GetItemLineL(KModuleName);
        CleanupStack::PushL(item);
        if (!item)
            {
            CleanupStack::PopAndDestroy(item);
            LogErrorAndLeaveL(
                    _L("CExecutor::ParseTestModulesL"),
                    _L("CStifItemParser::GetItemLineL - line not found from module section"),
                    KErrNotFound);
            return;
            }
        else
            {
            RDebug::Print(_L("'%S' found"), &KModuleName);
            }

        TPtrC name;
        TName moduleName;
        TInt ret(KErrNone);
        ret = item->GetString(KModuleName, name);
        if (ret != KErrNone)
            {
            CleanupStack::PopAndDestroy(item);
            LogErrorAndLeaveL(
                    _L("CExecutor::ParseTestModulesL"),
                    _L("CStifItemParser::GetString - Module name parsing left with error"),
                    ret);
            return;
            }
        else
            {
            RDebug::Print(_L("Module '%S' found from ini-file"), &name);
            moduleName.Copy(name);
            moduleName.LowerCase();
            ret = aModuleList->AddTestModule(moduleName);
            if (ret != KErrNone && ret != KErrAlreadyExists)
                {
                LogErrorAndLeaveL(
                        _L("CExecutor::ParseTestModulesL"),
                        _L("CTestModuleList::AddTestModule - Could not add module to list of modules"),
                        ret);
                return;
                }
            }
        CleanupStack::PopAndDestroy(item);

        //Get pointer to added module
        CTestModuleInfo* moduleInfo = aModuleList->GetModule(moduleName);
        if (!moduleInfo)
            {
            LogErrorAndLeaveL(
                    _L("CExecutor::ParseTestModulesL"),
                    _L("CTestModuleList::GetModule - Could not add get module info from list"),
                    KErrNotFound);
            return;
            }

        // Get ini file, if it exists
        RDebug::Print(_L("Start parsing ini file"));
        _LIT(KIniFile, "IniFile=");
        item = sectionParser->GetItemLineL(KIniFile);
        if (item)
            {
            RDebug::Print(_L("'%S' found"), &KIniFile);
            CleanupStack::PushL(item);
            TPtrC iniFile;
            ret = item->GetString(KIniFile, iniFile);
            if (ret == KErrNone)
                {
                RDebug::Print(
                        _L("Initialization file '%S' found, file can be empty"),
                        &iniFile);
                TFileName filename;
                filename.Copy(iniFile);
                filename.LowerCase();
                TStifUtil::CorrectFilePathL(filename);
                moduleInfo->SetIniFile(filename);
                }
            else
                {
                RDebug::Print(_L("Initialization file not found"));
                }
            CleanupStack::PopAndDestroy(item);
            }
        else
            {
            RDebug::Print(_L("'%S' not found"), &KIniFile);
            }

        // Get config (testcase) file
        RDebug::Print(_L("Start parsing cfg files"));
        TPtrC cfgTag;
        for (TInt i = 0; i < 2; i++)
            {
            //Set tag for config files
            if (i == 0)
                {
                cfgTag.Set(_L("ConfigFile="));
                }
            else
                {
                cfgTag.Set(_L("TestCaseFile="));
                }
            //Read data
            item = sectionParser->GetItemLineL(cfgTag);
            while (item)
                {
                CleanupStack::PushL(item);
                RDebug::Print(_L("Item '%S' found"), &cfgTag);
                TPtrC cfgFile;
                ret = item->GetString(cfgTag, cfgFile);
                if (ret == KErrNone)
                    {
                    TFileName ifile;
                    ifile.Copy(cfgFile);
                    ifile.LowerCase();
                    TStifUtil::CorrectFilePathL(ifile);
                    RDebug::Print(_L("Configuration file '%S' found"), &ifile);
                    moduleInfo->AddCfgFile(ifile);
                    }
                else
                    {
                    RDebug::Print(_L("Configuration file not found"));
                    }
                CleanupStack::PopAndDestroy(item);
                item = sectionParser->GetNextItemLineL(cfgTag);
                }
            }

        RDebug::Print(_L("Module '%S' information read correctly"),
                &moduleName);

        // Get next section
        CleanupStack::PopAndDestroy(sectionParser);
        sectionParser = aParser->NextSectionL(aSectionStart, aSectionEnd);
        }
    }

void CExecutor::RunTestsL()
    {
    RunAllTestCasesL();

    // Remove test module
    RDebug::Print(_L("Start removing test modules"));
    CTestModuleInfo* moduleInfo = NULL;
    TInt i;
    TInt modCnt = iModuleList->Count();

    for (i = 0; i < modCnt; i++)
        {
        RDebug::Print(_L("Processing module"));
        // Get module
        moduleInfo = iModuleList->GetModule(i);
        if (!moduleInfo)
            {
            RDebug::Print(_L("Could not get module info at index %d"), i);
            continue;
            }

        // Get module name
        TName moduleName;
        moduleInfo->GetModuleName(moduleName);
        RDebug::Print(_L("module name: '%S'"), &moduleName);

        // Remove test module
        User::LeaveIfError(iTestEngine.RemoveTestModule(moduleName));
        RDebug::Print(_L("Module '%S' removed"), &moduleName);
        }

    RDebug::Print(_L("End removing test modules"));
    }

void CExecutor::RunAllTestCasesL()
    {
    
    RArray<TName> testCasesTitleList;
    CleanupClosePushL(testCasesTitleList);
    
    for(TInt i=0;i<iTestCasesList->Count();i++)
        {
        TName title;
        title.Append((*iTestCasesList)[i].iTestCaseInfo.iTitle);
        testCasesTitleList.Append(title);
        }
    
    
    for(TInt i=0;i<iParser->TestCaseIndexList().Count();i++)
    {
    
       if(iParser->TestCaseIndexList()[i]>=testCasesTitleList.Count())
           {
               iConsole->Printf(_L("\ncase No.[%d] doesn't exist\n"),iParser->TestCaseIndexList()[i]);
           }
    }
    
    
    for(TInt i=0;i<iParser->TestCaseList().Count();i++)
    {
        
        TBool isfound(EFalse);
        for(TInt j=0;j<testCasesTitleList.Count();j++)
            {
                if(iParser->TestCaseList()[i].Compare(testCasesTitleList[j])==KErrNone)
                    {
                    isfound=ETrue;  
                    }
            
            }
        if(!isfound)
            {
            iConsole->Printf(_L("\ncase [%S] doesn't exist\n"),&iParser->TestCaseList()[i]);
            }
    
    
    }
    
    
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

    iConsole->Printf(version); //printing STIF version information (version and release date)

    TInt caseNum = iParser->CaseCount();
    if (caseNum == 0)
        {
        caseNum = iTestCasesList->Count();
        }

    iConsole->Printf(_L("Test case count: [%d]\n\n"), caseNum);

    // Loop through all test cases in buffer and run them
    const TInt count = iTestCasesList->Count();
    if (iParser->CaseCount() == 0)
        {
        //execute all
        for (TInt i = 0; i < count; i++)
            {
#ifdef _DEBUG
            RDebug::Print((*iTestCasesList)[i].iTestCaseInfo.iTitle);
#endif
            iConsole->Printf(_L("Now running test case: [%d] [%S] "), i,
                    &(*iTestCasesList)[i].iTestCaseInfo.iTitle);

            // Run test case
            RunTestCaseL((*iTestCasesList)[i]);
            }
        }
    else
        {
        for (TInt i = 0; i < count; i++)
            {
            if (iParser->CaseContains(i, (*iTestCasesList)[i].iTestCaseInfo.iTitle))
                {
#ifdef _DEBUG
                RDebug::Print((*iTestCasesList)[i].iTestCaseInfo.iTitle);
#endif
                iConsole->Printf(_L("Now running test case: [%d] [%S] "),
                        i, &(*iTestCasesList)[i].iTestCaseInfo.iTitle);
                // Run test case
                RunTestCaseL((*iTestCasesList)[i]);
                }
            }
        }
    
    CleanupStack::PopAndDestroy(&testCasesTitleList);
    }

void CExecutor::RunTestCaseL(TTestInfo& aTestInfo)
    {
    TInt testResult(KErrNone);
    CCaseRunner* runner;
    // Trap to catch errors from test case executing
    TRAPD( trapError, runner = CCaseRunner::NewL( this, aTestInfo );
            CleanupStack::PushL( runner );

            testResult = RunATestCaseL( runner );

            CleanupStack::PopAndDestroy( runner );
    );

    if (trapError != KErrNone)
        {
        testResult = trapError;
        }

    if (testResult != KErrNone) // Test case is FAILED
        {
        // Test case failed, print out the error
        iConsole->Printf(_L("\nTest case FAILED! err=[%d]\n"), testResult);
        }

    else // Test case is PASSED
        {
        iConsole->Printf(_L("\nTest case PASSED!\n"));
        testResult = KErrNone;
        }
    }

TInt CExecutor::RunATestCaseL(CCaseRunner* aCaseRunner)
    {
    iTestCompletedError = KErrNone;

    // Create timer
    CActiveTimer* timer = CActiveTimer::NewL(iConsole);
    CleanupStack::PushL(timer);

    // Start test case and timer
    aCaseRunner->StartTestL();
    timer->StartL();

    // Wait for test case completed
    CActiveScheduler::Start();

    timer->Cancel();
    CleanupStack::PopAndDestroy(timer);

    // test completion error is set in TestCompleted method
    return iTestCompletedError;
    }

RTestEngine& CExecutor::TestEngine()
    {
    return iTestEngine;
    }

void CExecutor::TestCompleted(TInt aError)
    {
    iTestCompletedError = aError;
    CActiveScheduler::Stop();
   
    }

void CExecutor::LogErrorAndLeaveL(const TDesC& aFunction,
        const TDesC& aDescription, const TInt aError)
    {
    RDebug::Print(_L("%S: %S [%d]"), &aFunction, &aDescription, aError);
    User::Leave(aError);
    }

