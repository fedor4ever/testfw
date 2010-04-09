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
* Main log server engine.
* Process log requests from multiple clients simultaneously.
*
*/



#include "utils.h"

#include <f32file.h>

TBool Utils::FileExists( RFs& aFs, const TDesC& aFileName )
	{
	TBool ret = EFalse;
	TUint att;
	TInt retValue = aFs.Att( aFileName, att );
	if( KErrNone == retValue )
		{
		ret = ETrue;
		}
	/*else if( KErrNotFound == retValue ) 
		{
		ret = EFalse;
		}
	else if( KErrPathNotFound == retValue )
		{
		ret = EFalse;
		}*/
		
	return ret;
	}

void Utils::DeleteFile( RFs& aFs, const TDesC& aFileName )
	{
	if ( FileExists( aFs, aFileName ) )
		{
		aFs.Delete( aFileName );
		}
	}

void Utils::MkDir( RFs& aFs, const TDesC& aDir )
	{
	if ( !FileExists( aFs, aDir ) )
		{
		aFs.MkDir( aDir );
		}
	}
