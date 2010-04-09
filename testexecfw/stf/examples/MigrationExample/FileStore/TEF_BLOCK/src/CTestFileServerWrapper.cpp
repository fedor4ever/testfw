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

#include "CTestFileServerWrapper.h"
_LIT(KCreateFile, "CreateFile");
_LIT(KReadFile, "ReadFile");
_LIT(KPanicExample, "PanicExample");
_LIT(KFileName, "FileName");
_LIT(KPanicString, "PanicString");
_LIT(KPanicCode, "PanicCode");

CTestFileServerWrapper::CTestFileServerWrapper() :
    iObject(NULL)
    {
    }

CTestFileServerWrapper::~CTestFileServerWrapper()
    {
    delete iObject;
    iObject = NULL;
    }

CTestFileServerWrapper*CTestFileServerWrapper::NewL()
    {
    CTestFileServerWrapper* ret = new (ELeave) CTestFileServerWrapper();
    CleanupStack::PushL(ret);
    ret->ConstructL();
    CleanupStack::Pop(ret);
    return ret;
    }

void CTestFileServerWrapper::ConstructL()
    {
    iActiveCallback = CActiveCallback::NewL(*this);
    // TODO : Initialize wrppped object

    }

void CTestFileServerWrapper::RunL(CActive* aActive, TInt aIndex)
    {
    TInt err = aActive->iStatus.Int();
    SetAsyncError(aIndex, err);
    DecOutstanding();
    }

TBool CTestFileServerWrapper::DoCommandL(const TTEFFunction& aCommand,
        const TTEFSectionName& aSection, const TInt /*aAsyncErrorIndex*/)
    {
    TBool ret = ETrue;

    if (KCreateFile() == aCommand)
        {
        // Testing CTestFileServer::CreateFile
        DoCmdCreateFile(aSection);
        }
    else if (KReadFile() == aCommand)
        {
        // Testing CTestFileServer::ReadFile
        DoCmdReadFile(aSection);
        }
    else if (KPanicExample() == aCommand)
        {
        DoCmdPanicExample(aSection);
        }
    else
        {
        ret = EFalse;
        }
    return ret;
    }

// Testing CTestFileServer::CreateFile
void CTestFileServerWrapper::DoCmdCreateFile(const TDesC& aSection)
    {
    INFO_PRINTF1(_L("Running CreateFile()"));
    // TODO : Add a method test here
    // iObject->CreateFile
    TInt err = 0;
    TPtrC fileName;

    if (!GetStringFromConfig(aSection, KFileName, fileName))
        {
        ERR_PRINTF1(_L("File Name noinline void DoCmdCreateFile(const TTEFSectionName& aSection); specified"));
        SetBlockResult(EFail);
        }
    else
        {
        INFO_PRINTF2(_L("File to be opened is= %S "),&fileName);

        RFs fileSession;
            TRAP(err, fileSession.Connect())
        if (err != KErrNone)
            {
            ERR_PRINTF1(_L("Failed to connect to the file server"));
            SetBlockResult(EFail);
            }
        else
            {
            RFile file;

            CleanupClosePushL(fileSession);
            err = file.Create(fileSession, fileName, EFileRead | EFileWrite);
            if (err == KErrNone)
                {
                INFO_PRINTF1(_L("===>Opening file passed"));
                }
            else
                {
                ERR_PRINTF2(_L("--->Error in opening file error returned %D"), err);
                SetError(err);    
                }
            CleanupStack::PopAndDestroy(&fileSession);
            }
        }
    // TODO : Please make changes for checking your test result
    //SetBlockResult(EPass);
    }

// Testing CTestFileServer::ReadFile
void CTestFileServerWrapper::DoCmdReadFile(const TDesC& aSection)
    {
    INFO_PRINTF1(_L("Running ReadFile()"));
    // TODO : Add a method test here
    // iObject->ReadFile
        {
        TInt err = 0;
        TPtrC fileName;

        INFO_PRINTF1(_L("In test step ReadFile"));

        //Getting the file name from ini file 
        if (!GetStringFromConfig(aSection, KFileName, fileName))
            {
            ERR_PRINTF1(_L("File Name not specified"));
            SetBlockResult(EFail);
            }
        else
            {
            INFO_PRINTF2(_L("File to be opened is= %S "),&fileName);
            }

        if (BlockResult() == EPass)
            {
            RFs fileSession;
                //Connecting to the file server
                TRAP(err, fileSession.Connect())
            if (err != KErrNone)
                {
                ERR_PRINTF1(_L("Failed to connect to the file server"));
                SetError(err);
                }
            else
                {
                RFile file;

                CleanupClosePushL(fileSession);
                //Opening the file
                err = file.Open(fileSession, fileName, EFileRead);
                if (err != KErrNone)
                    {
                    ERR_PRINTF1(_L("Error in Opening file"));
                    SetError(err);
                    }
                else
                    {
                    CleanupClosePushL(file);
                    TInt fileSize = 0;
                    err = file.Size(fileSize);
                    if (err == KErrNone)
                        {
                        HBufC8 *data = HBufC8::NewLC(fileSize);
                        TPtr8 ptr = data->Des();
                        //      RaiseInstrumentationEventNotificationL(MTestInstrumentation::EPointFilestoreReadFileStart); 
                        err = file.Read(ptr);
                        //      RaiseInstrumentationEventNotificationL(MTestInstrumentation::EPointFilestoreReadFileStop); 
                        if (err != KErrNone)
                            {
                            ERR_PRINTF1(_L("Error in reading the file"));
                            SetError(err);
                            }
                        else
                            {
                            INFO_PRINTF1(_L("Read File passed with no error code"));
                            SetBlockResult(EPass);
                            }
                        CleanupStack::PopAndDestroy(data);
                        }
                    else
                        {
                        ERR_PRINTF1(_L("Error in finding size of the file"));
                        SetError(err);
                        }
                    file.Close();
                    CleanupStack::PopAndDestroy(&file);

                    }
                CleanupStack::PopAndDestroy(&fileSession);
                }
            }

        // TODO : Please make changes for checking your test result    
        }
        SetBlockResult(EPass);
    }

void CTestFileServerWrapper::DoCmdPanicExample(const TDesC& aSection)
    {
    INFO_PRINTF1(_L("Running DoCmdPanicExample()"));
    //Getting the panic name from ini file 
    TInt panicCode = 0;
    TPtrC panicString;
    if (!GetStringFromConfig(aSection, KPanicString, panicString))
        {
        ERR_PRINTF1(_L("Panic string not specified"));
        SetBlockResult(EFail);
        }
    else
        {
        INFO_PRINTF2(_L("Get Panic String= %S "),&panicString);
        }
    if (!GetIntFromConfig(aSection, KPanicCode, panicCode))
        {
        ERR_PRINTF1(_L("Panic code not specified"));
        SetBlockResult(EFail);
        }
    else
        {
        INFO_PRINTF2(_L("Get Panic Code= %d "),panicCode);
        }
    
    User::Panic(panicString, panicCode);
    
    }
