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
 * File Name: ThreadNotifier.h
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

#ifndef THREADNOTIFIER_H_
#define THREADNOTIFIER_H_

// System Includes
#include <e32base.h>
#include <ImageConversion.h>

// User Includes
#include "Logger.h"
#include "Settings.h"
#include "ImageCapture.h"
#include "UserEmulatorScriptsView.h"
#include "Constants.h"

/**
 * Panic observer interface class
 */
class MPanicObserver
{
public:
    /**
    * Callback to notify AppUi (and eventually Random test) if a panic occured
    */
    virtual void PanicOccured()   = 0;
    virtual void RestartRandomTests() =0;
};
/**
 * CThreadNotifier 
 * Active object implementation for capturing panics
 */
class CThreadNotifier : public CActive,
                        public MScreenshotObserver
{
public:
   /**
	* Static constructor.
	* @param aObserver Reference to panic observer 
	* @param aLogger A reference to the CLogger class for logging panics.
	* @param aSettings A reference to the CSettings class
	* @param aEikonEnv A refernce to the CEikionEnv class
	* @return CThreadNotifier* A pointer to the newly alocated CThreadNotifier class.
	*                          NULL, if the class cannot be created
	*/
	static CThreadNotifier* NewL(MPanicObserver& aObserver, CLogger& aLogger, CSettings& aSettings,CEikonEnv* aEikonEnv);
	
   /**
	* Static constructor. On return the instance is left to the CleanupStack
	* @param aObserver Reference to panic observer 
	* @param aLogger A reference to the CLogger class for logging panics.
	* @param aSettings A reference to the CSettings class
	* @param aEikonEnv A refernce to the CEikionEnv class
	* @return CThreadNotifier* A pointer to the newly alocated CThreadNotifier class.
	*                          NULL, if the class cannot be created
	*/
	static CThreadNotifier* NewLC(MPanicObserver& aObserver, CLogger& aLogger, CSettings& aSettings,CEikonEnv* aEikonEnv);
	
   /**
	* Destructor
	*/
    ~CThreadNotifier();
     
   /**
	* Function to issues a request for notification of the death of a thread.
	*/
    void IssueRequest();
    
private:
   /**
	* Constructor
	* @param aObserver Reference to panic observer 
	* @param aLogger A reference to the CLogger class for logging panics.
	* @param aSettings A reference to the CSettings class
	* @param aEikonEnv A refernce to the CEikionEnv class
	*/
	CThreadNotifier(MPanicObserver& aObserver, CLogger& aLogger, CSettings& aSettings, CEikonEnv* aEikonEnv);
    /**
     * ConstructL
     */
	void ConstructL();
	/**
	 * Function that indicates the exit type of the thread
	 * @param aExitType Exit type
	 */
	void ExitType(TInt aExitType);
	
	/**
	 * Callback implementation from the Image Capturer
	 * @param aInterval 
	 */
	void PerformNextAction(TInt aInterval);

private: //From CActive
   /**
	* Cancels any outstanding request.
	*/
	void DoCancel();
	
   /**
	* Handles the completion of the active request.
	*/
	void RunL();
    
private:        
   /**
	* Reference to the Logger class
	*/
	CLogger& 					iLogger;
   /**
	* Reference to the settings class
	*/
	CSettings&  				iSettings;
   /**
	* Exit type
	*/
	TBuf<KBuffer64>       	    iExitType;
   /**
	* Exit type
	*/
	CEikonEnv* 					iEEnv;
   /**
	* Handle to thread death notifier
	*/
	RUndertaker 			    iUndertaker;
   /**
	* Thread handle
	*/
	TInt        			    iThreadHandle;
	/**
    * Handle to panic observer
    */
    MPanicObserver&             iObserver;
};

#endif /*THREADNOTIFIER_H_*/
