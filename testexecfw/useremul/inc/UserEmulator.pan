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
 * File Name: UserEmulator.pan
 * 
 * Created: 13/08/2009
 * Author(s): Marcell Kiss, Reshma Sandeep Das
 *   
 * Description:
 * This file contains panic codes
 *------------------------------------------------------------------
 -
 *
 */

#ifndef USEREMULATOR_PAN__
#define USEREMULATOR_PAN__

/** 
 * UserEmulator application panic codes 
 */

enum TUserEmulatorPanics
	{
	EUserEmulatorUi = 1
	// add further panics here
	};

inline void Panic(TUserEmulatorPanics aReason)
	{
	_LIT(KAPPLICATIONNAME, "UserEmulator");
	User::Panic(KAPPLICATIONNAME, aReason);
	}

#endif // __USEREMULATOR_PAN__
