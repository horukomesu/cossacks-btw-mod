@echo off
setlocal
rem Build Russia AI DLL for Cossacks (Win32)
if not exist "%~dp0AI_MODULES\AI_0995\AI\RussiaAI.vcxproj" (
  echo ERROR: Project file not found: AI_MODULES\AI_0995\AI\RussiaAI.vcxproj
  exit /b 1
)

call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat" -arch=x86 -no_logo || goto :eof

set LOG=AI_MODULES\AI_0995\AI\russia_build.log
del "%LOG%" 2>nul

msbuild "AI_MODULES\AI_0995\AI\RussiaAI.vcxproj" ^
  /p:Configuration=Release ^
  /p:Platform=Win32 ^
  /m /nologo /v:m ^
  /fl /flp:logfile="%LOG%";verbosity=normal

type "%LOG%"
if exist src\Testing\Ai\Russia.dll (
  echo OK: built src\Testing\Ai\Russia.dll
) else (
  echo ERROR: Russia.dll not found
)

endlocal

