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

#include "TestFileServer_TEFBlockServer.h"

CTestFileServer_TEFBlockServer* CTestFileServer_TEFBlockServer::NewL()
    {
	CTestFileServer_TEFBlockServer* server = new (ELeave) CTestFileServer_TEFBlockServer();
	CleanupStack::PushL(server);
	server->ConstructL();
	CleanupStack::Pop(server);
	return server; 
	}

CTestBlockController* CTestFileServer_TEFBlockServer::CreateTestBlock()
/**
 * @return - Instance of the test block controller
 * Called when CTestFileServer_TEFBlockServer initializing.
 */
    {
    CTestBlockController* controller = NULL;
    TRAPD(err, controller = CTestFileServer_TEFBlockController::NewL());
    return controller;
    }

// Secure variants much simpler
// For EKA2, just an E32Main and a MainL()
LOCAL_C void MainL()
/**
 * Secure variant
 * Much simpler, uses the new Rendezvous() call to sync with the client
 */
	{
	// Leave the hooks in for platform security
#if (defined __DATA_CAGING__)
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().DataCaging(RProcess::ESecureApiOn);
#endif
	CActiveScheduler* sched=NULL;
	sched=new(ELeave) CActiveScheduler;
	CActiveScheduler::Install(sched);
	CTestFileServer_TEFBlockServer* server = NULL;
	// Create the CTestFileServer_TEFBlockServer derived server
	TRAPD(err,server = CTestFileServer_TEFBlockServer::NewL());
	if(!err)
		{
		// Sync with the client and enter the active scheduler
		RProcess::Rendezvous(KErrNone);
		sched->Start();
		}
	delete server;
	delete sched;
	}



GLDEF_C TInt E32Main()
/**
 * @return - Standard Epoc error code on process exit
 * Secure variant only
 * Process entry point. Called by client using RProcess API
 */
	{
	__UHEAP_MARK;
	CTrapCleanup* cleanup = CTrapCleanup::New();
	if(cleanup == NULL)
		{
		return KErrNoMemory;
		}
	TRAPD(err,MainL());
	delete cleanup;
	__UHEAP_MARKEND;
	return err;
    }
