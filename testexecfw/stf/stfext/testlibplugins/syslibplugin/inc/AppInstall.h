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
* Description: This file contains the header file of the CAtsLogger 
* class.
*
*/

#if (!defined __TEST_SECURITY_APPINSTALL_H__)
#define __TEST_SECURITY_APPINSTALL_H__


class CAppInstall 

{
public:
	CAppInstall();

	static TInt Install(const TDesC&);
	static TInt Uninstall(const TDesC&);	
	
protected:
	
private:
};

#endif // __TEST_SECURITY_APPINSTALL_H__
