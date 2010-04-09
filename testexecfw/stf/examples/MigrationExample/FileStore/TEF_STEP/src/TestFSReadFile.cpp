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

// User includes
#include "TestFSReadFile.h"

// System includes
#include <e32base.h>


// Literal constants
_LIT(KFileName,					"filename");

CTestFSReadFile::CTestFSReadFile()
/**
Constructor
 */
	{
	// Call base class method to set up the human readable name for logging
	SetTestStepName(KTestFSReadFile);
	}

CTestFSReadFile::~CTestFSReadFile()
/**
Destructor
 */
	{
	}

TVerdict CTestFSReadFile::doTestStepL()
/**
This is implementation of the pure virtual function of CTestStep.
It reads data from a file.
@return TVerdict - the test result
 */
	{
	TInt				err = 0;
	TPtrC				fileName;
	
	INFO_PRINTF1(_L("In test step ReadFile"));

	//Getting the file name from ini file 
	if(!GetStringFromConfig(ConfigSection(), KFileName ,fileName))
		{
		ERR_PRINTF1(_L("File Name not specified"));
		SetTestStepResult(EFail);
		}
	else
		{
		INFO_PRINTF2(_L("File to be opened is= %S "),&fileName);
		}

	if(TestStepResult() == EPass)
		{
		RFs fileSession;
		//Connecting to the file server
		TRAP(err, fileSession.Connect())
		if ( err != KErrNone)
			{
			ERR_PRINTF1(_L("Failed to connect to the file server"));
			SetTestStepError(err);
			}
		else
			{
			RFile file;

			CleanupClosePushL(fileSession);
			//Opening the file
			err = file.Open(fileSession,fileName,EFileRead);
			if ( err != KErrNone)
				{
				ERR_PRINTF1(_L("Error in Opening file"));
				SetTestStepError(err);
				}
			else
				{
				CleanupClosePushL(file);
				TInt fileSize = 0;
				err = file.Size(fileSize);
				if(err == KErrNone)
					{
					HBufC8 *data = HBufC8::NewLC(fileSize);
					TPtr8 ptr = data->Des();
			//		RaiseInstrumentationEventNotificationL(MTestInstrumentation::EPointFilestoreReadFileStart); 
					err = file.Read(ptr);
			//		RaiseInstrumentationEventNotificationL(MTestInstrumentation::EPointFilestoreReadFileStop); 
					if(err != KErrNone)
						{
						ERR_PRINTF1(_L("Error in reading the file"));
						SetTestStepError(err);			
						}
					else
						{
						INFO_PRINTF1(_L("Read File passed with no error code"));
						SetTestStepResult(EPass);
						}
					CleanupStack::PopAndDestroy(data);
					}
				else
					{
					ERR_PRINTF1(_L("Error in finding size of the file"));
					SetTestStepError(err);		
					}
				file.Close();
				CleanupStack::PopAndDestroy(&file);
						
				}
			CleanupStack::PopAndDestroy(&fileSession);
			}
		}
	return TestStepResult();
	}
