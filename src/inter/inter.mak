#############################################################################
# inter.mak
#	Makefile for interpreter library (librefal2.a) creation,
# 	result lib\librefal2.a library
#       Version for Windows (Win95/98/ME/NT/2000/XP)
#
# Copyright (C) Refal-2 Team
# Author: Leonid Belous (aka BLF)
# E-mail: belous@ilt.kharkov.ua
# Date: 28.05.2005
#############################################################################

####### Compiler, tools and options

.SUFFIXES: .c .ref

CC	=	gcc
CFLAGS	=	-std=c11 -pipe -w -O2 -DNO_DEBUG
AR	=	ar
REFXCV	=	src\inter\refxcv.bat 	# refal2 + assembler for xcv
TAR	=	tar -cf
GZIP	=	gzip -9f
DEL	=	del
REFLIB	=	lib\librefal2.a
USERLIB	=	lib\libr2user.a
S	=      	src\inter
OBJ	=	lib\r2objects
NU	=	-fno-leading-underscore
####### Files

SOURCES =		\
	$(S)\d.c	\
	$(S)\mainrf.c 	\
	$(S)\rfdbg.c 	\
	$(S)\rfintf.c 	\
	$(S)\rfrun1.c 	\
	$(S)\rfrun2.c 	\
	$(S)\xapply.c 	\
	$(S)\xar.c 	\
	$(S)\xcf.c 	\
	$(S)\xgcd.c 	\
	$(S)\xjak.c 	\
	$(S)\xmo.c 	\
	$(S)\xrename.c 	\
	$(S)\xvv.c 	\
	$(S)\xvv5.c 	\
	$(S)\xcv.ref	\
	$(S)\..\lib.c

OBJECTS =		\
	$(S)\d.o 	\
	$(S)\mainrf.o 	\
	$(S)\rfdbg.o 	\
	$(S)\rfintf.o 	\
	$(S)\rfrun1.o 	\
	$(S)\rfrun2.o 	\
	$(S)\xapply.o 	\
	$(S)\xar.o 	\
	$(S)\xcf.o 	\
	$(S)\xcv.ref.o 	\
	$(S)\xgcd.o 	\
	$(S)\xjak.o 	\
	$(S)\xmo.o 	\
	$(S)\xrename.o 	\
	$(S)\xvv.o 	\
	$(S)\xvv5.o     \
	$(S)\..\lib.o

####### Implicit rules

%.ref.o: 
	$(REFXCV)
x%.o:
	$(CC) $(CFLAGS) $(NU) -c $< -o $@  
.c.o:
	$(CC) $(CFLAGS) -c $< -o $@ 

####### Build rules

all: $(REFLIB)

$(REFLIB): $(OBJECTS)
	$(AR) rs $(REFLIB) $(OBJECTS) 
	$(AR) rs $(USERLIB)
	-copy $(S)\mainrf.o $(OBJ)
	-copy $(S)\rfdbg.o $(OBJ)


####### Dependences

$(S)\d.o:	$(S)\d.c $(S)\..\refal.h $(S)\debug.def

$(S)\mainrf.o: 	$(S)\mainrf.c

$(S)\rfdbg.o:	$(S)\rfdbg.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\rfintf.o: 	$(S)\rfintf.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\rfrun1.o: 	$(S)\rfrun1.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\rfrun2.o: 	$(S)\rfrun2.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xapply.o: 	$(S)\xapply.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xar.o: 	$(S)\xar.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xcf.o: 	$(S)\xcf.c $(S)\..\refal.h $(S)\..\lib.c  

$(S)\xgcd.o: 	$(S)\xgcd.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xjak.o: 	$(S)\xjak.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xmo.o: 	$(S)\xmo.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xrename.o: $(S)\xrename.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xvv.o: 	$(S)\xvv.c $(S)\..\refal.h $(S)\..\lib.c

$(S)\xvv5.o: 	$(S)\xvv5.c $(S)\..\refal.h $(S)\..\lib.c
