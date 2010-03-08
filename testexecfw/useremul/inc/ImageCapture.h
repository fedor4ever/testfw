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
 * File Name: ImageCapture.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Active object implementation for Image capture
 *------------------------------------------------------------------
 -
 *
 */
#ifndef IMAGECAPTURE_H_
#define IMAGECAPTURE_H_

//System Includes
#include <e32base.h>
#include <ImageConversion.h>

//User Includes
#include "Settings.h"

/**
 * MScreenshotObserver, an observer to CImageCapture class.
 */
class MScreenshotObserver
{
public:
	/**
	 * Function that gets called to perform the next action after saving an image
	 * @param aInterval duration after which the next action is performed 
	 */
    virtual void PerformNextAction(TInt aInterval) = 0;
};

/**
 * CImageCapture
 * Class that handles image capturing functionality
 */
class CImageCapture : public CActive
{
public:
	/**
	 * States of the Image capture active object
	 */
	enum TScreenShotStatus
	{
		EIdle,
		EWaiting,
		ECapturing,
		ESaveComplete
	};
	
   /**
	* Static constructor.	 
	* @param aSettings A reference to the CSettings class
	* @param aObserver A reference to the MScreenshotObserver class
	* @param aEikonEnv A refernce to the CEikionEnv class	
	* @return CImageCapture* A pointer to the newly alocated CImageCapture class.
	*                          NULL, if the class cannot be created
	*/
	static CImageCapture* NewL(CSettings& aSettings,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv);
   /**
	* Static constructor. On return the instance is left to the CleanupStack
	* @param aSettings A reference to the CSettings class
	* @param aObserver A reference to the MScreenshotObserver class
	* @param aEikonEnv A refernce to the CEikionEnv class	
	* @return CImageCapture* A pointer to the newly alocated CImageCapture class.
	*                          NULL, if the class cannot be created
	*/
	static CImageCapture* NewLC(CSettings& aSettings,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv);
		
	/**
	 * Destructor
	 */
	~CImageCapture();
     
	/**
	 * Function to start capturing images. This function waits until the 
	 * application UID passed matches with the foreground running application UID
	 * If NULL UID is passed, the Image capturer will not wait and continues the 
	 * image saving operation  
	 * @param aName file path
	 * @param aXmlFileName name of the script file
	 * @param aAppUid application UID
	 */
     void CaptureL(const TDesC& aName, const TDesC& aXmlFileName, TUid aAppUid);

private:
   /**
	* Constructor
	* @param aSettings A reference to the CSettings class
	* @param aObserver A reference to the MScreenshotObserver class
	* @param aEikonEnv A refernce to the CEikionEnv class	
	*/
	CImageCapture(CSettings& aSettings,MScreenshotObserver& aObserver, CEikonEnv* aEikonEnv);
	
	/**
	 * ConstructL
	 */
	void ConstructL();
	
	/**
	 * Function to start capturing images.
	 * 
	 */
	void DoCaptureL();
	
	/**
	 * Function that saves images
	 * aName Screenshot name
	 */
	void DoSaveL(const TDesC& aName);
	/**
	 * Gets the file name. This function checks if the filename
	 * already exists on the phone or not. This function prevents 
	 * the capturer overriding the existing file. 
	 * aName Screenshot name
	 */	
    HBufC* GetNextFileNameLC(const TDesC& aName) const;
	    
   /**
	* Fires an event.
	* @param aStatus Image Capture Active objects status
	*/
	void Fire(TScreenShotStatus aStatus);

   /**
	* Fires capture event after aTime.
	* @param aTime time interval
	*/
	void FireCapture(TTimeIntervalMicroSeconds32 aTime);

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
	* Handle to the CImageEncoder class
	*/ 
	CImageEncoder*       iImageEncoder;
   /**
	* Handle to the CFbsBitmap class
	*/ 
	CFbsBitmap* 	     iBitmap;
   /**
	* Handle to the CSettings class
	*/
	CSettings&           iSettings;
   /**
	* Enumeration with different 
	* states of the Image capture active object
	*/
	TScreenShotStatus    iScreenShotStatus;
   /**
	* Timer handle 
	*/
	RTimer               iTimer;
   /**
	* Screenshot file name
	*/	
	TBuf<KPATHLEN>       iFileName;
   /**
	* XML script name
	*/
	TBuf8<KPATHLEN>		 iXmlFileName;
   /**
	* Application UID
	*/	
	TUid                 iAppUid;
   /**
	* Handle to CEikonEnv class
	*/
	CEikonEnv* 			 iEEnv;
   /**
	* Reference to the MScreenshotObserver class
	*/	
	MScreenshotObserver& iObserver;
};

#endif /*IMAGECAPTURE_H_*/
