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

#ifndef TEFTESTMODULE_H
#define TEFTESTMODULE_H

// INCLUDES
#include "stiftestmodule.h"
#include <stiflogger.h>
#include <normalhardcodedassert.h>

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def
#define TEST_MODULE_VERSION_MAJOR 0
#define TEST_MODULE_VERSION_MINOR 0
#define TEST_MODULE_VERSION_BUILD 0

// Logging path
_LIT( KTEFTestModuleLogPath, "\\logs\\testframework\\TEFTestModule\\" ); 
// Log file
_LIT( KTEFTestModuleLogFile, "TEFTestModule.txt" ); 
_LIT( KTEFTestModuleLogFileWithTitle, "TEFTestModule_[%S].txt" );

typedef TBuf<200> TScriptName;
typedef TBuf<200> TTcsName;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// CLASS DECLARATION

/**
*  This a TEFTestModule class.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CTEFTestModule) : public CTestModuleBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CTEFTestModule* NewL();

        /**
        * Destructor.
        */
        virtual ~CTEFTestModule();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CTestModuleBase InitL is used to initialize the TEFTestModule. 
        *       It is called once for every instance of TEFTestModule after
        *       its creation.
        * @since ?Series60_version
        * @param aIniFile Initialization file for the test module (optional)
        * @param aFirstTime Flag is true when InitL is executed for first 
        *               created instance of TEFTestModule.
        * @return Symbian OS error code
        */
        TInt InitL( TFileName& aIniFile, TBool aFirstTime );

        /**
        * From CTestModuleBase GetTestCasesL is used to inquiry test cases 
        *   from TEFTestModule. 
        * @since ?Series60_version
        * @param aTestCaseFile Test case file (optional)
        * @param aTestCases  Array of TestCases returned to test framework
        * @return Symbian OS error code
        */
        TInt GetTestCasesL( const TFileName& aTestCaseFile, 
                            RPointerArray<TTestCaseInfo>& aTestCases );

        /**
        * From CTestModuleBase RunTestCaseL is used to run an individual 
        *   test case. 
        * @since ?Series60_version
        * @param aCaseNumber Test case number
        * @param aTestCaseFile Test case file (optional)
        * @param aResult Test case result returned to test framework (PASS/FAIL)
        * @return Symbian OS error code (test case execution error, which is 
        *           not reported in aResult parameter as test case failure).
        */   
        TInt RunTestCaseL( const TInt aCaseNumber, 
                           const TFileName& aTestCaseFile,
                           TTestResult& aResult );

        /**
        * From CTestModuleBase; OOMTestQueryL is used to specify is particular
        * test case going to be executed using OOM conditions
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailureType OOM failure type (optional)
        * @param aFirstMemFailure The first heap memory allocation failure value (optional)
        * @param aLastMemFailure The last heap memory allocation failure value (optional)
        * @return TBool
        */
        virtual TBool OOMTestQueryL( const TFileName& /* aTestCaseFile */, 
                                     const TInt /* aCaseNumber */, 
                                     TOOMFailureType& /* aFailureType */,
                                     TInt& /* aFirstMemFailure */, 
                                     TInt& /* aLastMemFailure */ );

        /**
        * From CTestModuleBase; User may add implementation for OOM test 
        * environment initialization. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestInitializeL( const TFileName& /* aTestCaseFile */, 
                                    const TInt /* aCaseNumber */ ); 

        /**
        * From CTestModuleBase; OOMHandleWarningL. User may add implementation 
        * for OOM test warning handling. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @param aFailNextValue FailNextValue for OOM test execution (optional)
        * @return None
        */
        virtual void OOMHandleWarningL( const TFileName& /* aTestCaseFile */,
                                        const TInt /* aCaseNumber */, 
                                        TInt& /* aFailNextValue */);

        /**
        * From CTestModuleBase; OOMTestFinalizeL may be used to finalize OOM
        * test environment. Usually no implementation is required.
        * @param aTestCaseFile Test case file (optional)
        * @param aCaseNumber Test case number (optional)
        * @return None
        */
        virtual void OOMTestFinalizeL( const TFileName& /* aTestCaseFile */, 
                                       const TInt /* aCaseNumber */ );

        /**
         * Method used to log version of test module
         */
        void SendTestModuleVersion();
        
        /**
        * Get stack size set by user, in TestFramework.ini
        */ 
        static TInt ParseStackSizeL(TInt& aStackSize);


    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CTEFTestModule();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        /**
        * Read the Cfg File data into a heap buffer
        * And populate the arrays of selective test case IDs and 
        * Ranges to be used by the state machine
        * NOTE: we do not support nested cfgs...
        */
        void CreateTitleLoggerL(void);

        /**
        * Check for logger settings and delete title logger.
        */
        void DeleteTitleLogger(void);

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );
        
        /**
         * Load and wait to start, only used under Non-Lite version.
         */
        
		#if !(defined TEF_LITE)
        void StartSystemL(void);
		#endif   
		
        
        /**
         * Initialization.
         */
        void TEFInitialize(void);
        
        /**
         * Parse configuration file to get config information and script list.
         */
        TInt ParseTEFTestCfgFileL(const TFileName& aConfigFile, RPointerArray<TTestCaseInfo>& aTestCases);
        
        /**
         * Execute sigle script.
         */
        TInt RunScriptL(CActiveScheduler* aScheduler, const TScriptName& aScriptFilePath, const TTcsName& aTcsData);
        
        /**
         * Read Testexecute.ini to get configuration, otherwise use a default config.
         */ 
        void CreateCfgDataFromFileL(TPtrC& aCfgFilePath, RArray<TRange>& aSelectiveCaseRange, TDesC*& aSelTestCfgFileData);
        
        /**
        * Get elements from configuration file.
        */ 
        void DistinguishElement(const TPtrC& aElement, RArray<TRange>& aSelectiveCaseRange);
        
        
        /**
        * Parse TcsData from configration file to select test case flag. Get cases Included or Excluded.
        */ 
        void ParseTcsDataForSelectiveTestingOptions( const TTcsName& aTcsData, 
        											 CTestExecuteIniData& aParseTestExecuteIni,
        											 RArray<TRange>& aSelectiveCaseRange,  
        											 TDesC*& aSelTestCfgFileData );
        	
        
        /**
        * Add script full name(c:/xx/xxx.script) to iScriptFileNames, add cases based on script.
        */ 
        TInt MatchScriptsToCasesL(const TScriptName& aSscriptFile, TInt aScriptCount, RPointerArray<TTestCaseInfo>& aTestCases);
        

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

        CStifLogger * iLog;

        // Standard logger
        CStifLogger *iStdLog;

        // Logger for currently running test case
        CStifLogger *iTCLog;
        
        // Flag saying if test case title should be added to log file name
        TBool iAddTestCaseTitleToLogName;
        
        // Flag saying if test module version was already sent
        TBool iVersionLogged;

        // Reserved pointer for future extension
        //TAny* iReserved;
        
        //For iCfg_tcs
    	enum TCfgTcs
    		{
    		EInclude,
    		EExclude,
    		EOFF
    		};
    	
    	//For Tcs in configration file
    	TCfgTcs	iCfg_tcs;
    	
    	//For Debug in configration file
    	TBool	iCfg_Debug;
    	
    	//For SeparateLog in configration file
    	TBool	iCfg_SeparateLog;
    	
    	//For Pipe in configration file
    	TName	iCfg_Pipe;
 	    	
    	//Store script full name(with path), such as c:/xxxx/xxxx.script
    	//This array's sequence is same as test cases.
    	RPointerArray<TScriptName>	iArray_ScriptFilePaths;
    	
    	//Store TcsData in configration file, TcsData is paired with Script.
    	RPointerArray<TTcsName>		iArray_Cfg_TcsData;
    	
    	//System drive name
    	TDriveName iDefaultSysDrive;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // TEFTESTMODULE_H

// End of File
