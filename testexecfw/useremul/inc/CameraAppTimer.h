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
 * File Name: CameraAppTimer.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Camera app's timer CActive class
 *------------------------------------------------------------------
 -
 *
 */

#ifndef CCAMERAAPPTIMER_H_
#define CCAMERAAPPTIMER_H_

// System Includes
#include <E32BASE.H>

// Class Declarations
/**
 *  Camera observer interface
 */
class MCameraAppObserver
	{
	public:
	    /**
        *  Callback function. Called when timer is expired
        */
		virtual void CheckCameraAppL() = 0;
		};
 
/**
 * Timer active object
 */
class CCameraAppTimer: public CActive
		{
	public:
	    /**
        * Static constructor
        * @param aNotify A reference to the camera app. related timer observer.
        * @return CCameraAppTimer* A pointer to the newly allocated CCameraAppTimer class.
        *                      NULL, if the class cannot be created
        */
		static CCameraAppTimer* NewL(MCameraAppObserver& aNotify);
		/**
        * Destructor
        */
		~CCameraAppTimer();
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
        * @param aNotify  A reference to the camera app. related timer observer.
        */
		CCameraAppTimer(MCameraAppObserver& aNotify);
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
        * Reference to camera app. related timer observer
        */
		MCameraAppObserver&		iNotify;
		};

#endif /* CCAMERAAPPTIMER_H_ */
