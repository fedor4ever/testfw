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
 * File Name: CaptureKeyTimer.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Key Timer class
 *------------------------------------------------------------------
 -
 *
 */
 
// User Includes
#include "CaptureKeyTimer.h"

// ======== MEMBER FUNCTIONS ========

CCaptureKeyTimer* CCaptureKeyTimer::NewL(MCaptureKeyTimerNotify& aNotify)
{
    CCaptureKeyTimer* me = new (ELeave) CCaptureKeyTimer(aNotify);
    CleanupStack::PushL(me);
    me->ConstructL();
    CleanupStack::Pop(me);
    return me;
} 
 
CCaptureKeyTimer::~CCaptureKeyTimer()
{	
	Cancel();
	iTimer.Close();
}
// ----------------------------------------------------------
// Set timer
// ----------------------------------------------------------
//
void CCaptureKeyTimer::After(TTimeIntervalMicroSeconds32 aInterval)
{
    Cancel();
    iTimer.After(iStatus,aInterval);
    SetActive();
}
// ----------------------------------------------------------
// Notifies AppUi that timer expired
// ----------------------------------------------------------
//
void CCaptureKeyTimer::RunL()
{
    iNotify.KeyTimerExpired();
}
// ----------------------------------------------------------
// Cancel timer
// ----------------------------------------------------------
//
void CCaptureKeyTimer::DoCancel()
{
    iTimer.Cancel();
}
// ----------------------------------------------------------
// CCaptureKeyTimer::CCaptureKeyTimer
// ----------------------------------------------------------
//
CCaptureKeyTimer::CCaptureKeyTimer(MCaptureKeyTimerNotify& aNotify)
:CActive(EPriorityNormal),iNotify(aNotify)
{
}
// ----------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------
//
void CCaptureKeyTimer::ConstructL(void)
{
	CActiveScheduler::Add(this);
	iTimer.CreateLocal();
}
