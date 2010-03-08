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
 * File Name: UserEmulatorAppUi.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Application User Interface class
 *------------------------------------------------------------------
 -
 *
 */
 
#ifndef USEREMULATORAPPUI_H__
#define USEREMULATORAPPUI_H__

// System Includes
#include <aknappui.h>
#include <apgcli.h>
#include <akntabgrp.h>              // CAknTabGroup
#include <aknnavide.h>              // CAknNavigationDecorator

// User Includes
#include "XmlHandler.h"
#include "ScanFolders.h"
#include "DelayTimer.h"
#include "Settings.h"
#include "ThreadNotifier.h"
#include "GlobalCapturer.h"
#include "Logger.h"
#include "clientdll.h"
#include "clientcommander.h"
#include "RandomTest.h"
#include "ImageCapture.h"
#include "Constants.h"
// Constant
_LIT( KServerDllFilename, "\\system\\apps\\anim\\ANIMATION_SERVER_0x20025CA8" );

// Forward declarations
class CUserEmulatorScriptsView;

// Class Declaration

/**
 * CUserEmulatorAppUi application UI class.
 * Interacts with the user through the UI and request message processing
 * from the handler class
 */
class CUserEmulatorAppUi : public CAknAppUi,
                           public MXmlHandlerObserver,
                           public MActionObserver,
                           public MKeyObserver,
                           public MPanicObserver,
                           public MScreenshotObserver
{
public:
	/**
	 * ConstructL.
	 * 2nd phase constructor.
	 */
	void ConstructL();

	/**
	 * CUserEmulatorAppUi.
	 * C++ default constructor. This needs to be public due to
	 * the way the framework constructs the AppUi
	 * @param aSession AppUi's Window session
	 */
	CUserEmulatorAppUi(RWsSession& aSession);

	/**
	 * ~CUserEmulatorAppUi.
	 * Virtual Destructor.
	 */
	virtual ~CUserEmulatorAppUi();
	
   /**
    * Callback function. Called when Power button is pressed and recording stops
    */
	void TerminationKeyReceived();
	
   /**
    * Put this application foreground at the end of recording and at the end of script execution
    */
	void BringUserEmulatorToFront();
   /**
    * Callback from Thread Notifier when a panic occures
    */
	void PanicOccured();
	
	void RestartRandomTests();
   /**
    * Simply calls ScriptView's ShowScriptsL function
    */
	void ShowScriptsL();
	
   /**
    * Gets Logger's reference
    * @return   A CLogger instance
    */
	CLogger& Logger() const
    {
       return *iLogger;
    }
	
   /**
	* Starts script execution
	*/
	void StartTestL();
	
  /**
   * Sets timer which calls PerformAction when expired.
   * @param  aInterval Time in microsecs
   */
	void PerformNextAction(TInt aInterval);
  
  /**
   * Simply calls XmlHandler's StartParsingL function
   * @param  aFileName  Name of the xml script to parse
   * return  Error code
   */
   TInt StartParsingL( const TDesC&  aFileName );
  /**
   * Put this application foreground.
   */
   void BringToForeground();
  /**
   * Stops random test
   */
   void RandomTestEndsL();
   
  /**
   * Load application's TRect to a member variable of Setting
   */
   void SetApplicationRectInSettings();
   
  /**
   * Set system info flag
   */
   void SetSystemInfoFlag(TBool aValue);
  /**
   * Queries if there was a change in script file folder
   */
   void RefreshFileNotification();
   /**
   * Reads command line arguments
   * @param aParams - The arguments
   */
   void ProcessMessageL(TUid /*aUid*/, const TDesC8 &aParams);
   /**
   * Reads command line arguments
   * @param aCommand - Launch command code
   * @param aDocumentName - Buffer for arguments
   * @param aTail - Command line tail
   */
   TBool ProcessCommandParametersL(TApaCommand aCommand,TFileName &aDocumentName,
                const TDesC8 &aTail);
    /**
    * FolderScanCompletedL
    * Callback function. Called when script file folder is scanned and list box is initialised.
    */
    void FolderScanCompletedL();
private:

    /**
    * Converts portrait mode and landscape mode coordinates and it will converts coordinates for different devices
    * @param   x       Input x coordinate
    * @param   y       Input y coordinate
    * @param   xMod    Output x coordinate
    * @param   yMod    Output y coordinate
    */
    void CoordConversionByOrientation(TInt x, TInt y, TInt& xMod, TInt& yMod);
    /**
    * Finds app name in app list and gives back app uid
    * @param aAppName Name of app
    * @return App uid
    */
    TUid CUserEmulatorAppUi::GetAppUidFromName(TDesC& aAppName);
    /**
    * Processes command line arguments
    * @param aParams  - Arguments
    * @param aProcessByFolderScanCompleted - Flag. True if arguments have to be stored
    * in an array and to be processed later. False if process can take place sraight away.
    */
    void ProcessCmdParamsL(const TDesC &aParams,TBool aProcessByFolderScanCompleted);
    /**
    * Reads command line arguments in the original way (args process)
    * @param - output buffer for arguments
    * @param - Number of arguments
    * @return - False if params are longer (or equal) than 256 chars. True if shorter.
    */
    TBool GetArgsL(TDes& aParam, TInt& aArgs);
    
   /**
    * Creates and loads client side dll
    */
    void SetupClientDllL();
   /**
    * Creates RClientCommander object to communicate with the server side
    */
    void SetupCommander();
   /**
    *  Displays Settings dialog
    */
    void DoCmdSettingsL();
   /**
    * Context specific menu function
    * @param aResourceId   Menu Id
    * @param aMenuPane     Menu pane
    */
    void DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane);
   
    //From CEikAppUi 
   /** 
    * From CEikAppUi, HandleCommandL.
    * Takes care of command handling.
    * @param aCommand Command to be handled.
    */
    void HandleCommandL(TInt aCommand);
  
   /**
    * Callback function. Called when parsing is completed.
    * @param aError    Result of parsing
    * @param aState     not used
    */
	void OnParseCompletedL( TInt aError , TState aState);		
	
    /**
     * Main worker function. Executes parsed scripts
     */
	void PerformActionL();
	/**
	 * Starts Browser using given URL
	 * @param aUrl URL to Browser
	 */
    void OpenUrlL( const TPtrC& aUrl);
    /**
     *  Cancels script execution and resets the main list on screen after Power button pressed
     */
    void CancelScriptExecutionL();

   /**
    *  Start a Java app in Symbian app.
    */   
	TInt StartJavaApp(const TDesC&  aAppName);
	
   /**
    *  Cut one input string into two strings. Delimiter character is ',' . Input string
    *  can have 4 params and 3 delimiter. 
    *  @param aParamString  Input string (Eq: 23,27,43,21)
    *  @param aArgument1    First part of the original string with two params (Eq: 23,27)
    *  @param aArgument2    The second part with two params (Eq: 43,21)
    *  @return              Result if the function succeeded or not
    */
	TInt ParseMultipleParams(const TDesC& aParamString, TDes& aArgument1, TDes& aArgument2) const;
	
   /**
    *  Cut one input string into two strings. Limiter chatacter is ','
    *  @param aParamString  Input string
    *  @param aAppName      First part of the original string
    *  @param aArgument     The second part
    *  @return              Result if the function succeeded or not
    */
    TInt ParseParams( const TDesC&  aParamString, TDes& aAppName, TDes& aArgument) const;
    
   /**
    *  Cut one input string into one UId and one string. Limiter chatacter is ','
    *  @param aParamString  Input string
    *  @param anAppUid      Uid
    *  @param aArgument     The second part
    *  @return              Result if the function succeeded or not
    */
    TInt ParseParams( const TDesC&  aParamString, TUid& anAppUid, TDes& aArgument) const;
    
   /**
    *  Cut one input string into three UIds and one string. Limiter chatacter is ','
    *  @param aParamString  Input string
    *  @param anAppUid      Uid
    *  @param aViewUid      View Id
    *  @param aCustomMessageUid Message UId
    *  @param aCustomMessage    Custom message string
    *  @param aAppName      App name instead of Uid
    *  @return              Result if the function succeeded or not
    */
    TInt ParseParams( const TDesC&  aParamString, TUid& aAppUid,  TUid& aViewUid,
			          TUid& aCustomMessageUid, TDes& aCustomMessage, TDes& aAppName ) const;
	/**
	 *  From CCoeAppUi, HelpContextL.
	 *  Provides help context for the application.
	 *  size is changed.
	 *  @return    Help context array
	 */
	CArrayFix<TCoeHelpContext>* HelpContextL() const;
	
	/**
     * Resets the action list (Action list contains parsed actions from xml script)
     */
	void ResetLists();
	
	/**
     * Sends a 'button down' raw event with the given params
     * @param intargx   X coordinate of  the event
     * @param intargy   Y coordinate of  the event
     */
	void ButtonDownEvent(TInt intargx, TInt intargy);
	
	/**
     * Sends a 'button up' raw event with the given params
     * @param intargx   X coordinate of  the event
     * @param intargy   Y coordinate of  the event
     */
	void ButtonUpEvent(TInt intargx, TInt intargy);
	
	/**
     * Sends a 'drag' raw event with the given params.
     * @param intargx   X coordinate of  the end point of drag
     * @param intargy   Y coordinate of  the end point of drag
     */
	void PointerMoveEvent(TInt intargx, TInt intargy);
	
	/**
     * Starts random event test
     */
	void StartRandomTestL();
	
	/**
	 *  GetUid
	 *  Get application Uid from the ApaList
	 *  @param aAppuid AppUid Ex: [20001cf3]
	 *  @return HBufC* appplication Uid in the Hex format Ex: 0x20001cf3
	 */
	HBufC* GetUidL(const TDesC& aAppuid);	

public:
  
  /**
   * Flag that indicates the system information status. 
   */
	TBool         iSystemInfo;

private:
    /**
     * Reference to script view class
     */
	CUserEmulatorScriptsView* iScriptsView;
	
	/**
     * Reference to xml handler class 
     */
	CXmlHandler*   iXmlHandler; // to parse XML document
	
	/**
     * Reference to delay timer class 
     */
	CDelayTimer*   iDelayTimer;
	
	/**
     * Action list related indexes 
     */
	TInt  iActionIndex;	
	TInt  iLoopCount;
	TBool iLoopExists;
   /**
    * A session with the application architecture server
    */
	RApaLsSession	iSession;
	
   /**
    * Reference to Setting class
    */
	CSettings* iSettings;
	
   /**
    * Flag. If true then last script was stopped
    */
	TBool      iStopScripts;
	
   /**
    * Index of the selected script
    */
	TInt      iSelectedIndex;
	
   /**
    * Descriptor for log entries
    */
	TBuf<KBuffer512> iLogBuffer;
	
   /**
    * Action list (script execution)
    */
    RPointerArray<CAction>	iActionList;    
    RArray<TUint>    		iScriptErrorIndexes;
    
   /**
    * Flag. True if an error occured inside the application
    */
    TBool                   iError;
    
   /**
    * Reference to thread notifier class
    */
    CThreadNotifier*        iThreadNotifier;
    
   /**
    * Reference to global capturer class
    */
    CGlobalCapturer*        iGlobalCapturer;
    
   /**
    * Reference to logger class
    */
    CLogger* 				iLogger;
    
   /**
    * Represents actual loop's number
    */
    TInt                    iLoopCounter;
    
   /**
    * Represents the selected script's number
    */
    TInt                    iSelectedScriptCount;
    
   /**
    * Flag. True if a script is started
    */
	TBool					iScriptRun;
	
   /**
    * Flag. True if recording started
    */
	TBool					iRecording;
	
   /**
    * Flag. True if random test is started
    */
	TBool					iRandomTestStarted;
	
   /**
    * Loads RClientDll object
    */
	RClientDll 				iClientDll;
	
   /**
    * Initialises RClientCommander object
    */
	RClientCommander 		iClientCommander;
	
   /**
    * Reference to random test class
    */
	CRandomTest*			iRandomTest;
	
   /**
    * Uid of started Java applet
    */
	TUid					imidletUID;
	
   /**
    * Counter. An error message 'Script might not work properly on Non touch enabled device'
    * will be displayed only if this counter is zero. Otherwise the message would displayed many
    * times in a loop 
    */
	TInt                    iPointerCount;   
    /**
    * Array for loops. Stores loop counter and 'action' number where execution has to continue
    * after end of loop tag.
    */	
	RArray<TUint>           iNestedLoopArray;
   /**
    * Array for command-line arguments when they are processed later. (After list box initialisation)
    */  
	CDesC8Array*            iCommandLineScriptsArray;
	
	TBool                   iIsFolderScanCompletedCalled;
	
   /**
    * Manufacturer code of the device
    */
    TInt iManufacturer;
};
#endif // USEREMULATORAPPUI_H__
// End of File
