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

#ifndef SYMBIANUNITTESTMACROS_H
#define SYMBIANUNITTESTMACROS_H

// INCLUDES
#include <e32std.h>

// MACROS
#ifdef SYMBIAN_UNIT_TEST

    /** 
    * Calls the base class constructor that sets the name of unit test.
    */
    #define BASE_CONSTRUCT\
        CSymbianUnitTest::ConstructL( _L8( __PRETTY_FUNCTION__ ) );

    /**
    * Adds a new unit test case to this unit test.
    * The default setup and teardown functions will be used.
    * @param aTestPtr a function pointer to the unit test case
    */
    #define ADD_SUT( aTestPtr )\
        AddTestCaseL(\
            _L( #aTestPtr ),\
            CSymbianUnitTest::FunctionPtr( SetupL ),\
            CSymbianUnitTest::FunctionPtr( aTestPtr ),\
            CSymbianUnitTest::FunctionPtr( Teardown ) );

    /**
    * Adds a new unit test case to this unit test.
    * The user can specify
    * @param aSetupPtr a function pointer to the setup function 
    *        that will be executed before the actual unit test case
    * @param aTestPtr a function pointer to the unit test case
    * @param aTeardownPtr a function pointer to the teardown function
    *        that will be executed after the actual unit test case
    */
    #define ADD_SUT_WITH_SETUP_AND_TEARDOWN( aSetupPtr, aTestPtr, aTeardownPtr )\
        AddTestCaseL(\
            _L( #aTestPtr ),\
            CSymbianUnitTest::FunctionPtr( aSetupPtr ),\
            CSymbianUnitTest::FunctionPtr( aTestPtr ),\
            CSymbianUnitTest::FunctionPtr( aTeardownPtr ) );

    /**
    * Asserts a condition in a unit test case.
    * Leaves with a Symbian unit test framework specific error code
    * if the condition evaluates to EFalse.
    * In case of a failed assertion, the framework records 
    * the failure reason, line number and file name to the test results.
    * @param aCondition the asserted condition.
    */
    #define SUT_ASSERT( aCondition )\
        if ( !TBool( aCondition ) )\
            {\
            AssertionFailedL( _L8( #aCondition ), __LINE__, _L8( __FILE__ ) );\
            }

    /**
    * Asserts that two values are equal.
    * Leaves with a Symbian unit test framework specific error code
    * if the values are not equal.
    * In case of a failed assertion, the framework records 
    * the failure reason, line number and file name to the test results.
    * @param aExpectedValue the expected value
    * @param aActualValue the actual value
    */
    #define SUT_ASSERT_EQUALS( aExpected, aActual )\
        AssertEqualsL( aExpected, aActual, __LINE__, _L8( __FILE__ ) );

    /**
    * Asserts that a statement leaves an expected value.
    * Leaves with a Symbian unit test framework specific error code
    * if the leave code is not the expected one.
    * In case of a failed assertion, the framework records 
    * the failure reason, line number and file name to the test results.
    * @param aStatement the statement
    * @param aError the expected leave code
    */
    #define SUT_ASSERT_LEAVE_WITH( aStatement, aError )\
        {\
        TInt KLine( __LINE__ );\
        TRAPD( err, aStatement )\
        AssertLeaveL( _L8( #aStatement ), err, aError, KLine, _L8( __FILE__ ) );\
        }

    /**
    * Asserts that a statement leaves.
    * The macro itself leaves with a Symbian unit test framework 
    * specific error code if the statement leaves.
    * In case of a failed assertion, the framework records 
    * the failure reason, the line number and file name to the test results.
    *
    * Note: SUT_ASSERT_LEAVE_WITH should be used instead 
    * whenever possible, because the implementation of 
    * SYMBIAN_UT_ASSERT_LEAVE TRAPs also KErrNoMemory.
    * This means that all the memory allocations are not looped through 
    * during the memory allocation failure simulation.
    * @param aStatement the statement
    */
    #define SUT_ASSERT_LEAVE( aStatement )\
        {\
        TInt KLine( __LINE__ );\
        TBool leaveOccured( ETrue );\
        TRAPD( err, aStatement; leaveOccured = EFalse; )\
        if ( !leaveOccured )\
            {\
            RecordNoLeaveFromStatementL( _L8( #aStatement ), KLine, _L8( __FILE__ ) );\
            }\
        }

    /**
    * Can be used to hide the friend declaration for a unit test class.
    */
    #define SYMBIAN_UNIT_TEST_CLASS( ClassName ) friend class ClassName;
#else 
    #define SYMBIAN_UNIT_TEST_CLASS( ClassName )
#endif // SYMBIAN_UNIT_TEST

#endif // SYMBIANUNITTESTMACROS_H
