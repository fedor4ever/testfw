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
 * File Name: ScanFolders.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for folder scanning operation
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <EikApp.h>
#include <EikEnv.h>
#include <F32File.h>

//User Includes
#include "ScanFolders.h"
#include "FolderContentsListener.h"
#include "UserEmulatorScriptsView.h"

//Constants
const TInt KDRIVESIZE      = 3;

// -----------------------------------------------------------------------------
// CScanFolders::NewL
// -----------------------------------------------------------------------------
//
CScanFolders* CScanFolders::NewL()
{
	CScanFolders* self = CScanFolders::NewLC();
	CleanupStack::Pop(self);
	return self;
}
// -----------------------------------------------------------------------------
// CScanFolders::NewLC
// -----------------------------------------------------------------------------
//
CScanFolders* CScanFolders::NewLC()
{
	CScanFolders* self = new ( ELeave ) CScanFolders;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;	
}
// -----------------------------------------------------------------------------
// CScanFolders::CScanFolders
// -----------------------------------------------------------------------------
//
CScanFolders::CScanFolders() : CActive( EPriorityStandard )
{

}
// -----------------------------------------------------------------------------
// CScanFolders::~CScanFolders
// -----------------------------------------------------------------------------
//
CScanFolders::~CScanFolders()
{
	Cancel();
	iListeners.Close();
	iCurrentDir.Close();
	iFs.Close();

	delete iDirectories; 
	iDirectories = NULL;
	delete iFileNames; 
	iFileNames = NULL;
}
// -----------------------------------------------------------------------------
// CScanFolders::ConstructL
// -----------------------------------------------------------------------------
//
void CScanFolders::ConstructL()
{
	// Create an array to hold the directories
	iDirectories = new ( ELeave ) CDesCArrayFlat( 10 );

	// Create an array to hold the full filenames
	iFileNames = new ( ELeave ) CDesCArrayFlat( 10 );

	// Create a file server session
	User::LeaveIfError(iFs.Connect());

	// Register this as an active object (for the async file scanning)
	CActiveScheduler::Add( this );
}
// -----------------------------------------------------------------------------
// CScanFolders::AddListenerL
// -----------------------------------------------------------------------------
//
void CScanFolders::AddListenerL( CUserEmulatorScriptsView* aListener )
{
	if ( iListeners.Find( aListener ) != KErrNotFound )
	{
		User::Leave(KErrAlreadyExists);
	}
	iListeners.Append( aListener );
}
// -----------------------------------------------------------------------------
// CScanFolders::RemoveListener
// -----------------------------------------------------------------------------
//
void CScanFolders::RemoveListener( CUserEmulatorScriptsView * aListener )
{
	TInt index = iListeners.Find( aListener );
	iListeners.Remove( index );
}
// -----------------------------------------------------------------------------
// CScanFolders::SetFilter
// -----------------------------------------------------------------------------
//
void CScanFolders::SetFilter( const TDesC & aFilter )
{
	iFileFilter.Copy( aFilter );
	if ( iFileFilter[ iFileFilter.Length() - 1 ] != ';' )
	{
		iFileFilter.Append( ';' );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::SetFilter
// -----------------------------------------------------------------------------
//
void CScanFolders::SetFilter( const TDesC8 & aFilter )
{
	iFileFilter.Copy( aFilter );
	if ( iFileFilter[ iFileFilter.Length() - 1 ] != ';' )
	{
		iFileFilter.Append( ';' );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::ScanFolderL
// -----------------------------------------------------------------------------
//
void CScanFolders::ScanFolderL( const TDesC & aPath )
{

	iCurrentPath.Set( aPath, NULL, NULL );

	if ( IsActive() )
	{
		iStartNewScan = ETrue;
	}
	else
	{
		StartScanningL();
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::ScanFolderL
// -----------------------------------------------------------------------------
//
void CScanFolders::ScanFolderL( const TDesC8 & aPath )
{
	TBuf< KBuffer256 > temp;
	temp.Copy( aPath );
	ScanFolderL( temp );
}
// -----------------------------------------------------------------------------
// CScanFolders::DoCancel
// -----------------------------------------------------------------------------
//
void CScanFolders::DoCancel()
{
	iAbortScan = ETrue;
}
// -----------------------------------------------------------------------------
// CScanFolders::RunL
// -----------------------------------------------------------------------------
//
void CScanFolders::RunL()
{
	switch ( iScanState )
	{
	case EIdle:
		User::Panic(KIDLE, 0);
		break;

	case EScanning:
		{
			if ( iStatus == KErrNone )
			{
				FireEntriesL();
				Reset();

				// After the initial entries have been fired, set the
				// #iContinued flag so that #FolderStarting is not called
				// again for this reading session.
				iContinued = ETrue;

				ContinueScanning();
			}
			else if ( iStatus == KErrEof )
			{
				// Before delivering this last set of entries, set the
				// #iComplete flag so that #FolderComplete is called in
				// #FireEntries.
				iComplete = ETrue;

				FireEntriesL();
				Reset();
				StopScanning();
			}
			else
			{
				Reset();
				FireError( iStatus.Int() );
			}
		}
		break;

	case EStartNewScan:
		Reset();
		StartScanningL();
		break;

	case EAbortScan:
		Reset();
		break;
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::StartScanningL
// -----------------------------------------------------------------------------
//
void CScanFolders::StartScanningL()
{
	if ( iCurrentPath.FullName().Compare( KPATHDELIM ) == 0 )
	{
		DeliverDrivesL();
		return;
	}

	TInt error = iCurrentDir.Open( iFs, iCurrentPath.FullName(),
		KEntryAttNormal|KEntryAttDir|KEntryAttReadOnly|KEntryAttSystem|KEntryAttHidden );

	if ( error != KErrNone )
	{
		FireError( error );
		return;
	}

	iContinued = EFalse;
	iComplete = EFalse;

	ContinueScanning();
}

// -----------------------------------------------------------------------------
// CScanFolders::ContinueScanning
// -----------------------------------------------------------------------------
//
void CScanFolders::ContinueScanning()
{
	iCurrentDir.Read( iEntries, iStatus );
	if(!IsActive())
		SetActive();

	iScanState = EScanning;
}
// -----------------------------------------------------------------------------
// CScanFolders::StopScanning
// -----------------------------------------------------------------------------
//
void CScanFolders::StopScanning()
{
	iCurrentDir.Close();
}
// -----------------------------------------------------------------------------
// CScanFolders::DeliverDrivesL
// -----------------------------------------------------------------------------
//
void CScanFolders::DeliverDrivesL()
{
	FireFolderStarting( ETrue );

	TDriveList drives;
	User::LeaveIfError( iFs.DriveList( drives ) );

	for ( TInt idx = 0; idx < KMaxDrives; idx++ )
	{
		if ( drives[ idx ] == 0 )
		{
			continue;
		}
		TChar driveLetter;
		User::LeaveIfError( iFs.DriveToChar( idx, driveLetter ) );

		TBuf< KDRIVESIZE > temp;
		temp.Append( driveLetter );
		temp.Append( KDRIVEDEL );

		FireFolder( temp );
	}

	FireFolderCompleteL();
	Reset();
}
// -----------------------------------------------------------------------------
// CScanFolders::Reset
// -----------------------------------------------------------------------------
//
void CScanFolders::Reset()
{
	iDirectories->Reset();
	iFileNames->Reset();

	iScanState = EIdle;
}
// -----------------------------------------------------------------------------
// CScanFolders::FireError
// -----------------------------------------------------------------------------
//
void CScanFolders::FireError( TInt aReasonCode )
{
	for ( TInt idx = 0; idx < iListeners.Count(); idx++ )
	{
		iListeners[ idx ]->ErrorOccured( aReasonCode );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireEntriesL
// -----------------------------------------------------------------------------
//
void CScanFolders::FireEntriesL()
{
	if ( iContinued == EFalse )
	{
		FireFolderStarting( EFalse );
	}

	for ( TInt idx = 0; idx < iEntries.Count(); idx++ )
	{
		if ( iEntries[ idx ].IsDir() || IsMatch( iEntries[ idx ].iName ) )
		{
			FireEntry( iEntries[ idx ] );
		}
	}

	if ( iComplete )
	{
		FireFolderCompleteL();
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireEntry
// -----------------------------------------------------------------------------
//
void CScanFolders::FireEntry( const TEntry & aEntry )
{
	if ( aEntry.IsDir() )
	{
		FireFolder( aEntry.iName );
	}
	else
	{
		FireFile( aEntry.iName );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireFolder
// -----------------------------------------------------------------------------
//
void CScanFolders::FireFolder( const TDesC & aFolder )
{
	for ( TInt idx = 0; idx < iListeners.Count(); idx++ )
	{
		iListeners[ idx ]->NewFolder( aFolder );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireFile
// -----------------------------------------------------------------------------
//
void CScanFolders::FireFile( const TDesC & aFile )
{
	for ( TInt idx = 0; idx < iListeners.Count(); idx++ )
	{
		iListeners[ idx ]->NewFile( aFile );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireFolderStarting
// -----------------------------------------------------------------------------
//
void CScanFolders::FireFolderStarting( TBool aIsDriveListFlag )
{
	for ( TInt idx = 0; idx < iListeners.Count(); idx++ )
	{		
		iListeners[ idx ]->FolderStarting( iCurrentPath.FullName(), aIsDriveListFlag );
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::FireFolderCompleteL
// -----------------------------------------------------------------------------
//
void CScanFolders::FireFolderCompleteL()
{
	for ( TInt idx = 0; idx < iListeners.Count(); idx++ )
	{
		iListeners[ idx ]->FolderCompleteL();
	}
}
// -----------------------------------------------------------------------------
// CScanFolders::IsMatch
// -----------------------------------------------------------------------------
//
TBool CScanFolders::IsMatch( const TDesC & aFilename )
{
	TPtrC remaining = iFileFilter.Mid( 0 );
	while ( remaining.Length() > 0 )
	{
		TInt delimeterPos = remaining.Locate( ';' );
		if ( delimeterPos == KErrNotFound )
		{
			delimeterPos = remaining.Length();
		}
		TPtrC pattern = remaining.Mid( 0, delimeterPos );
		if ( aFilename.MatchF( pattern ) != KErrNotFound )
		{
			return ETrue;
		}
		if( delimeterPos + 1 <= remaining.Length() )
			remaining.Set( remaining.Mid( delimeterPos + 1 ) );
	}
	return EFalse;
}
