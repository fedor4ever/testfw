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

// INCLUDE FILES
// For TEF
#include "scriptengine.h"
#include <test/testexecuteclient.h>
#include <test/testexecutelogger.h>
#include "testwatcher.h"
#include "version.h"
#include "tefutils.h"
#include <f32file.h>
#include <test/wrapperutilsplugin.h>
// For STIF
#include <StifTestInterface.h>
#include "teftestmodule.h"
#include "SettingServerClient.h"
#include "logging.h"

GLDEF_D TInt CScriptControl::commentedCommandsCount;
GLDEF_D TInt CScriptControl::iRunScriptFailCount;


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
// CTEFTestModule::CTEFTestModule
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTEFTestModule::CTEFTestModule()
    {

    }

// -----------------------------------------------------------------------------
// CTEFTestModule::ConstructL
// Symbian 2nd phase constructor can leave.
// Note: If OOM test case uses STIF Logger, then STIF Logger must be created
// with static buffer size parameter (aStaticBufferSize). Otherwise Logger 
// allocates memory from heap and therefore causes error situations with OOM 
// testing. For more information about STIF Logger construction, see STIF Users 
// Guide.
// -----------------------------------------------------------------------------
//
void CTEFTestModule::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();
    iAddTestCaseTitleToLogName = loggerSettings.iAddTestCaseTitle;

    iStdLog = CStifLogger::NewL( KTEFTestModuleLogPath, 
                          KTEFTestModuleLogFile);
    iLog = iStdLog;
    
    iVersionLogged = EFalse;
    
    iCfg_tcs = EOFF;
    iCfg_Debug = EFalse;
    iCfg_SeparateLog = EFalse;
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTEFTestModule* CTEFTestModule::NewL()
    {
    CTEFTestModule* self = new (ELeave) CTEFTestModule;

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    return self;

    }

// Destructor
CTEFTestModule::~CTEFTestModule()
    { 
    iLog = NULL;
    delete iStdLog;
    iStdLog = NULL;
    delete iTCLog;
    iTCLog = NULL;
    iArray_ScriptFilePaths.Close();
    iArray_Cfg_TcsData.Close();
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::InitL
// InitL is used to initialize the Test Module.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::InitL( 
    TFileName& /*aIniFile*/, 
    TBool /*aFirstTime*/ )
    {
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTEFTestModule::CreateTitleLoggerL
// Check for settings and create logger with test case title in file name.
// -----------------------------------------------------------------------------
//
void CTEFTestModule::CreateTitleLoggerL(void)
    {
    //Open new log file with test case title in file name
    if(iAddTestCaseTitleToLogName)
        {
        // Check if there is no test case logger already created.
        if(iTCLog) 
            { 
            delete iTCLog; 
            iTCLog = NULL; 
            } 
            
        TFileName logFileName;
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        
        logFileName.Format(KTEFTestModuleLogFileWithTitle, &title);

        iTCLog = CStifLogger::NewL(KTEFTestModuleLogPath, 
                                 logFileName);
        iLog = iTCLog; 
        }
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::CreateStdLoggerL
// Check for logger settings and create standard logger.
// -----------------------------------------------------------------------------
//
void CTEFTestModule::DeleteTitleLogger(void)
    {
    //Delete title logger
    if(iAddTestCaseTitleToLogName)
        {
        iLog = iStdLog;
        delete iTCLog;
        iTCLog = NULL;
        }
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::GetTestCasesL
// GetTestCases is used to inquire test cases from the Test Module. Test
// cases are stored to array of test cases. The Test Framework will be 
// the owner of the data in the RPointerArray after GetTestCases return
// and it does the memory deallocation. 
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::GetTestCasesL( 
    const TFileName& aConfig, 
    RPointerArray<TTestCaseInfo>& aTestCases )
    {
    iLog->Log(_L("GetTestCasesL"));
    TInt ret = KErrNone;
    
    TRAPD(err, ret = ParseTEFTestCfgFileL(aConfig, aTestCases));
    if(err != KErrNone)
    	{
    	//ret = KErrNotFound;
    	_LIT(KErrLog, "Error in given test configuration file %S. Error = %d.");
    	iLog->Log(CStifLogger::ERed, KErrLog, &aConfig, ret);
    	}
    
    return ret;
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::RunTestCaseL
// RunTestCaseL is used to run an individual test case specified 
// by aTestCase. Test cases that can be run may be requested from 
// Test Module by GetTestCases method before calling RunTestCase.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::RunTestCaseL(
    const TInt aCaseNumber,
    const TFileName& /*aConfig*/,
    TTestResult& aResult )
    {
    iLog->Log(_L("RunTestCaseL"));
    SendTestModuleVersion();

    CreateTitleLoggerL();
    //Test case implementation or function call here
    TRAPD(err, TEFInitialize() );
    __ASSERT_ALWAYS(!err, User::Panic(KTestExecuteName,err));

#if (defined __DATA_CAGING__)
	// Platform security hooks    
	RProcess().DataCaging(RProcess::EDataCagingOn);
	RProcess().SecureApi(RProcess::ESecureApiOn);
#endif
    
	CActiveScheduler* sched = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(sched);
	CActiveScheduler::Install(sched);

    if(!sched)
    {
    __RDEBUG( (_L("Error: TEFTestModule.dll, Error in getting current active scheduler.") ) );
	iLog->Log(CStifLogger::ERed, _L("Error: TEFTestModule.dll, Error in getting current active scheduler."));
    User::Panic(_L("TEFTestModule"), -2);
    }
	
	TInt scriptCount = iArray_ScriptFilePaths.Count();
	TInt tcsDataCount = iArray_Cfg_TcsData.Count();

	if(aCaseNumber > scriptCount)
		{
		__RDEBUG( (_L("Error: Case number to run is bigger than expected.") ) );
		iLog->Log(CStifLogger::ERed, _L("Error: Case number to run is bigger than expected."));
		// Cleanup the scheduler object
		CleanupStack::PopAndDestroy(sched);
		DeleteTitleLogger();
		User::Leave(KErrNotFound);
		}
	    
	TRAPD(errRun, RunScriptL(	sched,
								*(iArray_ScriptFilePaths[aCaseNumber-1]), 
								*(iArray_Cfg_TcsData[aCaseNumber-1])
							) );
	if (errRun != KErrNone)
		{
		__RDEBUG( (_L("Run script leave. code=%d"),errRun) );
		iLog->Log(CStifLogger::ERed, _L("Run script leave. code=%d"),errRun);
		}
  
	// Cleanup the scheduler object
	CleanupStack::PopAndDestroy(sched);

    aResult.SetResult( errRun, *(iArray_ScriptFilePaths[aCaseNumber-1]) );     
    
    // Test case was executed
    DeleteTitleLogger();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::OOMTestQueryL
// Used to check if a particular test case should be run in OOM conditions and 
// which memory allocations should fail.    
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
TBool CTEFTestModule::OOMTestQueryL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */, 
                                TOOMFailureType& /* aFailureType */, 
                                TInt& /* aFirstMemFailure */, 
                                TInt& /* aLastMemFailure */ ) 
    {
    _LIT( KOOMTestQueryL, "CTEFTestModule::OOMTestQueryL" );
    iLog->Log( KOOMTestQueryL ); 

    return EFalse;

    }

// -----------------------------------------------------------------------------
// CTEFTestModule::OOMTestInitializeL
// Used to perform the test environment setup for a particular OOM test case. 
// Test Modules may use the initialization file to read parameters for Test 
// Module initialization but they can also have their own configure file or 
// some other routine to initialize themselves.  
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CTEFTestModule::OOMTestInitializeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::OOMHandleWarningL
// In some cases the heap memory allocation should be skipped, either due to
// problems in the OS code or components used by the code being tested, or even 
// inside the tested components which are implemented this way on purpose (by 
// design), so it is important to give the tester a way to bypass allocation 
// failures.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//
void CTEFTestModule::OOMHandleWarningL( 
                                const TFileName& /* aTestCaseFile */,
                                const TInt /* aCaseNumber */, 
                                TInt& /* aFailNextValue */ )
    {
    }

// -----------------------------------------------------------------------------
// CTEFTestModule::OOMTestFinalizeL
// Used to perform the test environment cleanup for a particular OOM test case.
//
// NOTE: This method is virtual and must be implemented only if test case
// should be executed using OOM conditions.  
// -----------------------------------------------------------------------------
//                  
void CTEFTestModule::OOMTestFinalizeL( 
                                const TFileName& /* aTestCaseFile */, 
                                const TInt /* aCaseNumber */ )
    {
    }

//-----------------------------------------------------------------------------
// CTemplateScriptTEFTestModule::SendTestModuleVersion
// Method used to send version of test module
//-----------------------------------------------------------------------------
//
void CTEFTestModule::SendTestModuleVersion()
	{
	if(iVersionLogged)
		{
		return;
		}
	
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_MODULE_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_MODULE_VERSION_MINOR;
	moduleVersion.iBuild = TEST_MODULE_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("TEFTestModule.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	iVersionLogged = ETrue;
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point
// Returns: CTestModuleBase*: Pointer to Test Module object
// -----------------------------------------------------------------------------
//
EXPORT_C CTestModuleBase* LibEntryL()
    {
    return CTEFTestModule::NewL();

    }

// -----------------------------------------------------------------------------
// SetRequirements handles test module parameters(implements evolution
// version 1 for test module's heap and stack sizes configuring).
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt SetRequirements( CTestModuleParam*& aTestModuleParam, 
                                TUint32& aParameterValid )
    {

    /* --------------------------------- NOTE ---------------------------------
    USER PANICS occurs in test thread creation when:
    1) "The panic occurs when the value of the stack size is negative."
    2) "The panic occurs if the minimum heap size specified is less
       than KMinHeapSize".
       KMinHeapSize: "Functions that require a new heap to be allocated will
       either panic, or will reset the required heap size to this value if
       a smaller heap size is specified".
    3) "The panic occurs if the minimum heap size specified is greater than
       the maximum size to which the heap can grow".
    Other:
    1) Make sure that your hardware or Symbian OS is supporting given sizes.
       e.g. Hardware might support only sizes that are divisible by four.
    ------------------------------- NOTE end ------------------------------- */

    // Normally STIF uses default heap and stack sizes for test thread, see:
    // KTestThreadMinHeap, KTestThreadMinHeap and KStackSize.
    // If needed heap and stack sizes can be configured here by user. Remove
    // comments and define sizes.
    aParameterValid = KStifTestModuleParameterChanged;

    CTestModuleParamVer01* param = CTestModuleParamVer01::NewL();
    //We're quite heavy on the stack...
    // Stack size
    param->iTestThreadStackSize= 28672; // 28K stack
    // Heap sizes
    param->iTestThreadMinHeap = 4096;   // 4K heap min
    param->iTestThreadMaxHeap = 1260480;// 1.2M heap max
    
    TInt stackSizeFromUser = -1;
    TInt parseErr = KErrNone;
    TRAPD( err, parseErr = CTEFTestModule::ParseStackSizeL(stackSizeFromUser));
    if(err != KErrNone)
    	{
    	__RDEBUG( (_L("Error: Leaves when get StackSize from TestFramework.ini") ) );
    	}
    if(parseErr != KErrNone)
    	{
    	__RDEBUG( (_L("Error: Error when get StackSize from TestFramework.ini, err= %d"), parseErr) );
    	}

    if(stackSizeFromUser > 0)
    	{
    	param->iTestThreadStackSize = stackSizeFromUser; // user configed stack size
    	}
    aTestModuleParam = param;

    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTEFTestModule::TEFInitialize
// InitializeModule is used to do some preparations.
// Returns: void
// -----------------------------------------------------------------------------
//
void CTEFTestModule::TEFInitialize(void)
	{
	// Check to see if the plugin wrapper around the GetSystemDrive is loadable
	// If yes, then instantiate the wrapper object and obtain the default system drive
	// Else, use the hardcoded default drive as c:
	iDefaultSysDrive = KTEFLegacySysDrive;	
	
	RLibrary pluginLibrary;
	CWrapperUtilsPlugin* plugin = TEFUtils::WrapperPluginNew(pluginLibrary);
	
	if (plugin!=NULL)
		{
		TDriveUnit driveUnit(plugin->GetSystemDrive());
		iDefaultSysDrive.Copy(driveUnit.Name());
		delete plugin;
		pluginLibrary.Close();
		}
	
	TBool enableSysStart = ETrue;
	CTestExecuteIniData* iniData = NULL;
	TRAPD(err, iniData = CTestExecuteIniData::NewL(iDefaultSysDrive));
	if (err == KErrNone)
		{
		// Extract all the key values within the object
		iniData->ExtractValuesFromIni();
		iniData->GetKeyValueFromIni(KTEFSystemStarter, enableSysStart);
		}

	err = KErrNone;
	
	#if !(defined TEF_LITE)
	if (enableSysStart)
		{
		TRAP(err, StartSystemL());
		if (KErrNone != err)
			{
			__RDEBUG( (_L("Wait for system starter failed, error code=%d"),err) );
			iLog->Log(CStifLogger::ERed, _L("Wait for system starter failed, error code=%d"),err);
			User::After(5000000);
			}
		}
	#endif
	
	if (iniData != NULL)
		{
		delete iniData;
		}	
	
	}

// -----------------------------------------------------------------------------
// CTEFTestModule::ParseTEFTestCfgFileL
// ParseTEFTestCfgFileL is used to parse configuration file
// to get config information and script list.
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::ParseTEFTestCfgFileL( const TFileName& aConfigFile, 
		    								RPointerArray<TTestCaseInfo>& aTestCases)
	{
	iLog->Log(_L("Start parsing TEFTestModule configuration."));
	
    if( aConfigFile.Length() == 0 )
        {
        iLog->Log(CStifLogger::ERed, _L("Error: No test configuration file given") ) ;
        __RDEBUG( (_L("Error: No test configuration file given") ) );
        return KErrNotFound;
        }

    CStifParser* parser = NULL; 
    
    // Open .cfg file
    TRAPD( err, 
        parser = CStifParser::NewL( _L(""), 
                                    aConfigFile, 
                                    CStifParser::ECStyleComments ) );
    if( err != KErrNone )
        {
        __RDEBUG( (_L("Error: Given test configuration file [%S] not found"),
            &aConfigFile ) );
        iLog->Log(CStifLogger::ERed, _L("Error: Given test configuration file [%S] not found. Error=%d."),
        	&aConfigFile,err);
        return err;
        }
    
    CleanupStack::PushL( parser );
    CStifSectionParser* section = NULL;
    TPtrC tmp;
    TInt ret = KErrNone;

    // Find first section
    _LIT( KModuleStartTag, "[New_TEFTest]" );
    _LIT( KModuleEndTag, "[End_TEFTest]" );
    section = parser->SectionL( KModuleStartTag, KModuleEndTag );
    if( section == NULL )
        {
        __RDEBUG( (_L("Error: Can not find TEFTest section from %S."), 
                &aConfigFile) );
        iLog->Log(CStifLogger::ERed, _L("Error: Can not find TEFTest section from %S."), 
        		&aConfigFile);
        ret = KErrNotFound;
        }
    else
        {
        // Parse section
        CleanupStack::PushL( section );
            
        CStifItemParser* item = NULL; 
    	
        // Get Tcs from configuration file.
        TRAPD( getTcs, item = section ->GetItemLineL(_L( "Tcs=" ), ENoTag));
        if( getTcs != KErrNone )
            {
            __RDEBUG( (_L("Error: From file %S, get item 'Tcs=' leaves with error: %d."), &aConfigFile ,getTcs) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'Tcs=' leaves with error: %d."), &aConfigFile ,getTcs);
            delete item;
            item=NULL;
            return getTcs;
            }
        if( !item )
            {
            __RDEBUG( (_L("Warning: From file %S, can not get item 'Tcs=' value. Use default OFF."), &aConfigFile) );
            iLog->Log( _L("Warning: From file %S, can not get item 'Tcs=' value. Use default OFF."), &aConfigFile);
            //set Tcs to default OFF.
            iCfg_tcs = EOFF;
            iLog->Log(_L("Tcs = %d"),iCfg_tcs);
            delete item;
            item=NULL;
            }
        else
        	{
        	TPtrC tcs;
        	ret = item->GetString( _L( "" ), tcs );
    	
    		HBufC* tcsHbuf = tcs.AllocL();
    		TPtr modifiableString = tcsHbuf->Des();
    		modifiableString.UpperCase();
    		
    		if( modifiableString == _L( "INCLUDE" ) )
    			{
    			iCfg_tcs = EInclude;
    			}
    		else if( modifiableString == _L( "EXCLUDE" ) )
    			{
    			iCfg_tcs = EExclude;
    			}
    		else if( modifiableString == _L( "OFF" ) )
    			{
    			iCfg_tcs = EOFF;
    			}
    		else
    			{
                __RDEBUG( (_L("Warning: From file %S, item 'Tcs=' value is not recognized. Use default OFF."), &aConfigFile) );
                iLog->Log( _L("Warning: From file %S, item 'Tcs=' value is not recognized. Use default OFF."), &aConfigFile);    
                iCfg_tcs = EOFF;
    			}
    		iLog->Log(_L("Tcs = %d"),iCfg_tcs);
    		delete tcsHbuf;
    		tcsHbuf=NULL;
    		delete item;	  
    		item=NULL;
        	}
        
        //Get Debug from configuration file.
        TRAPD( getDebug, item = section ->GetItemLineL(_L( "Debug=" ), ENoTag));
        if( getDebug != KErrNone )
            {
            __RDEBUG( (_L("Error: From file %S, get item 'Debug=' leaves with error: %d."), &aConfigFile, getDebug) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'Debug=' leaves with error: %d."), &aConfigFile, getDebug);
            delete item;
            item=NULL;
            return getDebug;
            }
        if( !item )
            {
            __RDEBUG( (_L("Warning: From file %S, can not get item 'Debug=' value. Use default OFF."), &aConfigFile) );
            iLog->Log( _L("Warning: From file %S, can not get item 'Debug=' value. Use default OFF."), &aConfigFile);
            // set Debug to default OFF.
            iCfg_Debug = EFalse;
            iLog->Log(_L("Debug = %d"),iCfg_Debug);
            delete item;
            item=NULL;
            }
        else
        	{
        	TPtrC debug;
        	ret = item->GetString( _L( "" ), debug );
    	
    		HBufC* debugHbuf = debug.AllocL();
    		TPtr modifiableString = debugHbuf->Des();
    		modifiableString.UpperCase();
    		
    		if( modifiableString == _L( "ON" ) )
    			{
    			iCfg_Debug = ETrue;
    			}
    		else if( modifiableString == _L( "OFF" ) )
    			{
    			iCfg_Debug = EFalse;
    			}
    		else
    			{
                __RDEBUG( (_L("Warning: From file %S, item 'Debug=' value is not recognized. Use default OFF."), &aConfigFile) );
                iLog->Log( _L("Warning: From file %S, item 'Debug=' value is not recognized. Use default OFF."), &aConfigFile);  
                iCfg_Debug = EFalse;
    			}
    		iLog->Log(_L("Debug = %d"),iCfg_Debug);
    		delete debugHbuf;
    		debugHbuf=NULL;
    		delete item;	   
    		item=NULL;
        	}
         
        //Get SeparateLog from configuration file.
        TRAPD( getSep, item = section ->GetItemLineL(_L( "SeparateLog=" ), ENoTag));
        if( getSep != KErrNone )
            {
            __RDEBUG( (_L("Error: From file %S, get item 'SeparateLog=' leaves with error: %d."), &aConfigFile, getSep) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'SeparateLog=' leaves with error: %d."), &aConfigFile, getSep);
            delete item;
            item=NULL;
            return getSep;
            }
        if( !item )
            {
            __RDEBUG( (_L("Warning: From file %S, can not get item 'SeparateLog=' value. Use default OFF."), &aConfigFile) );
            iLog->Log( _L("Warning: From file %S, can not get item 'SeparateLog=' value. Use default OFF."), &aConfigFile);
            //set SeparateLog to OFF.
            iCfg_SeparateLog = EFalse;
            iLog->Log(_L("SeparateLog = %d"),iCfg_SeparateLog);
            delete item;
            item=NULL;
            }
        else
        	{
        	TPtrC separateLog;
        	ret = item->GetString( _L( "" ), separateLog );
    	
    		HBufC* separateLogHbuf = separateLog.AllocL();
    		TPtr modifiableString = separateLogHbuf->Des();
    		modifiableString.UpperCase();
    		
    		if( modifiableString == _L( "ON" ) )
    			{
    			iCfg_SeparateLog = ETrue;
    			}
    		else if( modifiableString == _L( "OFF" ) )
    			{
    			iCfg_SeparateLog = EFalse;
    			}
    		else
    			{
                __RDEBUG( (_L("Warning: From file %S, item 'SeparateLog=' value is not recognized. Use default OFF."), &aConfigFile) );
                iLog->Log( _L("Warning: From file %S, item 'SeparateLog=' value is not recognized. Use default OFF."), &aConfigFile);  
    			iCfg_SeparateLog = EFalse;
    			}
    		iLog->Log(_L("SeparateLog = %d"),iCfg_SeparateLog);
    		delete separateLogHbuf;
    		separateLogHbuf=NULL;
    		delete item;	            	
    		item=NULL;
        	}

        //Get Pipe from configuration file.
        TRAPD( getPipe, item = section ->GetItemLineL(_L( "Pipe=" ), ENoTag));
        if( getPipe != KErrNone )
            {
            __RDEBUG( (_L("Error: From file %S, get item 'Pipe=' leaves with error: %d."), &aConfigFile, getPipe) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'Pipe=' leaves with error: %d."), &aConfigFile, getPipe);
            delete item;
            item=NULL;
            return getPipe;
            }
        if( !item )
            {
            __RDEBUG( (_L("Warning: From file %S, can not get item 'Pipe=' value. Use default OFF."), &aConfigFile) );
            iLog->Log( _L("Warning: From file %S, can not get item 'Pipe=' value. Use default OFF."), &aConfigFile);
            iCfg_Pipe.Copy(_L("OFF"));	
            iLog->Log(_L("Pipe = %S"),&iCfg_Pipe);
            delete item;
            item=NULL;
            }
        else
        	{
        	TPtrC pipeName;
        	ret = item->GetString( _L( "" ), pipeName );
        	iCfg_Pipe.Copy(pipeName);		
    		if(iCfg_Pipe.Length()<=0)
			{
            __RDEBUG( (_L("Warning: From file %S, can not get item 'Pipe=' value. Use default OFF."), &aConfigFile) );
            iLog->Log( _L("Warning: From file %S, can not get item 'Pipe=' value. Use default OFF."), &aConfigFile);
            iCfg_Pipe.Copy(_L("OFF"));	
            delete item;
            item=NULL;
			}
    		iLog->Log(_L("Pipe = %S"),&iCfg_Pipe);
    		delete item;	   
    		item=NULL;
        	}        
        
        //Get Script from configuration file.
        TInt scriptCount = 0;
        TRAPD(getScript, item = section->GetItemLineL(_L( "Script=" ), ENoTag));
        if( getScript != KErrNone )
            {
            __RDEBUG( (_L("Error: From file %S, get item 'Script=' leaves with error: %d."), &aConfigFile, getScript) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'Script=' leaves with error: %d."), &aConfigFile, getScript);
            delete item;
            item=NULL;
            return getScript;
            }
        if( !item )
            {
            __RDEBUG( (_L("Error: From file %S, can not get item 'Script=' value."), &aConfigFile) );
            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, can not get item 'Script=' value."), &aConfigFile);
            delete item;
            item=NULL;
            return KErrNotFound;
            }

        while(item)
            {
            //script count, from 1 to ...
            scriptCount++;
            CleanupStack::PushL(item);
            TPtrC script;
            ret = item->GetString(_L( "" ), script);
                
            TScriptName scriptFile;
            scriptFile.Copy(script);
            TStifUtil::CorrectFilePathL( scriptFile );
            scriptFile.LowerCase();
            iLog->Log(_L("Script = %S"),&scriptFile);
            
            ret = MatchScriptsToCasesL(scriptFile, scriptCount, aTestCases);

            CleanupStack::PopAndDestroy(item);
            item=NULL;
            TRAPD(getScript2, item = section->GetNextItemLineL(_L( "Script=" ), ENoTag));
            if( getScript2 != KErrNone )
                {
                __RDEBUG( (_L("Error: From file %S, get item 'Script=' leaves with error: %d."), &aConfigFile, getScript2) );
                iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'Script=' leaves with error: %d."), &aConfigFile, getScript2);
                delete item;
                item=NULL;
                return getScript2;
                }
            }         
        
        //Get TcsFile from configuration file.
        if (iCfg_tcs != EOFF)
        	{
	        TInt tcsCount = 0;
	        TRAPD(getTcsData, item = section->GetItemLineL(_L( "TcsData=" ), ENoTag));
	        if( getTcsData != KErrNone )
	            {
	            __RDEBUG( (_L("Error: From file %S, get item 'TcsData=' leaves with error: %d."), &aConfigFile, getTcsData) );
	            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'TcsData=' leaves with error: %d."), &aConfigFile, getTcsData);
	            delete item;
	            item=NULL;
	            return getTcsData;
	            }
	        if( !item )
	            {
	            __RDEBUG( (_L("Error: From file %S, can not get item 'TcsData=' value."), &aConfigFile) );
	            iLog->Log(CStifLogger::ERed, _L("Error: From file %S, can not get item 'TcsData=' value."), &aConfigFile);
	            delete item;
	            item=NULL;
	            return KErrNotFound;
	            }
	
	        while(item)
	            {
	            //script count, from 1 to ...
	            tcsCount++;
	            CleanupStack::PushL(item);
	            TPtrC tcsFile;
	            ret = item->GetString(_L( "" ), tcsFile);
	                
	            TTcsName* tcsData = new(ELeave) TTcsName(tcsFile);
	            TStifUtil::CorrectFilePathL( *tcsData );
	            tcsData->LowerCase();
	            iLog->Log(_L("TcsData = %S"),&(*tcsData));
	    
	            //Add tcsData list to iArray_Cfg_TcsData.
	            CleanupStack::PushL( tcsData );
	            // Append tcsData to the iArray_Cfg_TcsData array. After appended succesfully 
	            // the tcsData object is owned (and freed) by the iArray_Cfg_TcsData in ~CTEFTestModule() method.
	            // The sequence of tcs in iArray_Cfg_TcsData is same as sequence of script list iArray_ScriptFilePaths.
	            // tcsData and script is paired.
	            User::LeaveIfError( iArray_Cfg_TcsData.Append( tcsData ) );
	            CleanupStack::Pop(tcsData);
	            
	            CleanupStack::PopAndDestroy(item);
	            item=NULL;
	            TRAPD(getTcsData2, item = section->GetNextItemLineL(_L( "TcsData=" ), ENoTag));
	            if( getTcsData2 != KErrNone )
	                {
	                __RDEBUG( (_L("Error: From file %S, get item 'TcsData=' leaves with error: %d."), &aConfigFile, getTcsData2) );
	                iLog->Log(CStifLogger::ERed, _L("Error: From file %S, get item 'TcsData=' leaves with error: %d."), &aConfigFile, getTcsData2);
	                delete item;
	                item=NULL;
	                return getTcsData2;
	                }
	            }
        	}
        else
        	//iCfg_tcs == EOFF, pair Script with TcsData(NULL).
        	{
        	TTcsName* tcsDataNull = new(ELeave) TTcsName();
        	CleanupStack::PushL( tcsDataNull );
        	for(TInt i=0; i<iArray_ScriptFilePaths.Count();i++)
        		{
        		// Append tcsDataNull to the iArray_Cfg_TcsData array. After appended succesfully 
        		// the tcsDataNull object is owned (and freed) by the iArray_Cfg_TcsData in ~CTEFTestModule() method.
        		User::LeaveIfError( iArray_Cfg_TcsData.Append( tcsDataNull ) );
        		}
        	CleanupStack::Pop(tcsDataNull);
        	}
      
        CleanupStack::PopAndDestroy( section );
        section = NULL;
        }
    CleanupStack::PopAndDestroy( parser );
    
    //check: At leasr one script exist in TEFTestModule configration file
    if(iArray_ScriptFilePaths.Count()<=0)
    	{
    	__RDEBUG( (_L("Error: From file %S, no Script founded."), &aConfigFile) );
    	iLog->Log(CStifLogger::ERed, _L("Error: From file %S, no Script founded."), &aConfigFile);
    	ret = KErrArgument;
    	}
    
    //check: Script is paired with TcsData.
    if(iArray_ScriptFilePaths.Count() != iArray_Cfg_TcsData.Count())
    	{
    	__RDEBUG( (_L("Error: From file %S, Script is not paired with TcsData."), &aConfigFile) );
    	iLog->Log(CStifLogger::ERed, _L("Error: From file %S, Script is not paired with TcsData."), &aConfigFile);
    	ret = KErrArgument;
    	} 	

    
    return ret; 
	}

// -----------------------------------------------------------------------------
// CTEFTestModule::MatchScriptsToCasesL
// MatchScriptsToCasesL is used to add cases based on script list.
// Case title use script short name(xxx, No path and No extension '.script'). 
// Script list stores in iArray_ScriptFilePaths, using full name(c:\xx\xxx\xxx.script)
// The sequence of Case list and script list are the same. 
// This is called by ParseTestFrameWorkCfgL().
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::MatchScriptsToCasesL(	const TScriptName& aScriptFile,
											TInt aScriptCount,
											RPointerArray<TTestCaseInfo>& aTestCases )
	{
	
	TInt ret = KErrNone;
	
	//Get the script name(xxx.script) from full name(c:\xx\xxx\xxx.script)
	TInt offset = aScriptFile.LocateReverse('\\');
	if (offset == KErrNotFound)
		{
		ret = KErrNotFound;
		return ret;
		}
	TFileName scriptName;
	scriptName.Copy( aScriptFile.Mid(offset+1));
	//Get the script name(xxx, No '.script')
	offset = scriptName.LocateReverse('.');
	if (offset == KErrNotFound)
		{
		ret = KErrNotFound;
		return ret;
		}
	scriptName.Copy( scriptName.Left(offset));
	
	//Add script list. Full name(c:\xx\xxx\xxx.script) to iArray_ScriptFilePaths.
	TScriptName* scriptfile = new(ELeave) TScriptName(aScriptFile);
	CleanupStack::PushL( scriptfile );
	// Append TTestCaseInfo to the iArray_ScriptFilePaths array. After appended succesfully 
	// the TTestCaseInfo object is owned (and freed) by the iArray_ScriptFilePaths in ~CTEFTestModule() method.
	// The sequence of script in iArray_ScriptFilePaths is same as sequence of case list.
	User::LeaveIfError( iArray_ScriptFilePaths.Append( scriptfile ) );
	CleanupStack::Pop(scriptfile);
	
	//Add case, case title is xxx, no path, no extesion '.script'
	TTestCaseInfo* testCase = new(ELeave) TTestCaseInfo();
	CleanupStack::PushL( testCase );
	testCase->iCaseNumber = aScriptCount;
	testCase->iTitle.Copy( scriptName );
	
    // Append TTestCaseInfo to the testcase array. After appended succesfully 
    // the TTestCaseInfo object is owned (and freed) by the TestServer. 
	User::LeaveIfError( aTestCases.Append( testCase ) );
	CleanupStack::Pop(testCase);
	
	return ret; 
	}

// -----------------------------------------------------------------------------
// CTEFTestModule::RunScriptL
// RunScriptL is used to Execute sigle script. 
// This is the main functionality of script execution based 
// on legacy TEF's functionality.
// Returns: TInt: Symbian error code.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::RunScriptL(CActiveScheduler* aScheduler, 
								const TScriptName& aScriptFilePath, 
								const TTcsName& aTcsData )
	{  
	TDriveName testSysDrive(KTEFLegacySysDrive);
	TInt waitForLoggingTime = 0;
	TBuf<KMaxTestExecuteNameLength> htmlLogPath;
	// Create a object of the Parser for TestExecute.ini
	CTestExecuteIniData* parseTestExecuteIni = NULL;
	TRAPD(err, parseTestExecuteIni = CTestExecuteIniData::NewL(iDefaultSysDrive));
	if (err == KErrNone)
		{
		CleanupStack::PushL(parseTestExecuteIni);
		// Extract all the key values within the object
		parseTestExecuteIni->ExtractValuesFromIni();
		}
	
	TDesC* selTestCfgFileData = NULL; //the pointer to the data of in the .tcs file
	
	// If Debug == ON, then set Just In Time debugging. Panicks break into debug on emulator
	(iCfg_Debug) ? (User::SetJustInTime(ETrue)) : (User::SetJustInTime(EFalse)); 
	
	// Create a console
	_LIT(KMessage,"TestExecute Script Engine");
	CConsoleBase* console = Console::NewL(KMessage,TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::PushL(console);
	console->SetCursorHeight(0);
	RConsoleLogger consoleLogger(*console);
	
	// Check whether script name is provided along with folder path in the command line
	// If not, take the path from testexecute.ini & name from the command line	
	TPtrC scriptFilePath(aScriptFilePath);
	TScriptName tempScriptPath(scriptFilePath);
	TInt ret=scriptFilePath.FindC(KTEFColon);
	if(ret==KErrNotFound)
		{
		if (parseTestExecuteIni != NULL)
			{
			TBuf<KMaxTestExecuteNameLength> tempBuffer;
			parseTestExecuteIni->GetKeyValueFromIni(KTEFDefaultScriptPath, tempBuffer);
			// If the relative script file path does not refer to the root,
			// we will look for DefaultScriptDir entry in testexecute.ini
			// If available prepend it to the relative path
			// else if the relative path refers to root,
			// then set the default system drive, i.e. c:
			// else leaving it as it is (considering invalid path)
			if (scriptFilePath.Left(1).CompareF(KTEFSlash) != 0 &&
			 tempBuffer.Length() > 0)
				scriptFilePath.Set(tempBuffer);
			else if (scriptFilePath.Left(1).CompareF(KTEFSlash) == 0)
				scriptFilePath.Set(iDefaultSysDrive);
			else
				scriptFilePath.Set(KNull);
			}
		else
			{
			// If the file path is not provided in command line as well as in testexecute.ini
			// then set the  script file path to be 'blank'
			scriptFilePath.Set(KNull);
			}

		// Construct the full file path from the values extracted from command line and ini file
		TScriptName storeScriptPathTemp(scriptFilePath);
		storeScriptPathTemp.Append(tempScriptPath);
		scriptFilePath.Set(storeScriptPathTemp);
		tempScriptPath.Copy(scriptFilePath);
		}

	//If scriptFilePath is not appended by .script Append .script 
	if(scriptFilePath.Find(KTEFScriptExtension)==KErrNotFound)
		{
		tempScriptPath.Append(KTEFScriptExtension);
		}
	scriptFilePath.Set(tempScriptPath);
	
	//Parse TcsData from configration file to select test case flag. Get cases Included or Excluded.
	TSelectiveTestingOptions* selTestingOptions =NULL;
	if(iCfg_tcs!=EOFF)
		{
		RArray<TRange> selectiveCaseRange;
		ParseTcsDataForSelectiveTestingOptions(aTcsData, *parseTestExecuteIni,selectiveCaseRange, selTestCfgFileData);
		
		//you need to sort these two arrays first, and also if they are both empty ignore the entire option altogether.
		if(  selectiveCaseRange.Count() > 0 )
			{
			CleanupStack::PushL(selTestCfgFileData);
			TLinearOrder<TRange> orderingrng(TRange::CompareTRangeStartOrder) ; 
			selectiveCaseRange.Sort(orderingrng );
			ESelectiveTesting selectiveTestingType(iExclusive);
			if(iCfg_tcs == EInclude)
				{
				selectiveTestingType = iInclusive ; 
				}
			selTestingOptions = new(ELeave) TSelectiveTestingOptions(selectiveCaseRange, selectiveTestingType);
			}
		else
			{
			//if iCfg_tcs == EOFF, no tcs option used, ignore it...
			delete selTestCfgFileData;
			}
		}
	
	if (scriptFilePath.CompareF(KNull)==0)
		{
		if (!iCfg_SeparateLog && !iCfg_Debug)
			{
			// Print the product version details through console window
			iLog->Log(KTEFVersionMessage);
			iLog->Log(KTEFProductVersion);
			}
		else
			{
			// Display a error message on the log file of error TEF configration file.
			__RDEBUG( (_L("Error: In TEFTestModule configuration file. No script founded.") ) );
			iLog->Log(CStifLogger::ERed, _L("Error: In TEFTestModule configuration file. No script founded."));
			}
		// Exit on a key press from user
		console->Printf(KTEFEnterKeyMessage);
		console->Getch();
		}
	else
		{
		// Create a Interface class object for generating HTML & XML logs
		CTestExecuteLogger *tefLogger = new(ELeave) CTestExecuteLogger();
		CleanupStack::PushL(tefLogger);

		TInt logMode;
		TInt logLevel;
		TInt remotePanicDetection;
		TBuf<KMaxTestExecuteNameLength> iniSysDrive;

		if (parseTestExecuteIni != NULL)
			{
			// Parse ini for retrieving logging options set through ini
			parseTestExecuteIni->GetKeyValueFromIni(KTEFLogMode, logMode);
			parseTestExecuteIni->GetKeyValueFromIni(KTEFLogSeverityKey, logLevel);
			parseTestExecuteIni->GetKeyValueFromIni(KTEFRemotePanicDetection, remotePanicDetection);
			parseTestExecuteIni->GetKeyValueFromIni(KTEFDefaultSysDrive, iniSysDrive);
			parseTestExecuteIni->GetKeyValueFromIni(KTEFWaitForLoggingTime, waitForLoggingTime);
			parseTestExecuteIni->GetKeyValueFromIni(KTEFHtmlKey, htmlLogPath);
			}
		else
			{
			// Set default values for logging options if parser is not functioning
			logMode = TLoggerOptions(ELogHTMLOnly);
			logLevel = RFileFlogger::TLogSeverity(ESevrAll);
			remotePanicDetection = 0;
			iniSysDrive.Copy(KTEFIniSysDrive);
			waitForLoggingTime = 5;
			htmlLogPath.Copy(KTestExecuteLogPath);
			htmlLogPath.Replace(0, 2, iDefaultSysDrive);
			}
		
		tefLogger->SetLoggerOptions(logMode);
		if(iCfg_Pipe!=_L("OFF"))
			{
			TPtrC pipeName(iCfg_Pipe); 
			tefLogger->SetPipeName(pipeName); 
			}
		
		// Initialise the logging passing in the script file path & log level to the interface
		tefLogger->InitialiseLoggingL(scriptFilePath, iCfg_SeparateLog, logLevel);

		// Check to see if defaultsysdrive key is set in testexecute.ini
		// if set to SYSDRIVE, assign the default system drive obtained from the plugin
		// else, if a true value is set in testexecute.ini, use it as system drive for all test artifacts
		if (iniSysDrive.Length() == 2 && iniSysDrive.Right(1).Compare(KTEFColon) == 0)
			testSysDrive.Copy(iniSysDrive);
		else if (iniSysDrive.CompareF(KTEFIniSysDrive) == 0)
			testSysDrive.Copy(iDefaultSysDrive);


		// Pass the first command line argument to the script master
		// which is always the command script
		CScriptMaster* scriptMaster = new (ELeave) CScriptMaster(scriptFilePath,*tefLogger, consoleLogger, iDefaultSysDrive, testSysDrive, selTestingOptions);
		// To kick the state machine of the script master off -
		// Call the kick method which jumps us into the RunL() of the CScriptMaster class
		// CScriptMaster is the top AO in the hierarchy.
		scriptMaster->Kick();
		
		// Construct and Install a test watcher object for capturing remote panics during test execution
		CTestWatcher* testWatcher = NULL;
		if (remotePanicDetection != 0)
			{
			testWatcher = CTestWatcher::NewL();
			testWatcher->StartL();
			}
		// Enter the Active Scheduler
		aScheduler->Start();
		
		// Cleanup
		delete scriptMaster;
		
		TInt commentedCommandsCnt=CScriptControl::commentedCommandsCount;
		if(commentedCommandsCnt==-1)
			{
			CScriptControl::commentedCommandsCount=0;
			//If the path specified fails check out for path from testexecute.ini
			if(ret!=KErrNotFound)
				{
				//To get scriptFile name  i.e get sampleFile.script
				TInt posOfLastSlash=scriptFilePath.LocateReverse('\\') ;
				scriptFilePath.Set(scriptFilePath.Mid(posOfLastSlash+1));
				TScriptName tempStore(scriptFilePath);
				if (parseTestExecuteIni != NULL)
					{
					TBuf<KMaxTestExecuteNameLength> tempBuffer;
					parseTestExecuteIni->GetKeyValueFromIni(KTEFDefaultScriptPath, tempBuffer);
					if (tempBuffer.CompareF(KNull) != 0)
						{
						scriptFilePath.Set(tempBuffer);
						TScriptName tempStoreScriptPath(scriptFilePath);
						tempStoreScriptPath.Append(tempStore);
						scriptFilePath.Set(tempStoreScriptPath);
						TBuf<KMaxTestExecuteNameLength> scriptFileLocation(scriptFilePath);
						CScriptMaster* scriptMaster = new (ELeave) CScriptMaster(scriptFilePath,*tefLogger, consoleLogger, iDefaultSysDrive, testSysDrive, selTestingOptions);
						// To kick the state machine of the script master off -
						// Call the kick method which jumps us into the RunL() of the CScriptMaster class
						// CScriptMaster is the top AO in the hierarchy.
						scriptMaster->Kick();
						// Enter the Active Scheduler                        
						aScheduler->Start();
						
						commentedCommandsCnt=CScriptControl::commentedCommandsCount;
						if(commentedCommandsCnt==-1)
							{
							CScriptControl::commentedCommandsCount=0;
							}
							
						// Cleanup
						delete scriptMaster;
						}
					}
				}
			}
		delete selTestingOptions;
		
		TInt commandsCount = CScriptControl::commentedCommandsCount;
		TInt countOfRemotePanics = 0;

		// Stop and Process the test watcher object for extracting panic informations and print them to the log files
		if (remotePanicDetection != 0)
			{
			testWatcher->Stop();
			countOfRemotePanics = testWatcher->iSharedData->iPanicDetails.Count();
			if (countOfRemotePanics > 0)
				{
				tefLogger->LogExtra((TText8*)__FILE__,__LINE__,ESevrErr,
					_L("The panic detection thread detected %d panics:"), countOfRemotePanics);
				for (TInt count = 0; count < countOfRemotePanics; count++)
					tefLogger->LogExtra((TText8*)__FILE__,__LINE__,ESevrErr,_L("Remote Panic %d - Name of Panicked Thread: %S, Exit Reason: %d, Exit Category %S"), count+1, &(testWatcher->iSharedData->iPanicDetails)[count]->iThreadName,(testWatcher->iSharedData->iPanicDetails)[count]->iReason,&(testWatcher->iSharedData->iPanicDetails)[count]->iCategory);
				}
			delete testWatcher;
			}

		// Call the Termination routine for logging within the interface
		tefLogger->TerminateLoggingL(commandsCount, countOfRemotePanics, CScriptControl::iRunScriptFailCount);
		CleanupStack::Pop(tefLogger);
		delete tefLogger;
		}
	
	if(iCfg_tcs != EOFF)
		{
		CleanupStack::PopAndDestroy(selTestCfgFileData);
		selTestCfgFileData = NULL;
		}
	
	// Close the parser instance if it is active
	CleanupStack::PopAndDestroy(console);
	if (parseTestExecuteIni != NULL)
		{
		CleanupStack::PopAndDestroy(parseTestExecuteIni);
		}
	
	if (scriptFilePath.CompareF(KNull)!=0)
		{
		// Wait for flogger to write to file
		_LIT(KHtmExtension,".htm");
		TParse fileNameParse;
		fileNameParse.Set(scriptFilePath, NULL, NULL);
		
		TPtrC fileName = fileNameParse.Name();
		htmlLogPath.Append(fileName);
		htmlLogPath.Append(KHtmExtension);

		__RDEBUG( (_L("Log file path--> %S"), &htmlLogPath) );
		
		RFs fs;
		fs.Connect();
		TBool answer = EFalse;
		while(ETrue)
			{
			TInt err = fs.IsFileOpen(htmlLogPath, answer);
			if ((KErrNone==err&&!answer) || KErrNotFound==err || KErrPathNotFound==err) 
				{
				break;
				}
			User::After(100000);
			}
		
		if (waitForLoggingTime > 0)
			{
			User::After(waitForLoggingTime*1000000);
			}
		fs.Close();
		}				
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CTEFTestModule::CreateCfgDataFromFileL
// ParseTestFrameWorkCfg is used to Read the Cfg File data into a heap buffer
// And populate the arrays of selective test case IDs and 
// Ranges to be used by the state machine.
// NOTE: we do not support nested cfgs...
// Returns: void
// -----------------------------------------------------------------------------
//
void CTEFTestModule::CreateCfgDataFromFileL(TPtrC& aCfgFilePath,RArray<TRange>& aSelectiveCaseRange, TDesC*& aSelTestCfgFileData)
	{
	RFs fS;
	User::LeaveIfError(fS.Connect());
	CleanupClosePushL(fS);
	RFile cfgFile;
	User::LeaveIfError(cfgFile.Open(fS,aCfgFilePath,EFileRead | EFileShareAny));
	CleanupClosePushL(cfgFile);
	TInt fileSize;
	User::LeaveIfError(cfgFile.Size(fileSize));
	// Create a 16bit heap buffer
	HBufC* cfgData = HBufC::NewL(fileSize);
	CleanupStack::PushL(cfgData);
	HBufC8* narrowData = HBufC8::NewL(fileSize);
	CleanupStack::PushL(narrowData);
	TPtr8 narrowPtr=narrowData->Des();
	// Read the file into an 8bit heap buffer
	User::LeaveIfError(cfgFile.Read(narrowPtr));
	TPtr widePtr(cfgData->Des());
	// Copy it to the 16bit buffer
	widePtr.Copy(narrowData->Des());
	CleanupStack::PopAndDestroy(narrowData);
	CleanupStack::Pop(cfgData);
	CleanupStack::Pop(2);
	cfgFile.Close();
	fS.Close();
	// Set up the instance token parser
	TLex cfgLex = cfgData->Des();
	aSelTestCfgFileData = cfgData; // to preserve the pointer of cfgdata and transfer the ownership to aSelTestCfgFileData 
	cfgData = NULL; // relinquish the ownership
	while(!cfgLex.Eos())
		{
		DistinguishElement(cfgLex.NextToken(),aSelectiveCaseRange) ; 
		}
	}

// -----------------------------------------------------------------------------
// CTEFTestModule::ParseTcsDataForSelectiveTestingOptions
// DistinguishElement is used to parse TcsData from configration file to select test case flag. 
// Get cases Included or Excluded.
// Returns: void
// -----------------------------------------------------------------------------
//
void CTEFTestModule::ParseTcsDataForSelectiveTestingOptions( const TTcsName& aTcsData, 
												CTestExecuteIniData& aParseTestExecuteIni,
												RArray<TRange>& aSelectiveCaseRange,  
												TDesC*& aSelTestCfgFileData )
	{
	TPtrC argument(aTcsData) ; 
	while(argument.Length() > 0 )	
		{
		TInt commaOccurance = argument.FindC(KTEFComma) ;
		TPtrC element ; 
		if(commaOccurance == -1)	
			{
			//handle the case where no comma is found, 
			//assume only one item in the list
			element.Set(argument) ; 
			//reset argument to break the while
			argument.Set(KNull);
			}
		else
			{
			element.Set(argument.Left(commaOccurance));
			//take the remaining in the argument
			TInt len = argument.Length()-commaOccurance-1 ; 
			argument.Set(argument.Right(len)) ; 	
			}
		
		TInt cfgExtensionOccurance = element.FindC(KTEFCfgExtension) ; 
		if (cfgExtensionOccurance!=KErrNotFound) //madatory extension to be given
			{
			
			TPtrC cfgFilePath(element);
			//its probably the cfg file path.
			TScriptName tempScriptPath(cfgFilePath);
			// Check whether cfg name is provided along with folder path in the command line
			// If not, take the path from testexecute.ini & name from the command line
			if(cfgFilePath.FindC(KTEFColon)==KErrNotFound)
				{
				TBuf<KMaxTestExecuteNameLength> tempBuffer;
				aParseTestExecuteIni.GetKeyValueFromIni(KTEFDefaultScriptPath, tempBuffer);
				cfgFilePath.Set(tempBuffer);
				// Construct the full file path from the values extracted from command line and ini file
				TScriptName storeScriptPathTemp(cfgFilePath);
				storeScriptPathTemp.Append(tempScriptPath);
				cfgFilePath.Set(storeScriptPathTemp);
				tempScriptPath.Copy(cfgFilePath);
				}
				// and parse this cfg File to populate our structures
				TRAP_IGNORE(CreateCfgDataFromFileL(cfgFilePath,aSelectiveCaseRange, aSelTestCfgFileData));
				//actually do nothing with the error...
			}
		else
			{
			DistinguishElement(element,aSelectiveCaseRange  );
			}
		}
	}
// -----------------------------------------------------------------------------
// CTEFTestModule::DistinguishElement
// DistinguishElement is used to get the element from configration
// Returns: void
// -----------------------------------------------------------------------------
//
void CTEFTestModule::DistinguishElement(const TPtrC& aElement, RArray<TRange>& aSelectiveCaseRange)
{

	TInt colonOccurance = aElement.FindC(KTEFColon);
	//we are expecting only a range or a test case ID over here...
	if( colonOccurance!=KErrNotFound )
		{
		//then this is a range of testcases, split it at the colon
		TRange newRange(aElement.Left(colonOccurance),aElement.Mid(colonOccurance+1));
		aSelectiveCaseRange.Append(newRange);
		}
	else
		{
		TRange newRange(aElement,aElement);
		aSelectiveCaseRange.Append(newRange);	
		}
}

// -----------------------------------------------------------------------------
// CTEFTestModule::ParseStackSize
// Get stack size set by user, in TestFramework.ini.
// Returns: TInt. System wide error code.
// -----------------------------------------------------------------------------
//
TInt CTEFTestModule::ParseStackSizeL(TInt& aStackSize)
	{
	//Parse from testframwork to get stack size set of TEFTestModule.
	//If not found, default stack size is 28672.
    CStifParser* parser = NULL; 
    TRAPD( err, 
           parser = CStifParser::NewL( _L(""), 
                                       _L("c:\\TestFramework\\TestFramework.ini"), 
                                       CStifParser::ECStyleComments ) );
    if( err != KErrNone )
        {
        __RDEBUG( (_L("Error: TestFramework.ini not found")));
        return err;
        }
    CleanupStack::PushL( parser );
    CStifSectionParser* section = NULL;
    _LIT( KModuleStartTag, "[New_Module]" );
    _LIT( KModuleEndTag, "[End_Module]" );
    section = parser->SectionL( KModuleStartTag, KModuleEndTag );
    if( section == NULL )
        {
        CleanupStack::PopAndDestroy( parser );
        parser = NULL;
        return KErrNone;
        }
    while(section)
    	{
        // Parse section
        CleanupStack::PushL( section );
            
        CStifItemParser* item = NULL; 
        // Get ModuleName from configuration file.
        TRAPD( getModuleName, item = section ->GetItemLineL(_L( "ModuleName=" ), ENoTag));
        if( getModuleName != KErrNone )
            {
            __RDEBUG( (_L("Error: From file TestFramework.ini, get item 'ModuleName=' leaves with error: %d."),getModuleName) );
            delete item;
            item=NULL;
            CleanupStack::PopAndDestroy( section );
            section = NULL;
            CleanupStack::PopAndDestroy( parser );
            parser = NULL;
            return getModuleName;
            }
        if( !item )
            {
            delete item;
            item=NULL;
            CleanupStack::PopAndDestroy( section );
            section = NULL;
            CleanupStack::PopAndDestroy( parser );
            parser = NULL;
            return KErrNone;
            }
        else
        	{
        	TPtrC moduleName;
        	item->GetString( _L( "" ), moduleName );
        	HBufC* moduleNameHbuf = moduleName.AllocL();
        	TPtr modifiableString = moduleNameHbuf->Des();
        	modifiableString.UpperCase();
        	
        	if( modifiableString == _L( "TEFTESTMODULE" ) )
	        	{
	        	CStifItemParser* itemStackSize = NULL; 
	        	TRAPD( getStackSize, itemStackSize = section ->GetItemLineL(_L( "StackSize=" ), ENoTag));
	        	if( getStackSize != KErrNone )
    	            {
    	            __RDEBUG( (_L("Error: From file TestFramework.ini, get item 'getStackSize=' leaves with error: %d."),getStackSize) );
    	            delete item;
    	            item=NULL;
    	    		delete moduleNameHbuf;
    	    		moduleNameHbuf=NULL;
    	            CleanupStack::PopAndDestroy( section );
    	            section = NULL;
    	            CleanupStack::PopAndDestroy( parser );
    	            parser = NULL;
    	            return getStackSize;
    	            }
    	        if( !itemStackSize )
    	            {
    	    		delete item;	  
    	    		item=NULL;
    	    		delete moduleNameHbuf;
    	    		moduleNameHbuf=NULL;
    	            CleanupStack::PopAndDestroy( section );
    	            section = NULL;
    	            CleanupStack::PopAndDestroy( parser );
    	            parser = NULL;
    	            return KErrNone;
    	            }
    	        else
    	        	{
    	        	itemStackSize->GetInt( _L( "" ), aStackSize );
    	        	CleanupStack::PopAndDestroy( section );
    	        	section = NULL;
    	        	delete itemStackSize;
    	        	itemStackSize = NULL;
    	        	}
	        	}
        	else
	        	{
	        	CleanupStack::PopAndDestroy(section);
	        	section = NULL;
	        	section = parser->NextSectionL(KModuleStartTag, KModuleEndTag);
	        	}
        	
    		delete moduleNameHbuf;
    		moduleNameHbuf=NULL;
    		delete item;	  
    		item=NULL;
        	
        	}
    	}
    CleanupStack::PopAndDestroy( parser );
    return KErrNone;
	}
// -----------------------------------------------------------------------------
// CTEFTestModule::StartSystemL
// DistinguishElement is used to load and wait to start. 
// StartSystemL() will be build and called only when Lite version is NOT defined.
// Returns: void
// -----------------------------------------------------------------------------
//

#if !(defined TEF_LITE)
void CTEFTestModule::StartSystemL(void)
	{	
	CActiveScheduler* theScheduler = new (ELeave) CActiveScheduler();
	CleanupStack::PushL(theScheduler);
	CActiveScheduler::Install(theScheduler);
	
	RLibrary pluginLibrary;
	CWrapperUtilsPlugin* plugin = TEFUtils::WrapperPluginNew(pluginLibrary);
	
	if (plugin!=NULL)
		{
		CleanupStack::PushL(plugin);
		plugin->WaitForSystemStartL();
		CleanupStack::PopAndDestroy(plugin);
		pluginLibrary.Close();
		}
	else
		{
		User::Leave(KErrGeneral);
		}
	
	CleanupStack::PopAndDestroy(theScheduler);
	}
#endif

//  End of File
