
set VCINSTALLDIR=C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC
set windeployqt="C:\Qt\Qt5.9.9\5.9.9\msvc2015_64\bin\windeployqt.exe"
set src="..\build-f3d-Desktop_Qt_5_9_9_MSVC2015_64bit-Release\release\f3d.exe"
set lib=.\lib
copy %src% .\Bundle
%windeployqt%  --no-opengl-sw .\Bundle\f3d.exe
pause