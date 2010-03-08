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
 * File Name: serverdll.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Declares Anim dll server side class
 *------------------------------------------------------------------
 -
 *
 */

#ifndef SERVERDLL_H__
#define SERVERDLL_H__

// System Includes
#include <w32adll.h>

// Class Declaration

/**
 *  Implements Anim dll server side class.
 *  Creates CServerCtrl object
 */
class CServerDll : public CAnimDll
    {
    public:
        /**
        * Constructor
        */
        CServerDll();

    public:
        /**
        * Creator function
        * @param aType not used
        */
        IMPORT_C CAnim* CreateInstanceL( TInt aType );

    };
#endif // SERVERDLL_H__

// End of File
