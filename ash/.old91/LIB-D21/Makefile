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

DEST          = /usr/local

VBIN          = ../../visi/bin
VLBP          = $(VBIN)/vlbp
LBP           = $(VBIN)/lbp

LIBRARY       = ash

MAKEFILE      = Makefile

OBJS	      = mainh.lb \
		phelp0.lb \
		sup.lb \
		suphlp.lb

SRCS	      = mainh.lav \
		phelp0.lav \
		sup.lav \
		suphlp.lav

SUFFIX        = .lav:s .la:s .lb:o

.SUFFIXES : .lav .la .lb

.la.lb:
	$(LBP) $< $@

.lav.la:
	$(VLBP) -o $@ $<

.lav.lb:
	$(VLBP) -o $*.la $<
	$(LBP) $*.la $@
	rm -f $*.la

all:            $(OBJS)

form:           $(OBJS)

clean:;         rm -f $(OBJS)

depend:;        mkmf -f $(MAKEFILE) DEST=$(DEST)

install:        $(OBJS) $(DEST) $(DEST)/lib $(DEST)/lib/$(LIBRARY)
		cp .??* $(OBJS) $(DEST)/lib/$(LIBRARY)

slink:          $(OBJS)
		ln -s `pwd` $(DEST)/lib/$(LIBRARY)
$(DEST):
		mkdir $(DEST)
$(DEST)/lib:
		mkdir $(DEST)/lib
$(DEST)/lib/$(LIBRARY):
		mkdir $(DEST)/lib/$(LIBRARY)
###
