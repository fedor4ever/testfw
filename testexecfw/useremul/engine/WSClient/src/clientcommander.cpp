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
 * File Name: clientcommander.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Declares Anim dll client side commander class
 *------------------------------------------------------------------
 -
 *
 */

// User Includes
#include "clientcommander.h"

// ======== MEMBER FUNCTIONS ========
// -------------------------------------------------------
// RClientCommander::RClientCommander
// -------------------------------------------------------
//
EXPORT_C RClientCommander::RClientCommander( RAnimDll &aAnimDll ) :
                                                              RAnim( aAnimDll )
    {// No implementation required
    }

// -------------------------------------------------------
// RClientCommander::AnimConstruct
// -------------------------------------------------------
//
EXPORT_C void RClientCommander::AnimConstruct( const RWindowBase& aDevice,
                                             TInt aType, const TDesC8& aParams )
    {
    RAnim::Construct( aDevice, aType, aParams );
    }

// -------------------------------------------------------
// Communication to the server side
// -------------------------------------------------------
//
EXPORT_C TInt RClientCommander::AnimCommand( TInt aCommand, const TPtrC8* aArgs )
    {
    TInt res;
    // Start and Poll needs to send data to server side. Others don't.
    if(aCommand==RClientCommander::EStart || aCommand==RClientCommander::EPoll)
    	res=RAnim::CommandReply( aCommand, *aArgs);
    else 
    	res=RAnim::CommandReply( aCommand);
    
    return res;
    }

// End of File
