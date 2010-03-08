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
 * File Name: DelayTimer.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for asynchronous key events handling
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <aknnavi.h> 
#include <aknnavide.h> 
#include <eikspane.h> 
#include <aknutils.h>

//User Includes
#include "DelayTimer.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDelayTimer::CDelayTimer()
// C++ Default constructor.
// -----------------------------------------------------------------------------
//
CDelayTimer::CDelayTimer(MActionObserver& aObserver) 
    : CTimer( CActive::EPriorityStandard),
      iActionObserver(aObserver)
{	 
}

// -----------------------------------------------------------------------------
// CDelayTimer::~CDelayTimer()
// Destructor
// -----------------------------------------------------------------------------
//    
CDelayTimer::~CDelayTimer()
{
	Cancel();   
}

// -----------------------------------------------------------------------------
// CDelayTimer::ConstructL()
// 2nd phase constructor.
// -----------------------------------------------------------------------------
//
void CDelayTimer::ConstructL( )
{
    CActiveScheduler::Add( this );
    CTimer::ConstructL(); 
}
  
// -----------------------------------------------------------------------------
// CDelayTimer::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//  
CDelayTimer* CDelayTimer::NewL( MActionObserver& aObserver )
{
    CDelayTimer* self = new ( ELeave ) CDelayTimer(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;    
}

// -----------------------------------------------------------------------------
// CDelayTimer::RunL()
// After completed timer period, execution comes to RunL.
// -----------------------------------------------------------------------------
//  
void CDelayTimer::RunL()
{  
    // Handle request completion
    iActionObserver.PerformActionL();
}

void CDelayTimer::Wait(TInt aInterval)
{
	if( !IsActive() )
	{
		//Wait for specified time interval
		CTimer::After(aInterval);
		User::ResetInactivityTime();
	}
}
