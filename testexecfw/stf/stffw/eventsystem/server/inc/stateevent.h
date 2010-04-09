/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/
#ifndef STATEEVENT_H
#define STATEEVENT_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>

// CLASS DECLARATION


/**
 *  CStateEvent - class representing state events 
 * 
 */
class CStateEvent : public CBase
    {
    public:
        // Destructor.
        ~CStateEvent();

        // Two-phased constructor.
        static CStateEvent* NewL(const TDesC& aEventName);

        // Two-phased constructor.
        static CStateEvent* NewLC(const TDesC& aEventName);
        
        // Checks if event name is the same as provided in argument
        TInt IsMatchingEvent(const TDesC& aEventName);
        
        // Set flag which says that event will be unset when matching event will be released
        void NotifyToBeUnset(TBool aValue);

        // Returns flag which says that event will be unset when matching waiting event will be released
        bool IsToBeUnset();
        
    private:
        // Constructor for performing 1st stage construction
        CStateEvent();

        // Constructor for performing 2nd stage construction
        void ConstructL(const TDesC& aEventName);
        
    private:
        // Event name
        HBufC* iEventName;
        
        // Candidate to be unset as only matching event will be released
        TBool iToBeUnset;
        
    };

#endif // STATEEVENT_H

// EOF
