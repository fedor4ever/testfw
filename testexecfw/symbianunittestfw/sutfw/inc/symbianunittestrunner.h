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

#ifndef SYMBIANUNITTESTRUNNER_H
#define SYMBIANUNITTESTRUNNER_H

// INCLUDES
#include "symbianunittestobserver.h"
#include <badesca.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MSymbianUnitTestInterface;
class CSymbianUnitTestResult;
class MSymbianUnitTestUiCallBack;

const TInt KSymbianUnitTestDefaultTimeout = 30;

// CLASS DECLARATION
class CSymbianUnitTestRunner : public CBase, public MSymbianUnitTestObserver
    {
    public: // Constructors and destructor
    
        IMPORT_C static CSymbianUnitTestRunner* NewLC(
            MSymbianUnitTestUiCallBack& aUiCallBack );
    
        IMPORT_C static CSymbianUnitTestRunner* NewL(
            MSymbianUnitTestUiCallBack& aUiCallBack );
    
        IMPORT_C ~CSymbianUnitTestRunner();
    
    public: // From MSymbianUnitTestObserver
        
        void IncrementExecutedTestsCount();    
        
    public: // New functions
        
        IMPORT_C TInt TestCount();
        
        IMPORT_C void ExecuteTestsL( 
            const MDesCArray& aTestDllNames,
            TBool aMemoryAllocationFailureSimulation,
            const TDesC& aOutputFileName,
            const TDesC& aOutputFormat,
	    const CDesCArray& aTestCaseNames, 
	    TInt aTimeout = KSymbianUnitTestDefaultTimeout );
        
        IMPORT_C TInt FailedTestCount();
    
    private: // Constructors
    
        CSymbianUnitTestRunner( MSymbianUnitTestUiCallBack& aUiCallBack );
        void ConstructL();
    
    private: // New functions
        
        static void DeleteTest( TAny* aTest );
        
    private: // Data
        
        MSymbianUnitTestUiCallBack& iUiCallBack;
        CSymbianUnitTestResult* iResult;
        TInt iTestCount;
    };

#endif // SYMBIANUNITTESTRUNNER_H
