/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#include <e32svr.h>
#include "stateevent.h"


/**
 * Constructor for performing 1st stage construction
 */
CStateEvent::CStateEvent()
    {
    }

/**
 * Destructor.
 */
CStateEvent::~CStateEvent()
    {
    delete iEventName;
    }

/**
 * Two-phased constructor.
 */
CStateEvent* CStateEvent::NewLC(const TDesC& aEventName)
    {
    CStateEvent* self = new (ELeave) CStateEvent();
    CleanupStack::PushL(self);
    self->ConstructL(aEventName);
    return self;
    }

/**
 * Two-phased constructor.
 */
CStateEvent* CStateEvent::NewL(const TDesC& aEventName)
    {
    CStateEvent* self = CStateEvent::NewLC(aEventName);
    CleanupStack::Pop();
    return self;
    }

/**
 * Constructor for performing 2nd stage construction
 */
void CStateEvent::ConstructL(const TDesC& aEventName)
    {
    iEventName = aEventName.AllocL();
    iToBeUnset = EFalse;
    }

/**
 * Checks if event name is the same as provided in argument
 */
TInt CStateEvent::IsMatchingEvent(const TDesC& aEventName)
    {
    return (aEventName.Compare(*iEventName) == 0) ? (1) : (0);
    }

/**
 * Set flag which says that event will be unset when matching waiting event will be released
 */
void CStateEvent::NotifyToBeUnset(TBool aValue)
    {
    iToBeUnset = aValue;
    }

/**
 * Returns flag which says that event will be unset when matching waiting event will be released
 */
bool CStateEvent::IsToBeUnset()
    {
    return iToBeUnset;
    }
    
// EOF
