
@echo off


setlocal enabledelayedexpansion

set topt=
set folder=%cd%

for %%a in (%*) do (
  set arg=%%a
  if "!arg:~0,1!"=="-" (
    if "!arg!"=="-t"  (
      set topt=/od
    ) 
  ) else (
    set folder=!arg!
  )
) 

dir /a /x %topt% %folder%





