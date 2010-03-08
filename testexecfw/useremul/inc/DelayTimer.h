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
 * File Name: DelayTimer.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for asynchronous key events handling
 *------------------------------------------------------------------
 -
 *
 */
#ifndef DELAYTIMER_H__
#define DELAYTIMER_H__

//System includes
#include <e32base.h>

/**
 * MActionObserver, an observer to CDelayTimer class.
 */
class MActionObserver 
{
public:
	/**
	 * Callback function to invoke the next action element
	 */
    virtual void PerformActionL() = 0;
};

/**
 *  CDelayTimer class.
 */
class CDelayTimer : public CTimer
{
    public:
       /**
        * Two-phased constructor.
        * @param aObserver Reference to the observer class
        */
        static CDelayTimer* NewL( MActionObserver& aObserver );        
       /**
        * C++ default constructor.
        */
        CDelayTimer(MActionObserver& aObserver);
       /**
        * Destructor
        */
        ~CDelayTimer();

    public:
       /**
		* Function to issue a request for notification of next action 
		* @param aInterval Time interval
		*/
        void Wait(TInt aInterval);
        
    private:
       /**
        * 2nd phase constructor.
        */
        void ConstructL();

    private: // From CActive
       /**
        * After completed timer period, execution comes to RunL
        */
        void RunL();

    private:
    	/**
    	 * Reference to the observer class
    	 */
        MActionObserver& iActionObserver;
};

#endif // DELAYTIMER_H__

// End of File

