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

#include "TestFileServer_TEFBlockController.h"

_LIT(KCTestFileServerWrapper, "CTestFileServerWrapper");

CTestFileServer_TEFBlockController* CTestFileServer_TEFBlockController::NewL()
/**
 * @return - Instance of the test block controller
 */
    {
    CTestFileServer_TEFBlockController* block = new (ELeave) CTestFileServer_TEFBlockController();
    return block;
    }


CDataWrapper* CTestFileServer_TEFBlockController::CreateDataL(const TDesC& aData)
    {
    CDataWrapper* wrapper = NULL;
	if (KCTestFileServerWrapper() == aData)
		{
		wrapper = CTestFileServerWrapper::NewL();
		}

    return wrapper;
    }
