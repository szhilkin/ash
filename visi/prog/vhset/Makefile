
SHELL         = /bin/sh

##
##      +----------+    ���������� �����-������
##     (C) LINLIB  !    ��� ���������-��������
##      +----------+    ���������������
##
##      $Header$
##      $Log$
##

#
#       ������ ���������� ��������� ��� ���������
#       ��������� LINLIB �� ����������� �����
VISI          = ../../../visi
VBIN          = $(VISI)/bin
VLIB          = $(VISI)/lib

PROGRAM       = vhset
EXECMODE      =  711
DEST          = /usr/local/bin

CFLAGS        = -I../../../visi/include
CC            = cc -O

LINKER        = cc
LDFLAGS       =
# STDLIBS       = -ltermcap
# STDLIBS       = -ltermnew
LIBS          = \
		$(VLIB)/liblinebp.a \
		$(VLIB)/liblinusr.a \
		$(VLIB)/libline.a \
		$(VLIB)/libtermnew.a

VCC           = $(VBIN)/vcc
VLBP          = $(VBIN)/vlbp
LBP           = $(VBIN)/lbp
MAKEFILE      = Makefile

EXTHDRS	      = ../../../visi/include/line.h \
		../../../visi/include/line0.h \
		../../../visi/include/linebp.h \
		../../../visi/include/lineva.h \
		/usr/include/stdio.h

HDRS	      = vhset.h

SRCS          = vhset0.cv \
		vhseta.cv \
		vhsetk.cv \
		vhsetm.cv

OBJS	      = vhset0.o \
		vhseta.o \
		vhsetk.o \
		vhsetm.o

SUFFIX        = .cv:sC

.SUFFIXES : .out .o .c .cm .f .e .r .y .yr .ye .l .s .cl .p .q \
	    .cv .la .lav .lb
.cv.c:
	$(VCC) -o $@ $<

#.cv.o:
#	$(VCC) -o $*.c $<
#	$(CC) $(CFLAGS) -c $*.c
#	rm -f $*.c

.cv.o:
	$(VCC) -o $*.c $<
	$(CC) $(CFLAGS) -c $*.c; rm -f $*.c

# ������� ������ ������ � ������ ����
# ����� ������������ ��� ���������� �������� �
# ��������� ��������, ���������� � ������ ld -n,
# ��. ����� ������� data-to-text:
# .cv.o:
#         $(VCC) -o $*.c $<
#         $(CC) $(CFLAGS) -S $*.c
#         ed - $*.s <data-to-text
#         $(AS) -o $*.o $*.s
#         rm -f $*.c $*.s

.la.lb:
	$(LBP) $< $@

.lav.la:
	$(VLBP) -o $@ $<

.lav.lb:
	$(VLBP) -o $*.la $<
	$(LBP) $*.la $@; rm -f $*.la

all:  $(PROGRAM)

data-to-text:
	echo 'g/\.data/s//\.text/' >data-to-text
	echo 'w' >>data-to-text
	echo 'q' >>data-to-text

program:        $(PROGRAM)

$(PROGRAM):     $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "Loading $(PROGRAM) ... "
		@$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) $(STDLIBS) -o $(PROGRAM)
		@echo " ...done"

clean:
	rm -f $(PROGRAM) $(OBJS) data-to-text $(NOTEXT)
	cd FORMS; make clean

depend:
	mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

calls:  $(SRCS)
	calls $(CFLAGS) $(SRCS) >calls

install:        $(DEST) $(PROGRAM)
		@echo "Install $(PROGRAM) to $(DEST) ...
		cp $(PROGRAM) $(DEST)
		cd FORMS; make install
		@echo done

update:		$(DEST)/$(PROGRAM)

$(DEST)/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		@make -f $(MAKEFILE) DEST=$(DEST) install

$(DEST):
	mkdir $(DEST)
###
vhset0.o: /usr/include/stdio.h ../../../visi/include/line.h \
	../../../visi/include/line0.h vhset.h
vhseta.o: /usr/include/stdio.h ../../../visi/include/line.h \
	../../../visi/include/line0.h ../../../visi/include/lineva.h
vhsetk.o: /usr/include/stdio.h ../../../visi/include/line.h \
	../../../visi/include/line0.h
vhsetm.o: /usr/include/stdio.h ../../../visi/include/line.h \
	../../../visi/include/line0.h ../../../visi/include/linebp.h vhset.h
