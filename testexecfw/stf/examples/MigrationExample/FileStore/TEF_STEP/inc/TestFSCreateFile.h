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

#if (!defined __TEST_FS_CREATE_FILE_H__)
#define __TEST_FS_CREATE_FILE_H__

// System includes

// User includes
#include <TestExecuteStepBase.h>

// Literal constant for test step name
_LIT(KCreateFile,"CreateFile");

class CTestFSCreateFile :public CTestStep
/**
@internalAll
@test
The class implements a test step which creates files or directories. 
 */
	{
public:
	CTestFSCreateFile();
	~CTestFSCreateFile();
	virtual TVerdict doTestStepL();
	
	};

#endif /* __TEST_FS_CREATE_FILE_H__ */
