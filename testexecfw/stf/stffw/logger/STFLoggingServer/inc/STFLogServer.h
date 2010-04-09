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

#ifndef __STFLOGGINGSERVER_H__
#define __STFLOGGINGSERVER_H__

//  Include Files

#include <e32base.h>
#include "STFLoggerCSCommon.h"
#include "stifloggerbase.h"

enum TServerPanic
	{
	EPanicBadDescriptor,
	EPanicNotSupported
	};

void PanicClient(const RMessage2& aMessage,TServerPanic TMyPanic);

const TInt KShutdownDelay=200000; // approx 2 seconds
class CShutdown : public CTimer
	{
public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();
private:
	void RunL();
	};


inline CShutdown::CShutdown()
	:CTimer(-1)
	{CActiveScheduler::Add(this);}
inline void CShutdown::ConstructL()
	{CTimer::ConstructL();}
inline void CShutdown::Start()
	{After(KShutdownDelay);}

class CSTFLogServer : public CServer2
	{
	public:
		static CServer2* NewLC();
		void AddSession();
		void RemoveSession();
	private:
		CSTFLogServer();
		void ConstructL();
		// From CServer2
		virtual CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const; 
	private:
		TInt iSessionCount;
		CShutdown iShutdown;		
	};

inline CSTFLogServer::CSTFLogServer()
	:CServer2(CActive::EPriorityStandard)
	{}

class CSTFLogSession : public CSession2
	{
public:
	CSTFLogSession();
	void CreateL();
public:
	virtual void ServiceL(const RMessage2& aMessage); // From CSession2
	virtual void ServiceError(const RMessage2& aMessage, TInt aError); // From CSession2
private:
	void CreateLogger(const RMessage2& aMessage);
	void Log_TInt_TDesCL(const RMessage2& aMessage);
	void Log_TInt_TDesC8L(const RMessage2& aMessage);
	void CreationResult(const RMessage2& aMessage);
	void OutputType(const RMessage2& aMessage);

private:
	~CSTFLogSession();
private:
	CStifLoggerBase* iLogger;
	};

inline CSTFLogSession::CSTFLogSession()
	{}


GLDEF_C TInt E32Main();

#endif  // __STFLOGGINGSERVER_H__

