
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

#       ��������� ��� ������ xstr
#       (��. ������� .c.o:  .cv.o)

VISI          = ../visi
VBIN          = $(VISI)/bin
VLIB          = $(VISI)/lib

PROGRAM       = ash
EXECMODE      =  711
DEST          = /usr/local

#CFLAGS_ASH    = -DJCLsuspend -DSYSV -DSVR4 -DMGROUPS
#CFLAGS_ASH    = -DJCLsuspend -DFbsd -DBSDpagesize -DBSDmagic
CFLAGS_ASH    =
CFLAGS        = $(CFLAGS_ASH) \
		-I../visi/include\
		-D'DESTLIB="$(DEST)/lib/$(PROGRAM)/"'

CPP           = cc -E -C
CC            = cc -O
LINT          = lint -h -n

LINKER        = cc
LDFLAGS       =

# STDLIBS       = -ltermcap
# STDLIBS       = -ltermnew
# STDLIBS       = -ljobs -lndir
STDLIBS       =

#		LIBnew/libnew.a \
#
LIBS          = \
		$(VLIB)/liblinebp.a \
		$(VLIB)/liblinusr.a \
		$(VLIB)/libline.a \
		$(VLIB)/libtermnew.a

VCC           = $(VBIN)/vcc
VLBP          = $(VBIN)/vlbp
LBP           = $(VBIN)/lbp
MAKEFILE      = Makefile

EXTHDRS	      =

HDRS	      = assist.h\
		astat.h

SRCS          = amenu.c\
		filetype.c\
		gidchk.c \
		main.c\
		menu2.c\
		vcmd.c\
		vcmddat.c\
		vcmdh.c\
		vcoprt.c\
		vdcanon.c\
		vex.c\
		vexdir.c\
		vfread.c\
		vmain.c\
		vmisc.c\
		vndir.c\
		vshcmd.c\
		vsusp.c\
		vsystem.c

OBJS	      = \
		amenu.o\
		filetype.o\
		gidchk.o\
		main.o\
		menu2.o\
		vcmd.o\
		vcmddat.o\
		vcmdh.o\
		vcoprt.o\
		vdcanon.o\
		vex.o\
		vexdir.o\
		vfread.o\
		vmain.o\
		vmisc.o\
		vndir.o\
		vshcmd.o\
		vsusp.o\
		vsystem.o

LIBNEW       = \
		LIBnew/lin330.o\
                LIBnew/lin340.o\
                LIBnew/lin360.o\
                LIBnew/lin425.o\
                LIBnew/lin460.o\
                LIBnew/lin472.o
 

SUFFIX        = .cv:sC

.SUFFIXES : .out .o .c .cm .f .e .r .y .yr .ye .l .s .cl .p .q \
	    .cv .la .lav .lb
.c.o:
	$(CC) -c $(CFLAGS) $<

.c.s:
	$(CC) -S $(CFLAGS) $<

.cv.c:
	$(VCC) -o $@ $<

# .cv.o:
#         $(VCC) -o $*.c $<
#         $(CC) $(CFLAGS) -c $*.c
#         rm -f $*.c
#
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

# data-to-text:
#         echo 'g/\.data/s//\.text/' >data-to-text
#         echo 'w' >>data-to-text
#         echo 'q' >>data-to-text

program:        $(PROGRAM)

objs:           $(OBJS)

# xs.o:           strings
#                 xstr
#                 $(CC) $(CFLAGS) -S $*.c
#                 ed - $*.s <data-to-text
#                 $(AS) -o $*.o $*.s
#                 rm -f $*.s

$(PROGRAM):     LIBnew/libnew $(OBJS) $(LIBS) $(MAKEFILE)
		@echo "Loading $(PROGRAM) ... "
		$(LINKER) $(LDFLAGS) $(OBJS) $(LIBNEW) $(LIBS) $(STDLIBS) -o $(PROGRAM)
		@echo " ...done"

clean:  otherclean
	rm -f $(OBJS) $(PROGRAM) *.o

# ����� xstr �� ������ ������� � ������
#
depend:
	rm -f xs.? x.?
	mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) DEST=$(DEST)

calls:  $(SRCS)
	calls $(CFLAGS) $(SRCS) >a.calls

cxref:  $(SRCS)
	cxref $(CFLAGS) $(SRCS) >a.cxref

lint lintcheck:;        $(LINT) $(CFLAGS) *.c LIBnew/*.c >a.lint

otherclean:
		cd ./LIBnew; make clean;
#                cd ./ashrc; make DEST=$(DEST) clean

otherinst:
#                cd ./ashrc; make DEST=$(DEST) install

#                 -c7t10 $(DEST)/bin/$(PROGRAM)
# install:        $(PROGRAM) $(DEST) $(DEST)/bin
install:        $(PROGRAM) $(DEST) $(DEST)/bin otherinst
		-mv $(DEST)/bin/$(PROGRAM) $(DEST)/bin/$(PROGRAM).old
		cp $(PROGRAM) $(DEST)/bin
		-strip $(DEST)/bin/$(PROGRAM)
		-c7t10 $(DEST)/bin/$(PROGRAM)
		chmod $(EXECMODE) $(DEST)/bin/$(PROGRAM)
		-rm -f $(DEST)/bin/$(PROGRAM).old

update:         $(DEST)/bin/$(PROGRAM)

$(DEST)/bin/$(PROGRAM): $(SRCS) $(LIBS) $(HDRS) $(EXTHDRS)
		@make -f $(MAKEFILE) DEST=$(DEST) "CC=$(CC)" install

$(DEST)/bin:
	mkdir $(DEST)/bin

$(DEST):
	mkdir $(DEST)
#
#
# portable rulez
#
#LIBnew/libnew.a:
#       cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" libnew.a

LIBnew/libnew:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" library

LIBnew/lib330.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib330.o

LIBnew/lib340.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib340.o

LIBnew/lib360.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib360.o

LIBnew/lib425.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib425.o

LIBnew/lib460.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib460.o

LIBnew/lib472.o:
	cd LIBnew; make "DEST=$(DEST)" "CC=$(CC)" lib472.o

###
amenu.o: assist.h
main.o: assist.h
menu2.o: assist.h
vcmd.o: assist.h astat.h
vcmddat.o: assist.h astat.h
vcmdh.o: assist.h
vex.o: assist.h astat.h
vfread.o: assist.h
vmain.o: assist.h
vmisc.o: assist.h
vndir.o: assist.h astat.h
vshcmd.o: assist.h
vsystem.o: assist.h
