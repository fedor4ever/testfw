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
* Description: CCmdParser is used to parse the command line parameters. 
*
*/

#ifndef CMDPARSER_H_
#define CMDPARSER_H_

#include <e32hashtab.h> 
#include <e32std.h>
#include <e32base.h>
#include <e32cmn.h> 

class CCmdParser : public CBase
    {
public:
    static CCmdParser* NewL();
    ~CCmdParser();
    
public:
    TBool Parse(TPtr& commandLine);
    TBool CaseContains(TInt index, TDesC& caseName);
    inline TInt CaseCount(){return iTestCaseIndexList.Count() + iTestCaseList.Count();}
    inline TName TestModule() {return iTestModule;}
    inline TFileName ConfigFile() {return iConfigFile;}
    inline TFileName EngineIniFile() {return iEngineIniFile;}
    inline TFileName ModuleIniFile() {return iModuleIniFile;}
    inline TBool IsVersion(){return iIsVersion;}
    inline TBool IsHelp() {return iIsHelp;}
    inline RArray<TName> TestCaseList() {return iTestCaseList;}
    inline RArray<TInt> TestCaseIndexList() {return iTestCaseIndexList;}
    inline TDes& EngineConfigList() {return iEngineConfigList;}
    inline TDes& LogConfigList() {return iLogConfigList;}
    inline TBool IsPrompt() {return iIsPrompt;}
    
private:
    CCmdParser();
    TBool IsNumber(const TDesC & aDes);
    
private:
    TName iTestModule;
    TFileName iConfigFile;
    TFileName iEngineIniFile;
    TFileName iModuleIniFile;
    TBuf<256> iEngineConfigList;
    TBuf<256> iLogConfigList;
    RArray<TName> iTestCaseList;
    
    RArray<TInt> iTestCaseIndexList;
    
    TName iTestCase;
    TInt iTestCaseIndex;
    TBool iIsVersion;
    TBool iIsHelp;
    TBool iIsPrompt;
    
    };


#endif /* CMDPARSER_H_ */
