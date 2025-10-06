@echo off

if "%2"=="" goto Incompleto

set FicEnt=%1
set DirSal=%2

echo "----------------------------------------------------------------------------"
echo " PostLinkGLEW"
echo "----------------------------------------------------------------------------"
echo " FicEnt = %FicEnt%"
echo " DirSal = %DirSal%"
echo "----------------------------------------------------------------------------"
echo " Copiamos las Dlls en la carpeta de ejecutables:"
echo "   glew32.dll o glew32d.dll para Win32"
echo "----------------------------------------------------------------------------"
echo "copy %FicEnt% %DirSal%"
copy %FicEnt% %DirSal%

goto Fin

:Incompleto
echo "----------------------------------------------------------------------------"
echo Comando %0 incompleto
goto Fin

:Fin
echo "----------------------------------------------------------------------------"
