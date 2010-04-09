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

#ifndef __CTESTFILESERVERWRAPPER_H__
#define __CTESTFILESERVERWRAPPER_H__

#include <DataWrapper.h>

class CTestFileServerWrapper : public CDataWrapper
	{
public:
	// Default Constructor
	CTestFileServerWrapper();
	// Default Destructor
	~CTestFileServerWrapper();

	// Two-Phase Constructor
	static CTestFileServerWrapper* NewL();

	// Command Dispatcher
	virtual TBool DoCommandL(const TTEFFunction& aCommand, const TTEFSectionName& aSection, const TInt aAsyncErrorIndex);

	// Getter for the wrapped object
	virtual TAny* GetObject() { return iObject; }

	// Setter for the wrapped object
	inline virtual void SetObjectL(TAny* aObject)
		{
		delete iObject;
		iObject = NULL;
		iObject = static_cast<CTestFileServerWrapper*> (aObject);
		}
	

	// Asynchronous Run
	void RunL(CActive* aActive, TInt aIndex);
	
    inline virtual void DisownObjectL() 
        {
        iObject = NULL;
        }
    inline virtual TCleanupOperation CleanupOperation()
        {
        return CleanupOperation;
        }

protected:
	void ConstructL();

private:
	// Testing CTestFileServer::CreateFile
	inline void DoCmdCreateFile(const TDesC& aSection);

	// Testing CTestFileServer::ReadFile
	inline void DoCmdReadFile(const TDesC& aSection);
	
	inline void DoCmdPanicExample(const TDesC& aSection);
	
    static void CleanupOperation(TAny* aAny)
        {
        CTestFileServerWrapper* myObject = static_cast<CTestFileServerWrapper*>(aAny);
        delete myObject;
        }	


private:
	CActiveCallback* iActiveCallback;
	TAny* iObject;
	};
#endif // __CTESTFILESERVERWRAPPER_H__
