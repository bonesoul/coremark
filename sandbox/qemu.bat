REM Start qemu on windows.
@ECHO OFF

cd qemu
qemu.exe -L . -m 32 -s -boot a -fda "..\kernel.img"