/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
/*
 * StfTestLibPluginInterface.h
 *
 *  Created on: Nov 10, 2009
 *      Author: h112wang
 */

#ifndef STFTESTLIBPLUGININTERFACE_H_
#define STFTESTLIBPLUGININTERFACE_H_

class CStfTestLibPlugin : public CBase {
public:
   

    /**
    * Result indicates eiterh plug-in is able to process command 
    * @param aCommand command to check
    */
    virtual TBool IsCommandSupported(const TDesC& aCommand) = 0;

    /**
    * Execution of the specified library command
    * implementation of , should return ASAP.
    * aMessage contan is service specific.
    * it will return KErrNone for successfully execution
    * otherwise, it will return the error code
    * @param aCommandLine command to execute
 
    */
    virtual TInt ExecuteCommandL(const TDesC& aCommandLine) = 0;
 };




#endif /* STFTESTLIBPLUGININTERFACE_H_ */
