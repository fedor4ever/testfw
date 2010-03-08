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
 * File Name: NotifyFileChange.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for notification of file changes
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <e32std.h>
#include <f32file.h>
#include <eikenv.h>

//User Includes
#include "NotifyFileChange.h"

// -----------------------------------------------------------------------------
// CNotifyFileChange::NewL
// Creates the instance of class and returns it.
// -----------------------------------------------------------------------------
//
CNotifyFileChange* CNotifyFileChange::NewL( MFileChangeObserver& aObserver, const TDesC& aPath)
	{
	CNotifyFileChange* self = CNotifyFileChange::NewLC(aObserver, aPath);
	CleanupStack::Pop( self );
	return self;
	}

// -----------------------------------------------------------------------------
// CNotifyFileChange::NewLC
// Creates the instance of class and pushes it to the CleanupStack and return
// it.
// -----------------------------------------------------------------------------
//
CNotifyFileChange* CNotifyFileChange::NewLC( MFileChangeObserver& aObserver, const TDesC& aPath)
	{
	CNotifyFileChange* self = new ( ELeave ) CNotifyFileChange( aObserver, aPath );
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CNotifyFileChange::CNotifyFileChange
// Calls base classes constructor with priority value. 
// -----------------------------------------------------------------------------
//
CNotifyFileChange::CNotifyFileChange( MFileChangeObserver& aObserver, const TDesC& aPath )
  :	CActive(EPriorityStandard), iObserver( aObserver ),iPath((TDes16&)aPath)
	{}

// -----------------------------------------------------------------------------
// CNotifyFileChange::ConstructL
// Add class to the active sheduler and issue request for any file or directory 
// change notifications
// -----------------------------------------------------------------------------
//
void CNotifyFileChange::ConstructL()
	{	
		CActiveScheduler::Add( this );  
		User::LeaveIfError(iFs.Connect());
		
		iStatus = KRequestPending;
		iFs.NotifyChange(ENotifyAll, iStatus, iPath);
		SetActive();
	}

// -----------------------------------------------------------------------------
// CNotifyFileChange::~CNotifyFileChange
// Destructor
// -----------------------------------------------------------------------------
//
CNotifyFileChange::~CNotifyFileChange()
	{
		Cancel();
		iFs.Close();
	}
// -----------------------------------------------------------------------------
// CNotifyFileChange::RefreshPath
// Function that requests a notification of change to files or directories
// -----------------------------------------------------------------------------
//
void CNotifyFileChange::RefreshPath(TDes& aPath)
	{
		Cancel();
		User::After(KWait001);
		iPath.Copy(aPath);
		iFs.NotifyChange(ENotifyAll, iStatus, iPath);
		SetActive();	
	}

// -----------------------------------------------------------------------------
// CNotifyFileChange::RunL
// Handles the completion of the active request and re-issues the notification 
// request
// -----------------------------------------------------------------------------
//
void CNotifyFileChange::RunL()
	{	
		if (iStatus == KErrNone)
		{
			if(!IsActive())
				{
				iObserver.FileChangeEventL();
				iFs.NotifyChange(ENotifyAll, iStatus, iPath);
				SetActive();
				}
		}
	}
	
// -----------------------------------------------------------------------------
// CNotifyFileChange::DoCancel
// Cancels any outstanding requests
// -----------------------------------------------------------------------------
//
void CNotifyFileChange::DoCancel()
	{		
		if (IsActive())
		{
			iFs.NotifyChangeCancel();
		}
	}	
// -----------------------------------------------------------------------------
// CNotifyFileChange::RunError
// -----------------------------------------------------------------------------
//
TInt CNotifyFileChange::RunError(TInt /*aError*/)
	{
		return KErrNone;
	}
