#############################################################################
# comp.mak
# 	makefile for building of the compiler from refal-2 language, 
# 	result bin\refal2 executable module.
#       Version for Windows (Win95/98/ME/NT/2000/XP)
#
# Copyright (C) Refal-2 Team
# Author: Leonid Belous (aka BLF) 
# E-mail: belous@ilt.kharkov.ua
# Date: 28.05.2005
#############################################################################

####### Compiler, tools and options

.SUFFIXES: .c 

CC		=	gcc
CFLAGS		=	-std=c11 -pipe -w -O2 -DNO_DEBUG
LINK		=	gcc

TAR		=	tar -cf
GZIP		=	gzip -9f
DEL		=	del
S		=	src\comp
BIN_DIR		=	bin

####### Files

SOURCES =			\
		$(S)\ccst.c 	\
		$(S)\comp.c 	\
		$(S)\cgop.c 	\
		$(S)\cj.c 	\
		$(S)\clu.c 	\
		$(S)\cs.c 	\
		$(S)\..\lib.c

OBJECTS =			\
		$(S)\ccst.o 	\
		$(S)\comp.o 	\
		$(S)\cgop.o 	\
		$(S)\cj.o 	\
		$(S)\clu.o 	\
		$(S)\cs.o 	\
		$(S)\..\lib.o

TARGET =	refal2


####### Implicit rules

.c.o:
	$(CC) -o $@ $< -c $(CFLAGS) 

####### Build rules

all:	$(TARGET)

$(TARGET): $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(BIN_DIR)\$(TARGET) $(OBJECTS) 

####### Dependences

$(S)\ccst.o: $(S)\ccst.c $(S)\..\lib.c

$(S)\comp.o: $(S)\cerr.c $(S)\..\lib.c

$(S)\cgop.o: $(S)\cgop.c $(S)\..\lib.c

$(S)\cj.o: $(S)\cj.c $(S)\..\lib.c

$(S)\clu.o: $(S)\clu.c $(S)\..\lib.c

$(S)\cs.o: $(S)\cs.c $(S)\..\lib.c