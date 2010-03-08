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
 * File Name: Logger.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used for logging purposes
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <eikenv.h>     // CEikonEnv
#include <f32file.h>    // TParsePtrC
#include <bautils.h>    // BaflUtils
#include <aknnotewrappers.h>

//User Includes
#include "Logger.h"
#include "Settings.h"

//Constants
_LIT8(KTimeFormat,"%04d-%02d-%02d, %02d:%02d:%02d:%02d");
_LIT8(KHashTimeFormat,"#%04d-%02d-%02d, %02d:%02d:%02d:%02d");
_LIT(KDriveError, "Drive might be locked. Missing or incomplete log file possible.");
const TInt KTimeRecordSize = 26;
// -----------------------------------------------------------------------------
// CLogger::ConstructL
// -----------------------------------------------------------------------------
//
void CLogger::ConstructL()
{
	TParsePtrC parser(iSettings.iLogPath);
    BaflUtils::EnsurePathExistsL(iEEnv->FsSession(), parser.DriveAndPath());
    User::LeaveIfError(iFs.Connect());
    iDriveLockError = EFalse;
}

// -----------------------------------------------------------------------------
// CLogger::CLogger
// -----------------------------------------------------------------------------
//
CLogger::CLogger(CUserEmulatorScriptsView& aScriptView,CSettings& aSettings, CEikonEnv* aEikonEnv)
		: iScriptView(aScriptView),iSettings(aSettings),iEEnv(aEikonEnv)
{
}

// -----------------------------------------------------------------------------
// CLogger::~CLogger
// -----------------------------------------------------------------------------
//
CLogger::~CLogger()
{
    iFile.Close();
    iFs.Close();
}
// -----------------------------------------------------------------------------
// CLogger::WriteLogL
// -----------------------------------------------------------------------------
//
void CLogger::WriteLogL(const TDesC16& aText, TBool aHeader, TBool aHash)
{
	if(Write(aText,aHeader,aHash)!=KErrNone)
		{
		if(!iDriveLockError)
			{
			iDriveLockError=ETrue;
			CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
			errorNote->ExecuteLD(KDriveError);
			}
		}
	else
		iDriveLockError=EFalse;
}
// -----------------------------------------------------------------------------
// CLogger::Write
// -----------------------------------------------------------------------------
//
TInt CLogger::Write(const TDesC16& aText, TBool aHeader, TBool aHash)
{
	if( iSettings.iLogStatus)
	{
	TBool bExists = BaflUtils::FileExists(iFs, iSettings.iLogPath);
	if(!bExists)
	{
		TParsePtrC parser(iSettings.iLogPath);
	    TRAPD(err, BaflUtils::EnsurePathExistsL(iEEnv->FsSession(), parser.DriveAndPath()));
	    if(err != KErrNone)
	    	return err;
	    CloseFileHandles();
	    TInt err1=iFile.Replace(iFs, iSettings.iLogPath, EFileShareAny | EFileWrite);
	    if(err1 != KErrNone)
	    	return err1;
	    
	    TRAPD(err2,iScriptView.CmdGetSystemInfoL(ETrue));
	}
	else
	{
		//File exists..Check if the file is already open
		TBool isOpen;
		TInt err = iFs.IsFileOpen(iSettings.iLogPath,isOpen);
		TInt err1=KErrNone;
		if(err != KErrNone)
			return err;
		
		if(!isOpen)
		{
			err1=iFile.Replace(iFs, iSettings.iLogPath, EFileShareAny | EFileWrite);
			if(err1!=KErrNone)
				return err1;
			TRAPD(err,iScriptView.CmdGetSystemInfoL(ETrue));
		}
		
	}
		
	iLogBuffer.Copy(aText);

	if(!aHeader)
	{
		TTime time;
		time.HomeTime();
		TDateTime dateTime;
		dateTime = time.DateTime();
		TBuf8<KTimeRecordSize> timeRecord;
		if(aHash)
			timeRecord.Format(KHashTimeFormat, dateTime.Year(), dateTime.Month()+1, 
					dateTime.Day()+1, dateTime.Hour(), dateTime.Minute(), dateTime.Second(),dateTime.MicroSecond()/1000);
		else
			timeRecord.Format(KTimeFormat, dateTime.Year(), dateTime.Month()+1, 
					dateTime.Day()+1, dateTime.Hour(), dateTime.Minute(), dateTime.Second(),dateTime.MicroSecond()/1000);
		timeRecord.Append(KComma);
		iLogBuffer.Insert(0, timeRecord);
	}
	TInt res=iFile.Write(iLogBuffer);
	if(res!=KErrNone)
		return res;
	res=iFile.Flush();	
	if(res!=KErrNone)
		return res;
	}
	return KErrNone;
}
// -----------------------------------------------------------------------------
// CLogger::WriteLogL
// -----------------------------------------------------------------------------
//
void CLogger::WriteLogL(const TDesC8& aText, TBool aHeader, TBool aHash)
{
	if(Write(aText,aHeader,aHash)!=KErrNone)
		{
		if(!iDriveLockError)
			{
			iDriveLockError=ETrue;
			CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
			errorNote->ExecuteLD(KDriveError);
			}
		}
	else
		iDriveLockError=EFalse;
}
// -----------------------------------------------------------------------------
// CLogger::Write
// -----------------------------------------------------------------------------
//
TInt CLogger::Write(const TDesC8& aText, TBool aHeader, TBool aHash)
{
	if( iSettings.iLogStatus)
	{
	TBool bExists = BaflUtils::FileExists(iFs, iSettings.iLogPath);
	if(!bExists)
	{
		TParsePtrC parser(iSettings.iLogPath);
	    TRAPD(err, BaflUtils::EnsurePathExistsL(iEEnv->FsSession(), parser.DriveAndPath()));
	    if(err != KErrNone)
	    	return err;
	    CloseFileHandles();
	    TInt err1=iFile.Replace(iFs, iSettings.iLogPath, EFileShareAny | EFileWrite);
	    if(err1 != KErrNone)
	 	    return err1;
	    
	    TRAPD(err2,iScriptView.CmdGetSystemInfoL(ETrue));
	}
	else
	{
		//File exists..Check if the file is already open
		TBool isOpen;
		TInt err = iFs.IsFileOpen(iSettings.iLogPath,isOpen);
		TInt err1=KErrNone;
		if(err != KErrNone)
			return err;
		
		if(!isOpen)
		{
			err1=iFile.Replace(iFs, iSettings.iLogPath, EFileShareAny | EFileWrite);
			if(err1!=KErrNone)
				return err1;
			TRAPD(err,iScriptView.CmdGetSystemInfoL(ETrue));
		}
	}
	iLogBuffer.Copy(aText);
	
	//Log Date and time
	_LIT8(KComma, ",");
	_LIT8(KTimeFormat,"%04d-%02d-%02d %02d:%02d:%02d:%02d");
	_LIT8(KHashTimeFormat,"#%04d-%02d-%02d %02d:%02d:%02d:%02d");
 
	const TInt KTimeRecordSize = 20;
 
	if(!aHeader)
	{
		TTime time;
		time.HomeTime();
		TDateTime dateTime;
		dateTime = time.DateTime();
		TBuf8<KTimeRecordSize> timeRecord;
		if(aHash)
			timeRecord.Format(KHashTimeFormat, dateTime.Year(), dateTime.Month()+1, 
					dateTime.Day()+1, dateTime.Hour(), dateTime.Minute(), dateTime.Second(),dateTime.MicroSecond()/1000);
		else
			timeRecord.Format(KTimeFormat, dateTime.Year(), dateTime.Month()+1, 
					dateTime.Day()+1, dateTime.Hour(), dateTime.Minute(), dateTime.Second(),dateTime.MicroSecond()/1000);
		timeRecord.Append(KComma);
		iLogBuffer.Insert(0, timeRecord);
	}
	
	TInt res=iFile.Write(iLogBuffer);
	if(res!=KErrNone)
		return res;
	res=iFile.Flush();	
	if(res!=KErrNone)
		return res;
	
	}	
	return KErrNone;
}
// -----------------------------------------------------------------------------
// CLogger::CloseFileHandles
// -----------------------------------------------------------------------------
//
void CLogger::CloseFileHandles()
{
	iFile.Close();
}
