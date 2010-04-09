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
#include <e32std.h>
#include <e32svr.h>

#include <stfeventsystemerrorcodes.h>
#include "stfasynceventlist.h"
#include <stfasynceventactive.h>


// Implementation of list of asynchronous events

/**
*/
EXPORT_C CAsyncEventList* CAsyncEventList::NewL()
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::NewL()"));
    CAsyncEventList* self = new (ELeave) CAsyncEventList();
    CleanupStack::PushL(self);

    // Construct the object
    self->ConstructL();

    // Remove from cleanup stack
    CleanupStack::Pop(self);

    return self;
    }

/**
*/
void CAsyncEventList::ConstructL()
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::ConstructL()"));
    }

/**
*/
CAsyncEventList::CAsyncEventList()
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventList::CAsyncEventList()"));
    }

/**
*/
EXPORT_C CAsyncEventList::~CAsyncEventList()
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::~CAsyncEventList() #1-start"));
    CancelAllAsyncEvents();
    iAsyncEvents.ResetAndDestroy();
    iAsyncEvents.Close();
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::~CAsyncEventList() #1-end"));
    }

/**
* Add active object to the list.
*/
EXPORT_C void CAsyncEventList::AddAsyncEventL(CAsyncEventActive* aAsyncEvent)
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::AddAsyncEventL()"));
    if(iAsyncEvents.Find(aAsyncEvent) >= 0)
        {
        User::Leave(KErrAlreadyExists);
        }
    iAsyncEvents.AppendL(aAsyncEvent);
    }
        
/**
* Remove active object from the list.
*/
EXPORT_C void CAsyncEventList::RemoveAsyncEventL(CAsyncEventActive* aAsyncEvent)
    {
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::RemoveAsyncEventL()"));
    TInt index = iAsyncEvents.Find(aAsyncEvent);
    //RDebug::Print(_L("STF [ESC]: CAsyncEventList::RemoveAsyncEventL() index=[%d]"), index);
    if(index >= 0)
        {
        iAsyncEvents.Remove(index);
        }
    }

/**
* Cancel and remove all active objects from the list.
*/
void CAsyncEventList::CancelAllAsyncEvents()
    {
    RDebug::Print(_L("STF [ESC]: CAsyncEventList::CancelAllAsyncEvents()"));
    while(iAsyncEvents.Count())
        {
        CAsyncEventActive* asyncEvent = iAsyncEvents[0];
        asyncEvent->Cancel();
        iAsyncEvents.Remove(0);
        delete asyncEvent;
        }
    }
