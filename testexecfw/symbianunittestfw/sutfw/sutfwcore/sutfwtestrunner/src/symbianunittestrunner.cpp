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

#include "symbianunittestrunner.h"
#include "symbianunittestuicallback.h"
#include "symbianunittestoutputformatter.h"
#include "symbianunittestoutputfactory.h"
#include "symbianunittestresult.h"
#include "symbianunittestversion.h"
#include "sutlogger.h"
#include <symbianunittestinterface.h>
#include <e32uid.h>


// Failures while loading dll:
_LIT( KFailedToFindDll, "Failed to find DLL:\n\"%S\"\n" );
_LIT( KNonCompatibleUIDs, "Cannot use DLL with non-compatible UIDs!\n" );
_LIT( KExportFuncNotFound, "Cannot find EXPORT function from test DLL!\n" );
_LIT( KLogVersion, "SymbianUnitTest v%d.%d.%d" );
_LIT( KLogFinish, "SymbianUnitTest finished" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CSymbianUnitTestRunner* CSymbianUnitTestRunner::NewLC( 
    MSymbianUnitTestUiCallBack& aUiCallBack )
    {
    CSymbianUnitTestRunner* self = 
        new( ELeave )CSymbianUnitTestRunner( aUiCallBack );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CSymbianUnitTestRunner* CSymbianUnitTestRunner::NewL( 
    MSymbianUnitTestUiCallBack& aUiCallBack )
    {
    CSymbianUnitTestRunner* self = CSymbianUnitTestRunner::NewLC( aUiCallBack );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CSymbianUnitTestRunner::CSymbianUnitTestRunner( 
    MSymbianUnitTestUiCallBack& aUiCallBack )
    : iUiCallBack( aUiCallBack )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestRunner::ConstructL()
    {
    iResult = CSymbianUnitTestResult::NewL();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CSymbianUnitTestRunner::~CSymbianUnitTestRunner()
    {
    delete iResult;
    }

// -----------------------------------------------------------------------------
// From MSymbianUnitTestObserver
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestRunner::IncrementExecutedTestsCount()
    {
    iUiCallBack.IncrementExecutedTestsCount();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSymbianUnitTestRunner::TestCount()
    {
    return iTestCount;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CSymbianUnitTestRunner::ExecuteTestsL(
    const MDesCArray& aTestDllNames,
    TBool aMemoryAllocationFailureSimulation,
    const TDesC& aOutputFileName,
    const TDesC& aOutputFormat,
    const CDesCArray& aTestCaseNames,
    TInt  aTimeout )
    {
    //init logger
    TBuf<50> version;
    version.Format(KLogVersion, SUT_MAJOR_VERSION, SUT_MINOR_VERSION, SUT_BUILD_VERSION);
    SUT_LOG_START(version);
    
    iTestCount = 0;
    
    MSymbianUnitTestInterface::TFailureSimulation failureSimulation = 
        MSymbianUnitTestInterface::ENoFailureSimulation;
    if ( aMemoryAllocationFailureSimulation )
        {
        failureSimulation = 
            MSymbianUnitTestInterface::EMemAllocFailureSimulation;
        }
    
    for ( TInt i = 0; i < aTestDllNames.MdcaCount(); i++ )
        {
        TPtrC16 testDllName( aTestDllNames.MdcaPoint( i ) );
        RLibrary library;
        if ( library.Load( testDllName ) != KErrNone )
            {
            iUiCallBack.InfoMsg( KFailedToFindDll, testDllName );
	    SUT_LOG_FORMAT(KFailedToFindDll, &testDllName);
            User::Leave( KErrNotFound );
            } 
        CleanupClosePushL( library );
        // The second UID of the dll to be used must be compatible
        if ( library.Type()[ 1 ] != KSymbianUnitTestDllUid )
            {
            iUiCallBack.InfoMsg( KNonCompatibleUIDs );
            User::Leave( KErrNotFound );
            }  
        TLibraryFunction entryFunction = library.Lookup( 1 );
        if ( !entryFunction )
            {
            iUiCallBack.InfoMsg( KExportFuncNotFound );
            User::Leave( KErrNotFound );
            }
        
        MSymbianUnitTestInterface* test = 
            reinterpret_cast< MSymbianUnitTestInterface* >( 
                entryFunction() );
        TCleanupItem cleanupItem( DeleteTest, test );
        CleanupStack::PushL( cleanupItem );
        iTestCount += test->TestCaseCount();
        test->ExecuteL( *this, *iResult, failureSimulation, aTestCaseNames, aTimeout);
        CleanupStack::PopAndDestroy(); // cleanupItem
        
        CleanupStack::PopAndDestroy( &library ); 
        }
    
    CSymbianUnitTestOutputFormatter* outputFormatter = 
        SymbianUnitTestOutputFactory::CreateOutputLC( 
            aOutputFileName, aOutputFormat );
    outputFormatter->PrintL( *iResult );
    CleanupStack::PopAndDestroy( outputFormatter );    
    SUT_LOG_INFO(KLogFinish);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CSymbianUnitTestRunner::FailedTestCount()
    {
    return iResult->Failures().Count();
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CSymbianUnitTestRunner::DeleteTest( TAny* aTest )
    {
    MSymbianUnitTestInterface* test = 
        reinterpret_cast< MSymbianUnitTestInterface* >( aTest );
    delete test;
    }
