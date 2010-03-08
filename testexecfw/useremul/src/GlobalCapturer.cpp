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
 * File Name: GlobalCapturer.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Key capturing class
 *------------------------------------------------------------------
 -
 *
 */
 
// System Includes
#include <e32base.h>
#include <e32std.h>
#include <e32cons.h>
#include <aknnotewrappers.h>
#include <apgwgnam.h>
#include <eikenv.h>
#include <apgtask.h>
#include <APGCLI.H>
#include <W32STD.H>
#include <bautils.h>
// User Includes
#include <UserEmulator_0x2001C3AF.rsg>
#include "GlobalCapturer.h"
#include "UserEmulatorApplication.h"

// Constants
const TUid KUidStandBy = { 0x102750f0 };

const TUint KKeyCodePowerKey  	= EKeyDevice2;
const TUint KCameraKey 			= 0xF887;
const TUint KEndKeyScanCode 	= 197;

const TInt KCaptureMaxPriority = 11;
const TInt KAppInfoLength = 10;
const TInt KZeroAscii = 48;
const TInt KZedAscii = 122;
const TInt KDotAscii = 46;

// ============================ MEMBER FUNCTIONS ===============================
// -------------------------------------------
// CGlobalCapturer::CGlobalCapturer
// -------------------------------------------
//
CGlobalCapturer::CGlobalCapturer(MKeyObserver& aObserver, CSettings& aSettings, TBool& aScriptRun, 
		TBool& aRandomTestStarted, RClientCommander& aClientCommander, CEikonEnv* aEikonEnv, RApaLsSession& aSession) :
		CActive( EPriorityNormal ),iObserver(aObserver),iSettings(aSettings), iScriptRun(aScriptRun),
		iRandomTestStarted(aRandomTestStarted),iClientCommander(aClientCommander), iEEnv(aEikonEnv), iSession(aSession)
{
}
// -------------------------------------------
// CGlobalCapturer::NewL
// -------------------------------------------
//
CGlobalCapturer* CGlobalCapturer::NewL(MKeyObserver& aObserver, CSettings& aSettings, TBool& aScriptRun, 
		TBool& aRandomTestStarted, RClientCommander& aClientCommander, CEikonEnv* aEikonEnv,RApaLsSession& aSession)
	{
	CGlobalCapturer* me = new (ELeave) CGlobalCapturer(aObserver,aSettings,aScriptRun, 
			aRandomTestStarted,aClientCommander,aEikonEnv,aSession);
	CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
	return me;
	}

// -------------------------------------------
// Symbian 2nd phase constructor
// -------------------------------------------
//
void CGlobalCapturer::ConstructL(void)
	{
	iRecord = EFalse;
	iKeyCaptureStart = EFalse;
	
	iCameraAppTimer = CCameraAppTimer::NewL(*this);
	}

// -------------------------------------------
// Destructor
// -------------------------------------------
//
CGlobalCapturer::~CGlobalCapturer()
{
	Cancel();			
	delete iWindowGroup;
	iWsSession.Close();
	iFs.Close();
	
	if(iCameraAppTimer)
		{
	    delete iCameraAppTimer;
	    iCameraAppTimer = NULL;
	    }
}

// -------------------------------------------
// CActive cancel function
// -------------------------------------------
//
void CGlobalCapturer::DoCancel()
{	
	if ( IsActive() )
	{
		iWsSession.EventReadyCancel();
	}
}

// -------------------------------------------
// Initialise key capturing
// -------------------------------------------
//
void CGlobalCapturer::StartCapturingL() 
{
	// Connect to the window server
	User::LeaveIfError( iWsSession.Connect() );
	// Create an invisible window group. Well, we'll make it invisible later
	iWindowGroup = new (ELeave) RWindowGroup ( iWsSession );
	iWindowGroup->Construct( (TUint32)iWindowGroup, EFalse );
	
	CApaWindowGroupName* wn;
	wn=CApaWindowGroupName::NewLC(iWsSession);
	wn->SetHidden(ETrue);
	wn->SetWindowGroupName(*iWindowGroup);	
	CleanupStack::PopAndDestroy(wn);
	
	CActiveScheduler::Add( this );
}

// -------------------------------------------
// Starts capturing Power button key presses
// -------------------------------------------
//
void CGlobalCapturer::StartL()
{ 
    User::LeaveIfError( iCaptureHandle1 = iWindowGroup->CaptureKey( EKeyDevice2, 0, 0, KCaptureMaxPriority ) );	
  
	// Tell window server we are ready to receive events
	if ( !IsActive() )
	{
		iWsSession.EventReady( &iStatus );
		SetActive();
	}		
}

// -------------------------------------------
// Stops capturing Power button key presses
// -------------------------------------------
//
void CGlobalCapturer::Stop()
{
		Cancel();
		iWindowGroup->CancelCaptureKey( iCaptureHandle1 );
}

// --------------------------------------------------------------
// CActive RunL function. Called when a key press happened
// --------------------------------------------------------------
//
void CGlobalCapturer::RunL()
{
	if( iStatus == KErrNone ) 	
	{
		TWsEvent we;
		iWsSession.GetEvent( we );
		iKeyEvent=we.Key();
		CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEEnv->WsSession(), iEEnv->WsSession().GetFocusWindowGroup());
		TUid foreTaskUid = gn->AppUid();
		CleanupStack::PopAndDestroy(gn);
		
		// Stops key recording
		if(iKeyEvent->iCode == EKeyDevice2 && iRecord)
			{
			StopRecordKeysL();
			return;
			}
		// Stops random tests
		else if(iKeyEvent->iCode == EKeyDevice2 && iRandomTestStarted)
			{
			iObserver.RandomTestEndsL();
			return;
			}
		// Stops running scripts
		else if(iKeyEvent->iCode == EKeyDevice2)
			{
			Stop();
			iObserver.TerminationKeyReceived();
			return;
			}
		
		// Send event to the application which is in foreground			
		TInt appId = iEEnv->WsSession().GetFocusWindowGroup();
		iEEnv->WsSession().SendEventToWindowGroup(appId, we);											
		
		if ( !IsActive() )
		{
			iWsSession.EventReady( &iStatus );
			SetActive();
		}		
	}
	
}

// --------------------------------------------------------------
// Checks if camera app is in memory or not
// --------------------------------------------------------------
//
void CGlobalCapturer::CheckCameraAppL()
{
    _LIT8(KDecimal,"%d");
    _LIT8(KAsterisk,"*");
	// Get foreground app's name
	CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEEnv->WsSession(),iEEnv->WsSession().GetFocusWindowGroup());
	TApaAppInfo appInfo;
	RApaLsSession ApaLs;
	User::LeaveIfError(ApaLs.Connect());
	ApaLs.GetAppInfo(appInfo, gn->AppUid());
	// Store length!
	iAppName.Format(KDecimal,appInfo.iCaption.Length());
	if(appInfo.iCaption.Length()<KAppInfoLength)
		iAppName.Append(KAsterisk);
	
	iAppName.Append(appInfo.iCaption);
	CleanupStack::PopAndDestroy(gn);
	ApaLs.Close();
	
	iAppName.ZeroTerminate();
	TPtrC8 ptr;
	ptr.Set(iAppName);
	// Poll. (And send foreground app's name) 
	if( iClientCommander.AnimCommand( RClientCommander::EPoll, &ptr) == 1 )
	{
		TUid CamId;
		CamId = KUidCamera;
		TApaTaskList taskList(iEEnv->WsSession());
		TApaTask task = taskList.FindApp(CamId);
		if (task.Exists())
			task.EndTask();
		else
		  iClientCommander.AnimCommand( RClientCommander::ETaskExisted, NULL);
	}
	// Restart camera app timer
	iCameraAppTimer->After(KWait08); // 0.8 sec
}

// ---------------------------------------------
// Filters not appropriate characters
// Allows only chars. from 0 to z and '.'
// ---------------------------------------------
//
TBool CGlobalCapturer::StrictFileNameCheck(const TDesC& aFileName)
{
	TBool res=ETrue;
	for(TInt t=0;t<aFileName.Length();t++)
	{
		const TUint16* a=aFileName.Mid(t,1).Ptr();
		if((*a<KZeroAscii || *a>KZedAscii) && *a!=KDotAscii) // 46 -> '.'
			res=EFalse;
	}
	return res;
}
// ---------------------------------------------
// Starts key record with Windows server dll
// ---------------------------------------------
//
TInt CGlobalCapturer::StartRecordKeysL()
	{
	// Checks if there's another process started
	if(!iRecord && !iScriptRun && !iKeyCaptureStart)
		{
		_LIT16(Ktitle,"Output file name?");
		TBuf16<KBuffer256> aBuffer;
		
		TBool exit = EFalse;
		while(!exit)
        {
          // Asks for output file name
		  CAknTextQueryDialog* Dialog = CAknTextQueryDialog::NewL(aBuffer,CAknQueryDialog::ENoTone); 
		  Dialog->PrepareLC(R_FILE_DIALOG); 
		  Dialog->SetPromptL(Ktitle); 
		
		  if(Dialog->RunLD())
			{
				iBuf8.Copy(iSettings.iScriptsPath);
				TInt a = iBuf8.LocateReverse('\\');
				if(a!=KErrNotFound)
				{
				iBuf8 = iBuf8.Left(a+1);
				}
								
				iBuf8.Append(aBuffer);
				
				if(iBuf8.LocateReverse('.')==KErrNotFound)
				{
			    _LIT(Kxml,".xml");
			    iBuf8.Append(Kxml);
			    }
							
				TBuf<KBuffer512> TempPath;
				TempPath.Copy(iBuf8);
				// Validate file name
				if(iEEnv->FsSession().IsValidName(TempPath) && StrictFileNameCheck(aBuffer))
				{
					exit = ETrue;
				}
				else
				{
				    _LIT(KInvalid,"Invalid file name! Please try again.");
					CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
					errorNote->ExecuteLD(KInvalid);
					continue;
				}
				
				User::LeaveIfError(iFs.Connect());
				
				// If file exists then overwrite it or not?
				TBool overwrite=ETrue;
				TBuf16<KBuffer512> TempiBuf8;
				TBuf16<KBuffer8> empty;
				TempiBuf8.Copy(iBuf8);
				if(BaflUtils::FileExists(iFs, TempiBuf8))
				{
					_LIT8(Kk,"Overwrite existing file?");
					_LIT8(Kk1,"");
					TempiBuf8.Copy(Kk);
					empty.Copy(Kk1);
					if(iEEnv->QueryWinL(TempiBuf8,empty))
						overwrite = ETrue;
					else
						overwrite = EFalse;
				}
				
				iFs.Close();
			
				if(overwrite)
				{
					TPtrC8 ptr;
					ptr.Set(iBuf8);
					// Starts all event capture with the Window server object!
					TInt res=iClientCommander.AnimCommand( RClientCommander::EStart, &ptr);
					
					if(res==1)
					{
					    _LIT(KWritingError,"Script file writing error. Drive might be locked");
						CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
						errorNote->ExecuteLD(KWritingError);
						return 1;
					}
					else
					{
                       TBuf<KBuffer64> buff;
                       _LIT(KWait,"Wait. Script recording will start from the Idle screen..");
                       
                       CAknConfirmationNote* note = new (ELeave) CAknConfirmationNote(ETrue);
                       buff.Copy(KWait);
                       note->ExecuteLD(buff);
                       User::After(KWait02);
					   // Closes apps (there are exceptions) as a reset
					   KillUnwantedAppsResetL();
					
					   // Starts wait timer
                       iClientCommander.AnimCommand( RClientCommander::EStartTiming, NULL);
					
					   // Resets menu screen
					   if(ScreenReset()!=KErrNone)
						  return 1;
					   iKeyCaptureStart = ETrue;				
					   iRecord = ETrue;
	
	                   // Starts Power button capturing
					   StartL();
					   // Starts camera app timer
					   iCameraAppTimer->After(KWait05); // 0.5 sec
					  
					}	
					
				}
			}
			else
				return 1;
		  }// while end
		}
	else
		{
		_LIT(KRunning,"A script or recording is already running!");
		CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
		errorNote->ExecuteLD(KRunning);
		return 1;
		}
	
	return 0;
	}

// --------------------------------------------------------------------
// Closes Menu screen and resets idle screen by two No key presses
// --------------------------------------------------------------------
//
TInt CGlobalCapturer::ScreenReset()
	{
	// Reset Menu app.
	TApaTaskList taskList(iEEnv->WsSession());
	TApaTask task = taskList.FindApp(KUidMenu);
        
	task.EndTask();
	
	//Brings 'Menu' to foreground and resets screen selecting 'Exit'
	TApaTaskList taskList2(iEEnv->WsSession());
    TApaTask task2 = taskList2.FindApp(KUidHomeScreen); //? old phones
    task2.BringToForeground();
    User::After(KWait1);
    
    TRawEvent lEventDown;
    TRawEvent lEventUp;
    
    // No key press (Idle screen reset!) //? old phones (No key opens a menu on old phones)
    lEventDown.Set(TRawEvent::EKeyDown, KEndKeyScanCode); // No key
    UserSvr::AddEvent(lEventDown);
    User::After(KWait01);               
    lEventUp.Set(TRawEvent::EKeyUp, KEndKeyScanCode);                       
    UserSvr::AddEvent(lEventUp);
    User::After(KWait1);
    
	return KErrNone;
	}

// --------------------------------------------------------------------
// Kills applications in the background except apps which has to run
// --------------------------------------------------------------------
//
void CGlobalCapturer::KillUnwantedAppsResetL()
	{	
	
    TInt numWindowGroups = iEEnv->WsSession().NumWindowGroups();
    CArrayFixFlat<TInt>* windowGroupList = new(ELeave) CArrayFixFlat<TInt>(numWindowGroups);
    CleanupStack::PushL(windowGroupList);

    // Populate array with current group list ids
    User::LeaveIfError(iEEnv->WsSession().WindowGroupList(windowGroupList));
    CApaWindowGroupName* windowGroupName = CApaWindowGroupName::NewLC(iEEnv->WsSession());

    for (TInt i = 0; i < numWindowGroups; ++i)
    	{
        TInt wgId = windowGroupList->At(i);
        windowGroupName->ConstructFromWgIdL(wgId);
        TUid Uid = windowGroupName->AppUid();
        TInt32 appUid = Uid.iUid;
        
                       
        // Exceptions
        if(appUid != KUidMenu.iUid && appUid != KUidPanicCapturer.iUid && appUid != KUidUserEmulatorApp.iUid &&
        		appUid != KUidHomeScreen.iUid && appUid != KUidAknCapServer.iUid &&
                   		appUid != KUidAknnfySrv.iUid && appUid != KUidTelephony.iUid && 
appUid != KUidPenInputSrv.iUid )
        	{
			// Only list 'visible' applications
			if (!windowGroupName->Hidden() && !windowGroupName->IsBusy() && !windowGroupName->IsSystem())
				{
				TApaTaskList taskList(iEEnv->WsSession());
				TApaTask task = taskList.FindApp(Uid);
				if (task.Exists())
					{
        
					task.EndTask();
					User::After(KWait005);
					}
				}
        	}
           
    	}
    CleanupStack::PopAndDestroy(2);
   
	}

// ----------------------------------------------------
// Notifies Window server object to stop capturing
// ----------------------------------------------------
//
void CGlobalCapturer::StopRecordKeysL()
{
	iKeyCaptureStart = EFalse;
	
	// Call client commander which calls server side object
	iClientCommander.AnimCommand( RClientCommander::EStop, NULL);
	// Cancels camera app timer
	iCameraAppTimer->Cancel();
	// Stop Power button capturing
	Stop();
	
	if(iRecord)
		iRecord = EFalse;
	// Bring this app to foreground
	iObserver.BringUserEmulatorToFront();
	// Display acknowledgement
	CAknInformationNote* note = new (ELeave) CAknInformationNote(ETrue);
	TBuf<KBuffer64> buff;
	_LIT(KCapturingEnded, "Capturing ended");
	buff.Format(KCapturingEnded);
	note->ExecuteLD(buff);
}
// End of file
