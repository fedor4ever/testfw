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
 * File Name: XmlHandler.cpp
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * XML Parser implementation
 * There are two methods to parse XML document, i.e.;
 *   - StartParsingL() -> parse XML document without active object.
 *   - StartParsingWithAoL() -> parse XML document with active object.
 *------------------------------------------------------------------
 -
 *
 */

// System Includes
#include <coemain.h>
#include <f32file.h>
#include <eikenv.h>
#include <xml/parserfeature.h>
#include <utf.h>
#include <aknnotewrappers.h>
#include <coecntrl.h>

// User Includes
#include <UserEmulator_0x2001C3AF.rsg>
#include "XmlHandler.h"
#include "Utils.h"
#include "UserEmulatorAppUi.h"

//Constants
/**
 * XML Tags
 */
_LIT8(KAction,"action");
_LIT8(KName,"name");
_LIT8(KType,"type");
_LIT8(KParams,"params");
_LIT8(KPrintLog,"print");
_LIT8(KKeys,"keys");
_LIT8(KLoop,"loop");
_LIT8(KCount,"count");
_LIT8(KScreenShot,"screenshot");

/**
 * Action Types
 */
_LIT( KParamActionsApp, "app" );
_LIT( KParamActionsView, "view" );
_LIT( KParamActionsJavaApp, "javaapp" );
_LIT( KParamActionsWait, "wait" );
_LIT( KParamActionsOrient, "orientation" );
_LIT( KParamActionsScreenReset, "screenreset" );
_LIT( KParamActionsPointerEvent, "pointerevent" );
_LIT( KParamActionsKeyPress, "keypress" );
_LIT( KParamCloseApp, "closeapp");
_LIT( KParamActionsLoop, "loop");
_LIT( KParamActionsLoopEnd, "loopend");

/**
 * Keys
 */
_LIT(KLeftSoftKey,"LSK");
_LIT(KRightSoftKey,"RSK");
_LIT(KMiddleSoftKey,"MSK");
_LIT(KDownArrowKey,"DAK");
_LIT(KUpArrowKey,"UAK");
_LIT(KRightArrowKey,"RAK");    		
_LIT(KLeftArrowKey,"LAK");
_LIT(KKeySpace,"SP");
_LIT(KKeyBackSpace,"BS");
_LIT(KKeyGreaterThan, "gt");
_LIT(KKeyAmpersand,"amp");
_LIT(KKeyLessThan,"lt");
_LIT(KKeyOkKey,"OK");
_LIT(KKeyMenu,"MENU"); //EAKeyApplication0
_LIT(KKeyYes,"KYES"); //EKeyYes
_LIT(KKeyNo,"KNO"); //EKeyNo
_LIT(KKeyCamera,"CAM");
_LIT(KKeyHash,"#");

/**
 * General Constants
 */
_LIT(KOpeningTag, "<" );
_LIT(KClosingTag, ">" );
_LIT(KSlash,      "/" );
_LIT(KEndOfLine,  "\f" ); // CEikEdwin uses '\f' as EOF mark.

_LIT(KF842,"0xF842"); 
_LIT(KF843,"0xF843");              
_LIT(Ka7,"0xa7");                
_LIT(KF80A,"0xF80A");              
_LIT(KF809,"0xF809");               
_LIT(KF808,"0xF808");               
_LIT(KF807,"0xF807");               
_LIT(K0008,"0x0008");               
_LIT(K0020,"0x0020");
_LIT(K0026,"0x0026");
_LIT(K003C,"0x003C");
_LIT(K003E,"0x003E");
_LIT(KF852,"0xF852");
_LIT(KF862,"0xF862");
_LIT(KF863,"0xF863");
_LIT(KF893,"0xF893");


_LIT8(KXmlMimeType, "text/xml" );

const TInt KMaxFileNameLength=200;


// METHODS DEFINITION
// -----------------------------------------------------------------------------
// CXmlHandler::NewL
// Creates the instance of class and returns it.
// -----------------------------------------------------------------------------
//

CXmlHandler* CXmlHandler::NewL( MXmlHandlerObserver& aObserver, RPointerArray<CAction>& aActionList)
    {
    CXmlHandler* self = CXmlHandler::NewLC( aObserver ,aActionList);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CXmlHandler::NewLC
// Creates the instance of class and pushes it to the CleanupStack and return
// it.
// -----------------------------------------------------------------------------
//
CXmlHandler* CXmlHandler::NewLC( MXmlHandlerObserver& aObserver ,
								 RPointerArray<CAction>& aActionList)
    {
    CXmlHandler* self = new ( ELeave ) CXmlHandler( aObserver, aActionList);
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// CXmlHandler::~CXmlHandler
// Cancels any outstanding requests and deletes members.
// -----------------------------------------------------------------------------
//
CXmlHandler::~CXmlHandler()
{
    Cancel();
    delete iParser;
    delete iDisplayResult;
    delete iBuffer;
    delete iContent; 
    iContent=NULL;
}

// -----------------------------------------------------------------------------
// CXmlHandler::CXmlHandler
// Calls base classes constructor with priority value. Add class to the 
// active sheduler.
// -----------------------------------------------------------------------------
//
CXmlHandler::CXmlHandler( MXmlHandlerObserver& aObserver ,RPointerArray<CAction>& aActionList)
    :CActive( EPriorityStandard ),
    iObserver( aObserver ),
    iParser( 0 ),
    iDisplayResult( 0 ),
    iState( EIdle ),
    iActionList(aActionList),
    iActionListIndex(0)
{
	iIndex = 0;
   
    CActiveScheduler::Add( this );
}
// -----------------------------------------------------------------------------
// CXmlHandler::DoCancel
// From CActive. Cancels any outstanding request according the engine state.
// -----------------------------------------------------------------------------
//
void CXmlHandler::DoCancel()
{
	TRAP_IGNORE(iParser->ParseEndL());
	iFile.Close();
	delete iBuffer;
	iBuffer = 0;
}

// -----------------------------------------------------------------------------
// CXmlHandler::RunL
// From CActive. Handles the state changes and notifing the observer.
// -----------------------------------------------------------------------------
//
void CXmlHandler::RunL()
    {
    if ( KErrNone == iStatus.Int() )
        {
        // If the buffer length is zero, it means we have reached
        // the end of the file.
        if ( iBuffer->Length() == 0)
            {
            iParser->ParseEndL();
            iFile.Close();
            delete iBuffer;
            iBuffer = 0;
            }
            
        // Otherwise, we continue reading the next chunk of the XML file.
        else
            {
            // Parse the next "part" of the XML document.
            iParser->ParseL( *iBuffer );
            
            // Read the next chunk of the file.
            TPtr8 bufferPtr( iBuffer->Des() );
            iFile.Read( bufferPtr, KBuffer1024, iStatus );
            
            // Don't forget to call this..
            SetActive();
            }
        }
    else
        {
        iObserver.OnParseCompletedL( iStatus.Int(), iState );
        }
    }

// -----------------------------------------------------------------------------
// CXmlHandler::ConstructL
// Construction of parser and buffer allocations
// -----------------------------------------------------------------------------
//
void CXmlHandler::ConstructL()
    {
    iDisplayResult = HBufC::NewL( KBuffer1024 );
    iContent = HBufC8::NewL(KBuffer128);
    }

// -----------------------------------------------------------------------------
// CXmlHandler::StartParsingL
// Function to start parsing the XML script without active object
// -----------------------------------------------------------------------------
//   
TInt CXmlHandler::StartParsingL( const TDesC& aFileName )
    {
    // If file name is too long then don't parse it just mark it as a failed script
    TInt pos=aFileName.LocateReverse('\\');
    if(aFileName.Right(aFileName.Length()-pos).Length()>KMaxFileNameLength)
        {
        TRAPD(err, iObserver.OnParseCompletedL( KErrGeneral,EOnError ));
        return err;
        }
    
    RFile file;
    User::LeaveIfError( file.Open( CCoeEnv::Static()->FsSession(), aFileName,
        EFileRead ) );
    CleanupClosePushL( file );
    
    iDisplayResult->Des().Zero();
    TInt size;
    User::LeaveIfError( file.Size( size ) );
    delete iBuffer;
    iBuffer = 0;
    iBuffer = HBufC8::NewL( size );
    TPtr8 bufferPtr( iBuffer->Des() );
    User::LeaveIfError( file.Read( bufferPtr ) );
    
    file.Close();
    CleanupStack::PopAndDestroy(); // file
    
    delete iParser;
    iParser= NULL;
    iParser = CParser::NewL( KXmlMimeType, *this );
    
    iActionListIndex = iActionList.Count();
    TRAPD(err,iParser->ParseBeginL());
    if(err!=KErrNone)
        return err;
    TRAPD(err1,iParser->ParseL( *iBuffer ));
    if(err1!=KErrNone)
        return err1;
    TRAPD(err2,iParser->ParseEndL());
    if(err2!=KErrNone)
        return err2;
        
    return KErrNone;
    
    }
    
// -----------------------------------------------------------------------------
// CXmlHandler::StartParsingWithAoL
// Function to start parsing the XML script 
// -----------------------------------------------------------------------------
//   
   
void CXmlHandler::StartParsingWithAoL( const TDesC& aFileName )
    {   
     // Remember to cancel any outstanding request first.
    if ( IsActive() )
        {
        Cancel();
        }
    // If file name is too long then don't parse it just mark it as a failed script
	TInt pos=aFileName.LocateReverse('\\');
	if(aFileName.Right(aFileName.Length()-pos).Length()>KMaxFileNameLength)
		{
		
		TRAPD(err, iObserver.OnParseCompletedL( KErrGeneral,EOnError ));
		return;
		}    
    User::LeaveIfError( iFile.Open( CCoeEnv::Static()->FsSession(), aFileName,
        EFileRead ) );

    // Create a buffer to store the file content.
    // Note that this method uses active object to read the file.
    // So we have to call SetActive() at the end. Then we call CParser::ParseL()
    // in RunL() method.
    iDisplayResult->Des().Zero();
    delete iBuffer;
    iBuffer = 0;
    iBuffer = HBufC8::NewL( KBuffer1024 );
    TPtr8 bufferPtr( iBuffer->Des() );
    iFile.Read( bufferPtr, KBuffer1024, iStatus );
    SetActive();

	// Tell the parser that we are about to parse a XML document.    
    iParser->ParseBeginL();
    }

// -----------------------------------------------------------------------------
// CXmlHandler::OnStartDocumentL
// Callback to indicate start of the document
// -----------------------------------------------------------------------------
// 
void CXmlHandler::OnStartDocumentL( const RDocumentParameters& /*aDocParam*/,
        TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
    {
    // Nothing for now
    }
    else
    {
    	iObserver.OnParseCompletedL( aErrorCode, EOnStartElement );
    }
}

// -----------------------------------------------------------------------------
// CXmlHandler::OnEndDocumentL
// Callback to indicate end of the document
// -----------------------------------------------------------------------------
//     
void CXmlHandler::OnEndDocumentL( TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
    {
    	// Create action object
		CAction *action = new(ELeave) CAction();
		CleanupStack::PushL(action);
		iActionList.AppendL(action);   
		
		CleanupStack::Pop(); // action
		action->iType = EActionTypeEndScript;
    }
    iObserver.OnParseCompletedL( aErrorCode , EOnEndElement);
}

// -----------------------------------------------------------------------------
// CXmlHandler::OnStartElementL
// Callback to indicate an element has been parsed
// -----------------------------------------------------------------------------
// 
void CXmlHandler::OnStartElementL( const RTagInfo& aElement,
        const RAttributeArray& aAttributes, TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
    {
    	const TDesC8& name = aElement.LocalName().DesC();
    	
    	iContent->Des().Zero();
    	
    	TInt i;
    	
    	if (name == KAction || name == KLoop)
    	{
    		// Create action object
    		CAction *action = new(ELeave) CAction();
    		CleanupStack::PushL(action);
    		
    		if(name == KLoop)
              {
              for (i=0; i<aAttributes.Count(); i++)
                {
                    RAttribute attrib = aAttributes[i];
                    const TDesC8& attribName = attrib.Attribute().LocalName().DesC();
                    const TDesC8& attribValue = attrib.Value().DesC();          
                    TBuf<KBuffer256> tmp;
                    tmp.Copy(attribValue);
                
                    if(attribName == KCount)
                    {
                        action->iType = EActionTypeLoopStart;
                        if(attribValue == KInfinite)
                           tmp.Copy(KMinus1);
                           
                        action->iParams = tmp.AllocL();      
                    }
                }
              }
    		
    		iActionList.AppendL(action);    			
    		
    		CleanupStack::Pop(); // action
    		iAction = action;    		
    		
    		   		
    	}
    	else if (name == KName)
    		iContentType = EContentName;
    	else if (name == KType)
    		iContentType = EContentType;
    	else if (name == KParams)
    		iContentType = EContentParams;
    	else if (name == KPrintLog)
    		iContentType = EContentPrintLog;
    	else if (name == KKeys)
    		iContentType = EContentKeys;
    	else if (name == KScreenShot)
    	    iContentType = EContentScreenshot;
    	else
    		{    		
    		}
    }
    else
    {
    	iObserver.OnParseCompletedL( aErrorCode, EOnStartElement );
    }
}
        
// -----------------------------------------------------------------------------
// CXmlHandler::OnEndElementL
// Callback to indicate the end of the element has been reached.
// -----------------------------------------------------------------------------
// 
void CXmlHandler::OnEndElementL( const RTagInfo &aElement, TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
    {
        // If we find the end of an element, we write it to the screen,
        // for example: "</tag>"
    	const TDesC8& name = aElement.LocalName().DesC();
    	
    	TPtr8 des(iContent->Des());
    	CUtils::StripSpaces(des);
    	
    	HBufC *txt = CnvUtfConverter::ConvertToUnicodeFromUtf8L(*iContent);
    	CleanupStack::PushL(txt);

    	if (name == KAction)
    	{
    		iAction = NULL;
    	}
    	else if (name == KLoop)
		{
			//reset the flag iLoopExists
			CAction *action = new(ELeave) CAction();
            CleanupStack::PushL(action);
            action->iType = EActionTypeLoopEnd;
            iActionList.AppendL(action);                
            
            CleanupStack::Pop(); // action
            iAction = action;  
		}
    	else if (name == KType && iContentType == EContentType && iAction)
    	{
    		TLex lex(txt->Des());
    		lex.SkipSpace();
    		TPtrC token = lex.NextToken();
    		if (token == KParamActionsView)
    			iAction->iType = EActionTypeView;
    		else if (token == KParamActionsApp)
    			iAction->iType = EActionTypeApp;
    		else if (token == KParamActionsJavaApp)
    			iAction->iType = EActionTypeJavaApp;
    		else if (token == KParamActionsPointerEvent)
        		iAction->iType = EActionTypePointerEvent;
    		else if (token == KParamActionsWait)
    			iAction->iType = EActionTypeWait;
    		else if (token == KParamActionsOrient)
    		    iAction->iType = EActionTypeOrientationChange;
    		else if (token == KParamActionsScreenReset)
    			iAction->iType = EActionTypeScreenReset;
    		else if (token == KParamActionsKeyPress)
    			iAction->iType = EActionTypeKeys;
    		else if (token == KParamCloseApp)
    			iAction->iType = EActionCloseApp;
    	}
    	else if (name == KParams && iContentType == EContentParams && iAction)
    	{
    		delete iAction->iParams; 
    		iAction->iParams=NULL;
    		iAction->iParams = txt->Des().AllocL();  
    	}
		else if (name == KPrintLog && iContentType == EContentPrintLog && iAction)
        	{
        		delete iAction->iPrintLog; 
        		iAction->iPrintLog=NULL;
        		iAction->iPrintLog = txt->Des().AllocL();  
        	}
    	else if (name == KScreenShot && iContentType == EContentScreenshot && iAction)
		{
			delete iAction->iScreenshotTag; 
			iAction->iScreenshotTag = NULL;
			iAction->iScreenshotTag = txt->Des().AllocL();  
		}
    	else if (name == KKeys && iContentType == EContentKeys && iAction)
    	{
    		TLex lex(txt->Des());
    		TBuf<KBuffer20> keyName(KN);
    		
    		for (lex.SkipSpace(); !lex.Eos(); lex.SkipSpace())
    		{
    			TPtrC token = lex.NextToken();
    			keyName.FillZ();
    			keyName.Copy(token);
  
    			if(token.Compare(KLeftSoftKey) == 0 )
    				keyName.Copy(KF842);
    			else if(token.Compare(KRightSoftKey)  == 0 )
    				keyName.Copy(KF843);
    			else if(token.Compare(KMiddleSoftKey) == 0 )
    				keyName.Copy(Ka7);
    			else if(token.Compare(KDownArrowKey)  == 0 )
    				keyName.Copy(KF80A);
    			else if(token.Compare(KUpArrowKey)    == 0 )
    				keyName.Copy(KF809);
    			else if(token.Compare(KRightArrowKey) == 0 )
    				keyName.Copy(KF808);
    			else if(token.Compare(KLeftArrowKey)  == 0 )
    				keyName.Copy(KF807);
    			else if(token.Compare(KKeyBackSpace)  == 0 )
    				keyName.Copy(K0008);
    			else if(token.Compare(KKeySpace)  == 0 )
    			    keyName.Copy(K0020);
    			else if(token.Compare(KKeyAmpersand)  == 0 )
    			    keyName.Copy(K0026);
    			else if(token.Compare(KKeyLessThan)  == 0 )
    			    keyName.Copy(K003C);
    			else if(token.Compare(KKeyGreaterThan)  == 0 )
    			    keyName.Copy(K003E);
    			else if(token.Compare(KKeyMenu)  == 0 ) 
    				keyName.Copy(KF852);
    			else if(token.Compare(KKeyYes)  == 0 ) 
    				keyName.Copy(KF862);
    			else if(token.Compare(KKeyNo)  == 0 ) 
    				keyName.Copy(KF863);
    			else if(token.Compare(KKeyCamera)  == 0 )
    			    keyName.Copy(KF893);
                else if(token.Compare(KKeyHash)  == 0 )
                    keyName.Copy(_L("0x23"));
    
    			if (keyName.Left(2) == KHEX)
    			{
    				//Special keys
    				TLex lex2(keyName.Mid(2));
    				TUint value;
    				if (lex2.Val(value,EHex) == KErrNone)
    					iAction->iKeys.AppendL(value);
    			}
    			else
    			{
    				//alpha numeric keys and symbols
    			    for(TInt i=0; i<token.Length(); i++)
    			    {
	    				TUint value = token.operator [](i);
	    				iAction->iKeys.AppendL(value);
    			    }
    			}
    		}
    	}
    	else
		{
		}
    	CleanupStack::PopAndDestroy(); // txt
    	iContentType = EContentNone;
    }
    else
    {
        iObserver.OnParseCompletedL( aErrorCode,EOnEndElement );
    }
}
    
// -----------------------------------------------------------------------------
// CXmlHandler::OnContentL
// Callback that sends the content of the element
// -----------------------------------------------------------------------------
//

void CXmlHandler::OnContentL( const TDesC8 &aBytes, TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
    {
        while (iContent->Des().MaxLength() < iContent->Des().Length()+aBytes.Length())
    	{
    		iContent = iContent->ReAllocL(iContent->Des().Length()*3/2);
    	}
    	iContent->Des().Append(aBytes);	 
    }
    else
    {
        iObserver.OnParseCompletedL( aErrorCode, EOnContent);
    }
}
    
// -----------------------------------------------------------------------------
// CXmlHandler::OnStartPrefixMappingL
// This method is a notification of the beginning of the scope of a prefix-URI 
// Namespace mapping. This method is always called before the corresponding 
// OnStartElementL method.
// -----------------------------------------------------------------------------
//

void CXmlHandler::OnStartPrefixMappingL( const RString& /*aPrefix*/,
        const RString& /*aUri*/, TInt aErrorCode )
{
	if ( KErrNone == aErrorCode )
    {
    }
	else
    {
    	iObserver.OnParseCompletedL( aErrorCode,EOnStartPrefixMapping );
    }
}
        
// -----------------------------------------------------------------------------
// CXmlHandler::OnEndPrefixMappingL
// This method is a notification of the end of the scope of a prefix-URI mapping.
// This method is called after the corresponding DoEndElementL method.
// -----------------------------------------------------------------------------
//

void CXmlHandler::OnEndPrefixMappingL( const RString& /*aPrefix*/,
        TInt aErrorCode )
{
	if ( KErrNone == aErrorCode )
    {
    }
	else
    {
    	iObserver.OnParseCompletedL( aErrorCode,EOnEndPrefixMapping );
    }
}    
// -----------------------------------------------------------------------------
// CXmlHandler::OnEndPrefixMappingL
// This method is a notification of ignorable whitespace in element content.
// -----------------------------------------------------------------------------
//
void CXmlHandler::OnIgnorableWhiteSpaceL( const TDesC8& /*aBytes*/,
        TInt aErrorCode )
{
	if ( KErrNone == aErrorCode )
    {
    }
	else
    {
    	iObserver.OnParseCompletedL( aErrorCode, EOnIgnorableWhiteSpace );
    }
}    
// -----------------------------------------------------------------------------
// CXmlHandler::OnSkippedEntityL
// This method is a notification of a skipped entity. If the parser encounters an 
// external entity it does not need to expand it - it can return the entity as aName 
// for the client to deal with.
// -----------------------------------------------------------------------------
//
void CXmlHandler::OnSkippedEntityL( const RString& /*aName*/,
        TInt aErrorCode )
{
	if ( KErrNone == aErrorCode )
    {
    }
	else
    {
    	iObserver.OnParseCompletedL( aErrorCode, EOnSkippedEntity );
    }
}

// -----------------------------------------------------------------------------
// CXmlHandler::OnProcessingInstructionL
// This method is a receive notification of a processing instruction.
// -----------------------------------------------------------------------------
//

void CXmlHandler::OnProcessingInstructionL( const TDesC8& /*aTarget*/,
        const TDesC8& /*aData*/, TInt aErrorCode )
{
    if ( KErrNone == aErrorCode )
	{
	}
    else
    {
    	iObserver.OnParseCompletedL( aErrorCode, EOnProcessingInstruction );
    }
}

// -----------------------------------------------------------------------------
// CXmlHandler::OnError
// This method indicates an error has occurred.
// -----------------------------------------------------------------------------
//
void CXmlHandler::OnError( TInt aErrorCode )
{
    _LIT( KErrorMessage, "*** OnError ***\f" );
    TRAPD(error, AppendTextL( KErrorMessage ));
    
    iAction = NULL;
    //reset the flag iLoopExists
    for(TInt i = iActionList.Count() - 1; i >= iActionListIndex; i--)
	{
		CAction *action = iActionList.operator [](iActionListIndex);
		if(action)
		{
			delete action;
			action = NULL;
		}
		iActionList.Remove(iActionListIndex);
	}
    TRAPD(err, iObserver.OnParseCompletedL( aErrorCode,EOnError )); 
}

// -----------------------------------------------------------------------------
// CXmlHandler::OnError
// This method obtains the interface matching the specified uid.
// -----------------------------------------------------------------------------
//

TAny* CXmlHandler::GetExtendedInterface( const TInt32 /*aUid*/ )
{
    return NULL;
}

// -----------------------------------------------------------------------------
// CXmlHandler::AppendTextL
// Function to expand the actual buffer size
// -----------------------------------------------------------------------------
//

void CXmlHandler::AppendTextL( const TDesC& aText )
{
    TPtr displayResultPtr( iDisplayResult->Des() );
    if ( displayResultPtr.Length() + aText.Length() > displayResultPtr.MaxLength() )
    {
        TRAPD(err, iDisplayResult = iDisplayResult->ReAllocL( displayResultPtr.MaxLength()
            + KBuffer1024 ));
        displayResultPtr.Set( iDisplayResult->Des() );
    }    
    displayResultPtr.Append( aText );
}

// -----------------------------------------------------------------------------
// CAction::~CAction
// Destructor for the CAction class
// -----------------------------------------------------------------------------
//
CAction::~CAction()
{
	delete iParams; 
	iParams=NULL;
	delete iPrintLog;
	iPrintLog = NULL;
	delete iScreenshotTag;
	iScreenshotTag = NULL;
	iKeys.Close();
}
// End of File
