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

#include "ut_symbianunittestcommandlineparser.h"
#include "symbianunittestcommandlineparser.h"
#include <symbianunittestmacros.h>

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTestCommandLineParser* UT_CSymbianUnitTestCommandLineParser::NewL()
    {
    UT_CSymbianUnitTestCommandLineParser* self = 
        UT_CSymbianUnitTestCommandLineParser::NewLC(); 
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTestCommandLineParser* UT_CSymbianUnitTestCommandLineParser::NewLC()
    {
    UT_CSymbianUnitTestCommandLineParser* self = 
        new( ELeave )UT_CSymbianUnitTestCommandLineParser();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTestCommandLineParser::UT_CSymbianUnitTestCommandLineParser()
    {
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::ConstructL()
    {
    BASE_CONSTRUCT
    
    ADD_SUT( UT_SetOutputFormatL )
    ADD_SUT( UT_SetTestDllNamesL )
    ADD_SUT( UT_FindArgumentL )
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
UT_CSymbianUnitTestCommandLineParser::~UT_CSymbianUnitTestCommandLineParser()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::SetupL()
    {
    iParser = CSymbianUnitTestCommandLineParser::NewL();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::Teardown()
    {
    delete iParser;
    iParser = NULL; 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::UT_SetOutputFormatL()
    {
    // Output format not found
    _LIT( KDefaultOutputFormat, "html" );
    iParser->SetOutputFormatL();
    SUT_ASSERT_EQUALS( KDefaultOutputFormat, iParser->OutputFormat() )
    
    // Output format found, long form
    _LIT( KOutputFormatLong, "-output=xml" );
    iParser->iArguments->Args().AppendL( KOutputFormatLong );
    iParser->SetOutputFormatL();
    SUT_ASSERT_EQUALS( _L( "xml" ), iParser->OutputFormat() )

    // Output format found, short form
    _LIT( KOutputFormatShort, "-o=txt" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KOutputFormatShort );
    iParser->SetOutputFormatL();
    SUT_ASSERT_EQUALS( _L( "txt" ), iParser->OutputFormat() ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::UT_SetTestDllNamesL()
    {
    // Test DLL name not found
    iParser->SetTestDllNamesL();
    SUT_ASSERT_EQUALS( 0, iParser->TestDllNames().MdcaCount() )
    SUT_ASSERT( iParser->ShowHelp() )
    
    // One test DLL name found
    _LIT( KOneTestDllName, "-tests=test1.dll" );
    iParser->iArguments->Args().AppendL( KOneTestDllName );
    iParser->SetTestDllNamesL();
    SUT_ASSERT_EQUALS( 1, iParser->TestDllNames().MdcaCount() )
    SUT_ASSERT_EQUALS( 
        _L( "test1.dll" ), iParser->TestDllNames().MdcaPoint( 0 ) )
    SUT_ASSERT( !iParser->ShowHelp() )
    
    // Two test DLL names found
    _LIT( KTwoTestDllNames, "-tests=test1.dll,test2.dll" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KTwoTestDllNames );
    iParser->SetTestDllNamesL();
    SUT_ASSERT_EQUALS( 2, iParser->TestDllNames().MdcaCount() )
    SUT_ASSERT_EQUALS( 
        _L( "test1.dll" ), iParser->TestDllNames().MdcaPoint( 0 ) )
    SUT_ASSERT_EQUALS( 
        _L( "test2.dll" ), iParser->TestDllNames().MdcaPoint( 1 ) )
    SUT_ASSERT( !iParser->ShowHelp() )
    
    // Two test DLL names found, short key name used
    _LIT( KTwoTestDllNamesWithShortForm, "-t=test3.dll,test4.dll" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KTwoTestDllNamesWithShortForm );
    iParser->SetTestDllNamesL();
    SUT_ASSERT_EQUALS( 2, iParser->TestDllNames().MdcaCount() )
    SUT_ASSERT_EQUALS( 
        _L( "test3.dll" ), iParser->TestDllNames().MdcaPoint( 0 ) )
    SUT_ASSERT_EQUALS( 
        _L( "test4.dll" ), iParser->TestDllNames().MdcaPoint( 1 ) )
    SUT_ASSERT( !iParser->ShowHelp() )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CSymbianUnitTestCommandLineParser::UT_FindArgumentL()
    {
    _LIT( KKey, "key" );
    TPtrC value;
    
    // Argument not found
    SUT_ASSERT( !iParser->FindArgument( KKey, KNullDesC, value ) )
    SUT_ASSERT_EQUALS( KNullDesC, value )
    
    // Argument found, no equals sign
    _LIT( KOnlyKeyArgument, "key" );
    iParser->iArguments->Args().AppendL( KOnlyKeyArgument );
    SUT_ASSERT( iParser->FindArgument( KKey, KNullDesC, value ) )
    SUT_ASSERT_EQUALS( KNullDesC, value )    
    
    // Argument found, equals sign as the first character
    _LIT( KKeyStartingWithEqualsSign, "=key" );
    _LIT( KKeyStartingWithEqualsSignArgument, "=keysomething" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KKeyStartingWithEqualsSignArgument );
    SUT_ASSERT( 
        iParser->FindArgument( KKeyStartingWithEqualsSign, KNullDesC, value ) )
    SUT_ASSERT_EQUALS( KNullDesC, value )    
    
    // Argument found, equals sign as the last character
    _LIT( KKeyEndingWithEqualsSignArgument, "key=" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KKeyEndingWithEqualsSignArgument );
    SUT_ASSERT( iParser->FindArgument( KKey, KNullDesC, value ) )
    SUT_ASSERT_EQUALS( KNullDesC, value )    
    
    // Argument found, equals sign as not the first and not the last
    _LIT( KValidArgument, "key=value" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KValidArgument );
    SUT_ASSERT( iParser->FindArgument( KKey, KNullDesC, value ) )
    SUT_ASSERT_EQUALS( _L( "value" ), value )
    
    // Short form of the argument found
    _LIT( KValidArgument2, "k=value2" );
    iParser->iArguments->Args().Reset();
    iParser->iArguments->Args().AppendL( KValidArgument2 );
    _LIT( KShortKey, "k" );
    SUT_ASSERT( iParser->FindArgument( KNullDesC, KShortKey, value ) )
    SUT_ASSERT_EQUALS( _L( "value2" ), value )    
    }
