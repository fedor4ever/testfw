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
*
*/

//  Include Files  
#include <e32base.h>
#include <f32file.h> 
#include "STFLogServer.h"
#include "STFLoggerCSCommon.h"


// Called by the CServer framework
void CSTFLogSession::CreateL()
	{
	CSTFLogServer* server = static_cast<CSTFLogServer*>(const_cast<CServer2*>(Server()));
	ASSERT(server);
	server->AddSession();
	iLogger = NULL;
	}

CSTFLogSession::~CSTFLogSession()
	{
	CSTFLogServer* server = static_cast<CSTFLogServer*>(const_cast<CServer2*>(Server()));
	ASSERT(server);
	server->RemoveSession();
	
	if(iLogger)
	    {
        delete iLogger;
        iLogger = 0;
	    }
	}

// Handle a client request
// Leaves are handled by CHerculeanSession::ServiceError()
void CSTFLogSession::ServiceL(const RMessage2& aMessage)
	{
	switch (aMessage.Function())
		{
		case (ESTFLogger_CreateLogger):
			CreateLogger(aMessage);
			break;
		case (ESTFLogger_Log_TInt_TDesC):
			Log_TInt_TDesCL(aMessage);
			break;
		case (ESTFLogger_Log_TInt_TDesC8):
			Log_TInt_TDesC8L(aMessage);
			break;
		case (ESTFLogger_CreationResult):
			CreationResult(aMessage);
			break;
		case (ESTFLogger_OutputType):
			OutputType(aMessage);
			break;
		default:
			PanicClient(aMessage,EPanicNotSupported);
		break;
		}
	}

// Handles leaves from CSTFLogSession::ServiceL()
// A bad descriptor error implies a badly programmed client, so panic it
// Report other errors to the client by completing the outstanding request with the error

void CSTFLogSession::ServiceError(const RMessage2& aMessage, TInt aError)
	{
	if (KErrBadDescriptor==aError)
		PanicClient(aMessage,EPanicBadDescriptor);
	else
		aMessage.Complete(aError);
	}


void CSTFLogSession::CreateLogger(const RMessage2& aMessage)
    {
    TSTFLoggerCreateData loggerCreateData;
    TPckg<TSTFLoggerCreateData> pckgData(loggerCreateData);
    aMessage.Read(0,pckgData);
    TInt err;
    if(!iLogger)
        {
        TRAP(err, iLogger = CStifLoggerBase::NewL( loggerCreateData.iTestPath,
                                                loggerCreateData.iTestFile,
                                                loggerCreateData.iLoggerType,
                                                loggerCreateData.iOutput,
                                                loggerCreateData.iOverWrite,
                                                loggerCreateData.iWithTimeStamp,
                                                loggerCreateData.iWithLineBreak,
                                                loggerCreateData.iWithEventRanking,
                                                loggerCreateData.iThreadIdToLogFile,
                                                loggerCreateData.iCreateLogDir,
                                                loggerCreateData.iStaticBufferSize,
                                                loggerCreateData.iUnicode,
                                                loggerCreateData.iThreadId) );
        }
    aMessage.Complete(err); 
    }

void CSTFLogSession::Log_TInt_TDesCL(const RMessage2& aMessage)
	{
	if(!iLogger)
		{
		aMessage.Complete(KErrBadHandle);
		}
	else
		{
		TInt length = aMessage.Int0();
		TInt style = aMessage.Int1();
		RBuf buf;
		buf.CleanupClosePushL();
		buf.Create(length);
		aMessage.Read(2, buf);
		TInt err = iLogger->Log(style, buf);	
		CleanupStack::PopAndDestroy(&buf);
		aMessage.Complete(err);
		}
	}
void CSTFLogSession::Log_TInt_TDesC8L(const RMessage2& aMessage)
	{
	if(!iLogger)
		{
		aMessage.Complete(KErrBadHandle);
		}
	else
		{
		TInt length = aMessage.Int0();
		TInt style = aMessage.Int1();
		RBuf8 buf8;
		buf8.CleanupClosePushL();
		buf8.Create(length);
		aMessage.Read(2, buf8);
		TInt err = iLogger->Log(style, buf8);	
		CleanupStack::PopAndDestroy(&buf8);
		aMessage.Complete(err);
		}
	}

void CSTFLogSession::CreationResult(const RMessage2& aMessage)
	{
	if(!iLogger)
		{
		aMessage.Complete(KErrBadHandle);
		}
	else
		{
		TInt result;
		TPckg<TInt> creationResult(result);
		result = iLogger->CreationResult();
		
		aMessage.Write(0,creationResult);
		aMessage.Complete(KErrNone);
		}
	
	}
void CSTFLogSession::OutputType(const RMessage2& aMessage)
	{
	if(!iLogger)
		{
		aMessage.Complete(KErrBadHandle);
		}
	else
		{
		CStifLogger::TOutput outputType;
		TPckg<CStifLogger::TOutput> type(outputType);
		outputType = iLogger->OutputType();
		
		aMessage.Write(0,type);
		aMessage.Complete(KErrNone);
		}
	}

CServer2* CSTFLogServer::NewLC()
	{
	CSTFLogServer* self=new(ELeave) CSTFLogServer;
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// Starts the server and constructs the shutdown object, starting the timer to ensure that
// the server will exit even if the starting client connection fails
void CSTFLogServer::ConstructL()
	{
	StartL(KSTFLoggerServerName);
	iShutdown.ConstructL();
	iShutdown.Start();
	}

// Example doesn't bother checking the version
CSession2* CSTFLogServer::NewSessionL(const TVersion& /*aVersion*/,const RMessage2& /*aMessage*/) const
	{
	return new(ELeave) CSTFLogSession();
	}
// Cancel the shutdown timer now, the new session is connected
void CSTFLogServer::AddSession()
	{
	++iSessionCount;
	iShutdown.Cancel();  // Cancel any outstanding shutdown timer
	}

// Decrement the session counter and start the shutdown timer if the last client has disconnected
void CSTFLogServer::RemoveSession()
	{
	if (--iSessionCount==0)
		iShutdown.Start();
	}

// Initiates server exit when the timer expires
void CShutdown::RunL()
	{
	CActiveScheduler::Stop();
	}

void PanicClient(const RMessage2& aMessage,TServerPanic aPanic)
	{
	_LIT(KPanic,"STFLoggingServer");
	aMessage.Panic(KPanic,aPanic);
	}

// Initialize and run the server
static void RunTheServerL()
	{// First create and install the active scheduler
	CActiveScheduler* scheduler = new (ELeave) CActiveScheduler;
	CleanupStack::PushL(scheduler);
	CActiveScheduler::Install(scheduler);

	// create the server
	CSTFLogServer::NewLC();
	
	// Naming the server thread after the server helps to debug panics
	User::LeaveIfError(User::RenameThread(KSTFLoggerServerName));
	
	RProcess::Rendezvous(KErrNone);

	// Enter the wait loop
	CActiveScheduler::Start();
	
	// Exited - cleanup the server and scheduler
	CleanupStack::PopAndDestroy(2, scheduler);
	}

// Server process entry-point
TInt E32Main()
	{
	__UHEAP_MARK; // Heap checking
	
	CTrapCleanup* cleanup=CTrapCleanup::New();
	TInt r=KErrNoMemory;
	if (cleanup)
		{
		TRAP(r,RunTheServerL());
		delete cleanup;
		}
	__UHEAP_MARKEND;
	return r;
	}
