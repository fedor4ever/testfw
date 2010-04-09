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



#ifndef TTEFWRAPPERAPPLICATION_H_
#define TTEFWRAPPERAPPLICATION_H_

#include <e32std.h>
#include <f32file.h>

const TInt KFileNameLength( 255 );

//class CAppLauncher;
class CConsoleBase;

class CTefWrapperApplication : public CBase
{
public:
	static CTefWrapperApplication* NewL();
	static CTefWrapperApplication* NewLC();
	~CTefWrapperApplication();

public:
	void ParseCommandLineL();
	void WriteConfigFileL();
	void ExecuteL();
	void DeleteConfigFile();

private:
	CTefWrapperApplication();
	void ConstructL();
	
private:	
	TBool IsScriptFile( const TDesC& );
	void WriteLineL( const TDesC&, const TDesC& );
	void WriteLineL( const TDesC&, const TDesC&, const TDesC& );
	void GetConfigFileName( TDes& );
	void PrintHelpMessage();

private:
	TBool iIsDebug;
	TBool iIsSlf;
	TBool iIsTcx;
	TBool iIsTci;
	TBool iHasScript;
	TBool iIsTcsPair;
	
	CConsoleBase* iConsole;
	
	TBuf<KFileNameLength> iTciParam;
	TBuf<KFileNameLength> iTcxParam;
	TBuf<KFileNameLength> iScriptFileName;
	TBuf<KFileNameLength> iCfgFileName;
	
private:
	RFs iFs;
	//RFile iFile;
//	CAppLauncher* iAppLauncher;
};

#endif	// TTEFWRAPPERAPPLICATION_H_
