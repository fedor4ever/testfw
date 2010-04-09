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

#ifndef ACTIVETIMER_H_
#define ACTIVETIMER_H_
#include <e32std.h>
#include <e32base.h>
#include <e32cons.h>

const TInt KPrintInterval = 1000000;

class CActiveTimer : public CTimer
    {
    
public:
    static CActiveTimer* NewL( CConsoleBase* aConsole );
    ~CActiveTimer();

public:
    void StartL();

public:
    void RunL();
    void DoCancel();

private:
    CActiveTimer( CConsoleBase* aConsole );
    void ConstructL();

private: 
    CConsoleBase* iConsole;
    TTime iStartTime;
    TInt iXPos;
    TInt iYPos;
    
    };

#endif /* ACTIVETIMER_H_ */
