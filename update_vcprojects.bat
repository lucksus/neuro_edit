@ECHO OFF
REM Dieses Skript erzeugt aus den qmake-Projektdateien aktuelle Visual Studio Projekt-Dateien.
REM Es sollte in TortoiseSvn als client-side hook script bei "post-update" angegeben werden.

REM Als Parameter können zusätzliche qmake-Parameter angegeben werden.

REM Variablen gelten nur lokal in diesem Script:
SETLOCAL

SET FORCE=NOFORCE
SET QMAKE_OPTIONS=%1
IF a%1 == aFORCE GOTO :force
:endforce

ECHO %QMAKE_OPTIONS%>qmake_options.cur
FC /A qmake_options.cur qmake_options.old

IF errorlevel 1 SET FORCE=FORCE

move /Y qmake_options.cur qmake_options.old

CALL update_vcproject_if_changed.bat NeuronModels\ NeuronModels NeuronModels.vcproj %FORCE% %QMAKE_OPTIONS%
CALL update_vcproject_if_changed.bat NeuroEdit\ NeuroEdit NeuroEdit.vcproj %FORCE% %QMAKE_OPTIONS%

CALL update_vcproject_if_changed.bat . Neuro Neuro.sln %FORCE% %QMAKE_OPTIONS%

ECHO Done.

GOTO :eof

:force
ECHO Forcing update...
SET FORCE=FORCE
SET QMAKE_OPTIONS=%2
GOTO :endforce
