
SHELL   = /bin/sh

#
# �������� ����� ��� ������ � �������� ����������;
#
# --����������� ����:
# all:          ���������� � ������ � �������� �������
# clean:        �������� ������������� ������ ����������
# depend:       ��������� Makefile ��� ������ mkmf;
# dis:          �� ��, ���� �������� ����� ��������� �� ../../lib
# std:          -----"------                            /usr/new/lib
# install:      ���������� ���������� �� �����
# update:       ������ ���������� � ���������
# library:      ����������� ����������
# lintcheck:    ��������� ������ ���������� ��� ������ lint
# lintlib:      ������� ������-���������� ��� lint (���� �� ������)
# co:           ������ �� ������ �������� ������
# rel:          ������ �� ������ �������� ������ ������, ��������� ����
# ci:           �������� � ����� ���������� ������ ���. �������
# unlock:       ����� ���������� ��� ���������� ����������� ���. �������

#��������� ���������� ������: (������ ��. rlog Makefile)
VERSION       = LINLIB_3

CC            = cc
DEST          = ../../lib
LIBRARY	      = liblinusr.a
MAKEFILE      = Makefile
CFLAGS        = -I../../include
LINT          = lint
CI            = ci -f -n$(VERSION) -m"������ $(VERSION)" -sRel
CO            = co
UNLOCK        = rcs -l
VCC           = ../../bin/vcc

EXTHDRS       = ../../include/line.h \
		../../include/line0.h \
		../../include/linebp.h \
		/usr/include/ctype.h \
		../../include/ediag.h \
		/usr/include/stdio.h

INCLUDE       = /usr/include
HDRS	      =

OBJS	      = lin420.o \
		lin425.o \
		lin426.o \
		lin440.o \
		lin450.o \
		lin460.o \
		lin465.o \
		lin470.o \
		lin471.o \
		lin472.o \
		lin475.o \
		lin476.o \
		lin490.o

SRCS	      = lin420.c \
		lin425.c \
		lin426.c \
		lin440.c \
		lin450.c \
		lin460.c \
		lin465.c \
		lin470.cv \
		lin471.c \
		lin472.c \
		lin475.c \
		lin476.c \
		lin490.c

SUFFIX        = .cv:sC

.SUFFIXES : .out .o .c .cm .f .e .r .y .yr .ye .l .s .cl .p .q \
	    .cv .la .lav .lb
.cv.c:
	$(VCC) -o $@ $<;

# ������� ������ ������ � ������ ����
# ����� ������������ ��� ���������� �������� �
# ��������� ��������, ���������� � ������ ld -n
.cv.o:
	$(VCC) -o $*.c $<;
	$(CC) $(CFLAGS) -c $*.c;        #-   - ������� ����������
	rm -f $*.c;                     #-
#        $(CC) $(CFLAGS) -S $*.c;        #+   + ��� �������
#        ed - $*.s <data-to-text;        #+
#        $(AS) -o $*.o $*.s;             #+
#        rm -f $*.c $*.s;                #+

.la.lb:
	$(LBP) $< $@;

.lav.la:
	$(VLBP) -o $@ $<;

.lav.lb:
	$(VLBP) -o $*.la $<;
	$(LBP) $*.la $@;
	rm -f $*.la;

all:            .co $(LIBRARY)

data-to-text:
	echo 'g/\.data/s//\.text/' >data-to-text;
	echo 'w' >>data-to-text;
	echo 'q' >>data-to-text;

$(LIBRARY):     $(OBJS)
		:     "Loading $(LIBRARY) ... "
		rm -f $(LIBRARY)
		ar cr  $(LIBRARY) `lorder $(OBJS) | tsort`
		-ranlib $(LIBRARY)
		:     "   ...OK"

# $(LIBRARY):     $(OBJS)
#                 @echo "Loading $(LIBRARY) ... "
#                 @rm -f $(LIBRARY)
#                 @ar cr $(LIBRARY) $(OBJS)
#                 @echo "   ...OK"

clean:;		@rm -f $(OBJS)

depend:;	@mkmf -f $(MAKEFILE) LIBRARY=$(LIBRARY) DEST=$(DEST)

extract:;       @ar x  $(DEST)/$(LIBRARY)
		@rm -f __.SYMDEF

install:        .co $(LIBRARY) $(HDRS)
		@echo Installing $(LIBRARY) in $(DEST)
		mv      $(LIBRARY) $(DEST)
		-ranlib $(DEST)/$(LIBRARY)
		@echo Library $(LIBRARY) installed in $(DEST) , OK.

library:        $(LIBRARY)

lintcheck:;     $(LINT) $(LINTFLAGS) $(CFLAGS) $(SRCS)

update:         $(DEST)/$(LIBRARY)

$(DEST)/$(LIBRARY): $(SRCS) $(HDRS) $(EXTHDRS)
		@-ar xo $(DEST)/$(LIBRARY)
		@make -f $(MAKEFILE) DEST=$(DEST) install

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
		@echo "�������� ������ �������� ��� �����������"

# ����� ���� ������� ������ �� �������:
#
###
lin420.o: /usr/include/stdio.h ../../include/line.h
lin425.o: /usr/include/stdio.h /usr/include/ctype.h ../../include/line.h \
	../../include/line0.h
lin426.o: /usr/include/stdio.h ../../include/line.h
lin440.o: /usr/include/stdio.h ../../include/line.h ../../include/linebp.h
lin450.o: ../../include/line.h
lin460.o: /usr/include/stdio.h ../../include/line.h
lin465.o: /usr/include/stdio.h ../../include/line.h
lin470.o: /usr/include/stdio.h ../../include/line.h
lin471.o: /usr/include/stdio.h ../../include/line.h
lin472.o: /usr/include/stdio.h ../../include/line.h
lin475.o: ../../include/line.h
lin476.o: ../../include/line.h
lin490.o: ../../include/ediag.h /usr/include/stdio.h ../../include/line.h
