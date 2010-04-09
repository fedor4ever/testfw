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
* Description: This file contains testclass implementation.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "TestFileStore.h"

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestFileStore::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CTestFileStore::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CTestFileStore::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestFileStore::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CTestFileStore::ExampleL ),
        ENTRY( "PanicExample", CTestFileStore::PanicExample ),
        ENTRY( "CreateFile", CTestFileStore::TestFileServerCreateFile ),
        ENTRY( "WriteFile", CTestFileStore::TestFileServerWriteFile ),   
        ENTRY( "ReadFile", CTestFileStore::TestFileServerReadFile )
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CTestFileStore::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CTestFileStore::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KTestFileStore, "TestFileStore" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KTestFileStore, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KTestFileStore, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }


TInt CTestFileStore::TestFileServerCreateFile( CStifItemParser& aItem )
    {
    // Print to UI
    _LIT( KSTIF_TestFileServer, "STIF_TestFileServer" );
    _LIT( KExample, "Create File" );
    TestModuleIf().Printf( 0, KSTIF_TestFileServer, KExample );
    // Print to log file
    iLog->Log( KExample );  
    
    TInt                retValue = KErrNone ;
    TInt                expErrorCode = 0;   
    TPtrC filename;

    TInt erro = aItem.GetNextString ( filename );
    if( erro == KErrNone )
        {
        iLog->Log( filename );
        RFs fileSession;
        TRAP(erro, fileSession.Connect())
        if ( erro != KErrNone)
            {
            iLog->Log(_L("Failed to connect to the file server"));
            retValue = erro;
            }
        else
            {
            RFile file;
            
            CleanupClosePushL(fileSession);
            
            erro = file.Create( fileSession, filename, EFileRead | EFileWrite );
            if(erro==expErrorCode)
                {
                iLog->Log(_L("===>Opening file passed with error code %D==="), erro);
                }
            else
                {
                iLog->Log(_L("--->Error in opening file expected error code %D error returned %D"), expErrorCode, erro);
                }
            CleanupStack::PopAndDestroy(&fileSession);
            retValue = erro;
            }       
        }

    return retValue;
    }


TInt CTestFileStore::TestFileServerWriteFile( CStifItemParser& aItem )
    {
    // Print to UI
    _LIT( KSTIF_TestFileServer, "STIF_TestFileServer" );
    _LIT( KExample, "Open File" );
    TestModuleIf().Printf( 0, KSTIF_TestFileServer, KExample );
    // Print to log file
    iLog->Log( KExample );  
    
    TInt                retValue = KErrNone ;
    TInt                expErrorCode = 0;   
    TPtrC filename;

    TInt erro = aItem.GetNextString ( filename );
    if( erro == KErrNone )
        {
        iLog->Log( filename );
        RFs fileSession;
        TRAP(erro, fileSession.Connect())
        if ( erro != KErrNone)
            {
            iLog->Log(_L("Failed to connect to the file server"));
            }
        else
            {
            RFile file;
            
            CleanupClosePushL(fileSession);
            erro = file.Open(fileSession,filename,EFileRead);
            if(erro==expErrorCode)
                {
                iLog->Log(_L("===>Opening file passed with error code %D==="), erro);
                }
            else
                {
                iLog->Log(_L("--->Error in opening file expected error code %D error returned %D"), expErrorCode, erro);
                }
            CleanupStack::PopAndDestroy(&fileSession);
            }  
            retValue = erro;
        }
    
    return retValue;
    }

TInt CTestFileStore::TestFileServerReadFile( CStifItemParser& aItem )
    {
    // Print to UI
    _LIT( KSTIF_TestFileServer, "STIF_TestFileServer" );
    _LIT( KExample, "Open File" );
    TestModuleIf().Printf( 0, KSTIF_TestFileServer, KExample );
    // Print to log file
    iLog->Log( KExample );  
    
    TInt                retValue = KErrNone ;
    TInt                expErrorCode = 0;   
    TPtrC filename;

    TInt erro = aItem.GetNextString ( filename );
    if( erro == KErrNone )
        {
        iLog->Log( filename );
        RFs fileSession;
        TRAP(erro, fileSession.Connect())
        if ( erro != KErrNone)
            {
            iLog->Log(_L("Failed to connect to the file server"));
            }
        else
            {
            RFile file;
            
            CleanupClosePushL(fileSession);
            erro = file.Open(fileSession,filename,EFileRead);
            if(erro==expErrorCode)
                {
                iLog->Log(_L("===>Opening file passed with error code %D==="), erro);
                }
            else
                {
                iLog->Log(_L("--->Error in opening file expected error code %D error returned %D"), expErrorCode, erro);
                }
            CleanupStack::PopAndDestroy(&fileSession);
            }  
            retValue = erro;
        }
    
    return retValue;
    }


TInt CTestFileStore::PanicExample(CStifItemParser& aItem)
    {
    _LIT( KSTIF_TestFileServer, "STIF_TestFileServer" );
    _LIT( KFunction, "PanicExample" );
    TestModuleIf().Printf( 0, KSTIF_TestFileServer, KFunction );
    iLog->Log( KFunction );
    TInt expectedPanicCode = 0;
    TPtrC expectedPanicString;
    TInt err1 = aItem.GetNextString(expectedPanicString);
    TInt err2 = aItem.GetNextInt(expectedPanicCode);
    if ( err1 != KErrNone || err2 != KErrNone)
         {
         iLog->Log(_L("Failed to read expected panic code"));
         User::Panic(_L("USER-EXEC"), 1);
         }
     else
         {
         iLog->Log( _L("Create expected panic number= %D"),  expectedPanicCode);  
         iLog->Log( _L("Create expected panic String= %S"),  &expectedPanicString);  
         User::Panic(expectedPanicString, expectedPanicCode);           
         }
    return KErrNone;
    }
