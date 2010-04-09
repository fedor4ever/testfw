@rem
@rem Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description: 
@rem
@echo off
rem ---remove any old zip files if exists---
@del *.zip 2>1

rem --- prepare zip file name STF_Version_Timestamp.zip
set VER=1.0.0

rem Date
set DATE0=%date%
if %DATE0% equ 0 (set DATE0=%date:~-10%) else (set DATE0=%date:~,10%)
rem time
set tsp="%time:~0,1%"
if %tsp%==" " (set TIME0=0%time:~1,1%%time:~3,2%%time:~6,2%)&goto NEXTTASK:
set TIME0=%time:~0,2%%time:~3,2%%time:~6,2%
rem time stamp
:NEXTTASK
set TMST=%DATE0%_%TIME0%

echo STF Version: %VER%
set ZIPFILE=STF_%VER%_%TMST%.zip
echo Package file name: %ZIPFILE%

rem ---------BUILD----------
rem call abld reallyclean
rem call bldmake clean
rem call bldmake bldfiles
rem call abld build

rem ---------packaging---------
rem "abld build -what" to get all exported files
call abld build -what > TEMP

for /f %%l in (TEMP) do (
@zip %ZIPFILE% %%l
)

del TEMP


rem ---------BUILD STFQTUI----------
rem pushd %cd%

rem cd ..\stfui\qtui\StfQtUI
rem call qmake
rem call bldmake bldfiles
rem call abld build

rem ---------packaging STFQTUI---------
rem "abld build -what" to get all exported files
rem call abld build -what > TEMP

rem for /f %%l in (TEMP) do (
rem @zip ..\..\..\group\%ZIPFILE% %%l
rem )

rem del TEMP

rem popd

rem ---------packaging MigrationExample---------
set TEMPLATE_TARGET=\epoc32\tools\s60rndtools\stf\MigrationExample
zip %ZIPFILE% %TEMPLATE_TARGET%\*
zip %ZIPFILE% %TEMPLATE_TARGET%\*\*
zip %ZIPFILE% %TEMPLATE_TARGET%\*\*\*
zip %ZIPFILE% %TEMPLATE_TARGET%\*\*\*\*

rem --------packaging testmoduletemplates--------
set TEMPLATE_TARGET=\epoc32\tools\s60rndtools\stf\testmoduletemplates
zip %ZIPFILE% %TEMPLATE_TARGET%\*
zip %ZIPFILE% %TEMPLATE_TARGET%\*\*
zip %ZIPFILE% %TEMPLATE_TARGET%\*\*\*

rem ---------create and add sis package into zip file--------
set SIS_TARGET=\epoc32\tools\s60rndtools\stf\sis
md %SIS_TARGET%
rem set KEY_DIR to the real path
rem set KEY_DIR=YOUR_KEY_DIR
copy ..\sis\stf.pkg %SIS_TARGET%\stf.pkg /y
rem copy ..\stfui\qtui\StfQtUI\StfQtUI.pkg %SIS_TARGET%\StfQtUI.pkg /y
rem ----create sis----
makesis %SIS_TARGET%\stf.pkg %SIS_TARGET%\stf.sis
rem makesis %SIS_TARGET%\StfQtUI.pkg %SIS_TARGET%\StfQtUI.sis
rem ----sign sis------
rem signsis -s %SIS_TARGET%\stf.sis %SIS_TARGET%\stf.sisx %KEY_DIR%\rd.crt %KEY_DIR%\rd.key
rem signsis -s %SIS_TARGET%\StfQtUI.sis %SIS_TARGET%\StfQtUI.sisx %KEY_DIR%\rd.crt %KEY_DIR%\rd.key
rem -----zip all------
zip %ZIPFILE% %SIS_TARGET%\*

rem -----STF_read_me.txt------
zip %ZIPFILE% STF_read_me.txt