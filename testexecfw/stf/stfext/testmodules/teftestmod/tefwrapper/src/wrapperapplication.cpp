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



#include <e32math.h>
#include <e32cons.h>

#include "utils.h"
#include "wrapperapplication.h"
//#include "applauncher.h"

_LIT( KScriptFileExt, ".script" );

const TInt KExtLength( 7 );
extern CConsoleBase* console;

CTefWrapperApplication::CTefWrapperApplication() 
				: iIsDebug( EFalse), iIsSlf( EFalse ), 
				  iIsTcx( EFalse ), iIsTci( EFalse ), iHasScript( EFalse )
	{
	}
	
CTefWrapperApplication* CTefWrapperApplication::NewLC()
	{
	CTefWrapperApplication* self = new (ELeave) CTefWrapperApplication;
	CleanupStack::PushL( self );
	self->ConstructL();
	
	return self;
	}
	
CTefWrapperApplication* CTefWrapperApplication::NewL()
	{
	CTefWrapperApplication* self = CTefWrapperApplication::NewLC();
	CleanupStack::Pop( self );
	
	return self;
	}
	
void CTefWrapperApplication::ConstructL()
	{
	User::LeaveIfError( iFs.Connect() );
	//iConsole = Console::NewL( KConsoleTitle, TSize( KConsFullScreen, KConsFullScreen ) );
	//iAppLauncher = CAppLauncher::NewL();
	}
	
CTefWrapperApplication::~CTefWrapperApplication()
	{
	iFs.Close();
	//delete iConsole;
	//delete iAppLauncher;
	}

TBool CTefWrapperApplication::IsScriptFile( const TDesC& aFileName )
	{
	TBool ret = EFalse;
	TBuf<KFileNameLength> scriptFile = aFileName;
	scriptFile.LowerCase();
	
	if ( scriptFile.Right(KExtLength) == KScriptFileExt ) 
		{
		ret = ETrue;
		}
		
	return ret;
	}

void CTefWrapperApplication::PrintHelpMessage()
	{
	console->Printf( _L("testexecute2 Version : 0.1\r\n\r\n") );
	console->Printf( _L("testexecute2 <scriptfilepath>\\<scriptfilename> [-d] [-slf]\r\n\r\n") );
	console->Printf( _L("            : Executes the scriptfile with optional modes for JustInTime debugging or sepqrate log files\r\n\r\n\r\n") );
	console->Printf( _L("testexecute2 [-h/--help/-v]\r\n\r\n") );
	console->Printf( _L("            : Run testexecute2 with optional modes to display list of help options and the version numer\r\n\r\n") );
	console->Printf( _L("-h/--help   : For displaying the help options available for using testexecute2 framework\r\n\r\n") );
	console->Printf( _L("-v          : For displaying the version of the testexecute2 framework\r\n\r\n") );
	console->Printf( _L("-slf        : For printing the messages for each of the tests in separate log files\r\n\r\n") );
	console->Printf( _L("-d          : For enableing just in time debug mode while execution\r\n\r\n") );
	}
void CTefWrapperApplication::ParseCommandLineL()
	{
	// todo : handle tcs and tcsdata. 
	
	_LIT( KDebugMode, "-d" );
    _LIT( KSlfTag, "-slf" );
    _LIT( KTciFlag, "-tci" );
    _LIT( KTcxFlag, "-tcx" );
    _LIT( KHelp, "-h" );
    _LIT( KHelpMsg, "--help" );
    _LIT( KVer, "-v" );
    
    const TInt length = User().CommandLineLength();
    if ( length == 0 )
    	{
    	PrintHelpMessage();
        User::Leave( Utils::EErrHelp );
    	}
    	
    HBufC* cmdLine = HBufC::NewLC( length );
    TPtr ptr = cmdLine->Des();
	User().CommandLine( ptr );
	
	TLex lex( ptr );

	// Parse the command line
    while ( !lex.Eos() )
        {
        TPtrC tmpPtr = lex.NextToken();
        
		if ( IsScriptFile( tmpPtr )) 
			{
			// Script=xxxx.script
			iScriptFileName = tmpPtr;
			if ( Utils::FileExists( iFs, iScriptFileName ) )
				{
				iHasScript = ETrue;
				}
			else 
				{
				User::Leave( Utils::EErrFileNotFound );
				}
			}
        else if ( tmpPtr == KDebugMode )// Check -d 
            {
            // Debug = ON / Debug = OFF
            iIsDebug = ETrue;
            }
        else if ( tmpPtr == KSlfTag )	// Check -slf 
            {
            // SeparateLog = OFF / SeparateLog = ON
            iIsSlf = ETrue;
            }
        else if ( tmpPtr == KTciFlag )	// Check -tci
            {
            TPtrC tmpTciParam = lex.NextToken();
            iTciParam = tmpTciParam;
            iIsTci = ETrue;
            }
        else if ( tmpPtr == KTcxFlag )	// Check tcx
            {
            TPtrC tmpTcxParam = lex.NextToken();
            iTcxParam = tmpTcxParam;
            iIsTcx = ETrue;
            }
        else if ( (tmpPtr == KHelp)||(tmpPtr == KHelpMsg) ) 
        	{
        	PrintHelpMessage();
        	User::Leave( Utils::EErrHelp );
        	}
        else if ( tmpPtr == KVer )
        	{
        	console->Printf( _L("testexecute2 Version : 0.1\r\n\r\n") );
        	User::Leave( Utils::EErrVersion );
        	}
        else 
        	{
            User::Leave( Utils::EErrBadParam );
            }
        } // while
        
    if ( !iHasScript )
    	{
    	User::Leave( KErrNotFound );
    	}
    	
    if ( iIsTci || iIsTcx )
    	{
    	TBool r = EFalse;
    	if ( iTciParam.Length() != 0 )
    		{
    		r = ETrue;
    		}
    		
    	if ( iTcxParam.Length() != 0 )
    		{
    		r = ETrue;
    		}

    	if( !r )
    		{
    		User::Leave( KErrNotFound );
    		}
    	}
        
    // Destroy command line buffer
    CleanupStack::PopAndDestroy( cmdLine );
	}
	
void CTefWrapperApplication::WriteLineL( const TDesC& aFileName, const TDesC& aString )
	{
	_LIT( KCR, "\r\n" );
    
    RFile file;
    if ( Utils::FileExists( iFs, aFileName ) )
        {
        User::LeaveIfError( file.Open( iFs, aFileName, EFileWrite ) );
        }
    else 
    	{
    	User::LeaveIfError( file.Create( iFs, aFileName, EFileWrite ) );
    	}
    
    CleanupClosePushL( file );
    
    TBuf8<KFileNameLength> strLine;
    strLine.Copy( aString );
    strLine.Append( KCR );
    
    TInt pos = 0;
    file.Size( pos );
    /*RFileWriteStream writeStream( file, pos );
    writeStream.PushL();
    writeStream.WriteL( strLine );
    writeStream.CommitL();
    writeStream.Pop();
    writeStream.Release();*/
    
    file.Write( pos, strLine );
    file.Close();

    CleanupStack::PopAndDestroy( &file );
	}
	
void CTefWrapperApplication::WriteLineL( const TDesC& aFileName, const TDesC& aKey, const TDesC& aValue )
	{
	_LIT( KAssignOpr, "=" );
	TBuf<KFileNameLength> strLine;
	strLine.Append( aKey );
	strLine.Append( KAssignOpr );
	strLine.Append( aValue );
	
	WriteLineL( aFileName, strLine );
	}
	
void CTefWrapperApplication::GetConfigFileName( TDes& aConfigFileName )
	{
	_LIT( KFormatString, "c:\\testframework\\stf_%x.cfg" );
	_LIT( KTestFrameWork, "c:\\testframework\\" );
    
    TTime theTime;
    theTime.UniversalTime();
	TInt64 randSeed( theTime.Int64() ); 
	TInt number( Math::Rand( randSeed ) );
	aConfigFileName.Format( KFormatString, number );
    
    // check if the specific dir exists. 
    if ( !Utils::FileExists( iFs, iCfgFileName ) )
        {
        Utils::MkDir( iFs, KTestFrameWork );
        }
	}
	
void CTefWrapperApplication::WriteConfigFileL()
	{
	_LIT( KSectionBegin, "[New_TEFTest]" );
    _LIT( KSectionEnd, "[End_TEFTest]" );
    _LIT( KTcs, "Tcs" );
    _LIT( KInclude, "Include" );
    _LIT( KExclude, "Exclude" );
    _LIT( KDebug, "Debug" );
    _LIT( KOn, "ON" );
    _LIT( KOff, "OFF" );
    _LIT( KSeparateLog, "SeparateLog" );
    _LIT( KPipe, "Pipe" );
    _LIT( KScript, "Script" );
    _LIT( KTcsData, "TcsData" );
	
	GetConfigFileName( iCfgFileName );
    WriteLineL( iCfgFileName, KSectionBegin );
    
    // write tcs flag. 
    if ( iIsTci || iIsTcx )
        {
        WriteLineL( iCfgFileName, KTcs, iIsTci?KInclude:KExclude );
        }
    
    // write debug flag. 
    if( iIsDebug )
    	{
    	WriteLineL( iCfgFileName, KDebug, KOn );
    	}
    else 
    	{
    	WriteLineL( iCfgFileName, KDebug, KOff );
    	}
    
    // write slf flag. 
    if( iIsSlf )
    	{
    	WriteLineL( iCfgFileName, KSeparateLog, KOn );
    	}
    else 
    	{
    	WriteLineL( iCfgFileName, KSeparateLog, KOff );
    	}
    
    // write Pipe=OFF. 	// handle pipe section. 
    WriteLineL( iCfgFileName, KPipe, KOff );
    
    // write script file. 
    WriteLineL( iCfgFileName, KScript, iScriptFileName );
    
    // write tcs
    if ( iIsTci || iIsTcx )
        {
        WriteLineL( iCfgFileName, KTcsData, iIsTci?iTciParam:iTcxParam );
        }
    
    // write Section End. 
    WriteLineL( iCfgFileName, KSectionEnd );
}

void CTefWrapperApplication::ExecuteL()
	{
	_LIT( KAtsInterface, "atsinterface.exe" );
	_LIT( KAtsInterfaceArgs, "-testmodule TEFTestModule -config " );
		
	TBuf<KFileNameLength> strArgs;
	strArgs.Copy( KAtsInterfaceArgs );
	strArgs.Append( iCfgFileName );
	
	RProcess proc;
	proc.Create( KAtsInterface, strArgs );
	TRequestStatus status;
	proc.Logon( status );
	proc.Resume();
	
	//iAppLauncher->ExecuteL( KAtsInterface, strArgs );
	//User::WaitForRequest( iAppLauncher->iStatus );
	
	User::WaitForRequest( status );
	proc.Close();
    if ( KErrNone != status.Int() )
        {
        console->Printf( _L("Launch TEF Test Module failed.\n") );
        User::LeaveIfError( status.Int() );
        }
	}

void CTefWrapperApplication::DeleteConfigFile()
	{
	if ( Utils::FileExists( iFs, iCfgFileName ) )
        {
        Utils::DeleteFile( iFs, iCfgFileName );
        }
	}
