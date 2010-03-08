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
 * File Name: serverdll.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Implementation of Anim dll server side class
 *------------------------------------------------------------------
 -
 *
 */

// User Includes
#include "serverdll.h"
#include "serverCtrl.h"

// ======== MEMBER FUNCTIONS ========
CServerDll::CServerDll()
    : CAnimDll()
    {// No implementation required
    }

CAnim* CServerDll::CreateInstanceL( TInt /* aType */ )
    {
    return ( new ( ELeave ) CServerCtrl );
    }

// ============ OTHER EXPORTED FUNCTIONS ============

EXPORT_C CAnimDll* CreateCAnimDllL()
    {
    return ( new ( ELeave ) CServerDll );
    }

// End of File
