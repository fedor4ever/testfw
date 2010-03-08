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
 * File Name: Utils.h
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * Utility class having helper functions 
 *------------------------------------------------------------------
 -
 *
 */
//System Includes
#include <e32base.h>
#include <w32std.h>

// Class Declaration
/**
 *  Implements helper functions.
 */

class CUtils : public CBase
{
public:
   
   /**
	* Function displaying Information notes
	* @param aMessage Message to be displayed.
	*/
	static void MessageBoxL(const TDesC& aMessage);
	 
   /**
	* Function used to strip or remove extra spaces 
	* @param aText 8 bit Input descriptor
	*/
	static void StripSpaces(TDes8& aText);
	
   /**
	* Function used to strip or remove extra spaces 
	* @param aText 16 bit Input descriptor 
	*/
	static void StripSpaces(TDes& aText);
};
