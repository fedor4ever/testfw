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



#ifndef UTILS_H_
#define UTILS_H_

#include <e32std.h>

class RFs;

class Utils 
	{
	public:
		enum TErrCode 
			{
			EErrNone = 100,
			EErrVersion,
			EErrHelp,
			EErrFileNotFound, 
			EErrBadParam
			};
	
	public:
		static TBool FileExists( RFs&, const TDesC& );
		static void DeleteFile( RFs&, const TDesC& );
		static void MkDir( RFs&, const TDesC& );
	};

#endif	// UTILS_H_
