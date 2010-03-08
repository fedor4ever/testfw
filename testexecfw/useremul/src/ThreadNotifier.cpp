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
 * File Name: ThreadNotifier.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for capturing panics
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <icl\ImageData.h>
#include <icl\ImageCodecData.h>
#include <coemain.h>
#include <PathInfo.h>
#include <bautils.h>
#include <apgwgnam.h>

// User Includes
#include "ThreadNotifier.h"

//Constants
_LIT(KTHREADPANICMSG, "THREAD-NOTIFIER");
_LIT(KKill,"Kill");
_LIT(KTerminate,"Terminate");
_LIT(KPanic,"Panic");
_LIT(KPending,"Pending");

// -----------------------------------------------------------------------------
// CThreadNotifier::NewL
// Creates the instance of class and returns it.
// -----------------------------------------------------------------------------
//
CThreadNotifier* CThreadNotifier::NewL(MPanicObserver& aObserver, CLogger& aLogger,
                                        CSettings& aSettings, CEikonEnv* aEikonEnv)
{
	CThreadNotifier* self=CThreadNotifier::NewLC(aObserver,aLogger,aSettings,aEikonEnv);
	CleanupStack::Pop(); // self;
	return self;
}
// -----------------------------------------------------------------------------
// CThreadNotifier::NewLC
// Creates the instance of class and pushes it to the CleanupStack and return
// it.
// -----------------------------------------------------------------------------
//
CThreadNotifier* CThreadNotifier::NewLC(MPanicObserver& aObserver, CLogger& aLogger,
                                            CSettings& aSettings, CEikonEnv* aEikonEnv)
{
	CThreadNotifier* self = new (ELeave)CThreadNotifier(aObserver,aLogger,aSettings,aEikonEnv);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}


// -----------------------------------------------------------------------------
// CThreadNotifier::CThreadNotifier
// Calls base classes constructor with priority value. Add class to the 
// active sheduler.
// -----------------------------------------------------------------------------
//
CThreadNotifier::CThreadNotifier(MPanicObserver& aObserver, CLogger& aLogger, CSettings& aSettings,
                                                                               CEikonEnv* aEikonEnv)
    : CActive(CActive::EPriorityStandard), iThreadHandle(0),
      iObserver(aObserver), iLogger(aLogger), iSettings(aSettings), iEEnv(aEikonEnv)   
{
    CActiveScheduler::Add(this);
}
 
// -----------------------------------------------------------------------------
// CThreadNotifier::ConstructL
// Construction of parser and buffer allocations
// -----------------------------------------------------------------------------
//
void CThreadNotifier::ConstructL()
{
    User::LeaveIfError(iUndertaker.Create());
}
 
// -----------------------------------------------------------------------------
// CThreadNotifier::~CThreadNotifier
// Cancels any outstanding requests and deletes members.
// -----------------------------------------------------------------------------
//
CThreadNotifier::~CThreadNotifier()
{
    Cancel();
}
// -----------------------------------------------------------------------------
// CThreadNotifier::IssueRequest
// Function to issues a request for notification of the death of a thread.
// -----------------------------------------------------------------------------
// 
void CThreadNotifier::IssueRequest()
{
    __ASSERT_ALWAYS(!IsActive(), User::Panic(KTHREADPANICMSG, 0));
 
    iUndertaker.Logon(iStatus, iThreadHandle);
    SetActive();
}
 
// -----------------------------------------------------------------------------
// CThreadNotifier::RunL
// From CActive. Handles the state changes and notifing the observer.
// -----------------------------------------------------------------------------
//

void CThreadNotifier::RunL()
{
    if (iStatus == KErrDied)
    {
        RThread thread;
        thread.SetHandle(iThreadHandle);
        CleanupClosePushL(thread);
        TExitCategoryName categ = thread.ExitCategory();
		TBuf<KBuffer1024> buf;
		_LIT(KPanicFormat, "*PANIC* Thread %S (%d) died (ExitType:%S, Reason:%S-%d)\n");
		ExitType(thread.ExitType());
		if(thread.ExitType() != 0)
		{
		    iObserver.PanicOccured();
		
			buf.Format(KPanicFormat, &thread.Name(), (int)thread.Id(), &iExitType,&categ,thread.ExitReason());
			iLogger.WriteLogL(buf,EFalse);
			
			//Capture a screenshot
			CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEEnv->WsSession(), iEEnv->WsSession().GetFocusWindowGroup());
			TUid id = gn->AppUid().Null(); 
			CleanupStack::PopAndDestroy(gn);
			
			CImageCapture* imageCapture = CImageCapture::NewL(iSettings,*this,iEEnv);
			imageCapture->CaptureL(thread.Name(),KN,id);
		}								    
        thread.Close();
        CleanupStack::PopAndDestroy();//thread
        IssueRequest();
    }   
}
// -----------------------------------------------------------------------------
// CThreadNotifier::PerformNextAction
// -----------------------------------------------------------------------------
//
void CThreadNotifier::PerformNextAction(TInt aInterval)
{
    iObserver.RestartRandomTests();
}
// -----------------------------------------------------------------------------
// CThreadNotifier::ExitType
//  Function that indicates the exit type of the thread
// -----------------------------------------------------------------------------
//
void CThreadNotifier::ExitType(TInt aExitType)
{
	switch(aExitType)
	{
		case EExitKill:
			iExitType.Copy(KKill);
			break;
		case EExitTerminate:
			iExitType.Copy(KTerminate);
			break;
		case EExitPanic:
			iExitType.Copy(KPanic);
			break;
		case EExitPending:
			iExitType.Copy(KPending);
			break;
	}
}
// -----------------------------------------------------------------------------
// CThreadNotifier::DoCancel
// From CActive. Cancels any outstanding request according the engine state.
// -----------------------------------------------------------------------------
//
void CThreadNotifier::DoCancel()
{
    TInt res = iUndertaker.LogonCancel();
    if(res!=-2)
    	iUndertaker.Close();
}
