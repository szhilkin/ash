
SHELL   = /bin/sh

#       visi
#
#       ������ ������������� �������� �������
#       ��� ����������� �� � ��� lbp
#
#  ����������:
#       �� ������ � ���� �� ������ ���������� ��� ���������.

CC      = cc

DEST    = ../../bin
CFLAGS  = -I../../include

CO      = co
COFLAGS =

MAKEFILE = Makefile

all:    vlbp    vcc

vcc:    vcc.c vgen.h
	$(CC) -o vcc -DVCC $(CFLAGS) vcc.c

vlbp:   vcc.c vgen.h
	$(CC) -o vlbp -DVLBP $(CFLAGS) vcc.c

install: vcc vlbp
	-strip vcc vlbp
	mv vcc vlbp $(DEST)

clean:; -rm -f *.o

depend:;        mkmf -f $(MAKEFILE) DEST=$(DEST)

$(DEST):;       mkdir $(DEST)

co:     .co

.co:
	$(CO) $(COFLAGS) vcc.c vgen.h
	touch .co

.DEFAULT:
	$(CO) $(COFLAGS) $@
###
vcc.o: /usr/include/ediag.h /usr/include/stdio.h /usr/include/ctype.h \
	../../include/line.h ../../include/linebp.h vgen.h
