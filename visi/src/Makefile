
SHELL    = /bin/sh

#       +----------+    ���������� �����-������
#      (C) LINLIB  !    ��� ���������-��������
#       +----------+    ���������������
#
#       ��������� ��������� � �������� ������������
#

#       $Header$
#       $Log$

DEST     = ../..
CC       = cc

# ����������� �� ../src/Makefile, ������ ��������� �������� ������� �� line.h
#CGLAGS_VISI = -DUSE_TERMIO
#CGLAGS_VISI = -DUSE_SGTTYH
#CGLAGS_VISI = -DUSE_TERMIOS_386BSD
#CGLAGS_VISI = -DUSE_TERMIOS

# ����� �� ������������ CLAGS � ���� Makefile
CFLAGS   =
LINKER   = cc

MAKEFILE = Makefile

ALL      =  lbp  vcc  linebp  linusr  linlib  termnew
ALLINST  = .lbp .vcc .linebp .linusr .linlib .termnew

all:            $(ALLINST)

install:        $(ALLINST)

first:
	 -cd ../include; make co
	 sh -cx "for i in $(ALL); do (cd \$$i; make co); done"
	 -cd linlib; mkdir $(DEST)/lib $(DEST)/bin

depend:; sh -cx "for i in $(ALL); do (cd \$$i; make depend); done"

clean:;  sh -cx "for i in $(ALL); do (cd \$$i; make clean); done"
	 rm -f $(ALLINST)

std:    .std

local:  .local

.std:
		mkmf -f $(MAKEFILE) "DEST=/usr"
		: "Makefile ��������:"
		: "���������� ����� ����������� �� ����������� �����"

.local:
		mkmf -f $(MAKEFILE) "DEST=../.."
		: "Makefile ��������:"
		: "���������� ����� ����������� � ������� �������� ./.."

.termnew:;      cd termnew; make install "DEST=$(DEST)/lib" "CC=$(CC)"
		touch .termnew

.linlib:;       cd linlib;  make install "DEST=$(DEST)/lib" "CC=$(CC)" \
			"CFLAGS_VISI=$(CFLAGS_VISI)"
		touch .linlib

.linusr:;       cd linusr;  make install "DEST=$(DEST)/lib" "CC=$(CC)"
		touch .linusr

.linebp:;       cd linebp;  make install "DEST=$(DEST)/lib" "CC=$(CC)"
		touch .linebp

.lbp:;          cd lbp;     make install "DEST=$(DEST)/bin" "CC=$(CC)" "LINKER=$(LINKER)"
		touch .lbp

.vcc:;          cd vcc;    make install  "DEST=$(DEST)/bin" "CC=$(CC)" "LINKER=$(LINKER)"
		touch .vcc

$(DEST)/include:
		mkdir $(DEST)/include

$(DEST)/lib:
		mkdir $(DEST)/lib

$(DEST)/bin:
		mkdir $(DEST)/bin

###
