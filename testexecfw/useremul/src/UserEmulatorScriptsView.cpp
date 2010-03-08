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
 * File Name: UserEmulatorScriptsView.cpp
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

// System Includes
#include <coemain.h>
#include <eikedwin.h>
#include <akniconarray.h> 
#include <gulicon.h> 
#include <eikclbd.h> 
#include <bautils.h>
#include <aknnotewrappers.h>
#include <hal.h>
#include <aknmessagequerydialog.h> 
#include <icons.mbg>
// User Includes
#include <UserEmulator_0x2001C3AF.rsg>
#include "UserEmulatorScriptsView.h"
#include "UserEmulatorAppUi.h"
#include "UserEmulatorApplication.h"
#include "XmlHandler.h"
#include "UserEmulator.hrh"
#include "ScanFolders.h"
#include "Utils.h"
// Constant
// Listbox icon file name
_LIT(KIconFileName, "\\system\\apps\\useremulator\\Icons.mbm");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorScriptsView* CUserEmulatorScriptsView::NewL(const TRect& aRect, CSettings* aSettings, CEikonEnv* aEikonEnv)
	{
	CUserEmulatorScriptsView* self = CUserEmulatorScriptsView::NewLC( aRect, aSettings, aEikonEnv);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorScriptsView* CUserEmulatorScriptsView::NewLC(const TRect& aRect, CSettings* aSettings, CEikonEnv* aEikonEnv)
	{
	CUserEmulatorScriptsView* self = new ( ELeave ) CUserEmulatorScriptsView(aSettings, aEikonEnv);
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	return self;
	}

// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::ConstructL(const TRect& aRect)
{
     _LIT8(KXml,"*.xml");
	 // Create a window for this application view
	 CreateWindowL();

	 iScanFolder = CScanFolders::NewL();
 	 iScanFolder->AddListenerL(this);
 	 iScanFolder->SetFilter( KXml );
	 	
	 // Create a single numbered style listbox
	 iListBox = new (ELeave) CAknSingleGraphicStyleListBox(); // markable
	 // Construct listbox   
	 iListBox->ConstructL( this, EAknListBoxMarkableList | EAknListBoxLoopScrolling );    
	 // Set scrollbars
	 iListBox->CreateScrollBarFrameL( ETrue );
	 iListBox->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOn, CEikScrollBarFrame::EAuto );
	 
	 iListBox->UpdateScrollBarsL();
     iListBox->SetRect( Rect() );
        
	 iFilesArray = new (ELeave) CDesCArrayFlat(1);
	 iSelectedFilesArray = new (ELeave) CDesCArrayFlat(1);
	 	 
	 iListBox->Model()->SetItemTextArray( iFilesArray );
	 // Listbox deletes the item array
	 iListBox->Model()->SetOwnershipType( ELbmOwnsItemArray );
	 
	 // Add icons to listbox
 	 SetupListIconsL();
 	 // Activate Listbox    
     iListBox->ActivateL();
 	 
  	 iNotifyFileChange = CNotifyFileChange::NewL(*this, iSettings->iScriptsPath);
  	 iIMEIReader = CImeiReader::NewL(*this);
  	 
 	 ShowScriptsL();
 	
 	 // Set the windows size
	 SetRect(aRect);

	 // Activate the window, which makes it ready to be drawn
	 ActivateL();
}
// ---------------------------------------------------
// Gets the reference to CNotifyFileChange object
// ---------------------------------------------------
//
CNotifyFileChange* CUserEmulatorScriptsView::GetNotifyFileChange()
	{
	return iNotifyFileChange;
	}
// ---------------------------------------------------
// Creates a list from files in script folder
// ---------------------------------------------------
//
void CUserEmulatorScriptsView::ShowScriptsL()
{
	iFileName.Copy(iSettings->iScriptsPath);
	TParsePtrC parser(iFileName);
	BaflUtils::EnsurePathExistsL(iEEnv->FsSession(), parser.DriveAndPath());
	iScanFolder->ScanFolderL(iFileName);
}
// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::CUserEmulatorScriptsView()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUserEmulatorScriptsView::CUserEmulatorScriptsView(CSettings* aSettings, CEikonEnv* aEikonEnv) 
                          : iSettings(aSettings), iEEnv(aEikonEnv)//,iStartLog(EFalse),iSystemInfo(ETrue)
{
	// No implementation required
}

// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::~CUserEmulatorScriptsView()
// Destructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorScriptsView::~CUserEmulatorScriptsView()
{
	delete iListBox;
	
	if(iScanFolder)
	{
	iScanFolder->RemoveListener(this);
	delete iScanFolder;
	iScanFolder = NULL;
	}
	
	delete iSelectedFilesArray;
	
	delete iNotifyFileChange;
	delete iIMEIReader;
	
}

// -----------------------------------------------------------------------------
// CUserEmulatorAppView::Draw()
// Draws the display.
// -----------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::Draw(const TRect& /*aRect*/) const
{
	// Get the standard graphics context
	CWindowGc& gc = SystemGc();

	// Gets the control's extent
	TRect drawRect(Rect());

	// Clears the screen
	gc.Clear(drawRect);
}
// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::SizeChanged()
// Called by framework when the view size is changed.
// -----------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::SizeChanged()
{
	if(iListBox)
	{
		iListBox->SetRect(Rect());	
		TRAP_IGNORE(iListBox->UpdateScrollBarsL());
	}
	DrawNow();
}
// -----------------------------------------------------------------------------
// CUserEmulatorScriptsView::PositionChanged()
// Called by framework when the view position is changed.
// -----------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::PositionChanged()
{
	if(iListBox)
	{
		iListBox->SetRect(Rect());	
		TRAP_IGNORE(iListBox->UpdateScrollBarsL());
	}	
	DrawNow();
}
// ---------------------------------------------------
// Returns number of components in View
// ---------------------------------------------------
//
TInt CUserEmulatorScriptsView::CountComponentControls() const
{
	return 1;
}
// ----------------------------------------------------------------------------------
// Constructs a TVwsViewId object with User Emulator's UID and EScriptsView ID
// ----------------------------------------------------------------------------------
//
TVwsViewId CUserEmulatorScriptsView::ViewId() const
{
    const TUid viewId = { EScriptsView };
    return TVwsViewId(KUidUserEmulatorApp, viewId);
}
// ----------------------------------------------------------------
// Stores device's IMEI details
// ----------------------------------------------------------------
//
void CUserEmulatorScriptsView::GotIMEI(const TDesC& aIMEI,TInt /*aError*/)
{
	iImeiNo.Copy(aIMEI);
}
// --------------------------------------------------------------------------
// Sends the CCoeControl interface of the control which has the given index
// --------------------------------------------------------------------------
//
CCoeControl* CUserEmulatorScriptsView::ComponentControl(TInt aIndex) const
{
    switch ( aIndex )
    {
        case 0:
			return iListBox;
        default:
            return NULL;
    }
}
// --------------------------------------------------------------------------
// Initialises icons of the script list
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::SetupListIconsL()
{
	CFbsBitmap* markBitmap = NULL;
    CFbsBitmap* markBitmapMask = NULL;
    
    TFileName iconFileName;
	TBuf<KDriveSize> appDrive; 
	appDrive.Copy(iEEnv->EikAppUi()->Application()->AppFullName().Left(2)); 
	// Insert drive letter into path
	iconFileName.Insert(0, appDrive);
	iconFileName.Append(KIconFileName());

    //CListItemDrawer is using this logical color as default for its marked icons
    TRgb defaultColor;
    defaultColor = iEEnv->Color(EColorControlText);
    	
    CAknIconArray* iconArray = new(ELeave) CAknIconArray(4);
    CleanupStack::PushL(iconArray);
    
    //Mark Icon
    AknsUtils::CreateColorIconLC(AknsUtils::SkinInstance(),
    			KAknsIIDQgnIndiMarkedAdd,
    			KAknsIIDQsnIconColors,
    			EAknsCIQsnIconColorsCG13,
    			markBitmap,
    			markBitmapMask,
    			KAvkonBitmapFile,
    			EMbmAvkonQgn_indi_marked_add,
    			EMbmAvkonQgn_indi_marked_add_mask,
    			defaultColor
    			);
    
    CGulIcon* markIcon = CGulIcon::NewL( markBitmap,markBitmapMask );
    CleanupStack::Pop( 2 ); // markBitmap, markBitmapMask
    CleanupStack::PushL( markIcon );      
    iconArray->AppendL( markIcon );
    
    iconArray->AppendL(iEikonEnv->CreateIconL(iconFileName, EMbmIconsPass, EMbmIconsPass));
    iconArray->AppendL(iEikonEnv->CreateIconL(iconFileName, EMbmIconsFail, EMbmIconsFail_mask));
    iconArray->AppendL(iEikonEnv->CreateIconL(iconFileName, EMbmIconsCancel, EMbmIconsCancel_mask));
    	
    iListBox->ItemDrawer()->ColumnData()->SetIconArray(iconArray);
    
    CleanupStack::Pop( 2 );// markIcon, iconArray
}
// --------------------------------------------------------------------------
// Changes icon of a script in list
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::ChangeIconL(TInt aScriptStatus, TInt aIndex)
{
	TBuf<KBuffer256> listItem;
	_LIT(KListItemUnMark, "\t%S\t\t");
	_LIT(KListItemWithPassIcon, "1\t%S\t\t");
	_LIT(KListItemWithFailIcon, "2\t%S\t\t");
	_LIT(KListItemWithCancelIcon, "3\t%S\t\t");
				
	CTextListBoxModel* model = iListBox->Model();  // not taking ownership
	const CArrayFix<TInt> *SelectedIndexes = iListBox->SelectionIndexes();
		
	if(aIndex < 0 || aIndex > SelectedIndexes->Count())
			return;

	// Get the index of the selected item
    TInt selectedItemIndex = (*SelectedIndexes)[aIndex];
    if(iFilesArray->Count()>0 && selectedItemIndex <= iFilesArray->Count())
    {
    	TPtrC filePtr = model->ItemText(selectedItemIndex);
    	TPtrC buff = filePtr.Mid(filePtr.LocateF('\t')+1);
		TPtrC fileNamePtr = buff.Mid(0,buff.LocateF('\t'));
		
    	HBufC* fileName = HBufC::NewLC(fileNamePtr.Length());
    	fileName->Des().Copy(fileNamePtr);
    	
    	//delete old item
    	CDesCArray* itemArray = STATIC_CAST(CDesCArray*, model->ItemTextArray());
    	
    	itemArray->Delete(selectedItemIndex);
    	// Script statuses
    	switch(aScriptStatus)
		{
    		case EScriptPassed:
      			listItem.AppendFormat(KListItemWithPassIcon(), fileName);	// Format new item
  			break;
    		case EScriptFailed:
    			listItem.AppendFormat(KListItemWithFailIcon(), fileName);	// Format new item
			break;
    		case EScriptCancelled:
    			listItem.AppendFormat(KListItemWithCancelIcon(), fileName);	// Format new item
			break;
    		case EScriptUnMark:
    			listItem.AppendFormat(KListItemUnMark(), fileName);	// Format new item
    		break;
			
    		default:
    			break;
		}
		
    	itemArray->InsertL(selectedItemIndex, listItem);	// Insert new item - changes icon to green
		SelectedIndexes->Count();
		
    	CleanupStack::PopAndDestroy(fileName);
    }
}
// --------------------------------------------------------------------------
// Script view's activation function
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::DoViewActivatedL(const TVwsViewId& /*aPrevViewId*/, 
		           TUid /*aCustomMessageId*/, const TDesC8& /*aCustomMessage*/)
{
    iListBox->ScrollBarFrame()->SetScrollBarVisibilityL(
    		CEikScrollBarFrame::EOn, CEikScrollBarFrame::EOn);
    
    UpdateSystemCba(EFalse);
}
// --------------------------------------------------------------------------
// Updates command button(s)
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::UpdateSystemCba(TBool aConnStatus)
{
    TInt res = 0;
    res = GetScriptsCount();
    
    if( res > 0 && !aConnStatus)
        res = R_SOFTKEYS_SCRIPTS;
    else
        res = R_SOFTKEYS_NOSCRIPTS;

    CEikButtonGroupContainer* cba = iEikonEnv->AppUiFactory()->Cba();
    TRAPD(err,cba->SetCommandSetL(res));
    cba->DrawDeferred();
}
// --------------------------------------------------------------------------
// Script view's deactivation function
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::DoViewDeactivated()
{
}
// --------------------------------------------------------------------------
// Gets script name by index
// --------------------------------------------------------------------------
//
HBufC* CUserEmulatorScriptsView::GetScriptNameLC(TInt aIndex)
{
	HBufC* text = HBufC::NewLC(KBuffer512);
    TPtr textPtr(text->Des());
	
    // Get the selected item indexes an array
	iSelectedIndexes = iListBox->SelectionIndexes();			
	CTextListBoxModel* model = iListBox->Model();  // not taking ownership
	
	if(aIndex < 0 || aIndex > iSelectedIndexes->Count())
		return NULL;
	
	if ( iSelectedIndexes && iSelectedIndexes->Count() != 0 && aIndex<=iSelectedIndexes->Count() )
	{
		// Get the index of the selected item
		TInt selectedItemIndex = (*iSelectedIndexes)[aIndex];
		
		TPtrC filePtr = model->ItemText(selectedItemIndex);
		TInt pos=filePtr.LocateF('\t');
		TPtrC buff;
		if(pos!=KErrNotFound && pos+1<=filePtr.Length())
			buff.Set(filePtr.Mid(pos+1));
		
		pos=buff.LocateF('\t');
		TPtrC fileNamePtr;
		if(pos!=KErrNotFound)
			fileNamePtr.Set(buff.Mid(0,pos));
    	textPtr.Copy(fileNamePtr);
	}
	return text;
}
// --------------------------------------------------------------------------
// Gets sum of scripts selected
// --------------------------------------------------------------------------
//
TInt CUserEmulatorScriptsView::GetSelectedScriptsCount()
{
	return iSelectedIndexes->Count();
}
// --------------------------------------------------------------------------
// Gets sum of all scripts in script's folder
// --------------------------------------------------------------------------
//
TInt CUserEmulatorScriptsView::GetScriptsCount()
{
	CTextListBoxModel* model = iListBox->Model();  // not taking ownership
	return model->NumberOfItems();
}
// ------------------------------------------------------------
// Run selected scripts
// ------------------------------------------------------------
//
TInt CUserEmulatorScriptsView::RunScriptsL(TInt& aErrcode)
{
	TInt err=KErrNone;
	aErrcode=KErrNone;
	
	// Get the selected item indexes an array
	iSelectedIndexes = iListBox->SelectionIndexes();			

	iSelectedFilesArray->Reset();
	
	CTextListBoxModel* model = iListBox->Model();  // not taking ownership
	
	// Make sure the array is not null (no items)
	if ( iSelectedIndexes && iSelectedIndexes->Count() > 0) 
	{
	    // Loop through the selected item indexes
	    for ( TInt index=0; index < iSelectedIndexes->Count(); index++ )
        {
	        // Get the index of the selected item
	        TInt selectedItemIndex = (*iSelectedIndexes)[index];
	        if(iFilesArray->Count()>0 && selectedItemIndex <= iFilesArray->Count())
	        {
	        	TPtrC filePtr = model->ItemText(selectedItemIndex);
	        	// Disallow too long file names
	        	TInt pos=filePtr.LocateF('\t');
	        	TPtrC buff;
	        	if(pos!=KErrNotFound && pos+1<=filePtr.Length())
					buff.Set(filePtr.Mid(pos+1));
	        	TPtrC fileNamePtr;
	        	pos=buff.LocateF('\t');
	        	if(pos!=KErrNotFound)
					fileNamePtr.Set(buff.Mid(0,pos));
	        	
	        	iSelectedFilesArray->AppendL(fileNamePtr);
	        	//Append the selected file names to an array
	        	if(!iFileName.Length())
	        		iFileName.Copy(iSettings->iScriptsPath);
	        	iFileName.Append(fileNamePtr);
	        	
	        	//check if the iFilaName is valid
                err = iSettings->CheckDrive(iFileName);
                if(err == KErrNone)
	        	  // Parse the XML file
	        	  TRAPD(err,aErrcode=static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->StartParsingL(iFileName));
	        	  
	        	iFileName.Zero();
	        }			        
        }
	}
	else
	{
		//If no scripts selected
		_LIT(KSELECTSCRIPT, "Select scripts to run..");
		CUtils::MessageBoxL(KSELECTSCRIPT);
	}
	
	return err;
}

// ------------------------------------------------------------------------------
// Handles incoming pointer events (if pointer events are enabled on the device)
// ------------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::HandlePointerEventL( const TPointerEvent& aPointerEvent )
	{
	if(AknLayoutUtils::PenEnabled())
		{
		if (iListBox)
			{
			iListBox->HandlePointerEventL(aPointerEvent);
			}
		// Call base class HandlePointerEventL()
		CCoeControl::HandlePointerEventL(aPointerEvent);
		}
	}
// --------------------------------------------------------------------------
// Helper function
// --------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::HandleMarkCommandL(TInt aCommand)
{
	if (iListBox)
	{
		AknSelectionService::HandleMarkableListProcessCommandL (aCommand, iListBox);
	}
}
// -------------------------------------------------------------------------------
// Checks how many items are selected and the current one is selected or not
// This function is needed to display correct Mark, Unmark, Mark all, etc. menus
// -------------------------------------------------------------------------------
//
TInt CUserEmulatorScriptsView::CheckListBoxSelection()
	{
	TInt res=0;
	// Gets all entries
	TInt allEntries = GetScriptsCount();
	if(allEntries>0)
		{
		const CArrayFix<TInt> *SelectedIndexes = iListBox->SelectionIndexes();
		// Checks if current item is selected
		TInt n=iListBox->CurrentItemIndex();
		TBool IsSelected = iListBox->View()->ItemIsSelected(n);
		if(SelectedIndexes)
			{                
			TInt selEntries = SelectedIndexes->Count();
			// There is only one item in list box and it's selected
			if(allEntries==1 && IsSelected)
				res=EOneItemSel;
		    // There is only one item and it is not selected
			else if(allEntries==1 && !IsSelected)
				res=EOneItem;
			// There are more items and current one is selected and there's only one selected item
			else if(allEntries>1 && IsSelected && selEntries==1)
				res=EMoreItemsSel;
			// There are more items and current one is not selected and there's no selected item
			else if(allEntries>1 && !IsSelected && selEntries==0)
				res=EMoreItems;
			// There are more items and current one is selected and all items are selected in the list
			else if(allEntries>1 && IsSelected && selEntries==allEntries)
				res=EMoreItemsAll;
			// There are more items and current one is not selected and all items are selected in 
			// the list (except the current one)
			else if(allEntries>1 && !IsSelected && selEntries==allEntries-1)
				res=EMoreItemsAllExc;
		    // There are more items and current one is selected and there are more than one selected items
		    // but not all
			else if(allEntries>1 && IsSelected && selEntries>1 && allEntries>selEntries)
				res=EMoreItemsNotAll;
			// There are more items and at least one item is selected which is not the current one
			// but not all items are selected
			else if(allEntries>1 && !IsSelected && selEntries>0 && allEntries>selEntries)
				res=EMoreItemsNotAllNotCurrent;
			}
		}
	return res;
	}
// -------------------------------------------------------------------------------
// Unmark an item or unmark all items (according to aCommand param)
// -------------------------------------------------------------------------------
//
void CUserEmulatorScriptsView::UnMarkL(TInt aCommand)
{
	const CArrayFix<TInt> *SelectedIndexes = iListBox->SelectionIndexes();
	TInt currentItem = 0;
	
	// Make sure the array is not null (no items)
	if ( SelectedIndexes && SelectedIndexes->Count() != 0) 
	{
		if(aCommand == EAknCmdUnmark)
			currentItem = iListBox->CurrentItemIndex(); 
		
	    // Loop through the selected item indexes
	    for ( TInt index=0; index < SelectedIndexes->Count(); index++ )
	    {
	    	if(currentItem == (*SelectedIndexes)[index] && aCommand == EAknCmdUnmark)
	    	{
	    		ChangeIconL(EScriptUnMark,index);
	    		break;
	    	}
	    	else if(aCommand == EAknUnmarkAll)
	    		ChangeIconL(EScriptUnMark,index);
	    }
        HandleMarkCommandL(aCommand);
	}	
}
// -----------------------------------------
// Handles key events
// -----------------------------------------
//
TKeyResponse CUserEmulatorScriptsView::OfferKeyEventL(const TKeyEvent& aKeyEvent,
		     										  TEventCode aType)
{	
    // Only key events	
	if (aType == EEventKey)
    {
        switch (aKeyEvent.iCode)
        {
            case EKeyUpArrow:
            case EKeyDownArrow:
                return iListBox->OfferKeyEventL(aKeyEvent, aType);

            case EKeyBackspace:
                // Don't handle key if shift is pressed
                if ((aKeyEvent.iModifiers & EModifierShift) == 0)
                {
                    return EKeyWasConsumed;
                }
                break;
            default:
            	break;
        }
    }
    return EKeyWasNotConsumed;
}
// -----------------------------------------
// Handles resource change
// -----------------------------------------
//
void CUserEmulatorScriptsView::HandleResourceChange(TInt aType)
{
	TRect rect;
 
    if ( aType==KEikDynamicLayoutVariantSwitch )
    {    
        AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, rect);
        
        SetRect(rect);
        static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->SetApplicationRectInSettings();
    }
 
	CCoeControl::HandleResourceChange(aType);
}
// -----------------------------------------
// Gets system info details
// -----------------------------------------
//
void CUserEmulatorScriptsView::CmdGetSystemInfoL(TBool aValue)
{
    // Operating system version
    TVersionName versionName = User::Version().Name();

    class THal {
    public:
        const TPtrC Machine(TInt aUid) {
            static const struct SMachine {
                TInt Uid;
                const TText* iName;
            } machine[] = {
                { 0x101f4fc3, _S("Nokia 7650") },
                { 0x101f466a, _S("Nokia 3650/3660") },
                { 0x101f8c19, _S("Nokia N-Gage") },
                { 0x101fb2b1, _S("Nokia N-Gage QD") },
                { 0x101fb3f4, _S("Nokia 6260") },
                { 0x101fb3dd, _S("Nokia 6600") },
                { 0x101f3ee3, _S("Nokia 6620") },
                { 0x101fb3f3, _S("Nokia 6670/7610") },
                { 0x101fbb55, _S("Nokia 6630") },
                { 0x10200f99, _S("Nokia 6680") },
                { 0x10200f97, _S("Nokia 3230") },
                { 0x10200f9b, _S("Nokia 6682") },
                { 0x10200f9c, _S("Nokia 6681") },
                { 0x10200f98, _S("N90") },
                { 0x10200f9a, _S("N70") },
                { 0x101fa031, _S("Sendo X") },
                { 0x101f9071, _S("Siemens SX") },
          
                { HAL::EMachineUid_Win32Emulator, _S("Win32 emulator") },
                { 0x00000000, 0 }       // End of array
              };

            for (TInt i = 0; machine[i].Uid; ++i)
                if (aUid == machine[i].Uid)
                    return TPtrC(machine[i].iName);

            return TPtrC(_S("N/A"));
        }

        const TPtrC Family(TUint aIndex) {
            const TText* family[] = {
                            _S("Crystal"),
                            _S("Pearl"),
                            _S("Quartz"),
            };

            if (aIndex < sizeof family / sizeof *family)
                return TPtrC(family[aIndex]);
            else
                return TPtrC(_S("N/A"));
        }

        const TPtrC Cpu(TUint aIndex) {
            const TText* cpu[] = {
                            _S("ARM"),
                            _S("MCore"),
                            _S("x86"),
            };

            if (aIndex < sizeof cpu / sizeof *cpu)
                return TPtrC(cpu[aIndex]);
            else
                return TPtrC(_S("N/A"));
        }

        const TPtrC StartupReason(TUint aIndex) {
            const TText* startup[] = {
                            _S("Cold"),
                            _S("Warm"),
                            _S("Fault"),
            };

            if (aIndex < sizeof startup / sizeof *startup)
                return TPtrC(startup[aIndex]);
            else
                return TPtrC(_S("N/A"));
        }

    } hal;

    //Get UI Version
	TVersion UIversion = iEikonEnv->Version();
	TVersionName UIVersion( UIversion.Name() );

	// Get Machine Id
    TInt machineUid;
    TBuf<KBuffer20> machUid;
    HAL::Get(HAL::EMachineUid, machineUid);
    machUid.Format(K0x, machineUid);

    // Get Family
    TInt family;
    HAL::Get(HAL::EDeviceFamily, family);
    const TPtrC& familyName = hal.Family(family);

    // Get Family revision
    TInt familyRev;
    HAL::Get(HAL::EDeviceFamilyRev, familyRev);

    // Get CPU
    TInt cpu;
    HAL::Get(HAL::ECPU, cpu);
    const TPtrC& cpuName = hal.Cpu(cpu);

    // Get CPU speed
    TInt cpuSpeed;
    HAL::Get(HAL::ECPUSpeed, cpuSpeed);

    // Get Startup reason
    TInt startupReason;
    HAL::Get(HAL::ESystemStartupReason, startupReason);
    const TPtrC& startupReasonName = hal.StartupReason(startupReason);

    // Get total RAM
    TInt ram;
    HAL::Get(HAL::EMemoryRAM, ram);

    // Get Free RAM
    TInt ramFree;
    HAL::Get(HAL::EMemoryRAMFree, ramFree);

    // Get available ROM
    TInt rom;
    HAL::Get(HAL::EMemoryROM, rom);
    
   
    //Log system information
    if(aValue)
    {
		_LIT(KSystemHeader, "#Date, Time,OS-Version,UI-Version,Machine ID,IMEI No,Family,CPU,CPUSpeed,Startup reason,RAM,FreeRAM,ROM\n");
		static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->Logger().WriteLogL(KSystemHeader, ETrue);
    }
    
    HBufC* title = iCoeEnv->AllocReadResourceLC(R_TEXT_SYSTEM_DETAILS);
    HBufC* textFormat = iCoeEnv->AllocReadResourceLC(R_FORMAT_SYSTEM_DETAILS);
    HBufC* text = HBufC::NewLC(KBuffer1024);

    text->Des().Format(*textFormat, &versionName, &UIVersion, &machUid,&iImeiNo, &familyName, familyRev, &cpuName, cpuSpeed / 1000, 
        &startupReasonName, ram, ram >> 20, ramFree, static_cast<TReal32>(ramFree) / 0x100000, rom, rom >> 20);
  
    //Log system information
    if(aValue)
    {
		HBufC* textFormat1 = iCoeEnv->AllocReadResourceLC(R_FORMAT_SYSTEM_DETAILS_FILE);
		HBufC* text1 = HBufC::NewLC(KBuffer1024);
		   
		text1->Des().Format(*textFormat1, &versionName, &UIVersion, &machUid, &iImeiNo, &familyName, familyRev, &cpuName, cpuSpeed / 1000, 
				&startupReasonName, ram, ram >> 20, ramFree, static_cast<TReal32>(ramFree) / 0x100000, rom, rom >> 20);
		   
		static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->Logger().WriteLogL(text1->Des(), EFalse, ETrue);
		
		//Log drive information
		TVolumeInfo volumeInfo;
		TInt driveNumber=EDriveA; 
		TBuf<KBuffer200> buffer;
		TChar driveLetter;    
		_LIT(KDriveFormat,"Volume name: %c,Size: %Ld (%Ld MB),Free space: %Ld (%Ld MB)\n");
		
		for (driveNumber=EDriveA; driveNumber<EDriveZ; driveNumber++)
		{
			TInt err=iEEnv->FsSession().Volume(volumeInfo,driveNumber);
			if (err!=KErrNotReady) 
				// Volume() returns KErrNotReady if no volume present.
				// In this case, check next drive number
			{
				User::LeaveIfError(iEEnv->FsSession().DriveToChar(driveNumber,driveLetter));
				buffer.Zero();			
				buffer.AppendFormat(KDriveFormat,TUint(driveLetter),volumeInfo.iSize,volumeInfo.iSize >> 20, volumeInfo.iFree, volumeInfo.iFree >> 20);
				static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->Logger().WriteLogL(buffer, EFalse, ETrue);
			}
		}
		CleanupStack::PopAndDestroy(2);
    }
    // Don't log but display error info details
    if(!aValue)
    {
		CAknMessageQueryDialog* dlg = CAknMessageQueryDialog::NewL(*text);
		dlg->SetHeaderText(*title);
		dlg->PrepareLC(R_DIALOG_INFO);
		dlg->RunLD();
    }   

    CleanupStack::PopAndDestroy(3);
}


// ----------------------------------------------------------------
// This function gets called before a new folder is being scanned
// ----------------------------------------------------------------
//
void CUserEmulatorScriptsView::FolderStarting( const TDesC & /*aFullPath*/,
		                                       TBool /*aIsDriveListFlag*/ )
{
	 iFilesArray->Reset();
}
// ----------------------------------------------------------------
// This function gets called with every entry of the currently 
// scanned folder that is a folder. Not used
// ----------------------------------------------------------------
//
void CUserEmulatorScriptsView::NewFolder( const TDesC & /*aEntry*/ )
{
}
// ----------------------------------------------------------------
// This function gets called with every entry of the currently 
// scanned folder that is a plain file.
// ----------------------------------------------------------------
//
void CUserEmulatorScriptsView::NewFile( const TDesC & aEntry )
{
	TBuf<KBuffer256> nameBuf;
	_LIT(KFormatString, "\t%S\t\t");
	nameBuf.AppendFormat(KFormatString, &aEntry);

	TRAPD(err,iFilesArray->AppendL(nameBuf));	
	if(err != KErrNone)
	{
		//Display error message
	}
}
// ----------------------------------------------------------------
// This is called after all entries of the currently scanned folder
// have been delivered. This is NOT called when a scan operation is
// aborted by starting a new scan before this got called!
// ----------------------------------------------------------------
//
void CUserEmulatorScriptsView::FolderCompleteL()
{
	TRAPD(err,ShowFileListL());
	if(err != KErrNone)
	{
		//Display error message
	}
	UpdateSystemCba(EFalse);
	
	// Notifies the AppUi that listbox is initialised with script name list
	static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->FolderScanCompletedL();
}
// ------------------------------------------------------------------
// Called when an unexpected error occures during scanning. Not used
// ------------------------------------------------------------------
//
void CUserEmulatorScriptsView::ErrorOccured( TInt /*aReasonCode*/ )
{
}
// ------------------------------------------------------------------
// Make list box to redraw its items
// ------------------------------------------------------------------
//
void CUserEmulatorScriptsView::ShowFileListL()
{		
    iListBox->HandleItemAdditionL( );
	iListBox->SetFocus(ETrue, EDrawNow);	
	iListBox->ItemDrawer()->ColumnData()->EnableMarqueeL(ETrue);	
}
// ------------------------------------------------------------------------
// Calls ShowScriptL function because a change occured in the given folder
// ------------------------------------------------------------------------
//
void  CUserEmulatorScriptsView::FileChangeEventL()
{
	ShowScriptsL();
}
// ------------------------------------------------------------------------
//  Gets the index of a given script and mark it
// ------------------------------------------------------------------------
//
TInt CUserEmulatorScriptsView::GetScriptIndexAndMarkL(const TDesC8& aScriptName)
{   
	TInt ret = -1;
	TFileName scriptName;
	scriptName.Copy(aScriptName);
	// Steps through the file array
	for(TInt i=0; i < iFilesArray->Count();i++)
	{   
	    // Gets i. file name
		TPtrC fileName = iFilesArray->operator [](i);
		TFileName script;
		
		TInt pos=fileName.LocateF('\t');
		TPtrC buff;
		if(pos!=KErrNotFound && pos+1<=fileName.Length())
			buff.Set(fileName.Mid(pos+1));
		pos=buff.LocateF('\t');
		TPtrC fileNamePtr;
		if(pos!=KErrNotFound)
			fileNamePtr.Set(buff.Mid(0,pos));
		script.Copy(fileNamePtr);
		
    	// If that's the script we are looking for then
		if(scriptName.Compare(script) == 0)
		{
		    // the result will be it's index
			ret = i;
			break;
		}			
	}
	if(ret < GetScriptsCount() && ret!=-1)
	{
		iListBox->SetCurrentItemIndex(ret);
		HandleMarkCommandL(EAknCmdMark);
	}
	return ret;
}

//-------------------CImeiReader---------------------------------------------

// ----------------------------------
// CImeiReader::NewL
// ----------------------------------
//
CImeiReader* CImeiReader::NewL(MImeiObserver& aObserver)
    {
    CImeiReader* self = NewLC(aObserver);
    CleanupStack::Pop(self);
    return self;
    }
// ----------------------------------
// CImeiReader::NewL
// ----------------------------------
//  
CImeiReader* CImeiReader::NewLC(MImeiObserver& aObserver)
    {
    CImeiReader* self = new (ELeave) CImeiReader(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
    
// ----------------------------------
// CImeiReader::CImeiReader
// ----------------------------------
//  
CImeiReader::CImeiReader(MImeiObserver& aObserver)
:CActive(0),iObserver(aObserver),iIdV1Pkg(iIdV1)
{
}
// ----------------------------------
// CImeiReader::~CImeiReader
// ----------------------------------
// 
CImeiReader::~CImeiReader()
{
	Cancel();
	delete iTelephony;	
} 
// ----------------------------------
// CImeiReader::ConstructL
// ----------------------------------
//
void CImeiReader::ConstructL()
{	
	CActiveScheduler::Add(this);
	// Create telephony object
	iTelephony = CTelephony::NewL();
	iTelephony->GetPhoneId(iStatus,iIdV1Pkg);
	SetActive();	
}
// ----------------------------------
// CActive class's cancel function
// ----------------------------------
//
void CImeiReader::DoCancel()
{
	iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
}
// ----------------------------------
// CActive class's RunL function
// ----------------------------------
// 
void CImeiReader::RunL()
{
    // Calls GotIMEI and stores serial number 
	iObserver.GotIMEI(iIdV1.iSerialNumber,iStatus.Int());
}

// End of File
