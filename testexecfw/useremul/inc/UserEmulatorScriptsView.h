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
 * File Name: UserEmulatorScriptsView.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Application View class
 *------------------------------------------------------------------
 -
 *
 */
#ifndef USEREMULATORSCRIPTSVIEW_H__
#define USEREMULATORSCRIPTSVIEW_H__

// System Includes
#include <coecntrl.h>
#include <aknlists.h>
#include <Etel3rdParty.h>
#include <MTCLREG.H>
#include <e32cmn.h>
// User Includes
#include "FolderContentsListener.h"
#include "Settings.h"
#include "ScanFolders.h"
#include "NotifyFileChange.h"
#include "Constants.h"

// Forward Declarations
class CEikEdwin;
class CEikColumnListBox;

// Constants

typedef TBuf<KBuffer512> TBigFileName;

enum TSelectStates
    {
    EOneItemSel = 1,
    EOneItem,
    EMoreItemsSel,
    EMoreItems,
    EMoreItemsAll,
    EMoreItemsAllExc,
    EMoreItemsNotAll,
    EMoreItemsNotAllNotCurrent
    };

const TInt KDriveSize = 2;
// Class Declarations

/**
* IMEI observer interface class
*/
class MImeiObserver
{
	public:
	    /**
        * Gets device details
        * @param aIMEI Reference to a descriptor which will hold details
        * @param aError not used
        */
		virtual void GotIMEI(const TDesC& aIMEI,TInt aError) = 0;
};
 
/**
* CActive object. Reads IMEI data
*/
class CImeiReader : public CActive
{
public:
   /**
    * Static constructor
    * @param aObserver Reference to IMEI observer
    * @return CImeiReader* A pointer to the newly allocated CImeiReader class.
    *                      NULL, if the class cannot be created
    *
    */
	static CImeiReader* NewL(MImeiObserver& aObserver);
   /**
	* Static constructor. On return the instance is left to the CleanupStack
	* @param aObserver Reference to IMEI observer
    * @return CImeiReader* A pointer to the newly allocated CImeiReader class.
    *                      NULL, if the class cannot be created
	*/
	static CImeiReader* NewLC(MImeiObserver& aObserver);
	/**
	 * Destructor
	 */
	~CImeiReader();
	
protected:
   /**
    * Cancel function of CActive class
    */
	void DoCancel();
   /**
    * RunL function of CActive class
    */
	void RunL();
private:
   /**
    * Constructor
    * @param aObserver Reference to IMEI observer
    * 
    */
	CImeiReader(MImeiObserver& aObserver);
   /**
    * Second phase constructor
    *
    */
    void ConstructL();
    
private:
   /**
    * Reference to IMEI observer
    *
    */
	MImeiObserver& 				iObserver;
   /**
    * Reference to Telephony object
    *
    */
	CTelephony* 				iTelephony;
   /**
    * Define the mobile phone identity.
    */
	CTelephony::TPhoneIdV1 		iIdV1;
	CTelephony::TPhoneIdV1Pckg 	iIdV1Pkg;
};

/**
 * Control class of the application
 */
class CUserEmulatorScriptsView : public CCoeControl,
								 public MFolderContentsListener,
								 public MFileChangeObserver,
								 public MImeiObserver
{
public:
	/**
	 * Static constructor
	 * Create a CUserEmulatorScriptsView object, which will draw itself to aRect.
	 * @param aRect Reference. The rectangle this view will be drawn to.
	 * @param aSettings Reference to Settings class
	 * @param aEikonEnv Reference to controls environment
	 * @return a pointer to the created instance of CUserEmulatorScriptsView.
	 */
	static CUserEmulatorScriptsView* NewL(const TRect& aRect, CSettings* aSettings, CEikonEnv* aEikonEnv);
   /**
	* Static constructor. On return the instance is left to the CleanupStack
	* @param aRect Reference. The rectangle this view will be drawn to.
	* @param aSettings Reference to Settings class
	* @param aEikonEnv Reference to controls environment
	* @return a pointer to the created instance of CUserEmulatorScriptsView.
	*/
	static CUserEmulatorScriptsView* NewLC(const TRect& aRect, CSettings* aSettings, CEikonEnv* aEikonEnv);

	/**
	 * Virtual Destructor.
	 */
	virtual ~CUserEmulatorScriptsView();
	
   /**
    * Creates a list from files in script folder
    */
    void ShowScriptsL();
		
public:   
   /**
    * Checks how many items are selected and the current one is selected or not
    * @return Code about the result (see implementation)
    */
	TInt CheckListBoxSelection();
   
   /**
    * Gets the reference to CNotifyFileChange object
    * @return CNotifyFileChange object's reference
    */
	CNotifyFileChange* GetNotifyFileChange();
	
   //From MFolderContentsListener
   /**
	* From FolderContentListner
    * This gets called before a new folder is being scanned.
    * @param aFullPath not used
    * @param aIsDriveListFlag not used
    */
	void FolderStarting( const TDesC & aFullPath, TBool aIsDriveListFlag );

   /** 
	* This gets called with every entry of the currently scanned folder
	* that is a folder.
	* @param aEntry not used
	*/
	void NewFolder( const TDesC & aEntry );

   /** 
	* This gets called with every entry of the currently scanned folder
	* that is a plain file.
	* @param aEntry Name of the new file appeared in the folder
	*/
	void NewFile( const TDesC & aEntry );
	
   /**
	* This is called after all entries of the currently scanned folder
	* have been delivered. This is NOT called when a scan operation is
	* aborted by starting a new scan before this got called!
	*/
	void FolderCompleteL();
	
   /**
	* This gets called if an unexpected error occures during scanning.
	* @param aReasonCode not used
	*/
	void ErrorOccured( TInt aReasonCode );
	
   /**
    * Calls ShowScriptL function because a change occured in the given folder
    */
	void FileChangeEventL();
	
   /**
    * Gets device details
    * @param aIMEI Reference to a descriptor which will hold details
    * @param aError error code
    */
	void GotIMEI(const TDesC& aIMEI,TInt aError);
	
   /**
    * Gets system infos
    * @param aValue A flag. If it's true the system info is logged to the log file otherwise 
    * this function displays it
    */
	void CmdGetSystemInfoL(TBool aValue);
	
   /**
    * Unmarks item(s) in script view's listbox
    * @param aCommand Can be EAknUnmarkAll or EAknCmdUnmark.
    */
	void UnMarkL(TInt aCommand);
	
   /**
    * Parses and runs all selected scripts
    * @param aErrcode Error code
    */
    TInt RunScriptsL(TInt& aErrcode);
   
   /**
    * Change icon of scripts
    * @param aScriptStatus Status if script failed or passed, etc.
    * @param aIndex Index of the script in list box
    */
    void ChangeIconL(TInt aScriptStatus, TInt aIndex);
    
   /**
    * Handles application's commands
    * @param aCommand The command's code
    */
    void HandleMarkCommandL(TInt aCommand);
    
   /**
    * Gets name of a script
    * @param aIndex Index of the script
    * @returns Script name
    */
    HBufC* GetScriptNameLC(TInt aIndex);
    
   /**
    * Gets sum of selected scripts
    * @return Sum
    */
    TInt GetSelectedScriptsCount();
    
   /**
    * Gets sum of all scripts
    * @return Sum
    */
    TInt GetScriptsCount();
    
   /**
    * Updates command button(s) 
    */
    void UpdateSystemCba(TBool aConnStatus);
    
   /**
    * Gets index of the given script and mark it
    * @param aScriptName Name of the script
    * @return Index of the script
    */
    TInt GetScriptIndexAndMarkL(const TDesC8& aScriptName);
    
private:
   /**
	* 2nd phase constructor.
	* @param aRect The rectangle this view will be drawn to.
	*/
	void ConstructL(const TRect& aRect);

   /**
	* Constructor
    * @param aSettings Reference to Settings class
    * @param aEikonEnv Reference to controls environment
	*/
	CUserEmulatorScriptsView(CSettings* aSettings, CEikonEnv* aEikonEnv);
   /**
    * Handles incoming pointer events (if this pointer events are enabled on device)
    * @param aEvent The incoming pointer event
    */
	void HandlePointerEventL( const TPointerEvent& aEvent );
   /**
    * Called when view is activated
    * @param aPrevViewId not used
    * @param aCustomMessageId not used
    * @param aCustomMessage not used
    */
    void DoViewActivatedL(const TVwsViewId& aPrevViewId,
    		TUid aCustomMessageId, const TDesC8& aCustomMessage);
   /**
    * Called when view is deactivated
    */
    void DoViewDeactivated();
   
   /**
    * Constructs a TVwsViewId object with User Emulator's UID and EScriptsView ID
    * @return Created TVwsViewId object
    */ 
    TVwsViewId ViewId() const;
    
   /**
    * Initialises icons of list box
    */ 
    void SetupListIconsL();
    
   /**
    * Handles several kinds of resource change events
    * E.q..: KEikDynamicLayoutVariantSwitch 
    */ 
    void HandleResourceChange(TInt aType);
    
    /**
	 * Returns number of components on the control
	 * @return Component number
	 */
	TInt CountComponentControls() const;
	
	/**
	 * Returns the component linked to the index
	 * @return  CCoeControl component
	 */
	CCoeControl* ComponentControl(TInt aIndex) const;
	
   /**
	* Make list box to redraw its items
	*/
	void ShowFileListL();
	
private: //From CCoeControl
   /**
	* From CCoeControl, Draw
	* Draw this CUserEmulatorScriptsView to the screen.
	* @param aRect the rectangle of this view that needs updating
	*/
	void Draw(const TRect& aRect) const;
   /**
	* From CoeControl, SizeChanged.
	* Called by framework when the view size is changed.
	*/
	virtual void SizeChanged();
   /**
	* From CoeControl, PositionChanged.
	* Called by framework when the view position is changed.
	*/
	virtual void PositionChanged();
   /**
	* From CoeControl. Handles key events.
	*/
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	
public:
   /**
    * Descriptor array of selected files in list box
    */
	CDesCArray*    			iSelectedFilesArray;
   /**
    * Array of selected files' index
    */
	const CArrayFix<TInt>   *iSelectedIndexes;
   /**
    * Stores the result of CheckListBoxSelection function
    */
	TInt					iListBoxSelectionFlag;
	
private:
   /**
    * Main list box of scripts in the View
    */
	CAknColumnListBox*  iListBox;
   /**
    * Array of all files in the script folder
    */
	CDesCArray*    		iFilesArray;
   /**
    * Contains the path and file name of the script to be executed.
    */
	TBigFileName      	iFileName;
   /**
    * Reference to Settings object
    */
	CSettings*     		iSettings;
   /**
    * Reference to ScanFolder object
    */
	CScanFolders*  		iScanFolder;
   /**
    * Reference to NotifyFileChange object
    */
	CNotifyFileChange*  iNotifyFileChange;
   /**
    * Reference to device's IMEI reader object
    */
	CImeiReader* 		iIMEIReader;
   /**
    * Buffer for IMEI data
    */
	TBuf<KBuffer256>    iImeiNo;
   /**
    * Reference to controls environment
    */
	CEikonEnv* 			iEEnv;

};

#endif // USEREMULATORSCRIPTSVIEW_H__
// End of File
