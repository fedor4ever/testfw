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

#if (!defined __TESTFILESERVER_TEF_BLOCK_SERVER_H__)
#define __TESTFILESERVER_TEF_BLOCK_SERVER_H__
#include <TestServer2.h>
#include "TestFileServer_TEFBlockController.h"

class CTestFileServer_TEFBlockServer : public CTestServer2
	{
public:
	static CTestFileServer_TEFBlockServer* NewL();
	// Base class pure virtual override
	virtual CTestBlockController* CreateTestBlock();

// Please Add/modify your class members
private:
	};

#endif
