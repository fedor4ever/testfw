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

#if (!defined __TESTFILESERVER_TEF_BLOCK_CONTROLLER_H__)
#define __TESTFILESERVER_TEF_BLOCK_CONTROLLER_H__
#include <TestBlockController.h>
#include "CTestFileServerWrapper.h"

class CTestFileServer_TEFBlockController : public CTestBlockController
	{
public:
	static CTestFileServer_TEFBlockController* NewL();
	// Base class pure virtual override
	virtual CDataWrapper* CreateDataL(const TDesC& aData);

// Please Add/modify your class members
private:
	};

#endif
