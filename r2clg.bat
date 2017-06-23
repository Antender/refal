@echo off
rem *******************************************************************
rem r2clg.bat
rem	compile from refal to GNU-assembler + link + go
rem 	arg: %1 - name of refal programm (without ext ref)
rem	result: running of %1.exe 
rem
rem Copyright (C) Refal-2 Team
rem Author: Leonid Belous (aka BLF)
rem E-mail: belous@ilt.kharkov.ua
rem Date: 21.05.2005
rem *******************************************************************


set REFALROOT="D:\sys\refal"
%REFALROOT%\bin\refal2 %cd%\%1.ref (mm) > %cd%\%1.log
as -o %cd%\%1.o %cd%\%1.asm >> %cd%\%1.log
gcc -o %cd%\%1.exe %REFALROOT%\lib\r2objects\mainrf.o %cd%\%1.o -Wl,-L%REFALROOT%\lib -Wl,-lrefal2,-lr2user >> %cd%\%1.log
%cd%\%1.exe
