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
 * File Name: UserEmulatorDocument.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Declares document class for application
 *------------------------------------------------------------------
 -
 *
 */
#ifndef USEREMULATORDOCUMENT_H__
#define USEREMULATORDOCUMENT_H__

// System includes
#include <akndoc.h>

// Forward Declarations
class CUserEmulatorAppUi;
class CEikApplication;

// CLASS DECLARATION

/**
 * CUserEmulatorDocument application class.
 * An instance of class CUserEmulatorDocument is the Document part of the
 * AVKON application framework for the UserEmulator example application.
 */
class CUserEmulatorDocument : public CAknDocument
	{
public:
	// Constructors and destructor

	/**
	 * NewL.
	 * Two-phased constructor.
	 * Construct a CUserEmulatorDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CUserEmulatorDocument.
	 */
	static CUserEmulatorDocument* NewL(CEikApplication& aApp);

	/**
	 * NewLC.
	 * Two-phased constructor.
	 * Construct a CUserEmulatorDocument for the AVKON application aApp
	 * using two phase construction, and return a pointer
	 * to the created object.
	 * @param aApp Application creating this document.
	 * @return A pointer to the created instance of CUserEmulatorDocument.
	 */
	static CUserEmulatorDocument* NewLC(CEikApplication& aApp);

	/**
	 * ~CUserEmulatorDocument
	 * Virtual Destructor.
	 */
	virtual ~CUserEmulatorDocument();

public:
	// Functions from base classes

	/**
	 * CreateAppUiL
	 * From CEikDocument, CreateAppUiL.
	 * Create a CUserEmulatorAppUi object and return a pointer to it.
	 * The object returned is owned by the Uikon framework.
	 * @return Pointer to created instance of AppUi.
	 */
	CEikAppUi* CreateAppUiL();

private:
	// Constructors

	/**
	 * CUserEmulatorDocument.
	 * C++ default constructor.
	 * @param aApp Application creating this document.
	 */
	CUserEmulatorDocument(CEikApplication& aApp);

	};

#endif // USEREMULATORDOCUMENT_h__
// End of File
