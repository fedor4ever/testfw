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
 * File Name: Settings.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used for storing the settings information of the application
 *------------------------------------------------------------------
 -
 *
 */
#ifndef SETTINGS_H
#define SETTINGS_H

//System Includes
#include <e32base.h>    // CBase
#include <e32def.h>     // TBool
#include <aknapp.h>
#include <s32stor.h>    // CDictionaryStore
#include <aknsettingitemlist.h>     // CAknSettingItemList

// User Includes
#include "Constants.h"

//Forward declarations
class CApaApplication;

//Constants
#define KPATHLEN   512

_LIT(KDefaultPath,"C:\\Data\\TestScripts\\");
_LIT(KDefaultLogPath,"C:\\Data\\Logs\\UserEmulator\\UserEmulator.csv");

typedef TBuf<KBuffer256> TRandomTestAppID;

/**
 * CSettings 
 * This class is used for storing the settings information of the application
 */
class CSettings : public CBase
{
public:
   /**
	* Constructor
	* @param aApp Reference to CApaApplication class
	* @param aEikonEnv Reference to CEikonEnv class
	*/
    CSettings(CApaApplication& aApp, CEikonEnv* aEikonEnv);
    
   /**
	* ConstructL
	*/
    void ConstructL();
    
   /**
	* Function to Load the User Emulator settings
	*/
    void LoadL();
    
   /**
	* Function to Store the User Emulator settings
	*/
    void StoreL();
    
   /**
	* Function to validate the correct drive 
	* @param aPath Script/Log file path
	*/   
	TInt CheckDrive(const TDesC& aPath);
   
   /**
	* Function to validate the scripts or log file path
	* @param aPath Script/Log file path 
	*/
	TBool CheckPathL(const TDesC& aPath);
	
public:
   /**
	* Scripts path
	*/    
	TBuf<KPATHLEN> 	     iScriptsPath;
   /**
	* Log file path
	*/	
    TBuf<KPATHLEN> 	     iLogPath;
   /**
 	* Log status ON/OFF
 	*/    
    TBool        	     iLogStatus; 
   /**
 	* Random test Status 
 	* ON : Run random test on the specified App UIDs
 	* OFF: Run random test on all applications under application Menu
 	*/
    TBool	  	         iRandomTestAppIdStatus; 
   /**
  	* Application UID
  	*/
    TRandomTestAppID     iRandomTestAppID;
    
   /**
	* Main screen size
	*/    
    TRect		         iAppRect;
private:
   /**
	* CApaApplication instance
	*/
	CApaApplication& 	 iApp;
   /**
	* CEikEnv handle
	*/    
	CEikonEnv* 			 iEEnv;
};
#endif

