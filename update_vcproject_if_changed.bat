@ECHO OFF

SETLOCAL


REM Aktuellen Pfad in PWD speichern:
set PWD=%~dp0

rem SET QTDIR=%PWD%\3rdparty\Qt
rem SET QMAKE=%QTDIR%\bin\qmake.exe


SET PROJPATH=%1
SET PROJFILE=%2
SET OUTFILE=%3
SET FORCE=%4
SET QMAKE_OPTIONS=%5

cd %PROJPATH%

IF NOT EXIST %PROJFILE%.pro GOTO :filenotexists
IF NOT EXIST %OUTFILE% GOTO :update


FC /A %PROJFILE%.pro %PROJFILE%.pro.old 

REM Falls ERRORLEVEL == 0...(liest sich als "if errorlevel < 1")
IF "%FORCE%" == "FORCE" GOTO :update
IF NOT ERRORLEVEL 1 GOTO :uptodate


:update
ECHO *** 
ECHO *** Updating %PROJFILE%
ECHO *** 
REM %QMAKE% -Wlogic -spec %PWD%\Qt\mkspecs\win32-msvc2005 -o %PROJFILE%.vcproj %QMAKE_OPTIONS% %PROJFILE%.pro
qmake -Wlogic -tp vc -spec win32-msvc2005 %QMAKE_OPTIONS% %PROJFILE%.pro

copy %PROJFILE%.pro %PROJFILE%.pro.old /Y
ECHO *** 
ECHO *** Done.
ECHO *** 
GOTO :end


:filenotexists
ECHO *** 
ECHO *** Warning! File %PROJFILE%.pro not found!
ECHO *** 
GOTO :end

:uptodate
ECHO *** 
ECHO *** File %PROJFILE%.pro is up-to-date.
ECHO *** 
GOTO :end

:end
cd %PWD%
