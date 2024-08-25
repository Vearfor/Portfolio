@echo off

if "%2"=="" goto Incompleto

set DirEnt=%1
set DirSal=%2

echo "----------------------------------------------------------------------------"
echo " DirEnt = %DirEnt%"
echo " DirSal = %DirSal%"
echo "----------------------------------------------------------------------------"
echo " Copiamos las Dlls en la carpeta de ejecutables:"
echo "   SDL3.dll"
echo "   SDL3_image.dll"
echo "----------------------------------------------------------------------------"
echo "copy %DirEnt%\SDL3.dll %DirSal%"
copy %DirEnt%\SDL3.dll %DirSal%

echo "copy %DirEnt%\SDL3_image.dll %DirSal%"
copy %DirEnt%\SDL3_image.dll %DirSal%

goto Fin

:Incompleto
echo "----------------------------------------------------------------------------"
echo Comando %0 incompleto
goto Fin

:Fin
echo "----------------------------------------------------------------------------"
