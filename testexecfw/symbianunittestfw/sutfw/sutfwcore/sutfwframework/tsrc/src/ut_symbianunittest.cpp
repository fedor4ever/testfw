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

#include "ut_symbianunittest.h"
#include "symbianunittestresult.h"
#include <symbianunittestmacros.h>

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTest* UT_CSymbianUnitTest::NewL()
    {
    UT_CSymbianUnitTest* self = UT_CSymbianUnitTest::NewLC(); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTest* UT_CSymbianUnitTest::NewLC()
    {
    UT_CSymbianUnitTest* self = new( ELeave )UT_CSymbianUnitTest();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTest::UT_CSymbianUnitTest()
    {
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::ConstructL()
    {
    BASE_CONSTRUCT
    ADD_SUT_WITH_SETUP_AND_TEARDOWN( SetupL, UT_NameL, Teardown )
    ADD_SUT( UT_IsMemoryAllocationFailureSimulationUsedL )
    ADD_SUT( UT_PanicInTestL )
    ADD_SUT( UT_ErrorInSetupL )
    ADD_SUT( UT_LeaveInTestL )
    ADD_SUT( UT_MemoryLeakInTestL )
    ADD_SUT( UT_FailingAssertsL )
    ADD_SUT( UT_PassingAssertsL )
    ADD_SUT( UT_TestCaseNamesL )
    ADD_SUT( UT_TimeoutL )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTest::~UT_CSymbianUnitTest()
    {   
    }

// ---------------------------------------------------------------------------
// From MSymbianUnitTestObserver
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::IncrementExecutedTestsCount()
    {
    iExecutedTestCount++;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SetupL()
    {
    iTestResult = CSymbianUnitTestResult::NewL();
    iUnitTest = UT_CSymbianUnitTest::NewL();
    // Remove the test cases added in ConstructL. 
    // They will be replaced by the functions simulating failures. 
    iUnitTest->iTestCases.ResetAndDestroy();
    iTestCaseNames = new (ELeave)CDesCArrayFlat(1);
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::Teardown()
    {
    delete iUnitTest;
    iUnitTest = NULL;
    delete iTestResult;
    iTestResult = NULL;
    iExecutedTestCount = 0;
    delete iTestCaseNames;
    iTestCaseNames = 0;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_NameL()
    {
    SUT_ASSERT_EQUALS( _L( "UT_CSymbianUnitTest" ), iUnitTest->Name() )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_IsMemoryAllocationFailureSimulationUsedL()
    {
    SUT_ASSERT( !iUnitTest->IsMemoryAllocationFailureSimulationUsed() )
    
    iUnitTest->iAllocFailureType = RHeap::EDeterministic;
    SUT_ASSERT( iUnitTest->IsMemoryAllocationFailureSimulationUsed() )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_PanicInTestL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
 
    _LIT( KTestName, "PanicInTest" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulatePanicL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
    
    // With memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, EMemAllocFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 2, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 2, iExecutedTestCount ) 
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_ErrorInSetupL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
 
    _LIT( KTestName, "ErrorInSetup" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( SimulateLeaveL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
    
    // With memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, EMemAllocFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 2, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 2, iExecutedTestCount ) 
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_LeaveInTestL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
 
    _LIT( KTestName, "LeaveInTest" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateLeaveL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0);
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
    
    // With memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, EMemAllocFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 2, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 2, iExecutedTestCount )    
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_MemoryLeakInTestL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
    
    _LIT( KTestName, "MemoryLeakInTest" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateMemoryLeakL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
  
    // With memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, EMemAllocFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 2, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 2, iExecutedTestCount )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_FailingAssertsL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
    
    _LIT( KTestName, "TestAssertFailures" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertWithIntsL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) );   
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertWithDesC8sL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) );    
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertWithDesC16sL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) ); 
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertLeaveL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateFailingAssertLeaveWithSpecificValueL ),
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 6, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 6, iExecutedTestCount )
    
    // With memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, EMemAllocFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 12, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 12, iExecutedTestCount )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_PassingAssertsL()
    {
    SUT_ASSERT( ETrue )
    SUT_ASSERT( ETrue == ETrue )
    SUT_ASSERT( 1 == 1 )
    HBufC* ptr( NULL );
    SUT_ASSERT( !ptr )
    ptr = KNullDesC().AllocLC();
    SUT_ASSERT( ptr )
    CleanupStack::PopAndDestroy( ptr );
    
    SUT_ASSERT_EQUALS( 1, 1 )
    SUT_ASSERT_EQUALS( KNullDesC8, KNullDesC8 )
    SUT_ASSERT_EQUALS( KNullDesC, KNullDesC )
    
    SUT_ASSERT_LEAVE( User::Leave( KErrNone ) )
    SUT_ASSERT_LEAVE( User::Leave( KErrGeneral ) )
    
    SUT_ASSERT_LEAVE_WITH( User::Leave( KErrGeneral ), KErrGeneral )
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_TestCaseNamesL()
    {
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 0, iExecutedTestCount )
 
    _LIT( KTest1Name, "TestCase1" );
    iUnitTest->AddTestCaseL( 
        KTest1Name, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateLeaveL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );

    _LIT( KTest2Name, "TestCase2" );
    iUnitTest->AddTestCaseL( 
        KTest2Name, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateLeaveL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );

    _LIT( KTest3Name, "TestCase3" );
    iUnitTest->AddTestCaseL( 
        KTest3Name, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateLeaveL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );

    _LIT( LTest1FullName, "UT_CSymbianUnitTest::TestCase1");
    iTestCaseNames->AppendL(LTest1FullName);
    
    // Without memory allocation failure simulation
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::UT_TimeoutL()
    {
    _LIT( KTestName, "TimeoutTestCase" );
    iUnitTest->AddTestCaseL( 
        KTestName, 
        CSymbianUnitTest::FunctionPtr( Dummy ), 
        CSymbianUnitTest::FunctionPtr( SimulateTimeoutL ), 
        CSymbianUnitTest::FunctionPtr( Dummy ) );
    //run test case without timeout control
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 0 );
    SUT_ASSERT_EQUALS( 0, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 1, iExecutedTestCount )
    //run test case with 3 seconds time out
    iUnitTest->ExecuteL( *this, *iTestResult, ENoFailureSimulation, *iTestCaseNames, 3 );
    SUT_ASSERT_EQUALS( 1, iTestResult->Failures().Count() )
    SUT_ASSERT_EQUALS( 2, iExecutedTestCount )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateTimeoutL()
    {
    User::After(5* 1000000);
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulatePanicL()
    {
    HBufC* tmp( NULL );
    tmp->Length();
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateLeaveL()
    {
    User::Leave( KErrGeneral );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateMemoryLeakL()
    {
    HBufC::NewL( 1 );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertL()
    {
    SUT_ASSERT( EFalse )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertWithIntsL()
    {
    const TInt KFirstValue( -1 );
    const TInt KSecondValue( -2 );
    SUT_ASSERT_EQUALS( KFirstValue, KSecondValue )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertWithDesC8sL()
    {
    _LIT8( KSomeText, "some text" );
    SUT_ASSERT_EQUALS( KNullDesC8, KSomeText )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertWithDesC16sL()
    {
    _LIT( KSomeText, "some text" );
    SUT_ASSERT_EQUALS( KNullDesC, KSomeText )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertLeaveL()
    {
    SUT_ASSERT_LEAVE( Dummy() )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::SimulateFailingAssertLeaveWithSpecificValueL()
    {
    SUT_ASSERT_LEAVE_WITH( User::Leave( KErrNotFound ), KErrGeneral )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTest::Dummy()
    {
    }
