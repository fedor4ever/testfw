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
 * File Name: CameraAppTimer.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Camera app's timer CActive class
 *------------------------------------------------------------------
 -
 *
 */
 
// User Includes
#include "CameraAppTimer.h"

// ======== MEMBER FUNCTIONS ========

CCameraAppTimer::CCameraAppTimer(MCameraAppObserver& aNotify)
:CActive(EPriorityNormal),iNotify(aNotify)
{
}
 
CCameraAppTimer::~CCameraAppTimer()
{	
	Cancel();
	iTimer.Close();
}
 
CCameraAppTimer* CCameraAppTimer::NewL(MCameraAppObserver& aNotify)
{
	CCameraAppTimer* me = new (ELeave) CCameraAppTimer(aNotify);
    CleanupStack::PushL(me);
	me->ConstructL();
	CleanupStack::Pop(me);
    return me;
}
// ----------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------
// 
void CCameraAppTimer::ConstructL(void)
{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
}

// ----------------------------------------------------------
// Set timer
// ----------------------------------------------------------
//
void CCameraAppTimer::After(TTimeIntervalMicroSeconds32 aInterval)
{
	Cancel();
	iTimer.After(iStatus,aInterval);
	SetActive();
}
// ----------------------------------------------------------
// Cancel timer
// ----------------------------------------------------------
//
void CCameraAppTimer::DoCancel()
{
	iTimer.Cancel();
}
 
// ----------------------------------------------------------
// Notifies AppUi that timer expired
// ----------------------------------------------------------
//
void CCameraAppTimer::RunL()
{
	iNotify.CheckCameraAppL();
}
