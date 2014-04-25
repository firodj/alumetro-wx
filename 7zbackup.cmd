@echo off
set projname=wxAlcatel
For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
For /f "tokens=1-3 delims=/: " %%a in ('time /t') do (set mytime=%%a-%%b-%%c)
if defined ProgramFiles (
set szapp="%ProgramFiles%\7-Zip\7z.exe"
) else (
set szapp=".\7za.exe"
)
if exist %szapp% (
%szapp% a %projname%_%mydate%_%mytime%.7z *.* -x@7zexclude.inf -r -ssw -scsUTF-8 -mx=9
) else (
echo ERROR: 7-zip not found.
)
set szapp=
pause
