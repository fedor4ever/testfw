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
 * File Name: clientdll.h
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

#ifndef CLIENT_DLL_H__
#define CLIENT_DLL_H__

// System Includes
#include <w32adll.h>

// Class Declaration
/**
 * Implements RAnimdll class
 */
class RClientDll : public RAnimDll
    {
    public:
        /**
        * Initialise RAnimDll.
        * @param aSession   Window server session
        */
        IMPORT_C RClientDll( RWsSession& aSession );
    };

#endif //CLIENT_DLL_H__

// End of File
