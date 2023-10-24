@echo off

del cirsi.exe
mkdir build
pushd build

set src_files=../src/main.c
set src_files=%src_files% ../src/device.c
set src_files=%src_files% ../src/universal.c

dir
REM cl /Fe:hwrec.exe /Zi %src_files% /I"../vendor/" /I"../vendor/SDL2_VC/include/" /link /LIBPATH:"../vendor/SDL2_VC/lib/x64/" /SUBSYSTEM:CONSOLE shell32.lib SDL2.lib SDL2main.lib

cl /Fe:cirsi.exe /Zi %src_files% shell32.lib 
copy cirsi.exe ..\
popd
