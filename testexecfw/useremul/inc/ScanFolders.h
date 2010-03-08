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
 * File Name: ScanFolders.h
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
#ifndef SCANFOLDERS_H__
#define SCANFOLDERS_H__

//System includes
#include <E32Base.h>
#include <EikEnv.h>
#include <F32File.h>

//User Includes
#include "Constants.h"

//Forward Declarations
class MFolderContentsListener;
class CUserEmulatorScriptsView;


/**
 * CScanFolders 
 * Active object implementation for scanning for script files in a folder
 */
class CScanFolders : public CActive
{
public:
    /**
     * Static constructor
     * @return CScanFolders* A pointer to the newly alocated CScanFolders class.
     *                      NULL, if the class cannot be created
     */
	static CScanFolders * NewL();
	/**
	 * Static constructor
	 * @return CScanFolders* A pointer to the newly alocated CScanFolders class.
	 *                      NULL, if the class cannot be created
	 */
	static CScanFolders * NewLC();
	/**
	 * Destructor
	 */	
	~CScanFolders();

public:
	/**
	 * Function to add listener
	 * @param aListener Reference to the folder listener object 
	 */
	void AddListenerL( CUserEmulatorScriptsView* aListener );
	/**
	 * Function to remove listeners 
	 * @param aListener Reference to the folder listener object 
	 */
	void RemoveListener( CUserEmulatorScriptsView * aListener );

public:
	/**
	 * Function to set filters for folder scan
	 * @param aFilter 16 bit filter descriptor 
	 */
	void SetFilter( const TDesC & aFilter );
	/**
	 * Function to set filters for folder scan
	 * @param aFilter 8 bit filter descriptor 
	 */
	void SetFilter( const TDesC8 & aFilter );

	/**
	 * Scan the folder specified by the path asynchronously. Results will
	 * be delivered through the listener interface.
	 * @param aPath 8 bit path descriptor  
	 */
	void ScanFolderL( const TDesC & aPath );
	/**
	 * Scan the folder specified by the path asynchronously. Results will
	 * be delivered through the listener interface.
	 * @param aPath 16 bit path descriptor  
	 */
	void ScanFolderL( const TDesC8 & aPath );

protected:
  /**
	* Constructor
	*/
	CScanFolders();
   /**
	* ConstructL
	*/
	void ConstructL();

protected:
	/**
	 * Function to start folder scan
	 */
	void StartScanningL();
	/**
	 * Function to continue folder scan
	 */
	void ContinueScanning();
	/**
	 * Function to stop folder scan
	 */
	void StopScanning();
	/**
	 * Deliver Drives
	 */
	void DeliverDrivesL();
	/**
	 * Reset operation
	 */
	void Reset();

protected:
	/**
	 * Function that notifies any error during folder scan
	 * @param aReasonCode error code
	 */
	void FireError( TInt aReasonCode );
	/**
	 * Function that notifies on any new entry in the folder
	 */
	void FireEntriesL();
	/**
	 * Function that notifies any directory or file changes in the 
	 * folder
	 * @param aEntry entry type
	 */
	void FireEntry( const TEntry & aEntry );
	/**
	 * Function that notifies on any folder changes
	 * @param aFolder folder name
	 */
	void FireFolder( const TDesC & aFolder );
	/**
	 * Function that notifies on any file changes
	 * @param aFile File name
	 */
	void FireFile( const TDesC & aFile );
	/**
	 * Function that notifies at the beginning of folder scan 
	 * @param aIsDriveListFlag 
	 */
	void FireFolderStarting( TBool aIsDriveListFlag );
	/**
	 * Function that indicates folder scan complete 
	 */
	void FireFolderCompleteL();

protected:
	/**
	 * Helper function that checks for file types and notifies 
	 * on any file changes
	 * @param aFilename file name
	 */
	TBool IsMatch( const TDesC & aFilename );

private: //From CActive
	void DoCancel();
	void RunL();

private:
	/**
	 * Enumeration with the folder scanner states
	 */
	enum TScanState
	{
		EIdle,
		EScanning,
		EStartNewScan,
		EAbortScan
	};

	//Data Members
	TBool iAbortScan;
	TBool iStartNewScan;
	TBool iContinued;
	TBool iComplete;
	TScanState iScanState;
	TBuf<KBuffer256> iFileFilter;
	TParse iCurrentPath;
	RDir iCurrentDir;
	TEntryArray iEntries;
    CDesCArrayFlat * iDirectories;
    CDesCArrayFlat * iFileNames;
	RFs iFs;
	RArray< CUserEmulatorScriptsView * > iListeners;
};

#endif 
