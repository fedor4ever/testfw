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
 * File Name: SettingsDialog.h
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

#ifndef SETTINGS_DIALOG_H__
#define SETTINGS_DIALOG_H__

//System Includes
#include <akndialog.h>

//User Includes
#include "Constants.h"

//Forward declarations
class CSettings;
class CAknSettingItemList;

/**
 * CSettingsDialog 
 * Class used for handling configuration settings for User Emulator
 */
class CSettingsDialog : public CAknDialog
{
public:
   /**
	* Constructor
	* @param aSettings Reference to CSettings class
	*/
    CSettingsDialog(CSettings& aSettings);
   /**
	* Destructor
	*/
    CSettingsDialog::~CSettingsDialog();
    
private: // from CAknDialog
    void PreLayoutDynInitL();
    void PostLayoutDynInitL();
    TBool OkToExitL(TInt aButtonId);  
    
private: // from CCoeControl    
	TInt CountComponentControls() const;
	CCoeControl* ComponentControl(TInt aIndex) const;
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	    
private:
   /**
	* Function to validate the file name
	* @param aFilePath File path 
	*/
	TInt CheckFileName(const TDesC& aFilePath);   

private:
   /**
	* Reference to the settings class
	*/
    CSettings& iSettings;
   /**
 	* Reference to settings item list
 	*/
    CAknSettingItemList* iSettingsList;
   /**
 	* Reference to the settings class
 	*/
    TBuf<KBuffer512> iPrevLogPath;
};

#endif //SETTINGS_DIALOG_H__
