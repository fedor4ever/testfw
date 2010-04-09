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
#ifndef STFEVENTSYSTEM_H_
#define STFEVENTSYSTEM_H_

#include <e32base.h>

#include <stfeventsystemdefs.h>
#include <stfasynceventlist.h>

class REventSystem: public RSessionBase 
    {
    public:            
        // Wait for event
        IMPORT_C void WaitEventL(const TDesC& aEventName, const TInt aOwner);
        //IMPORT_C void CancelWaitEventL(const TDesC& aEventName, const TInt aOwner);
        
        // Request
        IMPORT_C void RequestEventL(const TDesC& aEventName, const TInt aOwner);
        
        // Release
        IMPORT_C void ReleaseEventL(const TDesC& aEventName, const TInt aOwner);
        
        // Set indication event
        IMPORT_C void SetIndicationEventL(const TDesC& aEventName);
        IMPORT_C void SetIndicationEventL(const TDesC& aEventName, TRequestStatus* aStatus);
                
        // State events
        IMPORT_C void SetEventL(const TDesC& aEventName);
        IMPORT_C void SetEventL(const TDesC& aEventName, TRequestStatus* aStatus);

        IMPORT_C void UnsetEventL(const TDesC& aEventName);
        IMPORT_C void UnsetEventL(const TDesC& aEventName, TRequestStatus* aStatus);
        
        // Cancel (remove) all requested events of given owner
        IMPORT_C TInt RemoveInRequestedState(const TInt aOwner);
                
    private:
        // Connect to event system
        IMPORT_C TInt Connect();

        // Close event system connection
        IMPORT_C void Close();

        // Return version
        IMPORT_C TVersion Version() const;
        
        // Friends
        friend class CAsyncEventActive;
        
    }; /* class REventSystem */
    

#endif /* STFEVENTSYSTEM_H_ */
// EOF
