@echo off
rem r2l.bat
rem 	linking
rem	result: %1.exe - executable module
rem Copyright (C) Refal-2 Team
rem Author: Leonid Belous (aka BLF) 
rem E-mail: belous@ilt.kharkov.ua
rem Date: 21.05.2005

bin\ld.exe -Bdynamic -o %1.exe obj\crt2.o obj\crtbegin.o -Llib obj\mainrf.o %1.o -lrefal2 -lmingw32 -lgcc -lmoldname -lmingwex -lmsvcrt -luser32 -lkernel32 -ladvapi32 -lshell32 obj\crtend.o > %1.log
