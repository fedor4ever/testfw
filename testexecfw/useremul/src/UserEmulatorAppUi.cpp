/*------------------------------------------------------------------
 -
 * Software Name : UserEmulator
 * Version       : v4.2.1309
 * 
 * Copyright (c) 2009 France Telecom. All rights reserved.
 * This software is distributed under the License 
 * "Eclipse Public License - v 1.0" the text of which is available
 * at the URL "http://www.eclipse.org/legal/epl-v10.html".
 *
 * Initial Contributors:
 * France Telecom 
 *
 * Contributors:
 *------------------------------------------------------------------
 -
 * File Name: UserEmulatorAppUi.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Application User Interface class
 *------------------------------------------------------------------
 -
 *
 */

// System Includes
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <e32property.h>
#include <APGCLI.H>
#include <aknlists.h> 
#include <utf.h>
#include <hal.h>
#include <APGWGNAM.H>
#include <eiklbv.h>
#include <aknglobalnote.h>
#include <BACLINE.H>
// User Includes
#include <UserEmulator_0x2001C3AF.rsg>
#include "UserEmulator_0x2001C3AF.hlp.hrh"
#include "UserEmulator.hrh"
#include "UserEmulatorApplication.h"
#include "UserEmulatorAppUi.h"
#include "Utils.h"
#include "SettingsDialog.h"
#include "clientdll.h"
#include "clientcommander.h"

// Local constants
_LIT(KAPPLAUNCH, "app, ");
_LIT(KVIEWSWITCH, "view, ");
_LIT(KKEYPRESS, "keypress, ");
_LIT(KORIENTATION,"orientation, ");
_LIT(KSCREENRESET,"screenreset, ");
_LIT(KPOINTEREVENT, "pointerevent, ");
_LIT(KWAIT, "wait, ");
_LIT(KCLOSEAPP, "closeapp, ");
_LIT(KRUNERROR, "A script or test is already running!");

_LIT(KButtonDown,"pointerdownAt");
_LIT(KButtonUp,"pointerupAt");
_LIT(KPointerMove,"moveTo");

// Orange World menu
#define		HS_VIEW_OSS_BROWSER_APPUID					0x10008d39
// Uid of the Aactive Idle framework (3.1 devices)
const TUid KUidStandBy1 = { 0x101fd64c };
// Uid of the Aactive Idle framework (3.2 devices)
const TUid KUidStandBy = { 0x102750f0 };
// Uid of the Active Idle framework (5.0 devices)
const TUid KUidStandby50 = {0x20017CD9};

const TInt KArrayGranularity = 10;

_LIT(KAppNotFound, "ERROR: Application Not found ");
_LIT(KStartTest,"*** Start Test ***" );
_LIT(KEndScript, "*** End of Script:  " );
_LIT(KViewSwitchingError, "ERROR: View not found");
_LIT(KStopScript,"*** Cancelled script: ");

_LIT(KStringLit,"%S\n");

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::ConstructL()
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::ConstructL()
	{
	iIsFolderScanCompletedCalled = EFalse;
	
	// Initialise app UI with standard value.
	BaseConstructL(CAknAppUi::EAknEnableSkin | EAknEnableMSK );
	
	iCommandLineScriptsArray = new (ELeave) CDesC8ArrayFlat(KArrayGranularity);

    iSettings = new(ELeave) CSettings(*Application(), iEikonEnv);
    iSettings->ConstructL();
   
    iScriptsView =  CUserEmulatorScriptsView::NewL(ClientRect(),iSettings, iEikonEnv);
    AddToStackL(iScriptsView);
    
    iLogger = new(ELeave) CLogger(*iScriptsView,*iSettings, iEikonEnv);
    iLogger->ConstructL();
    
    // Create an instance of XML handler.
	iXmlHandler = CXmlHandler::NewL( *this ,iActionList);
	
	iDelayTimer = CDelayTimer::NewL(*this);
	User::LeaveIfError(iSession.Connect());
	
	// Initialise Window server dll's client side
    SetupClientDllL();
    SetupCommander();
    
    HAL::Get(HAL::EManufacturer, iManufacturer);
	
	iScriptRun = EFalse;
	iRecording = EFalse;
	iRandomTestStarted = EFalse;
	
	iGlobalCapturer = CGlobalCapturer::NewL(*this, *iSettings, iScriptRun, iRandomTestStarted,
														iClientCommander, iEikonEnv,iSession);
	iGlobalCapturer->StartCapturingL();
	
	iThreadNotifier = CThreadNotifier::NewL(*this, *iLogger,*iSettings,iEikonEnv);
	iThreadNotifier->IssueRequest();

	iRandomTest = NULL;	
}
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::CUserEmulatorAppUi()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CUserEmulatorAppUi::CUserEmulatorAppUi(RWsSession& aSession): CAknAppUi(),iClientDll( aSession ),
											iClientCommander( iClientDll ),
											iActionIndex(0),iLoopExists(EFalse),
											iStopScripts(EFalse),iSelectedIndex(0),iError(EFalse),
											iLoopCounter(0),iSelectedScriptCount(0),
											iPointerCount(0)
											
{// No implementation required
}

// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::~CUserEmulatorAppUi()
// Destructor.
// -----------------------------------------------------------------------------
//
CUserEmulatorAppUi::~CUserEmulatorAppUi()
{
	iClientCommander.Close ();
    iClientDll.Close();
    
	delete iXmlHandler;
	delete iDelayTimer;
	iSession.Close();
	delete iSettings;
	
	if(iRandomTest)
	{
		delete iRandomTest;
		iRandomTest = NULL;
	}
	
	iActionList.ResetAndDestroy();
	iActionList.Close();
	
	iNestedLoopArray.Close();
	if(iCommandLineScriptsArray)
	{
	   iCommandLineScriptsArray->Reset();
	   delete iCommandLineScriptsArray;
	}
	iScriptErrorIndexes.Close();
	
	delete iThreadNotifier;
	delete iGlobalCapturer;
	delete iLogger;
	
	if(iScriptsView)
	{
		RemoveFromStack(iScriptsView);
		delete iScriptsView;
	}	
}
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::TerminationKeyReceived()
// Stop script run after Power button press
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::TerminationKeyReceived()
    {
    iScriptRun = EFalse;
    if(iActionList.Count() > 0)
        iStopScripts = ETrue;
    }

// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::BringUserEmulatorToFront()
// Stops recording and brings this application to foreground
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::BringUserEmulatorToFront()
    {
    iRecording = EFalse;
    BringToForeground();
    }
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::ShowScriptsL()
// Calls SCript View's ShowScriptsL function
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::ShowScriptsL()
{
	iScriptsView->ShowScriptsL();
	
	//UnMark all the scripts
	iScriptsView->UnMarkL(EAknUnmarkAll);
}
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::SetupClientDllL()
// Loads Window server dll's server side
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::SetupClientDllL()
    {
    // Create the server dll filename
    TFileName serverDllFilename( KServerDllFilename );

    // Load the animation server, if an error occurs then
    // let higher level handle the problem
    User::LeaveIfError( iClientDll.Load( serverDllFilename ) );
    }

// -----------------------------------------------------------------------------
// CAnimationAppUi::SetupImageCommander()
// Setup the client side commander.
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::SetupCommander()
    {
    // Tell client to construct a server side object
    TBuf8<1> params( TInt8( 0 ) );
    RWindowBase* windowBase = iScriptsView->DrawableWindow();
    iClientCommander.AnimConstruct( *windowBase,
										0,
                                     params );
    }
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::DoCmdSettingsL()
// Displays Settings dialog
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::DoCmdSettingsL()
{
    TInt button = 0;
    CSettingsDialog*  dlg = new(ELeave) CSettingsDialog(*iSettings);
    button = dlg->ExecuteLD(R_DIALOG_SETTINGS);
}
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::DynInitMenuPaneL
// Set context specific menu
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane* aMenuPane)
{
	if(R_MARKUNMARK_SUBMENU == aResourceId)
		{
				iScriptsView->iListBoxSelectionFlag = iScriptsView->CheckListBoxSelection();

				if(iScriptsView->iListBoxSelectionFlag==EOneItemSel)
					{
					aMenuPane->SetItemDimmed(EMark, ETrue);
					aMenuPane->SetItemDimmed(EMarkAll, ETrue);
					aMenuPane->SetItemDimmed(EUnmarkAll, ETrue);
					}
				if(iScriptsView->iListBoxSelectionFlag==EOneItem)
					{
					aMenuPane->SetItemDimmed(EUnmark, ETrue);
					aMenuPane->SetItemDimmed(EMarkAll, ETrue);
					aMenuPane->SetItemDimmed(EUnmarkAll, ETrue);
					}
				if(iScriptsView->iListBoxSelectionFlag==EMoreItemsSel)
					aMenuPane->SetItemDimmed(EMark, ETrue);
				if(iScriptsView->iListBoxSelectionFlag==EMoreItems)
					{
					aMenuPane->SetItemDimmed(EUnmark, ETrue);
					aMenuPane->SetItemDimmed(EUnmarkAll, ETrue);
					}
				if(iScriptsView->iListBoxSelectionFlag==EMoreItemsAll)
					{
					aMenuPane->SetItemDimmed(EMark, ETrue);
					aMenuPane->SetItemDimmed(EMarkAll, ETrue);
					}
				if(iScriptsView->iListBoxSelectionFlag==EMoreItemsAllExc)
					aMenuPane->SetItemDimmed(EUnmark, ETrue);
				if(iScriptsView->iListBoxSelectionFlag==EMoreItemsNotAll)
					aMenuPane->SetItemDimmed(EMark, ETrue);
				if(iScriptsView->iListBoxSelectionFlag==EMoreItemsNotAllNotCurrent)
					aMenuPane->SetItemDimmed(EUnmark, ETrue);
		}
    if (R_MENU == aResourceId)
    {
		iScriptsView->iListBoxSelectionFlag = iScriptsView->CheckListBoxSelection();
    
		if(iRandomTestStarted || iRecording || iScriptRun)
    	{
    		aMenuPane->SetItemDimmed(ERandomTestStart, ETrue);
			aMenuPane->SetItemDimmed(ESelectRunScript, ETrue);
			aMenuPane->SetItemDimmed(ERecordKeys, ETrue);
			aMenuPane->SetItemDimmed(ESettings,ETrue);
    	}
    	
		if (iScriptsView->GetScriptsCount() == 0)
		{
			aMenuPane->SetItemDimmed(ESelectRunScript, ETrue);
			aMenuPane->SetItemDimmed(EMenuSelectCmdId, ETrue);
		}
	
		// Order of this 'SetItemDimmed' must not be changed within this function
		if(iScriptsView->iListBoxSelectionFlag==0)
			aMenuPane->SetItemDimmed(EMenuSelectCmdId, ETrue);
    }    
}
// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::StartTestL()
// Run the test script
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::StartTestL()
{
	_LIT(KTESTLOG,"");
	iLogger->WriteLogL(KTESTLOG,ETrue, ETrue);
	iScriptRun = ETrue;
	TInt errcode;
	TInt err=iScriptsView->RunScriptsL(errcode);
	if(err!=KErrNone)
		{	
		iScriptRun = EFalse;
		_LIT(KSCRIPTREADERROR,"Script file reading error. Drive might be locked or file name error");
		CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);					
		errorNote->ExecuteLD(KSCRIPTREADERROR);
		}
	iGlobalCapturer->StartL();
}

// -----------------------------------------------------------------------------
// CUserEmulatorAppUi::HandleCommandL()
// Function called by the framework to handle all the commands from Menu option
// -----------------------------------------------------------------------------
//
void CUserEmulatorAppUi::HandleCommandL(TInt aCommand)
{
	switch (aCommand)
	{
		case ESelectRunScript:
		{
			if(!iScriptRun && !iRandomTestStarted && !iRecording)
			{
				//Select and run the test script
				iScriptRun = ETrue;
				iScriptsView->HandleMarkCommandL(EAknCmdMark);
				TInt errcode;
				TInt err=iScriptsView->RunScriptsL(errcode);
				
				if(errcode!=KErrNone)
				    iScriptRun = EFalse;
				
				if(err!=KErrNone)
				{	
					iScriptRun = EFalse;
					_LIT(KSCRIPTREADERROR,"Script file reading error. Drive might be locked or file name error.");
					CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);					
					errorNote->ExecuteLD(KSCRIPTREADERROR);
				}
				else
				{
				    iGlobalCapturer->StartL();  
				}
			}
			else
			{
				CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
				errorNote->ExecuteLD(KRUNERROR);
			}
			    
		}
		break;
		case ERecordKeys:
		{
			iScriptsView->UnMarkL(EAknUnmarkAll);
			if(!iRandomTestStarted)
			{
				if(iGlobalCapturer->StartRecordKeysL()==0)
					iRecording = ETrue;
				else
					iRecording = EFalse;
			}
			else
			{
				CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
				errorNote->ExecuteLD(KRUNERROR);
			}
		}
		break;
		case ERandomTestStart:
		{      
			StartRandomTestL();
		}
		break;	
    	case ESystemInfo:
    		iScriptsView->CmdGetSystemInfoL(EFalse);
        break;
    	        
    	case EMark:
    	case EAknCmdMark:
    		iScriptsView->HandleMarkCommandL(EAknCmdMark);
    		break;
    	case EMarkAll:
    	case EAknMarkAll:
    		iScriptsView->HandleMarkCommandL(EAknMarkAll);
	    break;
	    
    	case EUnmark:
    	case EAknCmdUnmark:
    		iScriptsView->UnMarkL(EAknCmdUnmark);
    	break;
    	case EUnmarkAll:
    	case EAknUnmarkAll:
    		iScriptsView->UnMarkL(EAknUnmarkAll);
	    break;
		     
		case ESettings:	
			DoCmdSettingsL();
		break;
				
		case EEikCmdExit:
		case EAknSoftkeyExit:
			if(!iScriptRun && !iRandomTestStarted && !iRecording)
				Exit();
			break;

		case EHelp:
		{
	       CArrayFix<TCoeHelpContext>* buf = CCoeAppUi::AppHelpContextL();
		   HlpLauncher::LaunchHelpApplicationL(iEikonEnv->WsSession(), buf);
			
		}
		break;
		
		case EAbout:
		{
			CAknMessageQueryDialog* dlg = new (ELeave)CAknMessageQueryDialog();
			dlg->PrepareLC(R_ABOUT_QUERY_DIALOG);
			HBufC* title = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TITLE);
			dlg->QueryHeading()->SetTextL(*title);
			CleanupStack::PopAndDestroy(); //title
			HBufC* msg = iEikonEnv->AllocReadResourceLC(R_ABOUT_DIALOG_TEXT);
			dlg->SetMessageTextL(*msg);
			CleanupStack::PopAndDestroy(); //msg
			dlg->RunLD();
		}
		break;
		default:
		
			break;
	}
}

// ----------------------------------------------------------------
// CUserEmulatorAppUi::RefreshFileNotification()
// Refresh script list if there was a change in script folder
// ----------------------------------------------------------------
// 
void CUserEmulatorAppUi::RefreshFileNotification()
	{
	CNotifyFileChange* nfc = iScriptsView->GetNotifyFileChange();
	nfc->RefreshPath(iSettings->iScriptsPath);
	}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::SetApplicationRectInSettings
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::SetApplicationRectInSettings()
	{
	iSettings->iAppRect = ApplicationRect();
	}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::StartRandomTestL()
// Start random test. Displays error message if it fails.
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::StartRandomTestL()
{
	if(!iRecording && !iScriptRun)
	{
		if(!iRandomTest)
		{
			SetApplicationRectInSettings();
			iRandomTest = CRandomTest::NewL(*this,iEikonEnv->WsSession(),*iSettings, iSession,*iLogger);
		}
		
		 _LIT(KRANDOMTEST,"Random test began");
         CAknConfirmationNote* errorNote = new (ELeave) CAknConfirmationNote(ETrue);
         errorNote->ExecuteLD(KRANDOMTEST);
         User::After(100000);
		
		iRandomTestStarted = ETrue;
		
		_LIT(KRANDOMTESTSTART, "***Random test started***\n");
		iLogger->WriteLogL(KRANDOMTESTSTART,EFalse,EFalse);
			
		iGlobalCapturer->StartL();
		iRandomTest->StartRandomTestL();					
	}
	else
	{
		CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);
		errorNote->ExecuteLD(KRUNERROR);
	}
}

// ----------------------------------------------------------------
// CUserEmulatorAppUi::PanicOccured()
// Callback from Thread Notifier when a panic occures
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::PanicOccured()
    {
    if(iRandomTestStarted && iRandomTest)
        iRandomTest->CancelRandomTest();
    }

// ----------------------------------------------------------------
// CUserEmulatorAppUi::HelpContextL()
// Set help context
// ----------------------------------------------------------------
//
CArrayFix<TCoeHelpContext>* CUserEmulatorAppUi::HelpContextL() const
	{
	CArrayFixFlat<TCoeHelpContext>* array = new(ELeave)CArrayFixFlat<TCoeHelpContext>(1);
	CleanupStack::PushL(array);
	array->AppendL(TCoeHelpContext(KUidUserEmulatorApp, KContext1));
	CleanupStack::Pop(array);
	return array;
	}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::OnParseCompletedL
// Callback function. Called at the end of xml script parse
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::OnParseCompletedL( TInt aError , TState aState)
{
	if ( KErrNone == aError )
    {
    	 iScriptErrorIndexes.Append(iSelectedScriptCount);
	     if(aState == EOnEndElement)
	       {
           iPointerCount = 0;
           TRAPD(error, PerformActionL());
           if(error != KErrNone)
               {
               _LIT(KErrorDec,"Error: %d\n");
               TBuf<KBuffer128> buffer;
               buffer.AppendFormat( KErrorDec,error );
               iLogger->WriteLogL(buffer,EFalse);
               return;
               }
         
	       }
    }
	else
    {
    	HBufC* scriptName = iScriptsView->GetScriptNameLC(iSelectedScriptCount);
		if(scriptName)
		{
		    const TInt KLitLength = 38;
			_LIT(KErrorFormat, "Error..Cannot execute script: %S\n");
			
			HBufC* buffer = HBufC::NewLC( scriptName->Length()+ KLitLength );
			TPtr bufferPtr( buffer->Des() );
			bufferPtr.AppendFormat(KErrorFormat,&scriptName->Des());
			iLogger->WriteLogL(*buffer,EFalse);    	
			CleanupStack::PopAndDestroy(); //buffer
		}	
		iScriptsView->ChangeIconL(EScriptFailed,iSelectedScriptCount);
		
		iScriptRun = EFalse;
		TRAPD(err, iScriptsView->HandleMarkCommandL(EAknCmdMark));
	    	
		CleanupStack::PopAndDestroy(); //scriptName
		
    }
	if(++iSelectedScriptCount == iScriptsView->GetSelectedScriptsCount())
		iSelectedScriptCount  = 0;
     
}

// ----------------------------------------------------------------
// CUserEmulatorAppUi::StartParsingL
// Simply calls xml handle StartParsingL function
// ----------------------------------------------------------------
//
TInt CUserEmulatorAppUi::StartParsingL( const TDesC& aFileName )
{
    // Parse XML document without active object.
    return iXmlHandler->StartParsingL( aFileName );
}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::OpenUrlL
// Starts browser with the given url
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::OpenUrlL(const TPtrC& aUrl)
{
	TApaTaskList taskList(iEikonEnv->WsSession());
	TApaTask task = taskList.FindApp(TUid::Uid(HS_VIEW_OSS_BROWSER_APPUID));
	
	if (task.Exists()) // Test whether we've been returned an empty task
	{
		task.BringToForeground();
		HBufC8 *url8 = CnvUtfConverter::ConvertFromUnicodeToUtf8L(aUrl);
		task.SendMessage(TUid::Uid(0), *url8); // UID not used
		delete url8;
	}
	else
	{
		TThreadId thread;
		iSession.StartDocument(aUrl, TUid::Uid(HS_VIEW_OSS_BROWSER_APPUID), thread);
	}
}

// ----------------------------------------------------------------
// CUserEmulatorAppUi::StartJavaApp
// Start a Java app
// ----------------------------------------------------------------
//
TInt CUserEmulatorAppUi::StartJavaApp(const TDesC& aAppName)
{
	TApaAppInfo		appInfo;
	TBool			found = EFalse;

	iSession.GetAllApps();
	_LIT(KFakeApp,".fakeapp");
	const TInt KFALength = 8;
	while (iSession.GetNextApp( appInfo ) == KErrNone)
	{
		if (appInfo.iFullName.Right(KFALength) == KFakeApp)
		{
			if (appInfo.iCaption == aAppName)
			{
				imidletUID = appInfo.iUid;
				found = ETrue;
			}
		}
	}
	
	// We've got the UID of the MIDlet so we can launch it
	if (!found)
	{
		_LIT(KJAVAAPPERROR, "Java Application not found");
		TRAPD(err, CUtils::MessageBoxL(KJAVAAPPERROR));
		return KErrNotFound;
	}
	
	TThreadId threadId;
	return iSession.StartDocument(KNullDesC, imidletUID, threadId);
}
void CUserEmulatorAppUi::RestartRandomTests()
{
     if(iRandomTestStarted && iRandomTest)
        iRandomTest->RestartRandomTest();
}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::PerformNextAction
// Sets timer. When timer expired PerformActionL is called.
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::PerformNextAction(TInt aInterval)
{
     iDelayTimer->Wait(aInterval);	
}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::PerformActionL
// Main worker function. Executes parsed actions form scripts
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::PerformActionL()
{
	CAction *action = NULL;
	
	if(!iActionIndex)
	{
		iLogBuffer.Zero();
		iLogBuffer.Copy(KStartTest);
		iLogBuffer.Append(KNEWLINE); 
		iLogger->WriteLogL(iLogBuffer,EFalse);
	}

	if(iActionList.Count()) //Action list is not NULL
	{
		if(iActionIndex >= iActionList.Count())
		{
			iActionIndex = 0;			
			iActionList.ResetAndDestroy();
			iActionList.Close();			
			iGlobalCapturer->Stop();
			iScriptRun = EFalse;
			return;
		}
		action = iActionList[iActionIndex++];
	}	
	if (!action)
		return;
	
	TInt64 interval = 0;
	TInt   performactiontimer = 10;
	TPtrC  params;
	
	if (action->iParams)
		params.Set(*action->iParams);

    // Execute actions
	switch(action->iType)
	{
		case EActionTypeEndScript:
		{
			TInt status = 0;
						
			if(iError)
			{		
				iError = EFalse;
				status = EScriptFailed;
			}
			else
				status = EScriptPassed;
			
			//Logging to file
			iLogBuffer.Zero();
			iLogBuffer.Append(KEndScript);
				
			HBufC* scriptName = iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex));
			if(scriptName)
			{
				iLogBuffer.Append(scriptName->Des());
				iLogBuffer.Append(KNEWLINE); 
				iLogger->WriteLogL(iLogBuffer,EFalse);
			}		
			
			CleanupStack::PopAndDestroy(); //scriptName
			
			iScriptsView->ChangeIconL(status,iScriptErrorIndexes.operator [](iSelectedIndex));
		
			iScriptsView->HandleMarkCommandL(EAknCmdMark);		
			
			if(++iSelectedIndex == iScriptErrorIndexes.Count())
			{
			    BringToForeground();
			    
				iSelectedIndex = 0;
				iScriptErrorIndexes.Close();
			}	
			
			// Reset loop array
			iNestedLoopArray.Reset();
		}
		break;
		
		case EActionTypeLoopStart:
		{
		if(!iError)
		  {
		  TInt Counter;
		  TPtr buffer = action->iParams->Des();
          buffer.Trim();      
          TLex16 lval(buffer);
          lval.Val(Counter);
		  iNestedLoopArray.Append(Counter);
		  iNestedLoopArray.Append(iActionIndex);
		  iLoopExists = ETrue;
		  }
		}
		break;
		
		case EActionTypeLoopEnd:
        {
        if(!iError && iNestedLoopArray.Count()>1)
            {
            TInt Counter=iNestedLoopArray.operator [](iNestedLoopArray.Count()-2);
            
            if(Counter==1 || Counter==0 || Counter < -1)
                {
                iNestedLoopArray.Remove(iNestedLoopArray.Count()-2);
                iNestedLoopArray.Remove(iNestedLoopArray.Count()-1);
                if(iNestedLoopArray.Count()==0)
                    iLoopExists = EFalse;
                }
            else
                {
                if(Counter!=-1) // Infinite loop
                    iNestedLoopArray.operator [](iNestedLoopArray.Count()-2) = Counter-1;
                    
                iActionIndex = iNestedLoopArray.operator [](iNestedLoopArray.Count()-1);
                }
            }
        }
        break;
		case EActionTypeView: // View switching
		{
			if(!iError){
			TUid appUid, viewUid, customMessageUid;
			TBuf<KBuffer64> tmp16;
			TBuf<KBuffer64> AppName;
							
			if(action->iPrintLog)
			{
				iLogBuffer.Zero();
				iLogBuffer.Copy(KVIEWSWITCH);
				iLogBuffer.Append(*action->iPrintLog);
				iLogBuffer.Append(KNEWLINE);
				iLogger->WriteLogL(iLogBuffer,EFalse);
			}
			
			if (ParseParams( *action->iParams, appUid, viewUid, customMessageUid, tmp16, AppName ) != KErrNone )
				return;
						
		    
			if(AppName.Length()>0)
			     {
		         appUid=GetAppUidFromName(AppName);
		         if(appUid.iUid==0)
		              return;
		         }
						
			TBuf8<KBuffer64> customMessage;
			customMessage.Copy(tmp16);
			
			// check params count, if only one, try to run applicationu
			if (viewUid.iUid == 0 && customMessageUid.iUid == 0 && customMessage.Length() == 0)
			{
				TThreadId threadId; // not used
				TInt res = iSession.StartDocument(KNullDesC, appUid, threadId);
				if(res != KErrNone)
				{
					_LIT(KAPPLAUNCERROR,"*ERROR* Application not found");
					CUtils::MessageBoxL(KAPPLAUNCERROR);
					iLogger->WriteLogL(KViewSwitchingError,EFalse);
					iError = ETrue;
				}
			}
			else
			{
				TApaTaskList taskList(iEikonEnv->WsSession());
				TApaTask task = taskList.FindApp(appUid);
				
				if (action->iKeys.Count())
				{
					if (task.Exists())
					{
						task.EndTask();
						User::After(KWait03);
					}
				}
				
				TRAPD(err,iCoeEnv->AppUi()->ActivateViewL( TVwsViewId(appUid,viewUid),customMessageUid, customMessage ));
			
				if	(err == KErrCancel)
				{
					// do nothing, user has hit red key.
				}
				else if (err != KErrNone)
				{
					// probably some problem with ActivateViewL func, we need to trap this case
					TBuf<KBuffer128> buf;
					buf.Format(KViewSwitchingError);
					CUtils::MessageBoxL(buf);
					//Logging to File
					iLogBuffer.Zero();
					iLogBuffer.AppendFormat(KStringLit,&KViewSwitchingError);
					iLogger->WriteLogL(iLogBuffer,EFalse);
					iError = ETrue;
				}
			}
			
			if (action->iKeys.Count())
			{
				User::After(KWait03);
				
				RWsSession& wsSession = iCoeEnv->WsSession();
				
				// switch focus
				TInt id = wsSession.GetFocusWindowGroup();
		
				// send key events
				TWsEvent wsEvent;
				wsEvent.SetType(EEventKey); 
				TKeyEvent& keyEvent(*wsEvent.Key());
				keyEvent.iModifiers=0;
				keyEvent.iRepeats=0;
		
				for (TInt i=0; i<action->iKeys.Count(); i++)
					{
					keyEvent.iCode = action->iKeys[i];
					keyEvent.iScanCode= 0;
					wsSession.SendEventToWindowGroup(id,wsEvent);
					User::After(KWait1);
					}					
			}
		
			if(action->iScreenshotTag)
			{	
				CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
				imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appUid);
				CleanupStack::PopAndDestroy();  //script name
			}
		   }
	    }
	    break;		 
	    // Launch either a new Java app or open a url
	    case EActionTypeJavaApp:
	    {
	    	if(!iError)
	    	{
				TBuf<KBuffer64> appName;
				TBuf<KBuffer256> url;
								
				if(action->iPrintLog)
				{
					iLogBuffer.Zero();
					iLogBuffer.Copy(KAPPLAUNCH);
					iLogBuffer.Append(*action->iPrintLog);
					iLogBuffer.Append(KNEWLINE);
					iLogger->WriteLogL(iLogBuffer,EFalse);
				}
				if (ParseParams( params, appName, url ) != KErrNone)
					return;
		
				// Try to start the Java app if not then open the url
				if (StartJavaApp(appName) != KErrNone)
					OpenUrlL(url);
				
				if(action->iScreenshotTag)
				{	
					CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
					imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),imidletUID);					
				}
			}
		}
		break;
			
	    case EActionTypeApp: // Launching new app
	    {
	    	if(!iError){
			TUid appUid;
			TBuf<KBuffer64> argument;
			TBuf<KBuffer64> AppName;
			TInt error = KErrNone;
					
			if(action->iPrintLog)
			{
				iLogBuffer.Zero();
				iLogBuffer.Copy(KAPPLAUNCH);
				iLogBuffer.Append(*action->iPrintLog);
				iLogBuffer.Append(KNEWLINE);
				iLogger->WriteLogL(iLogBuffer,EFalse);
			}
			
			// Parse app uid or app name from param
			if(params.Left(2)== KHEX)
			     {
			     if (ParseParams(params, appUid, argument ) != KErrNone )
				            return;
				 }
		    else
		         {
		         if (ParseParams(params, AppName, argument ) != KErrNone )
                            return;
		         }
				
		    if(AppName.Length()>0)
                {
                appUid=GetAppUidFromName(AppName);
                if(appUid.iUid==0)
                     return;
                }
						
			if (action->iKeys.Count())
			{
				TApaTaskList taskList(iEikonEnv->WsSession());
				TApaTask task = taskList.FindApp(appUid);
				
				if (task.Exists())
				{
					task.EndTask();
					User::After(KWait03);
				}
			}	
			
			TBuf<KBuffer64> argument16;
			argument16.Copy(argument);
			TThreadId threadId; // not used
			error = iSession.StartDocument(argument16, appUid, threadId);
									
			if ( error == KErrNotFound )
			{
				//Logging to File
				iLogBuffer.Zero();
				iLogBuffer.AppendFormat(KStringLit,&KAppNotFound);
				iLogger->WriteLogL(iLogBuffer,EFalse);
				iError = ETrue;				
			}
			else if ( error != KErrNone )
			{
				// Do nothing for the moment
			}
		
			if (action->iKeys.Count())
			{
				User::After(KWait005);
				
				RWsSession& wsSession=iEikonEnv->WsSession();
				
				// switch focus
				TInt id = wsSession.GetFocusWindowGroup();
	
				// send key events
				TWsEvent wsEvent;
				wsEvent.SetType(EEventKey); 
				TKeyEvent& keyEvent(*wsEvent.Key());
				keyEvent.iModifiers=0;
				keyEvent.iRepeats=0;
	
				for (TInt i=0; i<action->iKeys.Count(); i++)
				{
					keyEvent.iCode = action->iKeys[i];
					keyEvent.iScanCode= keyEvent.iCode;
					wsSession.SendEventToWindowGroup(id,wsEvent);
	
					User::After(KWait1);
				}
			}
			 if(action->iScreenshotTag)
			 {
				 CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
				 imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appUid);
				 CleanupStack::PopAndDestroy(); //script name
			 }
			
	    	}
	    }
	    break;
		case EActionTypeWait:
		{	
			if(!iError){
				if(action->iPrintLog)
				{
					iLogBuffer.Zero();
					iLogBuffer.Copy(KWAIT);
					iLogBuffer.Append(*action->iPrintLog);
					iLogBuffer.Append(KNEWLINE);
					iLogger->WriteLogL(iLogBuffer,EFalse);
				}
				
				if(action->iScreenshotTag)
				{
					CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->WsSession().GetFocusWindowGroup());
					TUid foreTaskUid = gn->AppUid();
					CleanupStack::PopAndDestroy(gn);
					
					CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
					imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(
								           iScriptErrorIndexes.operator [](iSelectedIndex))),foreTaskUid.Null());
					
					CleanupStack::PopAndDestroy(); //script name
				}
				
				TPtr buffer = action->iParams->Des();
				buffer.Trim();
				
				TLex16 lval(buffer);
				lval.Val(interval);
				if(interval>30)
					User::AfterHighRes(interval-30); // -30 -> 30 msec is around the delay of processing PerformAction func.
				
				
			}
		}
		break;
		case EActionCloseApp:
		{
			if(!iError)
			{
				 TUid appUid;
				 TBuf<KBuffer64> argument;
								
				 if(action->iPrintLog)
				 {
					iLogBuffer.Zero();
					iLogBuffer.Copy(KCLOSEAPP);
					iLogBuffer.Append(*action->iPrintLog);
					iLogBuffer.Append(KNEWLINE);
					iLogger->WriteLogL(iLogBuffer,EFalse);
				 }
					 
				 if (ParseParams( *action->iParams, appUid, argument ) != KErrNone )
					return;
					
				 TApaTaskList taskList(iEikonEnv->WsSession());
				 TApaTask task = taskList.FindApp(appUid);
				
				 if (task.Exists())
				 {
					task.EndTask();
					User::After(KWait03);
				 }
				 				
				 if(action->iScreenshotTag)
				 {
					CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), 
																	     iEikonEnv->WsSession().GetFocusWindowGroup());
					TUid foreTaskUid = gn->AppUid();
					CleanupStack::PopAndDestroy(gn);
					 
					CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
					imageCapture->CaptureL( *action->iScreenshotTag,
											*(iScriptsView->GetScriptNameLC(
												iScriptErrorIndexes.operator [](iSelectedIndex))),foreTaskUid.Null());
					CleanupStack::PopAndDestroy(); //script name
				 }
			}
		}
		break;
		case EActionTypeOrientationChange:
			{
			if(!iError)
				{			
				_LIT(KLandscape,"landscape");
				//Logging to File
				if(action->iPrintLog)
					{
					iLogBuffer.Zero();
					iLogBuffer.Copy(KORIENTATION);
					iLogBuffer.Append(*action->iPrintLog);
					iLogBuffer.Append(KNEWLINE);
					iLogger->WriteLogL(iLogBuffer,EFalse);
					}
				
				TInt state=0;
				TInt mode=0;
				if((*action->iParams).Compare(KLandscape)==0)
					{
					state = 1;
					mode = 1;
					}
				
				RProperty::Set( TUid::Uid(0x101F8773), 8, state);                
				iEikonEnv->ScreenDevice()->SetScreenMode(mode);

				if(action->iScreenshotTag)
					{
					CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->WsSession().GetFocusWindowGroup());
					TUid foreTaskUid = gn->AppUid();
					CleanupStack::PopAndDestroy(gn);
					TUid appuid = foreTaskUid.Null();
					
					CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
					if(iLoopExists)
						imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appuid);							 
					else
						imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),foreTaskUid);
										
					CleanupStack::PopAndDestroy(); //script name
					}			
				
				}
			}
		break;
		case EActionTypeScreenReset:
					{
					if(!iError)
						{			
						//Logging to File
						if(action->iPrintLog)
							{
							iLogBuffer.Zero();
							iLogBuffer.Copy(KSCREENRESET);
							iLogBuffer.Append(*action->iPrintLog);
							iLogBuffer.Append(KNEWLINE);
							iLogger->WriteLogL(iLogBuffer,EFalse);
							}
						
						iGlobalCapturer->KillUnwantedAppsResetL();
						
						iGlobalCapturer->ScreenReset();
						

						if(action->iScreenshotTag)
							{
							CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->WsSession().GetFocusWindowGroup());
							TUid foreTaskUid = gn->AppUid();
							CleanupStack::PopAndDestroy(gn);
						TUid appuid = foreTaskUid.Null();
						
						CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
						
						if(iLoopExists)
							imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appuid);							 
						else
							imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),foreTaskUid);
							
							CleanupStack::PopAndDestroy(); //script name
							}			
						
						}
					}
		break;
		case EActionTypePointerEvent:
		{
			if (!iError && AknLayoutUtils::PenEnabled())
			{	 
				 TBuf<KBuffer32> 	eventname;
				 TBuf<KBuffer32> 	argument;
				 TBuf<KBuffer32> 	arg1;
				 TBuf<KBuffer32> 	arg2;
				 TBuf<KBuffer32> 	argx;
				 TBuf<KBuffer32> 	argy;
				 TBuf<KBuffer32> 	argx1;
				 TBuf<KBuffer32> 	argy1;
				 TInt16		        intargx;
				 TInt16		        intargy;
				 TInt16		        intargx1;
				 TInt16		        intargy1;
				
				 if(action->iPrintLog)
				 {
					iLogBuffer.Zero();
					iLogBuffer.Copy(KPOINTEREVENT);
					iLogBuffer.Append(*action->iPrintLog);
					iLogBuffer.Append(KNEWLINE);
					iLogger->WriteLogL(iLogBuffer,EFalse);
				 }
				 // For example: PointerTo 10,10 (or MoveTo 12,12,100,100)
				 const TInt KParamMaxLength = 32;
				 if((*action->iParams).Length()<=KParamMaxLength)
				  {
				  if (ParseParams( *action->iParams, eventname, argument ) == KErrNone )
					 {
					 if (ParseParams( argument, argx, argy ) == KErrNone )
						 {
						 if (ParseParams( argy, argx1, argy1 ) == KErrNone )
							 {// 3 params
								 TLex16 lval1(argx);
								 lval1.Val(intargx);
								 TLex16 lval2(argx1);
								 lval2.Val(intargx1);
								 TLex16 lval3(argy1);
								 lval3.Val(intargy1);
								 if(eventname.Compare(KButtonDown)==0)
									 ButtonDownEvent(intargx+intargy1,intargx1+intargy1);
								 if(eventname.Compare(KButtonUp)==0)
									 ButtonUpEvent(intargx+intargy1,intargx1+intargy1);
								 if(eventname.Compare(KPointerMove)==0)
									 PointerMoveEvent(intargx+intargy1,intargx1+intargy1);
							 }
							 else
							 {
								 // 2 params
								 TLex16 lval1(argx);
								 lval1.Val(intargx);
								 TLex16 lval2(argy);
								 lval2.Val(intargy);
								 if(eventname.Compare(KButtonDown)==0)
									 ButtonDownEvent(intargx,intargy);
								 if(eventname.Compare(KButtonUp)==0)
									 ButtonUpEvent(intargx,intargy);
								 if(eventname.Compare(KPointerMove)==0)
									 PointerMoveEvent(intargx,intargy);
								 
							 }
						 }
					}
				}
					
				if(action->iScreenshotTag)
				{
					 CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->WsSession().GetFocusWindowGroup());
					 TUid foreTaskUid = gn->AppUid();
					 CleanupStack::PopAndDestroy(gn);
					 TUid appuid = foreTaskUid.Null();
					 
					 CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
					 if(iLoopExists)
					 	 imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appuid);							 
					 else
					 	 imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),foreTaskUid);
					 
					 CleanupStack::PopAndDestroy(); //script name
				}				 
			}
			else
			{
				if(iPointerCount == 0)
				{
					_LIT(KErrorNote, "Script might not work properly on Non touch enabled device!!");
					CAknGlobalNote* globalNote = CAknGlobalNote::NewL();
					CleanupStack::PushL( globalNote );
					globalNote->ShowNoteL( EAknGlobalErrorNote , KErrorNote );
					CleanupStack::PopAndDestroy();
					iPointerCount++;
				}
			}
		}
		break;
		
		case EActionTypeKeys: // keys handling
		{
			TUid foreTaskUid;
			if(!iError){
			
			//Logging to File
			if(action->iPrintLog)
			{
				iLogBuffer.Zero();
				iLogBuffer.Copy(KKEYPRESS);
				iLogBuffer.Append(*action->iPrintLog);
				iLogBuffer.Append(KNEWLINE);
				iLogger->WriteLogL(iLogBuffer,EFalse);
			}
						
			if (action->iKeys.Count())
			{
				User::After(KWait03);
					
				RWsSession& wsSession = iEikonEnv->WsSession();
				
				// switch focus
				wsSession.GetFocusWindowGroup();
				
				// send key events
				TWsEvent wsEvent;
				TKeyEvent& keyEvent(*wsEvent.Key());
				keyEvent.iModifiers=0;
				
				TInt repeats;
				if(action->iParams)
				{
					TPtr buffer = action->iParams->Des();
					buffer.Trim();
					TLex16 lval(buffer);
					lval.Val(repeats);
				}
				else
					repeats = 0;

				CApaWindowGroupName* gn = CApaWindowGroupName::NewLC(iEikonEnv->WsSession(), iEikonEnv->WsSession().GetFocusWindowGroup());
				foreTaskUid = gn->AppUid();
				CleanupStack::PopAndDestroy(gn);
							
				for (TInt i=0; i<action->iKeys.Count(); i++)
				{
					keyEvent.iCode = action->iKeys[i];
								
					AknLayoutUtils::TAknCbaLocation location = AknLayoutUtils::CbaLocation();
					TBool orientation = ETrue;
					
					if (location == AknLayoutUtils::EAknCbaLocationBottom)
					{
						//Potrait mode
						orientation = ETrue;											
					} 
					else
					{
						// Landscape mode
						orientation = EFalse;											
					}
					
					if(63498 == action->iKeys[i] && orientation) //Down Arrow Key
						keyEvent.iScanCode= EStdKeyDownArrow;
					else if(63498 == action->iKeys[i] && !orientation) //Landscape (Left Arrow Key)
						keyEvent.iScanCode= EStdKeyLeftArrow;					
					else if(63497 == action->iKeys[i] && orientation) //Up Arrow Key
						keyEvent.iScanCode= EStdKeyUpArrow;
					else if(63497 == action->iKeys[i] && !orientation) //Landscape (Right Arrow Key)
						keyEvent.iScanCode= EStdKeyRightArrow;					
					else if(63496 == action->iKeys[i] && orientation) //Right Arrow Key
						keyEvent.iScanCode= EStdKeyRightArrow;
					else if(63496 == action->iKeys[i] && !orientation) //Landscape (Down Arrow Key)
						keyEvent.iScanCode= EStdKeyDownArrow;					
					else if(63495 == action->iKeys[i] && orientation) //Left Arrow Key
						keyEvent.iScanCode= EStdKeyLeftArrow;
					else if(63495 == action->iKeys[i] && !orientation) //Landscape (Up Arrow Key)
							keyEvent.iScanCode= EStdKeyUpArrow;					
					else if(63554 == action->iKeys[i] ) //Left Soft Key (OK Key)
						keyEvent.iScanCode = EStdKeyDevice0;
					else if(63555 == action->iKeys[i] ) //Right Soft Key (OK Key)
						keyEvent.iScanCode = EStdKeyDevice1;
					else if(63586 == action->iKeys[i]) //KYES (Green Key)
						keyEvent.iScanCode = EStdKeyYes;
					else if(63587 == action->iKeys[i]) //KNO (Red Key)
						keyEvent.iScanCode = EStdKeyNo;
				    else if(8 == action->iKeys[i]) // BS (C key)
                        keyEvent.iScanCode = EStdKeyBackspace;
                    else if(35 == action->iKeys[i]) // Hash mark
                        keyEvent.iScanCode = EStdKeyHash;
					else if(63570 == action->iKeys[i]) //KMENU (Menu Key)
						keyEvent.iScanCode = EStdKeyApplication0;
					else if ((foreTaskUid != KUidStandBy1) && 
							 (foreTaskUid != KUidStandBy)  &&
							 (foreTaskUid != KUidStandby50) ||
							 (167 == action->iKeys[i])) //Middle Soft Key)
						keyEvent.iScanCode = keyEvent.iCode;
					else
						keyEvent.iScanCode = 0;
					
					TRawEvent lEventDown;
					lEventDown.Set(TRawEvent::EKeyDown, keyEvent.iScanCode);
					UserSvr::AddEvent(lEventDown);

					User::After(KWait01);
					
					// long key press
					if(repeats>0)
						User::After((KWait01*12)*repeats);
					
					TRawEvent lEventUp;
					lEventUp.Set(TRawEvent::EKeyUp, keyEvent.iScanCode);
					UserSvr::AddEvent(lEventUp);
													  
					User::After(KWait01);
				}
			 }	
			 if(action->iScreenshotTag)
			 {
				 CImageCapture* imageCapture = CImageCapture::NewL(*iSettings,*this,iEikonEnv);
				 TUid appuid = foreTaskUid.Null();
				 imageCapture->CaptureL(*action->iScreenshotTag,*(iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex))),appuid);
				 
				 CleanupStack::PopAndDestroy(); //script name
			 }
		    }
		}
		break;
		default:
			break;
    }
	
	if(!iStopScripts)
	{
		if(!action->iScreenshotTag)			
			PerformNextAction(performactiontimer);
	}
	else
	{
		CancelScriptExecutionL();
	}
}

// ------------------------------------------------------------------
// Selects and starts scripts according to command-line parameters
// ------------------------------------------------------------------
//
void CUserEmulatorAppUi::FolderScanCompletedL()
    {    
    TInt Count=0;
    // Selects scripts in list box. iCommandLineScriptsArray is filled with command-line params previously
    for(TInt i=0;i<iCommandLineScriptsArray->Count();i++)
        {
        
        if(iScriptsView->GetScriptIndexAndMarkL((*iCommandLineScriptsArray)[i])!=-1)
            {
            Count++;
            }
        }
                
        // If there was one match at least -> start tests!
        if(Count>0)
            StartTestL();
        else
            {
            // If there was no command-line param, don't display this message.
            if(iCommandLineScriptsArray->Count()!=0)
                {
                _LIT(KNoScriptToRun,"Argument doesn't contain any valid script name!");
                CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);                   
                errorNote->ExecuteLD(KNoScriptToRun);
                }
            }
                
        if(iCommandLineScriptsArray)
            iCommandLineScriptsArray->Reset();
            
        iIsFolderScanCompletedCalled = ETrue;
    }

// ------------------------------------------------------------------
// Finds App name in app list and returns it's app uid if app exists 
// ------------------------------------------------------------------
//
TUid CUserEmulatorAppUi::GetAppUidFromName(TDesC& aAppName)
    {
    TApaAppInfo     appInfo;
    TBool           res=EFalse;
    TUid            uid;
    uid.iUid = 0;
    iSession.GetAllApps();

    while (iSession.GetNextApp( appInfo ) == KErrNone)
        {
        if(aAppName.Compare(appInfo.iCaption)==0)
            {
            res=ETrue;
            break;
            }
        }
     
    if(res)
        return appInfo.iUid;
    else
        return uid;
    }

// ----------------------------------------------------------------
// Stops script execution after Power button press 
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::CancelScriptExecutionL()
{
	if(!iScriptErrorIndexes.Count())
		return;
	if(iStopScripts)
	{
		iLogBuffer.Zero();
		iLogBuffer.Append(KStopScript);
		iStopScripts = EFalse;
		iScriptsView->ChangeIconL(EScriptCancelled, iScriptErrorIndexes.operator [](iSelectedIndex));
		iScriptRun = EFalse;
		iScriptsView->HandleMarkCommandL(EAknCmdMark);
	}
	
	ResetLists();
	
	//Logging to file
	HBufC* scriptName = iScriptsView->GetScriptNameLC(iScriptErrorIndexes.operator [](iSelectedIndex));
	if(scriptName)
	{
		iLogBuffer.Append(scriptName->Des());
		iLogBuffer.Append(KNEWLINE); 
		iLogger->WriteLogL(iLogBuffer,EFalse);
	}		
	CleanupStack::PopAndDestroy(); //scriptName
	
	iSelectedIndex = 0;
	iScriptErrorIndexes.Close();
		
	BringToForeground();
}

// ---------------------------------------------------------------
// Converts coordinates because of different manufacturer devices
// ---------------------------------------------------------------
//      
void CUserEmulatorAppUi::CoordConversionByOrientation(TInt x,TInt y,TInt& xMod,TInt& yMod)
    {                   
        //TSize screen=iFunctions->ScreenDevice()->SizeInPixels();
        TSize screen=iEikonEnv->ScreenDevice()->SizeInPixels();
        TInt Width=screen.iWidth;
        TInt Height=screen.iHeight;
        
        // Coord conversion
        if(Width <= Height) // Portrait mode
        {   
            switch(iManufacturer)
            {
            	case KManufacturerNokia:
            		 xMod = y;
            		 yMod = Width-x;            		                
            		break;
            	
            	case KManufacturerSamsung:
            	case KManufacturerSonyEricsson:
            	default:
            		  xMod = x;
            		  yMod = y;
            	    break;
            		
            }
        }
        else // Landscape mode
        {
			switch(iManufacturer)
			{
				case KManufacturerNokia:
				default:
					xMod=x;
					yMod=y;           		                
					break;
				
				case KManufacturerSamsung:
				case KManufacturerSonyEricsson:
					xMod = Height-y;
					yMod = x;
					break;					
			}
        }
    }

// ----------------------------------------------------------------
// CUserEmulatorAppUi::ButtonDownEvent
// Sends pointer down raw event to the system
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::ButtonDownEvent(TInt intargx, TInt intargy)
{
	TRawEvent event;
	TInt x,y;
    CoordConversionByOrientation(intargx,intargy,x,y);
	event.Set(TRawEvent::EButton1Down, x, y);
	iEikonEnv->WsSession().SimulateRawEvent(event);
}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::ButtonUpEvent
// Sends pointer up raw event to the system
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::ButtonUpEvent(TInt intargx, TInt intargy)
{
	TRawEvent event;
	TInt x,y;
    CoordConversionByOrientation(intargx,intargy,x,y);
	event.Set(TRawEvent::EButton1Up, x, y);
	iEikonEnv->WsSession().SimulateRawEvent(event);
}
// ----------------------------------------------------------------
// CUserEmulatorAppUi::PointerMoveEvent
// Sends pointer drag raw event to the system
// ----------------------------------------------------------------
//
void CUserEmulatorAppUi::PointerMoveEvent(TInt intargx, TInt intargy)
{
	TRawEvent event;
	TInt x,y;
    CoordConversionByOrientation(intargx,intargy,x,y);
	event.Set(TRawEvent::EPointerMove, x, y);
	iEikonEnv->WsSession().SimulateRawEvent(event);
}
// ------------------------------------------------------------------------------
// CUserEmulatorAppUi::ResetLists
// Resets the action list (Action list contains parsed actions from xml script)
// ------------------------------------------------------------------------------
//
void CUserEmulatorAppUi::ResetLists()
{
	iActionIndex = 0;
	iActionList.ResetAndDestroy();
	iActionList.Close();
}
// ----------------------------------------
// CUserEmulatorAppUi::BringToForeground
// Puts this application foreground
// ----------------------------------------
//
void CUserEmulatorAppUi::BringToForeground()
{
	TApaTaskList taskList(iEikonEnv->WsSession());
	TApaTask task = taskList.FindApp(KUidUserEmulatorApp);
	if (task.Exists())
	    task.BringToForeground();
}

// ----------------------------------------
// CUserEmulatorAppUi::RandomTestEndsL
// Stops random test
// ----------------------------------------
//
void CUserEmulatorAppUi::RandomTestEndsL()
{
	iRandomTest->StopRandomTest();
	iRandomTestStarted = EFalse;
	iGlobalCapturer->Stop();
	
	BringToForeground();
	
	_LIT(KRANDOMTESTENDLOG, "***Random test ended***\n");
	iLogger->WriteLogL(KRANDOMTESTENDLOG,EFalse,EFalse);
	
	_LIT(KRANDOMTESTEND,"Random test ended");
	CUtils::MessageBoxL(KRANDOMTESTEND);
}

// ------------------------------------------------------
// CUserEmulatorAppUi::ParseParams
// Parses input string into three UIds and one string
// ------------------------------------------------------
//
TInt CUserEmulatorAppUi::ParseParams( const TDesC& aParamString,
							  TUid& aAppUid, TUid& aViewUid,
							  TUid& aCustomMessageUid, TDes& aCustomMessage, TDes& aAppName ) const
{
	TInt 	i, error;
	TLex	lex(aParamString);
		
	// Initialise all the values
	aAppUid.iUid = 0;
	aViewUid.iUid = 0;
	aCustomMessageUid.iUid = 0;
	aCustomMessage.Zero();
	
	// Extract the three Uids
	for ( i=0 ; i<3 ; i++ )
		{
		lex.SkipSpace();
		if ( !lex.Eos() )
			{
			TPtrC	token(lex.NextToken());
			
			TPtrC	param;
			
			// Extract the comma between parameters
			if ( token.Right(1) == KComma )
				{
				param.Set( token.Left(token.Length()-1) );
				}
			else
				{
				param.Set( token );
				}

			// Extract the value taking into account whether its an hex
			// value or not
			TInt32 uid;
			if ( param.Left(2) == KHEX )
				{
				TLex lex2(param.Mid(2));
				TUint val;
				error = lex2.Val( val, EHex );
				uid = val;
				}
			else
				{
				uid = 0;
				aAppName.Copy(param);
				error = KErrNone;
				}
				
			if ( error != KErrNone )
				{
				return error;
				}
				
			switch( i )
				{
				case 0:
					aAppUid.iUid = uid;
					break;
				
				case 1:
					aViewUid.iUid = uid;
					break;
					
				case 2:
					aCustomMessageUid.iUid = uid;
					break;
				}
			}
		}

	// Maybe we don't have a custom message
	lex.SkipSpace();
	if ( !lex.Eos() )
		{
		// Extract the custom message
		aCustomMessage.Copy( lex.Remainder().Left(aCustomMessage.MaxLength()) );
		}

	return KErrNone;
}

// --------------------------------------------------------
// CUserEmulatorAppUi::ParseParams
// Cuts one input string into one UId and one string
// --------------------------------------------------------
//
TInt CUserEmulatorAppUi::ParseParams(const TDesC& aParamString, TUid& anAppUid, TDes& aArgument) const
{
	TInt 	error;
	TLex	lex(aParamString);

	// Initialise all the values
	anAppUid.iUid = 0;
	aArgument.Zero();
	
	// Extract the Uid
	lex.SkipSpace();
	if ( !lex.Eos() )
		{
		TPtrC	token(lex.NextToken());

		TPtrC	param;

		// Extract the comma between parameters
		if ( token.Right(1) == KComma )
			{
			param.Set( token.Left(token.Length()-1) );
			}
		else
			{
			param.Set( token );
			}
	
		// Extract the value taking into account whether its an hex
		// value or not
		if ( param.Left(2) == KHEX )
			{
			TLex lex2(param.Mid(2));
			TUint val;
			error = lex2.Val(val, EHex);
			anAppUid.iUid = val;
			}
		else
			{
			TLex lex2(param);
			error = lex2.Val(anAppUid.iUid);
			}
	
		if ( error != KErrNone )
			{
			return error;
			}
		}
	
	// Maybe we don't have a custom message
	lex.SkipSpace();
	if ( !lex.Eos() )
		{
		// Extract the custom message
		aArgument.Copy(lex.Remainder().Left(aArgument.MaxLength()));
		}

	return KErrNone;
}
// ----------------------------------------------------------------------------------------
// CUserEmulatorAppUi::ParseMultipleParams
// Parses input string into two strings. Delimiter character is ',' . Input string
// can have 4 params and 3 delimiter. (Eq: 23,27,43,21)
// ----------------------------------------------------------------------------------------
//
TInt CUserEmulatorAppUi::ParseMultipleParams(const TDesC& aParamString, TDes& aArgument1, TDes& aArgument2) const
{
	// Initialise all the values
	aArgument1.Zero();
	aArgument2.Zero();
	
	TInt pos1 = aParamString.Locate(',');
	if(pos1==KErrNotFound || pos1+1>aParamString.Length() )
		return KErrNotFound;
	TInt pos = aParamString.Mid(pos1+1).Locate(',');
	if (pos == KErrNotFound)
		return KErrNotFound;
	else
		pos=pos+pos1+1;
	
	// Extract the app name
	aArgument1.Copy(aParamString.Left(pos));
	CUtils::StripSpaces(aArgument1);
	
	// Extract the argument
	if(pos+1>aParamString.Length())
		return KErrNotFound;
	aArgument2.Copy(aParamString.Mid(pos+1));
	CUtils::StripSpaces(aArgument2);
	
	return KErrNone;
}
// -----------------------------------------------
// CUserEmulatorAppUi::ParseParams
// Parses input string into two strings
// -----------------------------------------------
//
TInt CUserEmulatorAppUi::ParseParams(const TDesC& aParamString, TDes& aAppName, TDes& aArgument) const
{
	// Initialise all the values
	aAppName.Zero();
	aArgument.Zero();
	
		
	TInt pos = aParamString.Locate(',');
	if (pos == KErrNotFound)
		return KErrNotFound;
		
	// Extract the app name
	aAppName.Copy(aParamString.Left(pos));
	CUtils::StripSpaces(aAppName);
	
	// Extract the argument
	if(pos+1>aParamString.Length())
			return KErrNotFound;
	aArgument.Copy(aParamString.Mid(pos+1));
	CUtils::StripSpaces(aArgument);
	
	return KErrNone;
}

// -----------------------------------------------------
// SetSystemInfoFlag
// Set the system flag info
// -----------------------------------------------------
void CUserEmulatorAppUi::SetSystemInfoFlag(TBool aValue)
{
	iSystemInfo = aValue;
}

// -----------------------------------------------------
// ProcessCmdParamsL
// Processes command line arguments
// -----------------------------------------------------
void CUserEmulatorAppUi::ProcessCmdParamsL(const TDesC &aParams, TBool aProcessByFolderScanCompleted)
    {   
    // Params' length has to be at least 2 chars long
    if(aParams.Length()>0)
        {
        HBufC* buff = HBufC::NewLC(aParams.Length());
        buff->Des().Copy(aParams);
        // If first 2 chars of the argument are '/r' then run random test(s)
        if(buff->Left(2).Compare(KSLASHR)==0)
            {
            // Process params beyond '/r'
            if(buff->Length()>2)
                {
                TInt pos;
                TBuf<1> tmp;
                tmp.Copy(KComma);
                buff->Des().Copy(buff->Right(buff->Length()-2));
                buff->Des().TrimAll();
                // Transforms string to our inner format (Commas instead of spaces)
                while((pos=buff->Locate(' '))!=KErrNotFound)
                    {   
                    buff->Des().Replace(pos,1,tmp);
                    }
                iSettings->iRandomTestAppID.Copy(buff->Left(KBuffer256));
                iSettings->iRandomTestAppIdStatus = ETrue;
                }
            else
                { // Runs normal random test. There are no UIDs in argument
                 iSettings->iRandomTestAppIdStatus = EFalse;
                }
            
            TRAPD(err, StartRandomTestL());
            }
        else
            {
            // Scripts run               
            TInt pos;
            TBuf8<256> UId;
            TInt Count=0;
            while((pos=buff->Locate(' '))!=KErrNotFound)
                {   
                if(pos>255)
                    {
                   _LIT(KTooLongName,"One of the given script names is too long!");
                    CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);                   
                    errorNote->ExecuteLD(KTooLongName);
                    CleanupStack::PopAndDestroy(buff);
                    return;
                    }
                UId.Copy(buff->Left(pos));
                buff->Des().Copy(buff->Mid(pos+1));
                if(UId.Locate('.')==KErrNotFound)
                    UId.Append(KXMLEXT);
                // Set index and check if UId is in the list of scripts
                if(aProcessByFolderScanCompleted)
                    {
                    if(iScriptsView->GetScriptIndexAndMarkL(UId)!=-1)
                        Count++;
                    }
                else
                    iCommandLineScriptsArray->AppendL(UId);
                
                }   
                
            if(buff->Length()>0)
                {
                UId.Copy(*buff);
                if(UId.Locate('.')==KErrNotFound)
                    UId.Append(KXMLEXT);
               
                if(aProcessByFolderScanCompleted)
                    {
                    if(iScriptsView->GetScriptIndexAndMarkL(UId)!=-1)
                        Count++;
                    }
                else
                    iCommandLineScriptsArray->AppendL(UId);
                }
                
            if(aProcessByFolderScanCompleted)
                {
                if(Count>0)
                    StartTestL();
                else
                    {
                    _LIT(KNoScriptToRun,"Argument doesn't contain any valid script name!");
                    CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);                   
                    errorNote->ExecuteLD(KNoScriptToRun);
                    }
                }
            }
            
        CleanupStack::PopAndDestroy(buff);
        }
  
    }

// -----------------------------------------------------
// ProcessMessageL
// Reads command line arguments
// -----------------------------------------------------
void CUserEmulatorAppUi::ProcessMessageL(TUid /*aUid*/, const TDesC8 &aParams)
    {           
        if(aParams.Length()>=KBuffer256)
            {
            _LIT(KArgTooLong,"Command line argument is too long!");
            CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);                   
            errorNote->ExecuteLD(KArgTooLong);
            }
        else
            {
            HBufC* buff = HBufC::NewLC(aParams.Length()); 
            buff->Des().Copy(aParams);
            // In this case ProcessMessage called later than list box initialisation
            // so UE can process command-line params strait away. (Without storing params in an array
            // and processing it later)
            ProcessCmdParamsL(*buff,iIsFolderScanCompletedCalled);
            CleanupStack::PopAndDestroy(buff);
            }
            
    }
    
    

// -----------------------------------------------------
// ProcessCommandParametersL
// Reads command line arguments
// -----------------------------------------------------
TBool CUserEmulatorAppUi::ProcessCommandParametersL(TApaCommand aCommand,TFileName &aDocumentName,
    const TDesC8 &aTail)
    {
        TInt Args;
        TBuf<KBuffer256> buf;
        TInt res=GetArgsL(buf, Args);
    
        if(!res)
            {
             _LIT(KArgTooLong,"Command line argument is too long!");
             CAknErrorNote* errorNote = new (ELeave) CAknErrorNote(ETrue);                   
             errorNote->ExecuteLD(KArgTooLong);
            }
        else
            {
            if(Args>1)
                ProcessCmdParamsL(buf,iIsFolderScanCompletedCalled);
            else
                ProcessCmdParamsL(aDocumentName,iIsFolderScanCompletedCalled);
            }
            
        return CEikAppUi::ProcessCommandParametersL( aCommand , aDocumentName, aTail);
    }
    
// -----------------------------------------------------------------
// GetArgsL
// Reads command line arguments in the original way (args process)
// -----------------------------------------------------------------
TBool CUserEmulatorAppUi::GetArgsL(TDes& aParam,TInt& aArgs)
    {
     TBool res=ETrue;
     
     CCommandLineArguments* args=CCommandLineArguments::NewLC();
     aArgs = args->Count();
     for (TInt i=1; i<args->Count(); i++)
            {
            TPtrC argumentPrt(args->Arg(i));
            if(aParam.Length() + argumentPrt.Length() >= KBuffer256)
                {
                res=EFalse;
                break;
                }
            aParam.Append(argumentPrt);
            aParam.Append(_L(" "));
            }
    if(aParam.Length()>0)
        aParam.TrimAll();
        
    CleanupStack::PopAndDestroy(1);
     
    return res;
    }

// End of File
