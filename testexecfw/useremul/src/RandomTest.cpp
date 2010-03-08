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
 * File Name: RandomTest.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Random test class
 *------------------------------------------------------------------
 -
 *
 */

// System Includes
#include <e32math.h>
#include <APGTASK.H>
#include <APGWGNAM.H>
#include <aknnotewrappers.h>
// User Includes
#include "RandomTest.h"

// Constants
#define KOkButton 167
#define KBackspace 1

#define TESTSTEPSNUMBER 40
#define COUNTERFOROK 200

// Uid of the Aactive Idle framework (3.1 devices)
const TUid KUidStandBy1 = { 0x101fd64c };
// Uid of the Aactive Idle framework (3.2 devices)
const TUid KUidStandBy = { 0x102750f0 };
// Uid of the Active Idle framework (5.0 devices)
const TUid KUidStandby50 = {0x20017CD9};

enum {ETap=0,EDoubleTap,EFlick,EDrag,EKeys,EKeys1,EKeys2,EKeys3};
const TInt KMaxChance = 200;
const TInt KShortFlickDist = 40;
const TInt KEventTypeMax = 8;
const TInt KScancodeMax = 65535;
const TInt KLogAppAtStep = 100;

const TInt KLSK = 164;
const TInt KRSK = 165;

// ======== MEMBER FUNCTIONS ========

CRandomTest::CRandomTest(MKeyObserver& aObserver,RWsSession& aRWsSession,CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger)
:CActive(EPriorityNormal),iObserver(aObserver),iRWsSession(&aRWsSession),iSettings(aSettings),iSession(aSession),iLogger(aLogger)
{
}
 
CRandomTest::~CRandomTest()
{	
    iSlotUIDs.Close();
	Cancel();
	iTimer.Close();
}
 
CRandomTest* CRandomTest::NewL(MKeyObserver& aObserver,RWsSession& aRWsSession,CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger)
{
	CRandomTest* me = CRandomTest::NewLC(aObserver,aRWsSession,aSettings,aSession,aLogger);
	CleanupStack::Pop(me);
    return me;
}

CRandomTest* CRandomTest::NewLC(MKeyObserver& aObserver,RWsSession& aRWsSession,CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger)
	{
	CRandomTest* me = new (ELeave) CRandomTest(aObserver,aRWsSession,aSettings,aSession,aLogger);
	CleanupStack::PushL(me);
	me->ConstructL();
	return me;
	}
// ----------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------
// 
void CRandomTest::ConstructL(void)
{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
}
 
// ----------------------------------------------------------
// Starts random test
// ----------------------------------------------------------
//
void CRandomTest::StartRandomTestL()
{
	Cancel();
	iAppTestCounter = 0;
	iAppNameLoggingCounter = 0;
	iCounterForOkButton = 0;
	iCantStartAppToTest = ETrue;
	
	if(iSettings.iRandomTestAppIdStatus)
		{
		_LIT(KCantStartApp,"Can't start app %d.\n");
		TUid uid;
		
		TThreadId threadId;
		TBuf<KBuffer128> buf;
		TBuf<KBuffer12> CurrentAppUid;
		TBuf<KBuffer256> RandomTestAppIDs;
		RandomTestAppIDs.Copy(iSettings.iRandomTestAppID);
		TUint32 k;
		TInt res;
		TInt pos=0;
		TInt i=1;
		_LIT(KCommaZero,",0");
		
		// 'Close' string by these chars
		RandomTestAppIDs.Append(KCommaZero);
	
	    while((pos=RandomTestAppIDs.Locate(','))!=KErrNotFound)
	       {
	       k=0;
	       if(RandomTestAppIDs.Left(pos).Length()<=10)
	           CurrentAppUid = RandomTestAppIDs.Left(pos);
	       else
	           CurrentAppUid.Copy(KEMPTYTEXT);
	           
	       CurrentAppUid.Trim();
	       RandomTestAppIDs.Copy(RandomTestAppIDs.Right(RandomTestAppIDs.Length()-pos-1));
	       
		
		    // Converts decimal or hexa uid to integer
            if(CurrentAppUid.Left(2).Compare(KHEX)==0)
                {
                TLex id1(CurrentAppUid.Right(CurrentAppUid.Length()-2));
                res = id1.Val(k,EHex);
                }
            else
                {
                TLex id1(CurrentAppUid);
                res = id1.Val(k,EDecimal);
                }
            
            TInt doc_res=0;
            // Starts test application
            if(res==KErrNone)
                {
                uid.iUid = k;
                doc_res=iSession.StartDocument(KN, uid ,threadId);
                User::After(KWait01);
                }
            if(doc_res==KErrNone && res==KErrNone)
                {
                iSlotUIDs.Append(uid);
                }
            else
                {
                buf.Format(KCantStartApp, i);
                iLogger.WriteLogL(buf,EFalse);
                }
                
           i++;
           }
                
		User::After(KWait02);
		}		
	// Starts timer
	iTimer.After(iStatus,KWait02);
	SetActive();
}
// --------------------------------------------
// Stops random test (The active object)
// --------------------------------------------
//
void CRandomTest::CancelRandomTest()
    {
    Cancel();
    }

// --------------------------------------------
// Restarts random test (The active object)
// --------------------------------------------
//
void CRandomTest::RestartRandomTest()
    {
    iTimer.After(iStatus,KWait02);
    SetActive();
    }
    
// --------------------------------------------
// Stops random test
// --------------------------------------------
//
void CRandomTest::StopRandomTest()
	{
	if(iSettings.iRandomTestAppIdStatus)
		{
		// Stops test apps if uids were valid and apps are running
		for(TInt t=0;t<iSlotUIDs.Count();t++)
		    {
            if(iSlotUIDs[t].iUid!=0)
                {
                TApaTaskList taskList1(*iRWsSession);
                TApaTask task1 = taskList1.FindApp(iSlotUIDs[t]);
                if (task1.Exists())
                    {
                    task1.EndTask();
                    User::After(KWait01);
                    }
                }
		    }
		iSlotUIDs.Reset();
		}
	Cancel();
    }
// ----------------------------------------------------------
// CActive object cancel function
// ----------------------------------------------------------
//
void CRandomTest::DoCancel()
{
	iTimer.Cancel();
}

// -----------------------------------------
// Checks if given Uid is in test list
// -----------------------------------------
//
TBool CRandomTest::IsUidInTestList(TUid aUid)
    {
    TInt res=EFalse;
    for(TInt t=0;t<iSlotUIDs.Count();t++)
            if(iSlotUIDs[t].iUid==aUid.iUid)
                {
                res=ETrue;
                break;
                }
                
    return res;
    }

// ----------------------------------------------------------
// CActive object RunL function
// Runs test on test apps
// ----------------------------------------------------------
//
void CRandomTest::RunL()
{
	// For screen light
	User::ResetInactivityTime();
	// Gets screen width and height
	TInt MaxX = iSettings.iAppRect.Width();
	TInt MaxY = iSettings.iAppRect.Height();
	// Gets random coordinates for random pointer events
	TInt x = Math::Random()%MaxX*2;
	TInt y = Math::Random()%MaxY*2;
	// Repets value for long key press
	TInt repeats = Math::Random()%2;
	// Decides which key press event will be sent
	TInt chance = Math::Random()%KMaxChance;
	// Decides which event will be  sent (pointer/key press)
	TInt randomevent = Math::Random()%KEventTypeMax;
	
	// Sends Ok button press event according to counter 
	if(iCounterForOkButton++ > COUNTERFOROK)
		{// Ok button press event
		iCounterForOkButton = 0;
		repeats = 0;
		chance = KMaxChance-20;
		randomevent = EKeys;
		}
	
	TWsEvent wsEvent;
	TKeyEvent& keyEvent(*wsEvent.Key());
	
	TRawEvent event;
	TRawEvent event1;
	TRawEvent event2;
	TRawEvent event3;
	
	// Log foreground app's name
	CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(*iRWsSession, iRWsSession->GetFocusWindowGroup());
	TApaAppInfo appInfo;
	RApaLsSession ApaLs;
	User::LeaveIfError(ApaLs.Connect());
	ApaLs.GetAppInfo(appInfo, gn->AppUid());
	
	
	// If one of our apps is in foreground then don't close it just send it to background
	if(appInfo.iUid == KUidPanicCapturer )
	{ 
		TApaTaskList taskList(*iRWsSession);
		TApaTask task = taskList.FindApp(appInfo.iUid);

		if (task.Exists())
			task.SendToBackground();
		CleanupStack::PopAndDestroy(gn);
		ApaLs.Close();
		
		iTimer.After(iStatus,KWait01);
		SetActive();
						
		return;
	}
	// If app in foreground is different now or it's there for a long time then log it's name
	else if(iLastAppName.Compare(appInfo.iCaption)!=0 || iAppNameLoggingCounter++>KLogAppAtStep )
		{
		iAppNameLoggingCounter = 0;
		iLastAppName.Zero();
		iLastAppName.Copy(appInfo.iCaption);
		TBuf<KBuffer540> buf;
		_LIT(KText,"Launched app.: ");
		_LIT(KNL,"\n");
		buf.Copy(KText);
		buf.Append(iLastAppName);
		buf.Append(KNL);
		iLogger.WriteLogL(buf,EFalse);
		}
	CleanupStack::PopAndDestroy(gn);
	ApaLs.Close();
	
	// If random test run is on in Settings dialog...
	if(iSettings.iRandomTestAppIdStatus)
		{
		TThreadId threadId; // not used
		// Bring given ID's app to foreground
		TUid uid;
		uid.iUid = 0;
		
		// Decides which test app should be in foreground
		TInt count = iAppTestCounter/TESTSTEPSNUMBER;
		
		if(iSlotUIDs.Count()>count)
		      uid = iSlotUIDs[count];
		else
		    {
		    iAppTestCounter=0;
		    if(iSlotUIDs.Count()>0)
		      uid = iSlotUIDs[0];
		    }
		
		iAppTestCounter++;
		
		
		// If app in foreground is not one of our test apps then close it.
		if( iSlotUIDs.Count()>0 )
			{
			TApaTaskList taskList(*iRWsSession);
			TApaTask task = taskList.FindApp(uid);
			
			if( !IsUidInTestList(appInfo.iUid) && appInfo.iUid.iUid!=0x100058b3 && 
			                 appInfo.iUid.iUid!=_UID3 ) // Don't close telephony app. and User Emulator (exceptions)
				{
				TApaTask task1 = taskList.FindApp(appInfo.iUid);
				if (task1.Exists())
					{
					task1.EndTask();
					User::After(KWait01);
					}
				}
			// If our test app is not in foreground then bring it to foreground
			if (task.Exists())
				task.BringToForeground();
			else
			    // If it's not started then start it
				iSession.StartDocument(KN, uid ,threadId);
			}
		else
			{
			// If there's no valid app uid display error message
			if(iCantStartAppToTest)
				{
				_LIT(KNoValid,"No valid application to start! Random test ends.");
				iCantStartAppToTest = EFalse;
				CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
				errorNote->ExecuteLD(KNoValid);
				iObserver.RandomTestEndsL();
				return;
				}
			
			iTimer.After(iStatus,KWait05);
			SetActive();
			
			return;
			}
		}
	
	// Run pointer test if device enables it otherwise key events only
	if(!AknLayoutUtils::PenEnabled())
		randomevent = EKeys;
	
	// Events
	switch(randomevent)
		{
		case ETap:
			 event.Set(TRawEvent::EButton1Down, x, y);
			 iRWsSession->SimulateRawEvent(event);
			 User::After(KWait01);
			 event1.Set(TRawEvent::EButton1Up, x, y);
			 iRWsSession->SimulateRawEvent(event1);
		break;
		case EDoubleTap:			 
			 event.Set(TRawEvent::EButton1Down, x, y);
			 iRWsSession->SimulateRawEvent(event);
			 User::After(KWait01);
			 event1.Set(TRawEvent::EButton1Up, x, y);
			 iRWsSession->SimulateRawEvent(event1);
			 User::After(KWait01);
			 event2.Set(TRawEvent::EButton1Down, x, y);
			 iRWsSession->SimulateRawEvent(event2);
			 User::After(KWait01);
			 event3.Set(TRawEvent::EButton1Up, x, y);
			 iRWsSession->SimulateRawEvent(event3);
		break;
		case EFlick:
			event.Set(TRawEvent::EButton1Down, x, y);
			iRWsSession->SimulateRawEvent(event);
			User::After(KWait01);
		
			event1.Set(TRawEvent::EPointerMove, x+KShortFlickDist, y+KShortFlickDist);
			iRWsSession->SimulateRawEvent(event1);
			User::After(KWait01);
			
			event2.Set(TRawEvent::EButton1Up, x+KShortFlickDist, y+KShortFlickDist);
			iRWsSession->SimulateRawEvent(event2);
		break;
		case EDrag:
			event.Set(TRawEvent::EButton1Down, x, y);
			iRWsSession->SimulateRawEvent(event);
			User::After(KWait01);
			
			event1.Set(TRawEvent::EPointerMove, x+KShortFlickDist, y);
			iRWsSession->SimulateRawEvent(event1);
			User::After(KWait01);
			
			event2.Set(TRawEvent::EPointerMove, x+KShortFlickDist,y+KShortFlickDist);
			iRWsSession->SimulateRawEvent(event2);
			User::After(KWait01);
			
			event3.Set(TRawEvent::EButton1Up, x+KShortFlickDist, y+KShortFlickDist);
			iRWsSession->SimulateRawEvent(event3);
		break;
		case EKeys:
		case EKeys1:
		case EKeys2:
		case EKeys3:
		default:
			CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(*iRWsSession, iRWsSession->GetFocusWindowGroup());
			TUid foreTaskUid = gn->AppUid();
			CleanupStack::PopAndDestroy(gn);
			
			iRWsSession->GetFocusWindowGroup();
			keyEvent.iModifiers=0;
			
			// Increasing the chance of menu, ok, arrow ,etc. keys
			if( chance > KMaxChance-120)
				{
				if(chance<KMaxChance-100)
					keyEvent.iScanCode = EStdKeyDevice0;
				else if(chance<KMaxChance-80)
					keyEvent.iScanCode = EStdKeyDevice1;
				else if(chance<KMaxChance-70)
					keyEvent.iScanCode = EStdKeyUpArrow;
				else if(chance<KMaxChance-60)
					keyEvent.iScanCode = EStdKeyDownArrow;
				else if(chance<KMaxChance-50)
					keyEvent.iScanCode = EStdKeyLeftArrow;
				else if(chance<KMaxChance-40)
					keyEvent.iScanCode = EStdKeyRightArrow;
				else if(chance<KMaxChance-10)
					keyEvent.iScanCode = EStdKeyApplication0;
				else if(chance<=KMaxChance)
					keyEvent.iScanCode = EStdKeyBackspace;
				}
			else
				keyEvent.iScanCode = Math::Random()%KScancodeMax;
			
			if(iSettings.iRandomTestAppIdStatus)
				{
				// If there is valid uid application -> no menu button push
				// It's for random tests without predefined test apps
				while(keyEvent.iScanCode ==  EStdKeyApplication0 || keyEvent.iCode == EKeyDevice2)
					keyEvent.iScanCode = Math::Random()%KScancodeMax;
					 
				}
			
			// Sends the key event
		  keyEvent.iCode = keyEvent.iScanCode;
			
			
		  TRawEvent lEventDown;
		  lEventDown.Set(TRawEvent::EKeyDown, keyEvent.iScanCode);
		  UserSvr::AddEvent(lEventDown);

		  User::After(KWait01);
											
		  // long key press
		  if(repeats>0)
			 User::After((KWait01*12)*repeats); // 1.2 sec * repeats
											
		  TRawEvent lEventUp;
		  lEventUp.Set(TRawEvent::EKeyUp, keyEvent.iScanCode);
		  UserSvr::AddEvent(lEventUp);
			
		break;
		}
	// Restart timer
	iTimer.After(iStatus,KWait01);
	SetActive();
}
