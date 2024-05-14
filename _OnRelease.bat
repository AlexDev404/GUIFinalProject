@echo off
chcp 65001
echo Project: GUIFinalProject.sln
echo.
echo 🔃 Setting up environment...
set PATH=%PATH%;C:\Qt\6.6.2\msvc2019_64\bin;
cd release
echo ✅ Loading application...
call GUIFinalProject.exe
echo 🏁 Performing exit.
timeout 2 > nil
exit