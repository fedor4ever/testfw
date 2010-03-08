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
 * File Name: RandomTest.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used to perform random test on applications or
 * on the system generally
 *------------------------------------------------------------------
 -
 *
 */
#ifndef RANDOMTEST_H_
#define RANDOMTEST_H_

// System Includes
#include <W32STD.H>
#include <APGCLI.H>
#include <E32BASE.H>
// User Includes
#include "Settings.h"
#include "Logger.h"
#include "UserEmulatorApplication.h"
#include "Constants.h"
#include "GlobalCapturer.h"
 
// Class Declaration 
 
/**
 * Implements random test run. Active object.
 */
class CRandomTest: public CActive
	{
	public:
	    
	   /**
        * Static constructor
        * @param aObserver Key observer interface 
        * @param aRWsSession Reference to window server session
        * @param aSettings Reference to Setting class
        * @param aSession Reference to session with the application architecture server.
        * @param aLogger Reference to logger object
        * @return CRandomTest* A pointer to the newly allocated CRandomTest class.
        *                      NULL, if the class cannot be created
        */
		static CRandomTest* NewL(MKeyObserver& aObserver,RWsSession& aRWsSession,
		                          CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger);
	   /**
		* Static constructor. On return the instance is left to the CleanupStack
		* @param aObserver Key observer interface
		* @param aRWsSession Reference to window server session
        * @param aSettings Reference to Setting class
        * @param aSession Reference to session with the application architecture server.
        * @param aLogger Reference to logger object
        * @return CRandomTest* A pointer to the newly allocated CRandomTest class.
        *                      NULL, if the class cannot be created
       	*/
		static CRandomTest* NewLC(MKeyObserver& aObserver,RWsSession& aRWsSession,
		                          CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger);
	   /**
        * Destructor
        */
		~CRandomTest();
	public:
	    /**
        * Initialises and starts random test
        */
		void StartRandomTestL();
		/**
        * Stops random test
        */
		void StopRandomTest();
		/**
        * Restarts random test active object
        */
		void RestartRandomTest();
		/**
        * Cancels random test active object
        */
		void CancelRandomTest();
	protected:
	    /**
        * RunL from CActive class
        */
        void RunL();
        /**
        * DoCancel from CActive class
        */		
        void DoCancel();
	private:
	    /**
        * Constructor
        * @param aObserver Key observer interface
        * @param aRWsSession Reference to window server session
        * @param aSettings Reference to Setting class
        * @param aSession Reference to session with the application architecture server.
        * @param aLogger Reference to logger object
        */
		CRandomTest(MKeyObserver& aObserver,RWsSession& aRWsSession,CSettings& aSettings,RApaLsSession& aSession,CLogger& aLogger);
		/**
        * Second phase constructor
        */
		void ConstructL();
		/**
        * Test list check
        */
		TBool IsUidInTestList(TUid aUid);
	private:
	    /**
        * RTimer object member
        */
		RTimer			iTimer;
		/**
        * Reference to window server session
        */
		RWsSession* 	iRWsSession;
		/**
        * Reference to Settings class
        */
		CSettings& 		iSettings;
		/**
        * Reference to key observer interface
        */
		MKeyObserver&   iObserver;
		/**
        * Reference to session with the application architecture server.
        */
		RApaLsSession&	iSession;
		/**
        * Buffer. Contains last app's name on foreground (for logging)
        */
		TBuf<KBuffer512> iLastAppName;
		/**
        * Reference to Logger object
        */
		CLogger& 		iLogger;
		/**
        * Counts how many times one application runs. (If multiple test applications are running by random test)
        */
		TInt			iAppTestCounter;
		/**
        * Counts test steps and logs one app. name only if it's in foreground for a long time.
        */
		TInt			iAppNameLoggingCounter;
		/**
        * Random tester sends Ok button press event according to this counter.
        */
		TInt			iCounterForOkButton;
		/**
        * Flag. Shows if there was at least one valid app uid (True if there was one)
        */
		TBool			iCantStartAppToTest;
		/**
        * Array of random test UIDs
        */
		RArray<TUid>    iSlotUIDs;
		};

#endif // RANDOMTEST_H_ 
