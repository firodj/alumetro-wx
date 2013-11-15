@echo off
set projname=wxAlcatel
For /f "tokens=2-4 delims=/ " %%a in ('date /t') do (set mydate=%%c-%%a-%%b)
For /f "tokens=1-3 delims=/: " %%a in ('time /t') do (set mytime=%%a-%%b-%%c)
set szapp="c:\Program Files\7-Zip\7z.exe"
if not exist %szapp% set szapp=.\7za.exe
if exist %szapp% (
%szapp% a %projname%_%mydate%_%mytime%.7z *.* -x@7zexclude.txt -r -ssw -scsUTF-8 -mx=9
) else (
echo Could not find 7z executable.
)
pause