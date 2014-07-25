@ECHO OFF

ECHO ----------------------------------------
ECHO [ %~dp0 ]

FOR %%F IN (example_*.bat) DO CALL :RUN %%F

EXIT /B
GOTO :EOF

:RUN
SET SCRIPT=%~dp0%1
IF EXIST %SCRIPT% CALL %SCRIPT%
GOTO :EOF
