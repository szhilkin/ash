
SHELL   = /bin/sh

#
# �������� ����� ��� ������ � �������� ���������;
#
# --����������� ����:
# all:          ���������� � ������ � �������� �������
# program:      --- "" ---
# clean:        �������� ������������� ������ ����������
# depend:       ��������� Makefile ��� ������ mkmf;
# dis:          �� ��, ���� �������� ����� ��������� �� ../../bin
# std:          --- "" ---                              /usr/new/bin
# install:      ���������� ��������� �� �����
# lintcheck:    ��������� ������ ��������� ��� ������ lint (���� �� ������)
# update:       ������ ���������� � ���������
# co:           ������ �� ������ �������� ������
# rel:          ������ �� ������ �������� ������ ������, ��������� ����
# ci:           �������� � ����� ���������� ������ ���. �������
# unlock:       ����� ���������� ��� ���������� ����������� ���. �������

VERSION       = LINLIB_3

PROGRAM	      = lbp

CC            = cc
CFLAGS        = -I../../include
LINKER        = cc
LDFLAGS       =

LIBS          =
DEST	      = ../../bin
MAKEFILE      = Makefile
CI            = ci -f -n$(VERSION) -m"������ $(VERSION)" -sRel
CO            = co
UNLOCK        = rcs -l
VCC           = ../../bin/vcc


EXTHDRS	      = ../../include/line.h \
		../../include/linebp.h \
		/usr/include/ctype.h \
		/usr/include/stdio.h

HDRS	      =

OBJS	      = lbp.o

SRCS	      = lbp.c

all:            .co $(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS)
		:     "Loading $(PROGRAM) ... "
		$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
		:     " ...done"

clean:;         rm -f $(OBJS)

depend:;        mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

install:        .co $(PROGRAM)
		: ---  Installing $(PROGRAM) in $(DEST)
		mv $(PROGRAM) $(DEST)

program:        .co $(PROGRAM)

update:		$(DEST)/$(PROGRAM)

$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		make -f $(MAKEFILE) DEST=$(DEST) install

$(DEST):;       mkdir $(DEST)


rel:;   $(CO) -r$(VERSION) $(SRCS)
	date >.co

co:     .co

.co:;   $(CO) $(SRCS)
	date >.co

ci:;    $(CI) $(SRCS)
	rm -f .co

unlock:         .co .unlock

.unlock:;       $(UNLOCK) $(SRCS)
		:     "�������� ������ �������� ��� �����������"
#
# ����� ���� ������� ������ �� �������:
#
###
lbp.o: /usr/include/stdio.h /usr/include/ctype.h ../../include/line.h \
	../../include/linebp.h
