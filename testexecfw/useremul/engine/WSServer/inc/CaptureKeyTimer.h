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
 * File Name: CaptureKeyTimer.h
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
#ifndef CAPTUREKEYTIMER_H_
#define CAPTUREKEYTIMER_H_

// System Includes
#include <E32BASE.H>
 
// Class Declaration

/**
 * Key timer interface
 */ 
class MCaptureKeyTimerNotify
	{
	public:
	    /**
        * Callback function. Called when timer is expired
        */
		virtual void KeyTimerExpired() = 0;
		};

/**
 * Timer active object
 */
class CCaptureKeyTimer: public CActive
		{
	public:
	    /**
        * Static constructor
        * @param aNotify A reference to the key timer observer.
        * @return CCaptureKeyTimer* A pointer to the newly allocated CCaptureKeyTimer class.
        *                      NULL, if the class cannot be created
        */
		static CCaptureKeyTimer* NewL(MCaptureKeyTimerNotify& aNotify);
		/**
        * Destructor
        */
		~CCaptureKeyTimer();
	public:
	    /**
        * Start timer
        * @param aInterval  Microsecs after timer supposed to be expired
        */
		void After(TTimeIntervalMicroSeconds32 aInterval);
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
        * @param aNotify  A reference to the key timer observer.
        */
		CCaptureKeyTimer(MCaptureKeyTimerNotify& aNotify);
		/**
        * Second phase constructor
        */
		void ConstructL();
	private:
	    /**
        * RTimer object member
        */
		RTimer					iTimer;
		/**
        * Reference to key timer observer
        */
		MCaptureKeyTimerNotify&	iNotify;
		};

#endif // CAPTUREKEYTIMER_H_
