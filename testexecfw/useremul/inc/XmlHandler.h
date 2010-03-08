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
 * File Name: XmlHandler.h
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

#ifndef XMLHANDLER_H__
#define XMLHANDLER_H__

// System Includes
#include <e32base.h>
#include <xml\contenthandler.h> // for MContentHandler
#include <xml\parser.h> // for CParser

//User Includes
#include "Constants.h"

using namespace Xml;

/**
 * TActionType 
 * Enumaration defining different action types
 */
enum TActionType
{
	EActionTypeUnknown = 0,
	EActionTypeView,
	EActionTypeApp,
	EActionTypeUrl,
	EActionTypeAppUrl,
	EActionTypeJavaApp,
	EActionTypeWait,
	EActionTypeKeys,
	EActionTypeOrientationChange,
	EActionTypePointerEvent,
	EActionCloseApp,
	EActionTypeOrientation,
	EActionTypeScreenReset,
	EActionTypeStartScript,
	EActionTypeEndScript,
	EActionTypeLoopStart,
	EActionTypeLoopEnd
};

/**
 * TState 
 * Enumaration containing different states of the XML parser
 */
enum TState
{
	EIdle,
	EStartDocument,
	EEndDocument,
	EOnContent,
	EOnStartElement,
	EOnEndElement,
	EOnStartPrefixMapping,
	EOnEndPrefixMapping,
	EOnIgnorableWhiteSpace,
	EOnSkippedEntity,
	EOnProcessingInstruction,
	EOnError
};
/**
 * CAction 
 * Class that contains information for each action element in the XML file
 */
class CAction : public CBase
{
public:
	/**
	 * Destructor
	 */
	virtual ~CAction();
	
   /**
	* Enumeration describing the action type 
	*/
	TActionType	    iType;
   /**
	* buffer containing parameters like appuid, pointerevents
	*/
	HBufC*			iParams;
   /**
	* buffer containing screenshot tag
	*/
	HBufC*			iScreenshotTag;
   /**
	* buffer containing print logs
	*/
	HBufC*			iPrintLog;
   /**
	* An array contining list of keyevents
	*/
	RArray<TUint>   iKeys;
};

/**
 * MXmlHandlerObserver, an observer to CXmlHandler class.
 */
class MXmlHandlerObserver
{
public:
	/**
	 * Callback to notify the observer on XML file parse completion
	 * @param aError Error Code
	 * @param aState XML parser state
	 */
    virtual void OnParseCompletedL( TInt aError, TState aState) = 0;  
};

/**
 * CXmlHandler, a class to parse XML file and then output log information
 * to a buffer.
 */
class CXmlHandler: public CActive, MContentHandler
{
        
public: // Constructors and destructor
    
   /**
	* Static constructor
	* @param aObserver A reference to the XML handler observer.
	* @param aActionList Ref to the action list array
	* @return CXmlHandler* A pointer to the newly alocated CXmlHandler class.
	*                      NULL, if the class cannot be created
	*/
    static CXmlHandler* NewL( MXmlHandlerObserver& aObserver,RPointerArray<CAction>& aActionList);
   /**
	* Static constructor. On return the instance is left to the CleanupStack
	* @param aObserver A reference to the XML handler observer.
	* @param aActionList Ref to the action list array
	* @return CXmlHandler* A pointer to the newly alocated CXmlHandler class.
	*                      NULL, if the class cannot be created
	*/
    static CXmlHandler* NewLC( MXmlHandlerObserver& aObserver,RPointerArray<CAction>& aActionList);
    		 				   
    
   /**
	* Destructor
	*/
    virtual ~CXmlHandler();
    
public: // Public methods

   /**
	* Parses XML document  
	* @param aFileName XML file name
	* return error code
	*/
    TInt StartParsingL( const TDesC& aFileName );
   /**
	* Parses XML document with an Active object  
	* @param aFileName XML file name
	*/
    void StartParsingWithAoL( const TDesC& aFileName );
 
private: // Constructors

   /**
	* Constructor
	* @param aObserver A reference to the XML handler observer.
	* @param aActionList Ref to the action list array
	*/
    CXmlHandler( MXmlHandlerObserver& aObserver,RPointerArray<CAction>& aActionList);

    /**
     * ConstructL
     */
    void ConstructL();

private: // from CActive
   
   /**
	* Cancels any outstanding request.
	*/
	void DoCancel();
	
   /**
	* Handles the completion of the active request.
	*/
	void RunL();
    
private: 
	// from MContentHandler
    void OnStartDocumentL( const RDocumentParameters &aDocParam, TInt aErrorCode );
    void OnEndDocumentL( TInt aErrorCode );
    void OnStartElementL( const RTagInfo &aElement, const RAttributeArray &aAttributes,
						  TInt aErrorCode );
    void OnEndElementL( const RTagInfo &aElement, TInt aErrorCode );
    void OnContentL( const TDesC8 &aBytes, TInt aErrorCode );
    void OnStartPrefixMappingL( const RString &aPrefix, const RString &aUri,
								TInt aErrorCode );
    void OnEndPrefixMappingL( const RString &aPrefix, TInt aErrorCode );    
    void OnIgnorableWhiteSpaceL( const TDesC8 &aBytes, TInt aErrorCode );    
    void OnSkippedEntityL( const RString &aName, TInt aErrorCode );    
    void OnProcessingInstructionL( const TDesC8 &aTarget, const TDesC8 &aData,
									TInt aErrorCode);
    void OnError( TInt aErrorCode );    
    TAny *GetExtendedInterface( const TInt32 aUid );
    
private:
   /**
	* Function to expand the actual buffer size
	* @param aText buffer
	*/   
    void AppendTextL( const TDesC& aText );
    
private: // Private data

   /**
	* Enumeration with different content types
	*/ 
	enum {
			EContentNone,
			EContentName,
			EContentType,
			EContentParams,
			EContentPrintLog,
			EContentKeys,
			EContentScreenshot,
			EContentUnknown
			};
	
   /**
	* Handle to XML Observer
	*/ 
    MXmlHandlerObserver& iObserver;
   /**
	* Handle to parser object
	*/ 
    CParser*             iParser;
   /**
	* Handle to XML buffer
	*/ 
    HBufC8*              iBuffer;
   /**
	* Buffer containing the final display message
	*/
    HBufC*               iDisplayResult;
   /**
	* XML parser state
	*/
    TState               iState;
   /**
	* File handle
	*/
    RFile                iFile;
   /**
	* Reference to array containing list of action elements
	*/
    RPointerArray<CAction>&  iActionList; // not owned
    
   /**
	* XML content
	*/
	HBufC8*          iContent;
   /**
	* Content type
	*/
	TInt             iContentType;
   /**
	* Reference to action objects
	*/
	CAction*         iAction;
   /**
	* Index to the CLoopObj element in the iLoopObjList
	*/
	TInt             iIndex;
   /**
	* Action list index element
	*/
	TInt             iActionListIndex;
};

#endif /* XMLHANDLER_H__ */

// End of File
