/*------------------------------------------------------------------
 -
 * Software Name : UserEmulator
 * Version       : v4.2.1309
 * 
 * Copyright (c) 2009 France Telecom. All rights reserved.
 * This software is distributed under the License 
 * "Eclipse Public License - v 1.0" the text of which is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * France Telecom 
 *
 * Contributors:
 *------------------------------------------------------------------
 -
 * File Name: Settings.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used for storing the settings information of the application
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <coemain.h>    // CCoeEnv
#include <BAUTILS.H>

//User Includes
#include "Settings.h"
#include "UserEmulatorApplication.h"
#include "UserEmulator.hrh"

//Constants
_LIT(KFormat,":\\");
_LIT(KFakeFileName,"filename.csv");

// -----------------------------------------------------------------------------
// CSettings::CSettings
// Constructor 
// -----------------------------------------------------------------------------
//
CSettings::CSettings(CApaApplication& aApp, CEikonEnv* aEikonEnv) : iApp(aApp), iEEnv(aEikonEnv)
{
}
// -----------------------------------------------------------------------------
// CSettings::ConstructL
// -----------------------------------------------------------------------------
//
void CSettings::ConstructL()
{
    LoadL();
}
// -----------------------------------------------------------------------------
// CSettings::CheckDrive
// Function to validate the correct drive
// -----------------------------------------------------------------------------
//
TInt CSettings::CheckDrive(const TDesC& aPath)
{
	TInt err;
	TVolumeInfo volumeInfo;
	
	TInt pos = aPath.FindF(KFormat);
	if(pos == KErrNotFound)
		return KErrNotReady;
	
	TPtrC drive = aPath.Left(pos);
	if(drive.Length()!= 1)
		err = KErrNotReady;
	else
	{
	    TUint driveLetter = drive.operator [](0);
	    TInt driveNumber = 0;
	    iEEnv->FsSession().CharToDrive((TChar)driveLetter,driveNumber);
	    if(driveNumber == EDriveD)
	   	       	return KErrNotReady;
	    err = iEEnv->FsSession().Volume(volumeInfo,driveNumber);
	}
	return err;
}
// -----------------------------------------------------------------------------
// CSettings::CheckPathL
// Function to validate the scripts or log file path
// -----------------------------------------------------------------------------
//
TBool CSettings::CheckPathL(const TDesC& aPath)
{
	TBuf<KBuffer256> TempPath;
	TempPath.Copy(aPath);
	// Just because of IsValidName. (Without filename it fails)
	TempPath.Append(KFakeFileName);
	if(iEEnv->FsSession().IsValidName(TempPath))
		{
		TBuf<KBuffer256> path;
		TBuf<KBuffer256> tempPath;
		TBool space=EFalse;
		TInt pos;
		_LIT(KSpace," ");
		
		// No space before backslash in path
		tempPath.Copy(aPath);
		while((pos=tempPath.LocateReverse('\\'))!=KErrNotFound)
			{
			TPtrC a = tempPath.Mid(pos-1,1);
			if(a.Compare(KSpace)==0)
				{
				space=ETrue;
				break;
				}
			tempPath.Copy(tempPath.Left(pos));
			}
		
		if(!space)
			{
			TInt pos=aPath.LocateReverse('\\')+1;
			if(pos!=KErrNotFound)
				path.Copy(aPath.Left(pos));
			else
				return EFalse;
			TRAPD(err,BaflUtils::EnsurePathExistsL(iEEnv->FsSession(), path));
			if(err==KErrNone)
				return ETrue;
			}
		}
	return EFalse;
}
// -----------------------------------------------------------------------------
// CSettings::LoadL
// Function to Load the User Emulator settings
// -----------------------------------------------------------------------------
//
void CSettings::LoadL()
{
	CDictionaryStore* iniFile = iApp.OpenIniFileLC(CCoeEnv::Static()->FsSession());

    if (iniFile->IsPresentL(KUidUserEmulatorApp))
    {
    	RDictionaryReadStream readStream;
        readStream.OpenLC(*iniFile, KUidUserEmulatorApp);
        
        TInt len = static_cast<TInt> (readStream.ReadInt32L());
        readStream.ReadL(iScriptsPath,len);
        
        TInt ScriptPath = CheckDrive(iScriptsPath);
        if(ScriptPath == KErrNotReady)
              iScriptsPath = KDefaultPath;
        
        iLogStatus = static_cast<TBool> (readStream.ReadInt8L());
        TInt logLen = static_cast<TInt> (readStream.ReadInt32L());
        readStream.ReadL(iLogPath,logLen);
        
        TInt err = CheckDrive(iLogPath);

        if(err == KErrNotReady)
        	iLogPath = KDefaultLogPath;
        
        iRandomTestAppIdStatus = static_cast<TBool> (readStream.ReadInt8L());
        TInt IDLen = static_cast<TInt> (readStream.ReadInt32L());
        readStream.ReadL(iRandomTestAppID,IDLen);
        
        CleanupStack::PopAndDestroy(&readStream);
    }
    else
    {	    	
    	iScriptsPath  = KDefaultPath;
    	iLogStatus    = ETrue;
    	iLogPath      = KDefaultLogPath;
    	iRandomTestAppIdStatus = EFalse;
    }

    CleanupStack::PopAndDestroy(iniFile);
}
// -----------------------------------------------------------------------------
// CSettings::StoreL
// Function to Store the User Emulator settings
// -----------------------------------------------------------------------------
//
void CSettings::StoreL()
{
    CDictionaryStore* iniFile = iApp.OpenIniFileLC(CCoeEnv::Static()->FsSession());
    RDictionaryWriteStream writeStream;
    writeStream.AssignLC(*iniFile, KUidUserEmulatorApp);
    
    writeStream.WriteInt32L(iScriptsPath.Length());
    writeStream.WriteL(iScriptsPath);
    writeStream.WriteInt8L(iLogStatus);
    writeStream.WriteInt32L(iLogPath.Length());
    writeStream.WriteL(iLogPath);
    writeStream.WriteInt8L(iRandomTestAppIdStatus);
    writeStream.WriteInt32L(iRandomTestAppID.Length());
    writeStream.WriteL(iRandomTestAppID);

    writeStream.CommitL();
    iniFile->CommitL();
    
	CleanupStack::PopAndDestroy(2, iniFile);
}
