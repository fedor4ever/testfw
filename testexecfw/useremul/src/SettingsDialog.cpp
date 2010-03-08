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
 * File Name: SettingsDialog.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This class is used for handling settings dialog
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <stringloader.h> 
#include <eikfrlbd.h>
#include <aknquerydialog.h>
#include <bautils.h>    // BaflUtils
#include <aknnotewrappers.h>

//User Includes
#include <UserEmulator_0x2001C3AF.rsg>
#include "SettingsDialog.h"
#include "Settings.h"
#include "UserEmulator.hrh"
#include "UserEmulatorAppUi.h"

//Constants
_LIT(KAPPIDSTATUS,"App ID is empty");
_LIT(KINVALIDDRIVEEXT,"Invalid Drive/Path/Extension..Changing to default path");

// -----------------------------------------------------------------------------
// CSettingsDialog::CSettingsDialog
// Constructor 
// -----------------------------------------------------------------------------
//
CSettingsDialog::CSettingsDialog(CSettings& aSettings) : iSettings(aSettings)
{
	iPrevLogPath.Copy(iSettings.iLogPath);
}

// -----------------------------------------------------------------------------
// CSettingsDialog::PostLayoutDynInitL
// Initializes the dialog's controls after the dialog has been sized but before 
// it has been activated 
// -----------------------------------------------------------------------------
//
void CSettingsDialog::PostLayoutDynInitL()
{
    TRect mainPaneRect;
	AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane,mainPaneRect);
	iSettingsList->SetRect(mainPaneRect);
}
// -----------------------------------------------------------------------------
// CSettingsDialog::~CSettingsDialog
// Destructor 
// -----------------------------------------------------------------------------
//
CSettingsDialog::~CSettingsDialog()
{
	delete iSettingsList; 
	iSettingsList=NULL;
}
// -----------------------------------------------------------------------------
// CSettingsDialog::PreLayoutDynInitL
// Initializes the dialog's controls before the dialog is sized and layed out.
// -----------------------------------------------------------------------------
//
void CSettingsDialog::PreLayoutDynInitL()
{
	iSettingsList = new (ELeave) CAknSettingItemList;
	iSettingsList->SetMopParent(this);
	iSettingsList->ConstructFromResourceL(R_SETTING_ITEM_LIST);
	
	TBool isNumberedStyle = iSettingsList->IsNumberedStyle();
	CArrayPtr<CGulIcon>* icons = iSettingsList->ListBox()->ItemDrawer()->FormattedCellData()->IconArray();
				
	TInt id = 0;
	HBufC* label;
	
	//Scripts Settings Path 
	CAknSettingItem* settingItem = new (ELeave) CAknTextSettingItem(id++, iSettings.iScriptsPath);
	CleanupStack::PushL(settingItem);
	label = StringLoader::LoadLC(R_SETTING_PATH_SELECTOR);
	settingItem->SetEmptyItemTextL(KEmptyItemText);
	settingItem->ConstructL(isNumberedStyle, 0, *label, icons, R_TEXT_SETTING_PAGE, -1, 0, R_NAME_EDWIN);
	iSettingsList->SettingItemArray()->AppendL(settingItem);
	CleanupStack::PopAndDestroy(label);
	CleanupStack::Pop(settingItem);
	
	//Log Status (ON/OFF)
	CAknSettingItem* logStatusItem = new(ELeave) CAknBinaryPopupSettingItem(id++, iSettings.iLogStatus);
	CleanupStack::PushL(logStatusItem);
	label = StringLoader::LoadLC(R_SETTING_LOG_STATUS);
	logStatusItem->ConstructL(isNumberedStyle, 0, *label, icons, R_BINARY_SETTING_PAGE, -1, 0, R_BINARY_TEXTS);
	iSettingsList->SettingItemArray()->AppendL(logStatusItem);
	CleanupStack::PopAndDestroy(label);
	CleanupStack::Pop(logStatusItem);
		
	//Log Settings Path 
	CAknSettingItem* logSettingItem = new (ELeave) CAknTextSettingItem(id++, iSettings.iLogPath);
	CleanupStack::PushL(logSettingItem);
	label = StringLoader::LoadLC(R_SETTING_LOG_SELECTOR);
	logSettingItem->SetEmptyItemTextL(KEmptyItemText);
	logSettingItem->ConstructL(isNumberedStyle, 0, *label, icons, R_TEXT_SETTING_PAGE, -1, 0, R_NAME_EDWIN);
	iSettingsList->SettingItemArray()->AppendL(logSettingItem);
	CleanupStack::PopAndDestroy(label);
	CleanupStack::Pop(logSettingItem);
	
	//Random test app Status (ON/OFF)
	CAknSettingItem* RandomTestAppItem = new(ELeave) CAknBinaryPopupSettingItem(id++, iSettings.iRandomTestAppIdStatus);
	CleanupStack::PushL(RandomTestAppItem);
	label = StringLoader::LoadLC(R_SETTING_RANDOMTEST_STATUS);
	RandomTestAppItem->ConstructL(isNumberedStyle, 0, *label, icons, R_BINARY_SETTING_PAGE, -1, 0, R_BINARY_TEXTS);
	iSettingsList->SettingItemArray()->AppendL(RandomTestAppItem);
	CleanupStack::PopAndDestroy(label);
	CleanupStack::Pop(RandomTestAppItem);
	
    //Random test app Id
	CAknSettingItem* itemAppID = new (ELeave) CAknTextSettingItem(id++, iSettings.iRandomTestAppID );
    CleanupStack::PushL(itemAppID);
    label = StringLoader::LoadLC(R_SETTING_RANDOMTEST);
    itemAppID->SetEmptyItemTextL(KEMPTYTEXT);
    itemAppID->ConstructL(isNumberedStyle, 0, *label, icons, R_APPID_SETTING_PAGE, -1, 0, R_NAME_EDWIN);
    iSettingsList->SettingItemArray()->AppendL(itemAppID);
    CleanupStack::PopAndDestroy(label);
    CleanupStack::Pop(itemAppID);
	
	iSettingsList->HandleChangeInItemArrayOrVisibilityL();		
}
// -----------------------------------------------------------------------------
// CSettingsDialog::OkToExitL
// -----------------------------------------------------------------------------
//
TBool CSettingsDialog::OkToExitL(TInt aButtonId)
{
    switch (aButtonId)
    {
        case EAknSoftkeySelect:   
    	TInt index = iSettingsList->ListBox()->CurrentItemIndex();
    	CAknSettingItem *item = (*iSettingsList->SettingItemArray())[index];
    	
     	iSettingsList->EditItemL(index, EFalse);
    	iSettingsList->StoreSettingsL();
		switch (item->Identifier())
		{
			case 0:
			{
				TBool lengthOk = ETrue;
				if(iSettings.iScriptsPath.Length()>KBuffer256)
					lengthOk = EFalse;
				//check file extension
				TInt result = CheckFileName(iSettings.iScriptsPath);
									
				//check drive
				TInt err = iSettings.CheckDrive(iSettings.iScriptsPath);
								
				if(iSettings.iScriptsPath.Right(1).Compare(KBsh)!=0)
					iSettings.iScriptsPath.Append(KBsh);
				
				TBool res = iSettings.CheckPathL(iSettings.iScriptsPath);
				
				if(err != KErrNone || result == -1  || !res || !lengthOk)
				{
					//invalid drive Error note
					CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
					errorNote->ExecuteLD(KINVALIDDRIVEEXT);
					iSettings.iScriptsPath = KDefaultPath;
					iSettings.StoreL();
				}
				else
				{ 
					static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->ShowScriptsL();
					static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->RefreshFileNotification();
					iSettings.StoreL();
					return EFalse;
				}
			}
			break;
			case 1:
			case 3:
			case 5:
			case 6:
				iSettings.StoreL();
			return EFalse;
			
			case 4:	
				if(iSettings.iRandomTestAppIdStatus && iSettings.iRandomTestAppID.Length()<1)
				{
					CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
					errorNote->ExecuteLD(KAPPIDSTATUS);
					iSettings.StoreL();
					return CAknDialog::OkToExitL(aButtonId);
				}
				else
				{
					iSettings.StoreL();
					return EFalse;
				}
				break;
			case 2:
			{
				//Check whether log file exits or not
				TBool lengthOk = ETrue;
				if(iSettings.iLogPath.Length()>256)
					lengthOk = EFalse;
				TInt pos = iSettings.iLogPath.Find(KDOT);
				if(pos == KErrNotFound)
				{
					iSettings.iLogPath.Append(KCSVEXT);
				}
				//check file extension
				TInt result = CheckFileName(iSettings.iLogPath);
												
				//check drive
				TInt err = iSettings.CheckDrive(iSettings.iLogPath);
											
				TBool res = iSettings.CheckPathL(iSettings.iLogPath);
							
				if(err != KErrNone || result == -1  || !res || !lengthOk)
				{
					//invalid drive Error note
					CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
					errorNote->ExecuteLD(KINVALIDDRIVEEXT);
					iSettings.iLogPath = KDefaultLogPath;
					iSettings.StoreL();
				}
				else
				{
					if(iSettings.iLogPath.Compare(iPrevLogPath)!=0)
					{
						iPrevLogPath.Copy(iSettings.iLogPath);
						static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->Logger().CloseFileHandles();
					}
					static_cast<CUserEmulatorAppUi*>(iCoeEnv->AppUi())->SetSystemInfoFlag(ETrue);
					iSettings.StoreL();
					return EFalse;
				}
			}
			break;        		
		}   			
		return CAknDialog::OkToExitL(aButtonId);
    }
    return ETrue;
}

// -----------------------------------------------------------------------------
// CSettingsDialog::CheckFileName
// Function to validate file names
// -----------------------------------------------------------------------------
//
TInt CSettingsDialog::CheckFileName(const TDesC& aFilePath)
{
	TInt len = aFilePath.Length();
	TInt pos = aFilePath.Locate('.');
	if(pos!=KErrNotFound && (len-pos)>0)
		{
		TPtrC fileExt = aFilePath.Right(len - pos);
		if(fileExt.Compare(KCSVEXT) == 0 || fileExt.Compare(KXMLEXT)==0 )
			return 0;
		else
			return -1;
		}
	else
		return 0;
}
// -----------------------------------------------------------------------------
// CSettingsDialog::OfferKeyEventL
// -----------------------------------------------------------------------------
//
TKeyResponse CSettingsDialog::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
{
	return iSettingsList->OfferKeyEventL(aKeyEvent,aType);
}
// -----------------------------------------------------------------------------
// CSettingsDialog::CountComponentControls
// -----------------------------------------------------------------------------
//
TInt CSettingsDialog::CountComponentControls() const
{
	return CAknDialog::CountComponentControls() + 1;
}
// -----------------------------------------------------------------------------
// CSettingsDialog::ComponentControl
// -----------------------------------------------------------------------------
//
CCoeControl* CSettingsDialog::ComponentControl(TInt aIndex) const
{
    if (!aIndex--)
    		return iSettingsList;
    	return CAknDialog::ComponentControl(aIndex);
}
