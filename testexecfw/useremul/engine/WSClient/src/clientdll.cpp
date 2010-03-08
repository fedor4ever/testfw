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
 * File Name: clientdll.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Declares Anim dll client side class
 *------------------------------------------------------------------
 -
 *
 */
 
// User Includes
#include "clientdll.h"

// ======== MEMBER FUNCTIONS ========
// -------------------------------------------------------
// RClientDll::RClientDll
// -------------------------------------------------------
//
EXPORT_C RClientDll::RClientDll( RWsSession& aSession )
    : RAnimDll( aSession )
    {
    }

// End of File
