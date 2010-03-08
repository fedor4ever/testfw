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
 * File Name: NotifyFileChange.h
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
#ifndef NOTIFYFILECHANGE__
#define NOTIFYFILECHANGE__

//System Includes
#include <e32std.h>
#include <e32base.h>
#include <f32file.h>

//User Includes
#include "Constants.h"

/**
 * MFileChangeObserver 
 * Interface class for file change notifications
 */
class MFileChangeObserver
{
public:
	/**
	 * FileChangeEventL 
	 * Callback function that notifies any file change events
	 */
	virtual void FileChangeEventL() = 0;
};

/**
 * CNotifyFileChange 
 * Class implementation for notification of file changes
 */

class CNotifyFileChange : public CActive
{
public:
   /**
	* Static constructor
	* @param aObserver Reference to the MFileChangeObserver class
	* @param aPath file path
	* @return CNotifyFileChange* A pointer to the newly alocated CNotifyFileChange class.
	*                          NULL, if the class cannot be created
	*/
	static  CNotifyFileChange*  NewL(  MFileChangeObserver& aObserver, const TDesC& aPath );
   /**
	* Static constructor
	* @param aObserver Reference to the MFileChangeObserver class
	* @param aPath file path
	* @return CNotifyFileChange* A pointer to the newly alocated CNotifyFileChange class.
	*                          NULL, if the class cannot be created
	*/
	static  CNotifyFileChange*  NewLC( MFileChangeObserver& aObserver, const TDesC& aPath );
	
   /**
	* Destructor
	*/
	virtual ~CNotifyFileChange();

   /**
	* Function that requests a notification of change to files or directories
	* @param aPath File path
	*/
	void RefreshPath(TDes& aPath);
	
private: // from CActive
   /**
	* Cancels any outstanding request.
	*/
	void DoCancel();
	
   /**
	* Handles the completion of the active request.
	*/
	void RunL();
   /**
	* Handles a leave occurring in the request completion event handler CActive::RunL().
	*/  	
	TInt RunError(TInt aError);

private:
   /**
	* Constructor
	* @param aObserver Reference to the MFileChangeObserver class
	* @param aPath file path
	*/
	CNotifyFileChange( MFileChangeObserver& aObserver,const TDesC& aPath );
	/**
	 * ConstructL
	 */
	void ConstructL();

private:
   /**
	* Reference to the MFileChangeObserver class
	*/
	MFileChangeObserver&		iObserver;	
  /**
	* Reference to file server session
	*/
    RFs 						iFs;
   /**
  	* File path
  	*/
    TDes16&                     iPath;
};

#endif	//NOTIFYFILECHANGE__
