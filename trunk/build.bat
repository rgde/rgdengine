@echo off
if not defined VS90COMNTOOLS (
	echo In order to build this project you need MS Visual Studio 2008 to be installed.
	echo http://msdn.microsoft.com/vstudio/express/downloads/default.aspx
	exit
)

call "%VS90COMNTOOLS%\vsvars32.bat" || exit /b 1
devenv.com build.sln /rebuild release
devenv.com build.sln /rebuild debug
pause
