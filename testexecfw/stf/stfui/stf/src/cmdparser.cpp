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
#include "cmdparser.h"
#include <StifTestInterface.h>


_LIT( KTestModule, "-m" );
_LIT( KConfigFile, "-s" );
_LIT( KEngineIniFile, "-i" );
_LIT( KModuleIniFile, "-c" );
_LIT( KEngineConfigurationItem, "-engine" );
_LIT( KLogConfigurationItem, "-log" );
_LIT( KVersion1, "-v" );
_LIT( KVersion2, "-version" );
_LIT( KHelp1, "-?" );
_LIT( KHelp2, "-h" );
_LIT( KHelp3, "-help" );
_LIT( KNoPrompt, "-NOPROMPT");
_LIT( KDefaultEngineIni, "c:\\TestFramework\\TestFramework.ini" );




CCmdParser::CCmdParser() : iIsHelp(EFalse), iIsVersion(EFalse), iIsPrompt(ETrue)
    {
    iEngineIniFile.Zero();
    iModuleIniFile.Zero();
    iConfigFile.Zero();
    iTestModule.Zero();
    iTestCase.Zero();
    
    }

CCmdParser::~CCmdParser()
    {
    iTestCaseList.Close();
    iTestCaseIndexList.Close();
       
    }

CCmdParser* CCmdParser::NewL()
    {
    CCmdParser* self = new(ELeave)CCmdParser();
    return self;
    }

TBool CCmdParser::Parse(TPtr& commandLine)
    {
    TBool rst(ETrue);
    TBool findmodule(EFalse);
    TLex lex(commandLine);
    TBufC<1> comma(_L("\""));
    TBool continueName(EFalse); //parse case name, when count \" need continue read name.
    while ( !lex.Eos() )
        {
        TPtrC tmpPtr = lex.NextToken();
        if(tmpPtr == KTestModule)
            {
            TPtrC module = lex.NextToken();
            if(module.Ptr())
                {
                iTestModule.Copy(module);
                if(iTestModule != _L(""))
                    {
                    findmodule = ETrue;    
                    }
                }
            else
                {
                rst = EFalse;
                break;
                }
            }
        else if(tmpPtr == KConfigFile)
            {
            TPtrC config = lex.NextToken();
            if(config.Ptr())
                {
                //iTestModule.Copy(_L("testscripter"));//mark here.
                iConfigFile.Copy(config);
                TStifUtil::CorrectFilePathL(iConfigFile);
                if(iConfigFile != _L(""))
                    {
                    findmodule = ETrue;    
                    }
                }
            else
                {
                rst = EFalse;
                break;
                }
            }
        else if(tmpPtr == KEngineIniFile)
            {
            TPtrC iniFile = lex.NextToken();
            if(iniFile.Ptr())
                {
                iEngineIniFile.Copy(iniFile);
                TStifUtil::CorrectFilePathL(iEngineIniFile);
                }
            else
                {
                rst = EFalse;                
                break;
                }
            }
        else if(tmpPtr == KModuleIniFile)
            {
            TPtrC iniFile = lex.NextToken();
            if(iniFile.Ptr())
                {
                iModuleIniFile.Copy(iniFile);
                TStifUtil::CorrectFilePathL(iModuleIniFile);
                }
            else
                {
                rst = EFalse;
                break;
                }
            }
       
        else if(tmpPtr == KEngineConfigurationItem)
            {
            TName pname=lex.NextToken();
            
            if(pname.Length() > 0)
                {
                TPtrC pval = lex.NextToken();            
                if(pval.Ptr())
                    {
                    
                    iEngineConfigList.Append(pname);
                    iEngineConfigList.Append(_L(" "));
                    iEngineConfigList.Append(pval);
                    iEngineConfigList.Append(_L(" "));
                    
                    }
                else
                    {
                    rst = EFalse;
                    break;
                    }
                }
            else
                {
                rst = EFalse;
                break;
                }
            }
        else if(tmpPtr == KLogConfigurationItem)
            {
            TName pname=lex.NextToken();
            if(pname.Length() > 0)
                {
                TPtrC pval = lex.NextToken();            
                if(pval.Ptr())
                    {
                    
                    iLogConfigList.Append(pname);
                    iLogConfigList.Append(_L(" "));
                    iLogConfigList.Append(pval);
                    iLogConfigList.Append(_L(" "));
                    }
                else
                    {
                    rst = EFalse;
                    break;
                    }
                }
            else
                {
                rst = EFalse;
                break;
                }
            
            }
        else if(tmpPtr == KVersion1 || tmpPtr == KVersion2)
            {
            iIsVersion = true;
            rst = ETrue;
            break;            
            }
        else if(tmpPtr == KHelp1 || tmpPtr == KHelp2 || tmpPtr == KHelp3)
            {
            iIsHelp = true;
            rst = ETrue;
            break;
            }
        else if(tmpPtr == KNoPrompt)
            {
            iIsPrompt = false;
            }
        else
            {
            if(!tmpPtr.Left(1).Compare(comma))
                {
                continueName = ETrue;
                iTestCase.Copy(tmpPtr);
                }
            else
                {
                if(continueName)
                    {
                    iTestCase.Append(_L(" "));
                    iTestCase.Append(tmpPtr);
                    }
                else
                    {
                    //is a number without "\""
                    if(IsNumber(tmpPtr))
                        {
                        TLex myLex(tmpPtr);
                        myLex.Val(iTestCaseIndex);  
                        iTestCaseIndexList.Append(iTestCaseIndex);
                        continue;
                        }
                    else
                        {
                        iTestCase.Copy(tmpPtr);                    
                        }
                    
                    }
                }
            

            
            if(!iTestCase.Right(1).Compare(comma))
                {
                if(continueName)
                    {
                    iTestCase.Copy(iTestCase.Mid(1, iTestCase.Length() -2));
                    iTestCaseList.Append(iTestCase);
                    }
                //error input.
                iTestCase.Copy(_L(""));
                continueName = EFalse;
                }
            else
                {
                if(!continueName)
                    {
                    iTestCaseList.Append(iTestCase);
                    iTestCase.Copy(_L(""));
                    }
                }
            

            }
        }//end while
    if(iConfigFile.Length() != 0 && iTestModule.Length() == 0)
        {
        iTestModule.Copy(_L("testscripter"));
        }
    if(iEngineIniFile.Length()==0)
        {
        iEngineIniFile.Append(KDefaultEngineIni);
        }
    return (rst && findmodule);
    }


TBool CCmdParser::CaseContains(TInt index, TDesC& caseName)
    {
    //RArray<TName> iTestCaseList;
    //RArray<TInt> iTestCaseIndexList;
    TInt size = iTestCaseIndexList.Count();
    for(TInt i=0;i<size; i++)
        {
        if(iTestCaseIndexList[i] == index)
            {
            return ETrue;
            }
        }
    size = iTestCaseList.Count();
    for(TInt i=0;i<size;i++)
        {
        if(!iTestCaseList[i].Compare(caseName))
            {
            return ETrue;
            }
        }
    return EFalse;
    }

TBool CCmdParser::IsNumber(const TDesC & aDes)
    {
    TBuf<2> tempChar;
    for (TInt i=0;i<aDes.Length();i++)
        {
        tempChar.Zero();
        tempChar.Append(aDes.Mid(i,1));
        TLex myLex(tempChar);
        TInt32 model;
        if (myLex.Val(model))
            {
            return  EFalse;
            }
    }
    return ETrue;
    }

