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
* Description: This file contains the header file of the CTestReport.
*
*/

#ifndef TEF_TEST_REPORT_H
#define TEF_TEST_REPORT_H

//  INCLUDES
#include <e32std.h>
#include <e32base.h>

const TInt KTEFMaxResultDes = 128; // Maximum test result description length
typedef TBuf<KTEFMaxResultDes> TTEFResultDes;

// Different StifLogger presentations types.
enum TTEFTestReportLoggerType
    {
    ETEFTestReportLoggerType_Txt,       // txt file logging
    ETEFTestReportLoggerType_Data,      // data file logging(e.g. web page downloading )
    ETEFTestReportLoggerType_Html       // html file logging
    };

// Different StifLogger output modules.
enum TTEFTestReportOutput
    {
    ETEFTestReportOutput_File,      // File logging
    ETEFTestReportOutput_RDebug     // RDebug logging
    };

// TTestCaseInfo defines the information for test case execution.
class TTEFTestCaseInfo
    {
    public:
        // Default test case priority values
        enum TPriority
            {    
            EPriorityLow = -100,
            EPriorityNormal = 0,
            EPriorityHigh = 100,
            };

    public:

        /**
        * Default constructor of TTestCaseInfo.
        */
        inline TTEFTestCaseInfo()
        {
        iCaseNumber = -1;
        iTitle.Zero();
        iTimeout = 0;
        iPriority = EPriorityNormal;
        };

    public:

        // Test case number.
        TInt iCaseNumber;
        // Test case title.
        TName iTitle;
        // Test Case Timeout ( 0 == No timeout )
        TTimeIntervalMicroSeconds iTimeout; 
        // Test case priority
        TInt iPriority;

    };

// TTestResult defines the test case result information.
class TTEFTestResult 
    {
    public:

        /**
        * Default constructor of TTestResult.
        */
        inline TTEFTestResult()
        {
            iResult = 0;
            iResultDes.Zero();
        };

        /**
        * Sets result and result description of the test case.
        */
        inline void SetResult( TInt aResultCode, const TDesC& aResultDes )
        {
            iResult = aResultCode;
            // If description is too length then cut.
            if( aResultDes.Length() > KTEFMaxResultDes )
                {
                iResultDes = aResultDes.Left( KTEFMaxResultDes );
                }
            else
                {
                iResultDes = aResultDes;
                }
        };

    public:

        // Test case result as Symbian OS error code.
        TInt iResult;
        // Descriptive test case result.
        TTEFResultDes iResultDes;
    };

// TTestInfo defines the information for test case and Test Module.
class TTEFTestInfo 
    {
    public:
        inline TTEFTestInfo()
        {
            iModuleName.Zero();
            iConfig.Zero();
        };
    public:
        // The name of Test Module.
        TName iModuleName;
        // Test case info for test case execution.
        TTEFTestCaseInfo iTestCaseInfo;
        // Config file to be used.
        TFileName iConfig;
    };

// TFullTestResult defines the full test case result information.
class TTEFFullTestResult
    {
    public:
        inline TTEFFullTestResult()
        {
            iStartTime=0;
            iEndTime=0;
        };

        enum TCaseExecutionResult
        {
            ECaseOngoing,
            ECaseExecuted,
            ECaseCancelled,
            ECaseErrorFromModule,
            ECaseLeave,
            ECasePanic,
            ECaseException,
            ECaseTimeout,
            ECaseSuicided,
        };

    public:
        // Test case result.
        TTEFTestResult iTestResult;
        // Timestamp when test case was started.
        TTime iStartTime;
        // Timestamp when test case was ended.
        TTime iEndTime;
        
        // Test result type
        TCaseExecutionResult iCaseExecutionResultType;      
        // Test result code
        TInt iCaseExecutionResultCode;
    };

        
// Struct to Test report settings
class TTEFTestReportSettings
    {
    public:
        
        TTEFTestReportSettings(){ iCreateTestReport = ETrue;
                                iPath = NULL;
                                iName = NULL;
                                iFormat = ETEFTestReportLoggerType_Txt;
                                iOutput = ETEFTestReportOutput_File;
                                iOverwrite = ETrue; 
                                iXML = EFalse;
                             }

       ~TTEFTestReportSettings(){ delete iPath; delete iName; }

    public:

        TBool                   iCreateTestReport; // Test report creation mode
        HBufC*                  iPath;             // Test report path
        HBufC*                  iName;             // Test report name
        TTEFTestReportLoggerType iFormat;          // Test report file type
        TTEFTestReportOutput    iOutput;           // Test report output
        TBool                   iOverwrite;        // Test report overwrite
        TBool                   iXML;              // Test report in xml format

    };


// DESCRIPTION
// Class used to store version information of test module.
// iTestModuleName field keeps name of test module dll
// iVersion keeps version of test module dll
        
    struct TTEFTestModuleVersionInfo
    {
        TFileName iTestModuleName;
        TInt iMajor;
        TInt iMinor;
        TInt iBuild;
    };

    
// CONSTANTS
// Maximum length for date or time definitions 'Thursday 06th November 2003'
const TInt KMaxDateAndTimeStamp = 30;

// MACROS
// None

// DATA TYPES
// None

// FUNCTION PROTOTYPES
// None

// FORWARD DECLARATIONS


// CLASS DECLARATION
// None

// DESCRIPTION
// CTEFTestReport is a class that can generate different test reports.
class CTEFTestReport
        : public CBase
    {
    public:     // Enumerations

        // TTestReportMode defines the mode of test report
        enum TTestReportMode
            {
            ETestReportBlank = 0x00000000,
            ETestReportSummary = 0x00000001,
            ETestReportEnvironment = 0x00000002,
            ETestReportCases = 0x00000004,
            ETestReportFull = ETestReportSummary | ETestReportEnvironment | ETestReportCases
            };

        // TTestReportOutput defines the type of test report output
        enum TTestReportOutput
            {
            ETestReportNone = 0x00000000,
            ETestReportDebug = 0x00000001,
            ETestReportPlainText = 0x00000002,
            ETestReportHtml = 0x00000004,
            ETestReportAll = ETestReportDebug | ETestReportPlainText | ETestReportHtml
            };

        enum TStyle
            {
            ENoStyle    = 0x00000,  // No style parameter
            EBold       = 0x00001,  // Bold
            ECursive    = 0x00002,  // Cursive
            EUnderline  = 0x00004,  // Underline
            EBlue       = 0x00008,  // Blue
            EGreen      = 0x00010,  // Green
            ERed        = 0x00020,  // Red
            EImportant  = 0x00040,  // Important (Line will starts a 'IMPORTANT' text)
            EWarning    = 0x00080,  // Warning (Line will starts a 'WARNING' text)
            EError      = 0x00100   // Error (Line will starts a 'ERROR' text)
            };
        
    public:     // Structured classes

        /**
        *  TTestReportHeader includes header information for test report.
        *
        */
        struct THwInfo
            {
            TInt iManufacturer;
            TInt iMachineUid;
            TInt iModel;
            TInt iHwRev;
            TInt iCpu;
            TInt iCpuSpeed;
            TInt iLanguage;
            };

        struct TSwInfo
            {
            TInt iSwRev;
            TInt iSwBuild;
            };

        struct TMemoryInfo
            {
            TInt iRAM;
            TInt iRAMFree;
            };

        class TTestHWInfo
            {
            public: // New functions
                TTestHWInfo();

            public: // Data
                // HW Info
                THwInfo         iHwInfo;
                // SW Info
                TSwInfo         iSwInfo;
                // Memory Info
                TMemoryInfo     iMemoryInfo;
            };

        /**
        *  TTestCaseSummary includes information for summary of test cases.
        */
        class TTestCaseSummary
            {
            public:     // New functions
                TTestCaseSummary();

            public:     // Data
                // Test Info
                TTEFTestInfo           iTestInfo;
                // Test Result
                TTEFFullTestResult     iFullTestResult;
            };

        /**
        *  TTestSummary includes information for test report summary.
        */
        class TTestSummary
            {
            public:     // New functions
                TTestSummary( const TName& aName );

            public:     // Data
                // Name (e.g. test module name )
                const TName     iName;
                // Count of passed test cases
                TInt            iPassedCases;
                // Count of failed test cases
                TInt            iFailedCases;
                // Count of crashed cases
                TInt            iCrashedCases;
                // Count of timed out cases
                TInt            iTimeoutCases;
            };

    private:    // Enumerations

    public:     // Constructors and destructor
        
        /**
        * Two-phased constructor.
        */
        static CTEFTestReport* NewL( TTEFTestReportSettings& aTestReportSettings,
                                  const TTestReportMode aReportMode );
        
        /**
        * Destructor.
        */
        ~CTEFTestReport();
        
    public:     // New functions
        
        /**
        * Add test case result.
        */
        void AddTestCaseResultL( const TTEFTestInfo& aTestInfo,
                                 const TTEFFullTestResult& aTestResult,
                                 const TInt aError );

        /**
        * Generate result reports.
        */
        void GenerateReportL();
        
        /**
        * Update result reports.
        */
        void UpdateReportSummaryL();
        
        /**
         * Adds version of test module to RPointerArray
         */
        void AddTestModuleVersion(TTEFTestModuleVersionInfo& aVersion);

    public:     // Functions from base classes
        
    protected:  // New functions

    protected:  // Functions from base classes

    private:

        /**
        * By default Symbian OS constructor is private.
        */
        void ConstructL(  TTEFTestReportSettings& aTestReportSettings );

        /**
        * Default C++ constructor.
        */
        CTEFTestReport( const TTestReportMode aReportMode );
        
        /**
        * Write result file header.
        */
        void WriteHeaderL();
        
        /**
        * Write result file trailer.
        */
        void WriteTrailerL();
        
        /**
        * Write data to file.
        */
        void WriteLineL( TRefByValue<const TDesC> aStr,... );
        
        /**
        * Write delimiter line.
        */
        void WriteDelimiterL( const TDesC& aDelimiter, TInt aCount );
        
        /**
        * Adds needed tags to get valid xml file.
        */
        void CloseXMLTagsInUnfinishedFileL(void);

    public:     // Data
    
    protected:  // Data

    private:    // Data

        // Report file handles
        RFs                             iFs;
        RFile                           iFile;
        TTEFTestReportLoggerType        iFormat;
        TTEFTestReportOutput            iOutput;
    
        // Report Mode
        TTestReportMode                 iReportMode;

        // Report HW info
        TTestHWInfo                     iReportHWInfo;

        // Total summary
        TTestSummary*                   iTotalSummary;

        // Test summary array
        RPointerArray<TTestSummary>     iTestSummaries;
        
        // Summary start position
        TInt                            iSummaryPos;
        
        // Versions of test modules position
        TInt                            iModulesVersionsPos;
        
        // Test modules versions
        RPointerArray<TTEFTestModuleVersionInfo> iTestModulesVersionsInfo;

        // Will report be written in xml format 
        TBool                           iXML;

    public:     // Friend classes

    protected:  // Friend classes

    private:    // Friend classes

    };

#endif // TEF_TEST_REPORT_H

// End of File
