/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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



/**
 @file WriteIntStep.h
*/
#if (!defined __WRITEINT_STEP_H__)
#define __WRITEINT_STEP_H__
#include <TestExecuteStepBase.h>

class CWriteIntStep : public CTestStep
	{
public:
	CWriteIntStep();
	~CWriteIntStep();
	virtual TVerdict doTestStepPreambleL();
	virtual TVerdict doTestStepL();
	virtual TVerdict doTestStepPostambleL();

// Please add/modify your class members here:
private:
	};

_LIT(KWriteIntStep,"WriteIntStep");

#endif
