
SHELL         = /bin/sh

##      +----------+    ���������� �����-������
##     (C) LINLIB  !    ��� ���������-��������
##      +----------+    ���������������

#       ������ ���������� ��������� ��� ���������
#       ��������� LINLIB �� ����������� �����

# ����������� ���� ��� ����� ����-�����:
#
# all      - ��� ����������������� � �����������
# program  -    �� �� ����� (������� ���������)
# clean    -    ������� ������������� ����� ����������
# depend   -    ����������� ������ ������ (����� mkmf)
# install  -    ���������� ��������� �� ����������� �����
# update   -    �������� ��������� (���� ���������� ��������� ����������)

VBIN          = ../../bin
VLIB          = ../../lib

PROGRAM       = proto
EXECMODE      =  711
DEST          = /usr/new/bin

CFLAGS        = -DDEMOS2 -I../../include
CC            = cc

LINKER        = cc
LDFLAGS       =

# ������������� ������������ ������������ ����������� termcap:
# STDLIBS       = -ltermcap
LIBS          = $(VLIB)/liblinusr.a $(VLIB)/libline.a $(VLIB)/libtermnew.a
VCC           = $(VBIN)/vcc
VLBP          = $(VBIN)/vlbp
LBP           = $(VBIN)/lbp
MAKEFILE      = Makefile

EXTHDRS       = ../../include/line.h \
		/usr/include/stdio.h

HDRS	      =

SRCS          = pmain.c \
		pmainv.cv

OBJS	      = pmain.o \
		pmainv.o

SUFFIX        = .cv:sC

.SUFFIXES : .out .o .c .cm .f .e .r .y .yr .ye .l .s .cl .p .q \
	    .cv .la .lav .lb
.cv.c:
	$(VCC) -o $@ $<;

.cv.o:
	$(VCC) -o $*.c $<;
	$(CC) $(CFLAGS) -c $*.c;
	rm -f $*.c;

all:    $(PROGRAM)

program:        $(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "������ $(PROGRAM) ... "
		@$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) $(LIBS) $(STDLIBS) -o $(PROGRAM)
		@echo " ...������"

clean:;         @rm -f $(OBJS)

depend:;        @mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

install:	$(PROGRAM)
		@echo "== ��������� $(PROGRAM) � ������� $(DEST)
		@install -s -m $(EXECMODE) $(PROGRAM) $(DEST)

update:		$(DEST)/$(PROGRAM)

$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		@make -f $(MAKEFILE) DEST=$(DEST) install

###
pmain.o: /usr/include/stdio.h ../../include/line.h
pmainv.o: /usr/include/stdio.h ../../include/line.h
