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
 * File Name: Logger.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used for logging purposes
 *------------------------------------------------------------------
 -
 *
 */

#ifndef LOGGER_H__
#define LOGGER_H__

//System Includes
#include <eikenv.h>
#include <flogger.h>
#include <f32file.h>

//User Includes
#include "UserEmulatorScriptsView.h"
#include "Constants.h"

//Forward Declarations
class CSettings;

/**
 * CLogger 
 * Class used for logging purposes
 */
class CLogger : public CBase
{
public:
   
   /**
	* ConstructL
	*/
	void ConstructL();
   /**
	* Constructor
	* @param aScriptView Reference to the script view
	* @param aSettings Reference to the CSettings class
	* @param aEikonEnv Reference to the CEikonEnv class
	*/
	CLogger(CUserEmulatorScriptsView& aScriptView,CSettings& aSettings, CEikonEnv* aEikonEnv);
   /**
	* Destructor
	*/
	~CLogger();
 
   /**
	* Function to write log into the log file
	* @param aText Reference to a 8 bit descriptor
	* @param aHeader boolean indicating presence of headers or values in the logging information
	* @param aHash boolean indicating presence of # in the begging of the log line
	*/
    void WriteLogL(const TDesC8& aText, TBool aHeader,TBool aHash=EFalse);
   
   /**
	* Function to write log into the log file
	* @param aText Reference to a 16 bit descriptor
	* @param aHeader boolean indicating presence of headers or values in the logging information
	* @param aHash boolean indicating presence of # in the begging of the log line
	*/
    void WriteLogL(const TDesC16& aText, TBool aHeader,TBool aHash=EFalse);
   
   /**
	* Function to close log file handles
	*/
    void CloseFileHandles();
    
private:
   /**
	* Function to write log into the log file
	* @param aText Reference to a 8 bit descriptor
	* @param aHeader boolean indicating presence of headers or values in the logging information
	* @param aHash boolean indicating presence of # in the begging of the log line
	*/
	TInt Write(const TDesC8& aText, TBool aHeader,TBool aHash=EFalse);
   /**
	* Function to write log into the log file
	* @param aText Reference to a 16 bit descriptor
	* @param aHeader boolean indicating presence of headers or values in the logging information
	* @param aHash boolean indicating presence of # in the begging of the log line
	*/
	TInt Write(const TDesC16& aText, TBool aHeader,TBool aHash=EFalse);
private:
   /**
	* Reference to the file handle
	*/
    RFile 		 iFile;
   /**
 	* Reference to the file server session
 	*/
    RFs   		 iFs;
   /**
  	* Log buffer
  	*/
    TBuf8<KBuffer1024>  iLogBuffer;
   /**
  	* Reference to the CSettings class 
  	*/
    CSettings& 	 iSettings;
   /**
  	* Reference to the CEikonEnv class
  	*/
    CEikonEnv* 	 iEEnv;
   /**
   	* Reference to the CUSerEmulatorScriptsView class
   	*/
    CUserEmulatorScriptsView& iScriptView;
   /**
   	* Boolean indicating drive validity
   	*/
    TBool		 iDriveLockError;
};

#endif //__LOGGER_H__
