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
 * File Name: GlobalCapturer.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 *  Key capturing class
 *------------------------------------------------------------------
 -
 *
 */
 
#ifndef GLOBALKEYCAPTURE_H__
#define GLOBALKEYCAPTURE_H__

// System Includes
#include <e32base.h>
#include <w32std.h>
#include <aknglobalnote.h>
#include <COECNTRL.H>
// User Includes
#include "Settings.h"
#include "clientcommander.h"
#include "CameraAppTimer.h"
#include "Constants.h"

// Forward declarations
class RWindowGroup;
class CApaWindowGroupName;

// Class Declarations
/**
 * Key observer interface class
 */
class MKeyObserver
{
public:
    /**
    * Callback to handle Power key button press and stops scripts
    */
	virtual void TerminationKeyReceived()   = 0;
	/**
    * Callback to notify AppUi to bring app to foreground
    */
	virtual void BringUserEmulatorToFront() = 0;
	/**
    * Stops random test by this callback function
    */
	virtual void RandomTestEndsL() = 0;
};

/** 
 * Registers itself for event capturing.
 * Is notified about events via CActive calls
 */
class CGlobalCapturer : public CActive, public MCameraAppObserver
{
	public:
	    /**
        * Static constructor
        * @param aObserver Reference key press observer
        * @param aSettings Reference to Setting class
        * @param aScriptRun Reference to script run flag from AppUi
        * @param aRandomTestStarted Reference to random test started flag from AppUi
        * @param aClientCommander Reference to client commander client side object
        * @param aEikonEnv Reference to controls environment
        * @param aSession Reference to session with the application architecture server.
        * @return CGlobalCapturer* A pointer to the newly allocated CGlobalCapturer class.
        *                      NULL, if the class cannot be created
        */
		static CGlobalCapturer* NewL(MKeyObserver& aObserver, CSettings& aSettings, TBool& aScriptRun, 
				TBool& aRandomTestStarted, RClientCommander& aClientCommander, CEikonEnv* aEikonEnv,RApaLsSession& aSession);

	   /** 
         * Destructor
         */
		~CGlobalCapturer();
		
	   /** 
		 * Registers itself for the key press event
		 */
		void StartCapturingL();
				
       /** 
        * Starts capturing Power button
        */
		void StartL();
	   /** 
        * Stops capturing Power button
        */
		void Stop();
	   /** 
        * Stops capturing keys (in Window server)
        */
	    void StopRecordKeysL();
	   /** 
        * Starts capturing keys by Window server
        */
	    TInt StartRecordKeysL();
	   /** 
        * Checks if camera app is in memory or not
        */
	    void CheckCameraAppL();
	   /** 
        * Resets Menu screen and (Home screen's) idle screen
        */
		TInt ScreenReset();
	   /** 
        * Kill apps (with exceptions)
        */
		void KillUnwantedAppsResetL();
		
	private: //From CActive
		/**
		 * Is called by active scheduler when key press happens
		 */
		void RunL();

		/**
		 * Is called when event listening should be stopped
		 */
		virtual void DoCancel();
	
	private:
	   /**
		* Constructor
		* @param aObserver Reference key press observer
		* @param aSettings Reference to Setting class
		* @param aScriptRun Reference to script run flag from AppUi
		* @param aRandomTestStarted Reference to random test started flag from AppUi
		* @param aClientCommander Reference to client commander client side object
		* @param aEikonEnv Reference to controls environment
		* @param aSession Reference to session with the application architecture server
		*/
		CGlobalCapturer(MKeyObserver& aObserver, CSettings& aSettings, TBool& aScriptRun, TBool& aRandomTestStarted,
				RClientCommander& aClientCommander, CEikonEnv* aEikonEnv,RApaLsSession& aSession);
				
	   /**
        * Second phase constructor
        */
		void ConstructL();
		
	   /**
        * Filters characters which shouldn't be in a file name
        */
		TBool StrictFileNameCheck(const TDesC& aFileName);
			
	public:
	   /**
        * Flag. True if recording is running
        */
	    TBool		iRecord;
		
	private:
	   /**
        * Handler to Power button
        */
		TInt32 iCaptureHandle1;
	   /**
        * Session to the window server
        */
		RWsSession    iWsSession;
	   /**
        * Window group object
        */
		RWindowGroup* iWindowGroup;
	   /**
        * Key event details structure
        */
		TKeyEvent*		iKeyEvent;
	   /**
        * Handle to the capturing request
        */
		MKeyObserver& 	iObserver;
	   /**
        * Reference to settings
        */
		CSettings& 		iSettings;
	   /**
        * A handle to a file server session
        */
		RFs 			iFs;
	   /**
        * Reference to script run flag
        */
		TBool&			iScriptRun;
	   /**
        * Reference to randomtest started flag
        */
		TBool&			iRandomTestStarted;
	   /**
        * Flag. True if key capturing started
        */
		TBool			iKeyCaptureStart;
	   /**
        * Reference to client commander object
        */
		RClientCommander& iClientCommander;
	   /**
        * Buffer for script name and path
        */
		TBuf8<512> 		iBuf8;
	   /**
        * Reference to controls environment
        */
		CEikonEnv* 		iEEnv;
	   /**
        * Reference to camera timer
        */
		CCameraAppTimer* iCameraAppTimer;
	   /**
        * Buffer for app's name which are in foreground. 
        */
		TBuf8<512> 		iAppName;
	   /**
        * Reference to session with the application architecture server
        *
        */
		RApaLsSession&	iSession;
};

#endif //	GLOBALKEYCAPTURE_H__
