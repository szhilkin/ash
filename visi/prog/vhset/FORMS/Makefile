
SHELL        = /bin/sh

#
# LINLIB V3.3
# ���������� � ��������� ������ ������� ��������
#

# ����������� ����:
# all      - ����������������� �������� �������;
# form     - �� ��, ��� � "all";
# install  - ���������� ����� ������� � ������� LINLIB-��������;
# slink    - ������� ������������� ������ �� ������� �������
#            (������ "install"),
#            ����� ������������� ��� ������ ������������;
# clean    - �������� �������, �������� ������ �������� ������;
# depend   - ������������� Makefile ��� ������ mkmf (�������� ������ ������);

DEST          = /usr/local/lib/visi

VBIN          = ../../../../visi/bin
VLBP          = $(VBIN)/vlbp
LBP           = $(VBIN)/lbp

LIBRARY       = vhset

MAKEFILE      = Makefile

OBJS	      = phelp0.lb \
		vhseta.lb \
		vhsetk.lb \
		vhsetm.lb

SRCS	      = phelp0.lav \
		vhseta.lav \
		vhsetk.lav \
		vhsetm.lav

SUFFIX        = .lav:s .la:s .lb:o

.SUFFIXES : .lav .la .lb

.la.lb:
	$(LBP) $< $@

.lav.la:
	$(VLBP) -o $@ $<

.lav.lb:
	$(VLBP) -o $*.la $<
	$(LBP) $*.la $@; rm -f $*.la

all:            $(OBJS)

form:           $(OBJS)

clean:;         rm -f $(OBJS)

depend:;        mkmf -f $(MAKEFILE) DEST=$(DEST)

install:        $(OBJS) $(DEST) $(DEST)/$(LIBRARY)
		cp $(OBJS) $(DEST)/$(LIBRARY)

slink:          $(OBJS) $(DEST)
		ln -s `pwd` $(DEST)/$(LIBRARY)
.DEFAULT:
		mkdir $<
###
