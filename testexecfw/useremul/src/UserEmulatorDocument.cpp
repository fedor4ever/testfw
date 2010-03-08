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
 * File Name: UserEmulatorDocument.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Declares document class for application
 *------------------------------------------------------------------
 -
 *
 */

// User Includes
#include "UserEmulatorAppUi.h"
#include "UserEmulatorDocument.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CUserEmulatorDocument::NewL()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorDocument* CUserEmulatorDocument::NewL(CEikApplication& aApp)
	{
	CUserEmulatorDocument* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CUserEmulatorDocument::NewLC()
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorDocument* CUserEmulatorDocument::NewLC(CEikApplication& aApp)
	{
	CUserEmulatorDocument* self = new ( ELeave ) CUserEmulatorDocument( aApp );
	CleanupStack::PushL(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CUserEmulatorDocument::CUserEmulatorDocument()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUserEmulatorDocument::CUserEmulatorDocument(CEikApplication& aApp) :
	CAknDocument(aApp)
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CUserEmulatorDocument::~CUserEmulatorDocument()
// Destructor.
// ---------------------------------------------------------------------------
//
CUserEmulatorDocument::~CUserEmulatorDocument()
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CUserEmulatorDocument::CreateAppUiL()
// Constructs CreateAppUi.
// ---------------------------------------------------------------------------
//
CEikAppUi* CUserEmulatorDocument::CreateAppUiL()
	{
	// Create the application user interface, and return a pointer to it
	// the framework takes ownership of this object
	return new ( ELeave )CUserEmulatorAppUi(CCoeEnv::Static()->WsSession());
	}

// End of File
