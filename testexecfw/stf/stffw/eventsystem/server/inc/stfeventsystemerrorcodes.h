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
#ifndef STFEVENTSYSTEMERRORCODES_H_
#define STFEVENTSYSTEMERRORCODES_H_

    enum TEventSystemErrorCodes
        {
        EInvalidSessionCounter               = -1101,
        EEventSystemVersionNotSupported      = -1102,
        EEventSystemSessionUnknownMessage    = -1103,
        EEventSystemSessionAlreadyWaiting    = -1104, //Session is already waiting for event and can't wait for another
        EEventSystemListAlreadySet           = -1105,
        EEventSystemNameAlreadySet           = -1106,
        EEventSystemNotInRequestedState      = -1007, //Want to wait for event which is not in requested state
        EEventSystemSessionNotDefined        = -1008,
        EEventSystemNotRequested             = -1009, //Event was not requested
        };


#endif /* STFEVENTSYSTEMERRORCODES_H_ */
