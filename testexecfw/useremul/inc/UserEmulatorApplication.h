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
 * File Name: UserEmulatorApplication.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Main application class. Provides factory to create concrete document object
 *------------------------------------------------------------------
 -
 *
 */

#ifndef USEREMULATORAPPLICATION_H__
#define USEREMULATORAPPLICATION_H__

// System Includes
#include <aknapp.h>

// User Includes
#include "UserEmulator.hrh"

// UID for the application
// this should correspond to the uid defined in the mmp file
const TUid KUidUserEmulatorApp =
	{
	_UID3
	};

// CLASS DECLARATION

/**
 * CUserEmulatorApplication application class.
 * Provides factory to create concrete document object.
 * An instance of CUserEmulatorApplication is the application part of the
 * AVKON application framework for the UserEmulator application.
 */
class CUserEmulatorApplication : public CAknApplication
	{
public:
	// Functions from base classes

	/**
	 * From CApaApplication, AppDllUid.
	 * @return Application's UID (KUidUserEmulatorApp).
	 */
	TUid AppDllUid() const;

protected:
	// Functions from base classes

	/**
	 * From CApaApplication, CreateDocumentL.
	 * Creates CUserEmulatorDocument document object. The returned
	 * pointer in not owned by the CUserEmulatorApplication object.
	 * @return A pointer to the created document object.
	 */
	CApaDocument* CreateDocumentL();
	
	/**
     * From CApaApplication, overridden to enable INI file support.
     * @return CDictionaryStore*, A pointer to the dictionary store
     */
	CDictionaryStore* OpenIniFileLC(RFs& aFs) const;

	};

#endif // USEREMULATORAPPLICATION_H__
// End of File
