
SHELL = /bin/sh

# this is permanent, other value not tested yet.
DEST  = /usr/local

#
# 1996 by Sergey Vovk
#
# workaround to make all projects
#
# very early version, use it for Your own risk only.
#
# Please, first do  check all READ_ME files under sources tree.
#
# Uncomment exclusively the lines ASHLIB,CC,LINKER,CFLAGS_VISI,CFLAGS_ASH
# for the platform required
#
# make all     - compile sources & external visi pages
# make install - install binaries
# make clean   - clean all temporaries

# alpha osf1
# DEC ALPHA AXP, OSF/1 3.2 or DEC UNIX
#
#ASHLIB      = SVR4
#CFLAGS_VISI = -DUSE_TERMIOS
#CFLAGS_ASH  = -DJCLsuspend -DSYSV -DSVR4 -DMGROUPS
#CC	     = cc -std0 -g0 -O0 -taso -misalign
#CC	     = cc -std0 -g0 -O3 -taso -misalign
#LINKER      = cc -taso -misalign

# uw
# UnixWare 2.0.x, 1.1.x
# probably Solaris 2.x with WorkShop C (not tested)
#
#CFLAGS_VISI = -DUSE_TERMIOS
#CFLAGS_ASH  = -DJCLsuspend -DSYSV -DSVR4 -DMGROUPS
#ASHLIB      = SVR4
#CC	     = cc -O
#LINKER      = cc

# bsdi
# FreeBSD, BSDI, or similar
#
CFLAGS_VISI = -DUSE_TERMIOS_386BSD -DNOUSE_STRCHR
CFLAGS_VISI = -DUSE_TERMIOS_386BSD
CFLAGS_ASH    = -DJCLsuspend -DFbsd -DBSDpagesize -DBSDmagic -DMGROUPS
CFLAGS_ASH    = -DJCLsuspend -DFbsd -DBSDpagesize -DBSDmagic
CFLAGS_ASH    = -DJCLsuspend -DFbsd -DBSDmagic
ASHLIB      = BSD
CC          = gcc -traditional
LINKER      = gcc -traditional

# sunos
# SunOS 4.1.x
#
#CFLAGS_VISI = -DUSE_TERMIOS
#CFLAGS_ASH  = -DJCLsuspend -DBSDmagic -DMGROUPS
#ASHLIB      = BSD
#CC          = cc -O
#LINKER      = cc

# isc
# Interactive UNIX 3.0
#
#CFLAGS_VISI = -DUSE_TERMIO
#CFLAGS_ASH  = -DJCLsuspend -DSYSV
#ASHLIB      = ISC
#CC          = cc -O
#LINKER      = cc

# dynix
# Dynix/PTX 1.4.0
# NOTES: not tested yet
#
#CFLAGS_VISI = -DUSE_TERMIO
#CFLAGS_ASH  = -DJCLsuspend -DSYSV
#ASHLIB      = ISC
#CC          = cc -O -Wc,-bcs
#LINKER      = cc -Wc,-bcs

info:
	@echo 'Usage: manually edit Makefile'
	@echo '    make all'
	@echo '    make install'
	@echo '    make clean'

all:    visi/include/line.h ashlib/LIB-$(ASHLIB)
	cd visi/src;        make install "CC=$(CC)" "LINKER=$(LINKER)" \
			"CFLAGS_VISI=$(CFLAGS_VISI)"
	cd visi/prog/vhset; make all "DEST=$(DEST)" "CC=$(CC)" "LINKER=$(LINKER)"
	cd ash;             make all "DEST=$(DEST)" "CC=$(CC)" \
			"LINKER=$(LINKER)" "CFLAGS_ASH=$(CFLAGS_ASH)"
	cd ashlib/LIB;           make "DEST=$(DEST)" all
	cd ashlib/LIB-$(ASHLIB); make "DEST=$(DEST)" all

install:    $(DEST)
	cd visi/prog/vhset; make install "CC=$(CC)" "LINKER=$(LINKER)" \
			"CFLAGS_VISI=$(CFLAGS_VISI)"
	cd ash;             make install "CC=$(CC)" \
			"LINKER=$(LINKER)" "CFLAGS_ASH=$(CFLAGS_ASH)"
	cd ashlib/LIB;           make "DEST=$(DEST)" install
	cd ashlib/LIB-$(ASHLIB); make "DEST=$(DEST)" install
	cd visilib; make "DEST=$(DEST)" install

clean:
	cd visi/bin; rm -f *
	cd visi/lib; rm -f *
	cd visi/src; make clean
	cd visi/prog/vhset; make clean
	cd ash; make clean
	cd ashlib/LIB; make clean
	cd ashlib/LIB-$(ASHLIB); make clean

cleandist:
	find . '('      -name core -o \
			-name '*.b' -o \
			-name '*~' -o \
			-name 'core.*' -o \
			-name '*.core' -o \
			-name a \
		')' -exec rm -f '{}' ';'
	rm -f ./Makefile

$(DEST):
	-mkdir -p $(DEST)
	mkdir $(DEST)
