#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:  This make file will build the application help file (.hlp)
#
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : UserEmulator
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : UserEmulator
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : UserEmulator_0x2001C3AF.hlp
UserEmulator_0x2001C3AF.hlp : UserEmulator.rtf UserEmulator.cshlp Custom.xml
	cshlpcmp UserEmulator.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy UserEmulator_0x2001C3AF.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del UserEmulator_0x2001C3AF.hlp
	del UserEmulator_0x2001C3AF.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo UserEmulator_0x2001C3AF.hlp

FINAL : do_nothing
