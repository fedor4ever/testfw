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
#include "activetimer.h"

CActiveTimer::CActiveTimer(CConsoleBase* aConsole) : 
    CTimer(CActive::EPriorityStandard),
    iConsole(aConsole)
    {
    CActiveScheduler::Add(this);
    }

void CActiveTimer::ConstructL()
    {
    CTimer::ConstructL();
    iXPos = 0;
    iYPos = 1;
    }

CActiveTimer* CActiveTimer::NewL(CConsoleBase* aConsole)
    {
    CActiveTimer* self = new(ELeave) CActiveTimer(aConsole);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CActiveTimer::~CActiveTimer()
    {
    Cancel();    
    }

void CActiveTimer::StartL()
    {
    iStartTime.HomeTime();
    CTimer::After(KPrintInterval);
    }

void CActiveTimer::RunL()
    {
    TTimeIntervalSeconds seconds;
    TTime time;
    time.HomeTime();
    time.SecondsFrom(iStartTime, seconds);
    
    TInt x = iConsole->WhereX();
    TInt y = iConsole->WhereY();
    
    iConsole->SetPos(iXPos, iYPos);
    iConsole->Printf(_L("\n"));
    iConsole->Printf(_L("[Time: %d]   "), seconds.Int());
    iConsole->SetPos(x, y);
    
    CTimer::After(KPrintInterval);
    }

void CActiveTimer::DoCancel()
    {
    TTimeIntervalSeconds seconds;
    TTime time;
    time.HomeTime();
    time.SecondsFrom(iStartTime, seconds);
    iConsole->Printf(_L("Total Time: [%d]"), seconds.Int());
    CTimer::DoCancel();
    }



