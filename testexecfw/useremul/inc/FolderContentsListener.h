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
 * File Name: FolderContentsListener.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 *  An interface class for folder scan notifications
 *------------------------------------------------------------------
 -
 *
 */

#ifndef FOLDERCONTENTSLISTENER_H__
#define FOLDERCONTENTSLISTENER_H__

//System includes
#include <e32Std.h>

/**
 * MFolderContentsListener, an interface class for folder scan notifications
 */

class MFolderContentsListener
{
public:
   /**
	* This gets called before a new folder is being scanned.
	*/
	virtual void FolderStarting( const TDesC & aFullPath, TBool aIsDriveListFlag ) = 0;
   /**
	* This gets called with every entry of the currently scanned folder that is a folder
	*/
	virtual void NewFolder( const TDesC & aEntry ) = 0;

   /**
	* This gets called with every entry of the currently scanned folder that is a plain file
	*/
	virtual void NewFile( const TDesC & aEntry ) = 0;

   /**
	* This is called after all entries of the currently scanned folder
	* have been delivered. This is NOT called when a scan operation is
	* aborted by starting a new scan before this got called!
	*/
	virtual void FolderCompleteL() = 0;

   /**
	* This gets called if an unexpected error occures during scanning.
	*/
	virtual void ErrorOccured( TInt aReasonCode ) = 0;
};

#endif 
