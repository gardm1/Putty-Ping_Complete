@echo off

set input=%1

echo -----------------------------------------------------------------------
echo IMPORTANT: Be inside project root directory when running build.bat file
echo -----------------------------------------------------------------------

if "%input%" equ "all" (
	goto :labela
) else if "%input%" equ "c" (
	goto :labela
) else if "%input%" equ "go" (
	goto :labelb
) else if "%input%" equ "clean" (
	goto :labelc
) else (
	echo No build method selected...
	echo - build.bat all & echo - build.bat c & echo - build.bat go & echo - build.bat clean
	goto :end
)

:labela
echo - Building c files.
nmake >nul 2>&1

if %errorlevel% neq 0 (
	echo Building c files failed...
	echo Build [failed]. Exiting.
	goto :end
)
echo      Status: Success
echo.

if "%input%" equ "all" (
	goto :labelb
) else (
	echo Building c files [succeeded].
	goto :end
)

:labelb
echo - Building go files.
go build -o bin\main.exe src\main.go

if %errorlevel% neq 0 (
	echo Building go files failed...
	echo Build [failed]. Exiting.
	goto :end
)
echo      Status: Success

echo.

if "%input%" equ "all" (
	echo Build [succeeded]. exe and dll is located in bin
) else (
	echo Building go files [succeeded].
)

goto :end

:labelc
echo Removing bin\main.exe, bin\process.dll, src\*.obj
nmake clean
echo         del bin\main.exe
del "bin\main.exe"

goto :end

:end
pause
exit /b 0
