@echo off
if not defined VS80COMNTOOLS (
	echo In order to build this project you need MS Visual Studio 2005 to be installed.
	echo http://msdn.microsoft.com/vstudio/express/downloads/default.aspx
	exit
)

call "%VS80COMNTOOLS%\vsvars32.bat" || exit /b 1
devenv.com rgde.sln /rebuild release
devenv.com rgde.sln /rebuild debug
pause
