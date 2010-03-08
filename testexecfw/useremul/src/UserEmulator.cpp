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
 * File Name: UserEmulator.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Main application class
 *------------------------------------------------------------------
 -
 *
 */

//System Includes
#include <eikstart.h>

//User Includes
#include "UserEmulatorApplication.h"

// -----------------------------------------------------------------------------
// CApaApplication* NewApplication()
// Creates a new application
// -----------------------------------------------------------------------------
//
LOCAL_C CApaApplication* NewApplication()
	{
	return new CUserEmulatorApplication;
	}

// -----------------------------------------------------------------------------
// E32Main()
// Main entry function
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main()
	{
	return EikStart::RunApplication( NewApplication );
	}
