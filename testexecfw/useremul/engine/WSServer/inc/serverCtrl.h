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
 * File Name: serverCtrl.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Declares Anim dll server side class
 *------------------------------------------------------------------
 -
 *
 */

#ifndef SERVERCTRL_H__
#define SERVERCTRL_H__

// System Includes
#include <w32adll.h>
#include <f32file.h>
// User Includes
#include "CaptureKeyTimer.h"
#include "Constants.h"
// Constants
static const TInt KStepSize = 1;
static const TInt KAnimationReset = 1;
static const TInt KResolutionChange = 2;

// Class Declaration

/**
 *  Implements Anim dll server side class.
 *  Records event by OfferRawEvent callback function
 */
class CServerCtrl : public CWindowAnim, public MCaptureKeyTimerNotify
	{
public:
	/**
	 * Constructor
	 */
	CServerCtrl();
	/**
	 * Destructor
	 */
	virtual ~CServerCtrl();

public:
	/**
	 * Second phase constructor
	 * @param aArgs not used
	 * @param aHasFocus not used
	 */
	void ConstructL(TAny* aArgs, TBool aHasFocus);
	/**
	 * For communication between client and server side.
	 * @param aCommand	Command types (see TAnimCommands in clientcommander.h)
	 * @param aArgs  	Sent data from client side
	 * @return			Integer code as a response
	 */
	TInt CommandReplyL(TInt aCommand, TAny* aArgs);
	/**
	 * For communication between client and server side. This version is not used
	 * @param aCode code
	 * @return			Integer code as a response
	 */
	TInt CommandReplyL(TInt aCode);
	/**
	 * For communication between client and server side without response to client. This version is not used
	 * @param aCommand	Command types (see TAnimCommands in clientcommander.h)(Not Used)
	 * @param aArgs  	Sent data from client side (Not Used)
	 */
	void Command(TInt aCommand, TAny* aArgs);
	/**
	 * Not used
	 */
	void Redraw();
	/**
	 * Not used
	 * @param aDT date time 
	 */
	void Animate(TDateTime* aDT);
	/**
	 * Not used
	 * @param aState not used
	 */
	void FocusChanged(TBool aState);
	/**
	 * Callback function. Called when an event occurs (key press, pointer event, etc.)
	 * @param aRawEvent 	Type of the event
	 * @return			ETrue if event is processed.
	 */
	TBool OfferRawEvent(const TRawEvent &aRawEvent);
	/**
	 * Callback function. Counts seconds for long key presses
	 */
	void KeyTimerExpired();

private:
	/**
	 * Inserts time interval between two events into the xml script
	 */
	void CheckTimerInsertWait();
	/**
	 * Buffers and writes output to xml script 
	 * @param aBuf	The data to write to script
	 */
	void WriteFile(TDesC8& aBuf);
private:
	/**
	 * File session for scripts
	 */
	RFs iFs;
	/**
	 * Rfile object for scripts
	 */
	RFile iRFile;
	/**
	 * Name and path of xml script. Received from client side
	 */
	TBuf<512> iKeyFilePath;
	/**
	 * Timer active object for long key presses
	 */
	CCaptureKeyTimer* iCaptureKeyTimer;
	/**
	 * Flag to sign if record started
	 */
	TBool iRecord;
	/**
     * Seconds while a key is pressed
     */
	TUint32 iSecCounter;
	/**
     * Flag to sign that camera button is pressed (Insert screen shot command to xml script)
     */
	TBool iCameraButtonPressed;
	/**
     * Name of foreground application when scamera button is pressed. Received from client side.
     */
	TBuf8<256> iAppName;
    /**
     * Flag to sign delay. (It means waiting period until camre application closes) 
     */
	TBool iFirstDelay;
	/**
     * Flag to sign if screen reset is over and recording can start. 
     */
	TBool iIsEventEnabled;
	/**
     * Flag to sign if device is in landscape or portrait mode
     */
	TBool iOrientation;
	/**
     * Buffer for xml script output. For performance reasons.
     */
	HBufC8* iMainBuffer;
	/**
     * Actual number of events stored in the buffer  
     */
	TInt iWritePointer;
	/**
     * Time stamp of previous event 
     */
	TTime iWaitTimer;
	/**
     * Count of 'pressed' No buttons. (Screen reset) 
     */
	TInt iNoCount;

	};

#endif // SERVERCTRL_H__
// End of File
