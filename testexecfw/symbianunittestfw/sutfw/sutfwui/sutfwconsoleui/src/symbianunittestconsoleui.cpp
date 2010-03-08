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

#include "symbianunittestconsoleui.h"
#include "symbianunittestcommandlineparser.h"
#include "symbianunittestrunner.h"
#include "symbianunittestversion.h"

_LIT( KTitleTxt, "\n==== SymbianUnit v%d.%d.%d ====\n\n" );
_LIT( KAllocOnTxt, "  Alloc: ON\n\n" );
_LIT( KAllocOffTxt, "  Alloc: OFF\n\n" );
_LIT( KExecutingTestsTxt, "  Executing...\n" );
_LIT( KTestRunnerFailedTxt, "  Test run failed!\n  Reason: %d\n" );
_LIT( KNoTestsFoundTxt, "  No tests found!\n" );
_LIT( KTestsExecutedTxt, "  Executed: %d / %d\n" );
_LIT( KPassedTestsTxt, "  Passed: %d\n" );
_LIT( KFailedTestsTxt, "  Failed: %d\n" );
_LIT( KSymbianUnitTestPanic, "SymbianUnit creation" );
_LIT( KWindowName, "SymbianUnit" );
_LIT( KPressAnyKeyTxt, "\n==[ press any key ]==\n    " );

_LIT( KHelpTxt, 
"Allowed arguments:\n\n\
-tests|t=<dll,dll,...>\n\
-cases|c=<case,case,...>\n\
-alloc|a\n\
-help|h\n\
-output|o=<html|xml|txt>\n\
-timeout|to\n\
-noprompt\n" );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
GLDEF_C TInt E32Main() 
    {
    CTrapCleanup* cleanup = CTrapCleanup::New();
    TRAPD( err, MainL() );
    __ASSERT_ALWAYS( 
        err == KErrNone, User::Panic( KSymbianUnitTestPanic, err ) );
    delete cleanup;
    User::Heap().Reset();
    return err;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LOCAL_C void MainL() 
    {
    RThread().SetPriority( EPriorityAbsoluteForeground );    
    // install an active scheduler
    CActiveScheduler* scheduler = new( ELeave )CActiveScheduler;
    CActiveScheduler::Install( scheduler );
    CleanupStack::PushL( scheduler );

    TSize size( KConsFullScreen, KConsFullScreen );
    CConsoleBase* console = Console::NewL( KWindowName, size );
    CleanupStack::Pop( scheduler );

    CSymbianUnitTestConsoleUi* main = NULL;
    TRAPD( err, main = CSymbianUnitTestConsoleUi::NewL( *console ) );
    if ( err == KErrNone ) 
        {
        CActiveScheduler::Start();
        }
    delete main;
    delete scheduler;
    // Do not delete console. It will check for memory leaks.
    // This is not what is wanted if running tests without 
    // memory leak detection.
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSymbianUnitTestConsoleUi* CSymbianUnitTestConsoleUi::NewLC( 
    CConsoleBase& aConsole )
    {
    CSymbianUnitTestConsoleUi* self = 
        new( ELeave )CSymbianUnitTestConsoleUi( aConsole );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSymbianUnitTestConsoleUi* CSymbianUnitTestConsoleUi::NewL( 
    CConsoleBase& aConsole )
    {
    CSymbianUnitTestConsoleUi* self = 
        CSymbianUnitTestConsoleUi::NewLC( aConsole );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSymbianUnitTestConsoleUi::CSymbianUnitTestConsoleUi( CConsoleBase& aConsole )
 : CActive( EPriorityStandard ),
   iConsole( aConsole )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::ConstructL()
    {
    User::SetJustInTime( EFalse ); // Do not stop on test case panics
    iCommandLineParser = CSymbianUnitTestCommandLineParser::NewL();
    iTestRunner = CSymbianUnitTestRunner::NewL( *this );
    CActiveScheduler::Add( this );
    // Set ready to run immediately
    TRequestStatus *status = &iStatus;
    User::RequestComplete( status, KErrNone );
    SetActive ();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSymbianUnitTestConsoleUi::~CSymbianUnitTestConsoleUi()
    {
    Cancel();
    delete iTestRunner;
    delete iCommandLineParser;
    }

// -----------------------------------------------------------------------------
// From CActive
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::RunL()
    {
    iConsole.Printf( KTitleTxt, SUT_MAJOR_VERSION, SUT_MINOR_VERSION, SUT_BUILD_VERSION);
    if ( iCommandLineParser->ShowHelp() )
        {
        iConsole.Printf( KHelpTxt );
        }
    else
        {
        PrintAllocFailureSimulationText();
        iConsole.Printf( KExecutingTestsTxt );
        TRAPD( err, 
            iTestRunner->ExecuteTestsL( 
                iCommandLineParser->TestDllNames(),
                iCommandLineParser->MemoryAllocationFailureSimulation(),
                iCommandLineParser->OutputFileName(),
                iCommandLineParser->OutputFormat(),
	        iCommandLineParser->TestCaseNames(),
		iCommandLineParser->Timeout()) )
        if ( err != KErrNone )
            {
            iConsole.Printf( KTestRunnerFailedTxt, err );
            }
        else 
            {
            if ( iExecutedTestCount == 0 )
                {
                iConsole.Printf( KNoTestsFoundTxt );
                }
            }
        }
    if ( iCommandLineParser->PromptUser() )
        {
        iConsole.Printf( KPressAnyKeyTxt );
        iConsole.Getch(); // get and ignore character        
        }
    CActiveScheduler::Stop();
    }

// -----------------------------------------------------------------------------
// From CActive
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::DoCancel()
    {
    }

// -----------------------------------------------------------------------------
// From MSymbianUnitTestUiCallBack
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::InfoMsg( const TDesC& aMessage )
    {
    iConsole.Printf( aMessage );
    }

// -----------------------------------------------------------------------------
// From MSymbianUnitTestUiCallBack
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::InfoMsg( 
    const TDesC& aFormat, 
    const TDesC& aMessage )
    {
    iConsole.Printf( aFormat, &aMessage );
    }

// -----------------------------------------------------------------------------
// From MSymbianUnitTestUiCallBack
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::IncrementExecutedTestsCount()
    {
    TPoint pos = iConsole.CursorPos();
    pos.iY -= 1;
    if ( iExecutedTestCount > 0 )
        {
        pos.iY -= 2;
        }
    iConsole.SetCursorPosAbs( pos );
    iExecutedTestCount++;
    iConsole.Printf( 
        KTestsExecutedTxt, iExecutedTestCount, iTestRunner->TestCount());
    TInt passedTestCount = iExecutedTestCount - iTestRunner->FailedTestCount();
    iConsole.Printf( KPassedTestsTxt, passedTestCount );
    iConsole.Printf( KFailedTestsTxt, iTestRunner->FailedTestCount() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestConsoleUi::PrintAllocFailureSimulationText()
    {
    if ( iCommandLineParser->MemoryAllocationFailureSimulation() )
        {
        iConsole.Printf( KAllocOnTxt );
        }
    else
        {
        iConsole.Printf( KAllocOffTxt );
        }
    }
