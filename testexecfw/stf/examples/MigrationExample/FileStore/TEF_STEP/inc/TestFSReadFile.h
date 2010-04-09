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
#if (!defined __TEST_FS_READ_FILE_H__)
#define __TEST_FS_READ_FILE_H__

// User includes
#include <TestExecuteStepBase.h>

// Literal constant for test step name
_LIT(KTestFSReadFile,"ReadFile");

class CTestFSReadFile :public CTestStep
/**
@internalAll
@test
The class implements a test step which reads data from a file.
 */
	{
public:
	CTestFSReadFile();
	~CTestFSReadFile();
	virtual TVerdict doTestStepL();
	};

#endif /* __TEST_FS_READ_FILE_H__ */
