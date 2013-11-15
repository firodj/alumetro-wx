@echo off

For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
For /f "tokens=1-3 delims=/: " %%a in ('time /t') do (set mytime=%%a-%%b-%%c)

if not defined ProgramFiles goto :local7z
set szapp="%ProgramFiles%\7-Zip\7z.exe"
if exist %szapp% goto :doit

:local7z
set szapp=".\7za.exe"
if not exist %szapp% goto :notfound
goto :doit

:notfound
echo ERROR: 7-zip not found.
pause :done

:doit
%szapp% a wxAlcatel_%mydate%_%mytime%.7z *.* -x@7zexclude.inf -r -ssw -scsUTF-8 -mx=9

:done
set szapp=
pause
