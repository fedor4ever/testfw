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
 * File Name: Constants.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss
 *   
 * Description:
 * Constant container file
 *------------------------------------------------------------------
 -
 *
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

/**
 * Buffer constants
 */
const TInt KBuffer8 = 8;
const TInt KBuffer10 = 10;
const TInt KBuffer12 = 12;
const TInt KBuffer20 = 20;
const TInt KBuffer32 = 32;
const TInt KBuffer64 = 64;
const TInt KBuffer100 = 100;
const TInt KBuffer128 = 128;
const TInt KBuffer200 = 200;
const TInt KBuffer256 = 256;
const TInt KBuffer512 = 512;
const TInt KBuffer540 = 540;
const TInt KBuffer1024 = 1024;
/**
 * General constants
 */
_LIT(KN,"");
_LIT(KHEX,"0x");
_LIT(K0x,"0x%x");
_LIT(KNEWLINE, "\n");
_LIT(KComma, ",");
_LIT(KIDLE, "Should not be idle");
_LIT(KPATHDELIM, "\\\\");
_LIT(KDRIVEDEL, ":\\");
_LIT(KEmptyItemText, "(none)");
_LIT(KBsh,"\\");
_LIT(KDOT, ".");
_LIT(KCSVEXT,".csv");
_LIT(KXMLEXT,".xml");
_LIT(KEMPTYTEXT,"0");
_LIT(KSPACE," ");
_LIT(KSLASHR,"/r");

_LIT(KMinus1,"-1");
_LIT8(KInfinite,"infinite");
/**
 * UIDs
 */
const TUid KUidMenu =           { 0x101f4cd2 };
const TUid KUidPanicCapturer =  { 0x2001C3A9 };
const TUid KUidHomeScreen =     { 0x102750f0 };
const TUid KUidAknCapServer =   { 0x10207218 };
const TUid KUidAknnfySrv =      { 0x10281ef2 };
const TUid KUidPenInputSrv =    { 0x10281855 };
const TUid KUidTelephony =      { 0x100058b3 };
const TUid KUidCamera =         { 0x101f857a };
/**
 * Time constants
 */
const TInt KWait2 = 2000000;
const TInt KWait1 = 1000000;
const TInt KWait08 = 800000;
const TInt KWait05 = 500000;
const TInt KWait03 = 300000;
const TInt KWait02 = 200000;
const TInt KWait01 = 100000;
const TInt KWait005 = 50000;
const TInt KWait001 = 10000;
/**
 * Manufacturer constants
 */
const TInt KManufacturerNokia =         2;
const TInt KManufacturerSamsung =       270499451;
const TInt KManufacturerSonyEricsson =  270494957;

#endif /* CONSTANTS_H_ */
