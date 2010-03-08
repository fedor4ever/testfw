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
 * File Name: Utils.cpp
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

// System Includes
#include <aknutils.h>
#include <rpbkviewresourcefile.h>
#include <aknnotewrappers.h>

// User Includes
#include "Utils.h"

void CUtils::MessageBoxL(const TDesC& aMessage)
{
    CAknInformationNote* informationNote = new ( ELeave ) CAknInformationNote;
    informationNote->ExecuteLD( aMessage );
}
    
void CUtils::StripSpaces(TDes8& aText)
	{
	TInt i;
	for (i=0; i<aText.Length(); i++)
		if (aText[i] != ' ')
			break;
	if (i)
		aText.Delete(0,i);
	for (i=aText.Length()-1; i>=0; i--)
		if (aText[i] != ' ')
			break;
	if (i != aText.Length()-1)
		aText.Delete(i+1,aText.Length()-1-i);
	}

void CUtils::StripSpaces(TDes& aText)
	{
	TInt i;
	for (i=0; i<aText.Length(); i++)
		if (aText[i] != ' ')
			break;
	if (i)
		aText.Delete(0,i);
	for (i=aText.Length()-1; i>=0; i--)
		if (aText[i] != ' ')
			break;
	if (i != aText.Length()-1)
		aText.Delete(i+1,aText.Length()-1-i);
	}
