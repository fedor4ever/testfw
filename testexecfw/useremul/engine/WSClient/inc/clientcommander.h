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
 * File Name: clientcommander.h
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
 
#ifndef CLIENT_COMMANDER_H__
#define CLIENT_COMMANDER_H__

// System Includes
#include <w32adll.h>

// Class Declaration

/**
 * Implements RAnim class. Communication with server side
 */ 
class RClientCommander : public RAnim
    {
    public:
        /**
         * Constructor
         * @param aAnimDll Reference to the AnimDll object
         */ 
        IMPORT_C RClientCommander( RAnimDll& aAnimDll );
        /**
         * Constructs the RAnim object
         * @param aDevice Reference to RWindowBase object
         * @param aType not used
         * @param aParams not used
         */
        IMPORT_C void AnimConstruct( const RWindowBase& aDevice, TInt aType,
                                                        const TDesC8& aParams );
         /**
         * Sends command to server side
         * @param aCommand  Command to send (see TAnimCommands)
         * @param aArgs     Binary data to the server side 
         */
        IMPORT_C TInt AnimCommand( TInt aCommand, const TPtrC8* aArgs );
        
        /**
         * Commands to the server
         */
        enum TAnimCommands
                   {
                   EStart = 1,
                   EStartTiming,
                   EStop,
                   EPoll, 
                   ETaskExisted
                   };
    };
#endif // CLIENT_COMMANDER_H__

// End of File
