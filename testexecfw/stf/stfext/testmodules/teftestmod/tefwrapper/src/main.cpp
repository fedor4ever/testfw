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



// INCLUDE FILES
#include <e32base.h>
#include <e32cons.h>

#include "utils.h"
#include "wrapperapplication.h"

_LIT( KConsoleTitle, "TestExecute2" );
CConsoleBase* console;

LOCAL_C void startTEFTestL() 
	{

	CTefWrapperApplication* app = CTefWrapperApplication::NewLC();
	
	app->ParseCommandLineL();
	app->WriteConfigFileL();
	app->ExecuteL();
	app->DeleteConfigFile();
	
	CleanupStack::PopAndDestroy( app );
	}
	
LOCAL_C void callTEFTestL()
	{
	console = Console::NewL( KConsoleTitle,TSize( KConsFullScreen,KConsFullScreen ) );
	
	TRAPD( error, startTEFTestL() );
	switch( error )
		{
		case Utils::EErrBadParam:
			console->Printf( _L("Bad Params Input.\n") );
			console->Getch();
			break;
		case Utils::EErrFileNotFound:
			console->Printf( _L("Script file not found.\n") );
			console->Getch();
			break;
		case Utils::EErrHelp:
		case Utils::EErrVersion:
			console->Printf( _L("Press any key to continue...\n") );
			console->Getch();
		}
		
	delete console;
	}
	
GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	
	CTrapCleanup* cleanup=CTrapCleanup::New(); // get clean-up stack
		
	TRAPD( error, callTEFTestL() ); 
	
	
	delete cleanup; // destroy clean-up stack
	__UHEAP_MARKEND;
	return error; 
	}
