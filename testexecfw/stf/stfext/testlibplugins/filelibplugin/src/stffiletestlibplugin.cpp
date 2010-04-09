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

#include "stffiletestlibplugin.h"	
#include <f32file.h> 

_LIT(KSTFRunUtilsMakeReadWrite,"MakeReadWrite");
_LIT(KSTFRunUtilsCopyFile,"CopyFile");
_LIT(KSTFRunUtilsMkDir,"MkDir");
_LIT(KSTFRunUtilsDeleteFile,"DeleteFile");
_LIT(KSTFRunUtilsDelete,"Delete");
_LIT(KSTFRunUtilsDeleteDirectory,"DeleteDirectory");
EXPORT_C  CStfFileTestLibPlugin* LibEntryL()
    {
    CStfFileTestLibPlugin* self=CStfFileTestLibPlugin::NewL();
    return  self;
    }



//  Member Functions

CStfFileTestLibPlugin* CStfFileTestLibPlugin::NewL()
    {
    CStfFileTestLibPlugin* self = new (ELeave) CStfFileTestLibPlugin;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
CStfFileTestLibPlugin::~CStfFileTestLibPlugin()
    {

    }




EXPORT_C TBool CStfFileTestLibPlugin::IsCommandSupported(const TDesC& aCommand)
    {
    TBool result=EFalse;
    if(aCommand==KSTFRunUtilsMakeReadWrite)
        {
        result=ETrue;
        }
    else if(aCommand==KSTFRunUtilsCopyFile)
        {
        result=ETrue;
        }
    else if(aCommand==KSTFRunUtilsMkDir)
        {
        result=ETrue;
        }
    else if(aCommand==KSTFRunUtilsDeleteFile)
        {
        result=ETrue;
        }
    else if(aCommand==KSTFRunUtilsDelete)
        {
        result=ETrue;
        }
    else if(aCommand==KSTFRunUtilsDeleteDirectory)
        {
        result=ETrue;
        }
    
    return result;
    }

EXPORT_C TInt CStfFileTestLibPlugin::ExecuteCommandL(const TDesC& aCommandLine) 
    {
    TInt err = 0;
    _LIT(KDefault,              "?:\\default");
    _LIT(KCDrive,               "?:\\"); 
    _LIT(KSTFBackslash,         "\\" );
    TBuf<10> defaultPath(KDefault);
    defaultPath.Replace(0, 2, _L("c:"));

    TBuf<3> cDrive(KCDrive);
    cDrive.Replace(0, 2, _L("c:"));

    RFs fS;
    User::LeaveIfError(fS.Connect() );
    CleanupClosePushL(fS);

    TLex lex(aCommandLine);
  //  lex.NextToken();
    TPtrC token(lex.NextToken());
    if(token.CompareF(KSTFRunUtilsCopyFile) == 0)
        {
        TPtrC file1=lex.NextToken();
        TPtrC file2=lex.NextToken();
        TParse source, dest;
        CFileMan* fMan = CFileMan::NewL(fS);
        CleanupStack::PushL(fMan);
        User::LeaveIfError(source.Set(file1, &defaultPath, NULL) );
        User::LeaveIfError(dest.Set(file2, &defaultPath, NULL) );
        TPtrC sourcefile=source.FullName();
        TPtrC destfile=dest.FullName();
        err=fMan->Copy(file1, file2, CFileMan::EOverWrite);
        User::LeaveIfError(err);
        CleanupStack::PopAndDestroy(fMan);
        }
    else if(token.CompareF(KSTFRunUtilsMkDir) == 0)
        {
        token.Set(lex.NextToken());
        TParse fileName;

        if (!token.Length())
            User::Leave(KErrPathNotFound);
        
        TPtrC lastChar(token.Mid(token.Length() - 1));
        if ( lastChar.CompareF(KSTFBackslash) != 0 )
            {
            TBuf<64> tempToken(token);
            tempToken.Append(KSTFBackslash);
            token.Set(tempToken);
            }

        User::LeaveIfError( fileName.Set(token, &cDrive, NULL) );
        err=fS.MkDir( fileName.DriveAndPath() );
        User::LeaveIfError( err );
        }
    else if(token.CompareF(KSTFRunUtilsDeleteFile) == 0 ||
             token.CompareF(KSTFRunUtilsDelete) == 0)
        {
        token.Set(lex.NextToken());
  
        CFileMan* fMan = CFileMan::NewL(fS);
        CleanupStack::PushL(fMan);
        err=fMan->Delete(token);
        User::LeaveIfError(err );
        CleanupStack::PopAndDestroy(fMan);
        }
    else if(token.CompareF(KSTFRunUtilsMakeReadWrite) == 0)
        {
        token.Set(lex.NextToken());
        TParse fileName;
        User::LeaveIfError(fileName.Set(token, &defaultPath, NULL) );
        err = fS.SetAtt(fileName.FullName(),0, KEntryAttReadOnly);
        if (err != KErrNone && err != KErrNotFound)
            User::Leave(err);
        }
    //It deletes the specified directory
    else if(token.CompareF(KSTFRunUtilsDeleteDirectory) == 0)
        {
        token.Set(lex.NextToken());
        TParse fileName;

        if (!token.Length())
            User::Leave(KErrPathNotFound);
        
        TPtrC lastChar(token.Mid(token.Length() - 1));
        if ( lastChar.CompareF(KSTFBackslash) != 0 )
            {
            TBuf<64> tempToken(token);
            tempToken.Append(KSTFBackslash);
            token.Set(tempToken);
            }

        CFileMan* fMan = CFileMan::NewL(fS);
        CleanupStack::PushL(fMan);

        User::LeaveIfError(fileName.Set(token, &cDrive, NULL) );
        err=fMan->RmDir( fileName.DriveAndPath() );
        User::LeaveIfError(err  );

        CleanupStack::PopAndDestroy(fMan);
        }
    
    fS.Close();
    CleanupStack::Pop(&fS);
    return err;
    }

CStfFileTestLibPlugin::CStfFileTestLibPlugin()
// note, CBase initialises all member variables to zero
    {
    }

void CStfFileTestLibPlugin::ConstructL()
    {
    // second phase constructor, anything that may leave must be constructed here
  
    }



