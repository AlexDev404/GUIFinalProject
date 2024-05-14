@echo off
cd ..\
chcp 65001
echo Project: GUIFinalProject.sln
echo.
echo Hi, there:)
echo 🔃 Setting up environment...
set PATH=%PATH%;C:\Qt\6.6.2\msvc2019_64\bin;
echo ☑️ Environment setup.
echo.
set /p PROMPT=📦 Release Type (release/debug): 
cd %PROMPT%
echo ✅ Loading application...
echo.
call GUIFinalProject.exe
echo.
echo 🏁 Performing exit.
timeout 2 > nil
exit