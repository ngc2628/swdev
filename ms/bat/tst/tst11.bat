
@echo off
@echo/

set folder=%1
if "%folder%" == "" goto usage 
if not exist %folder%\* (
  @echo %folder% is not a directory
  goto usage
)

setlocal

set ftmp=%tmp%\fp.txt
pushd %folder%
set path_folder=%cd%

dir /b >%ftmp% 
set /p fL=<%ftmp%
rem del %ftmp%

@echo %fL%
set /p xx="### "

setlocal enabledelayedexpansion

for /f  %%a in (%tmp%\fp.txt) do (
  @echo line 25 : %%a
  set fname=%path_folder%\%%a
  @echo line 27 : !fname!
)

@echo/
popd

endlocal
exit /b 0

:usage
@echo usage : script "folder"
exit /b 1




