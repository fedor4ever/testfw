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
 * File Name: UserEmulatorApplication.cpp
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

// User Includes
#include "UserEmulator.hrh"
#include "UserEmulatorDocument.h"
#include "UserEmulatorApplication.h"

// ============================ MEMBER FUNCTIONS ===============================
// ---------------------------------------------------------
// CDictionaryStore* CUserEmulatorApplication::OpenIniFileLC(RFs& aFs) const
// overrides CAknApplication::OpenIniFileLC to enable INI file support
// ---------------------------------------------------------
//
CDictionaryStore* CUserEmulatorApplication::OpenIniFileLC(RFs& aFs) const
{
    return CEikApplication::OpenIniFileLC(aFs);
}
// -----------------------------------------------------------------------------
// CUserEmulatorApplication::CreateDocumentL()
// Creates CApaDocument object
// -----------------------------------------------------------------------------
//
CApaDocument* CUserEmulatorApplication::CreateDocumentL()
	{
	// Create an UserEmulator document, and return a pointer to it
	return CUserEmulatorDocument::NewL(*this);
	}

// -----------------------------------------------------------------------------
// CUserEmulatorApplication::AppDllUid()
// Returns application UID
// -----------------------------------------------------------------------------
//
TUid CUserEmulatorApplication::AppDllUid() const
	{
	// Return the UID for the UserEmulator application
	return KUidUserEmulatorApp;
	}

// End of File
