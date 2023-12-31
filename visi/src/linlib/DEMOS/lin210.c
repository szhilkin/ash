/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin210.c,v 1.1 90/12/27 16:28:54 vsv Rel $
 *
 *      $Log:	lin210.c,v $
 * Revision 1.1  90/12/27  16:28:54  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.4  89/08/29  15:14:53  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.3  88/07/28  09:23:43  vsv
 * ������ � �������� 'DEL' ��������� � ������ ����
 * 
 * Revision 3.2  88/06/27  15:19:23  vsv
 * ������� ������ RCS
 * 
 * Revision 3.1  88/04/29  12:24:13  vsv
 * ��������� ������� � ���������,
 * ������� ��� ������ ������
 * 
 */

#include <stdio.h>
#include "line.h"
#include "line0.h"
#include "lineva.h"

/*
 * ��������� �� ��� ���������
 *
 * termcap �� ������ ������, ��� ���
 * ������� ������� ����, �������
 * ������������ ������ ���� ���.
 * ��������, ����� ��������� �� termcap
 * �������� ��������, ����� � ��� ����������� ����
 * ��������� �� ���� ��������� � �����.
 * ����������.  ��������
 * � termcap, ������ HELP ����������� �� ����� :lh= ... :
 *
 * � ���� ����� ��'����� ������, ��������� ���
 * �������� ������������ ���������.
 * ����� ��'����� ��� ������ ���-�������������������
 * �����������, � ������� hw_set ��� ������ -
 * ������ ���� ����������� ����������
 * ���������� �� ���� ������ ������������� ��
 * ���� ���������.
 */


/* ������� ���������� ��� tgoto() �� termcap */
char *UP = NULL;        /* ����������� ������ ����� �� 1 ������� */
char *BC = NULL;        /* ---------""------- ����� -----""----- */

/* ������� ������ */
int     maxli = 24;     /* ������ �� ��������� */
int     maxco = 80;     /* --"-- �� ����������� */


/*---------------------*/
/* �������� ���������� */
/*---------------------*/

KBF   kbf[] = {

/*      ��� ������������ �� ������� r_key();
 *      ������ �������� ����������� hw_set(),
 *      ��� ������� ��� �������� �������������
 *      �������. � ��������� ������ �����
 *      �������� ���������� ������, �/��� �������
 *      ����������� �������� � ������� kbl (��. ����).
 */
      { 'ku', 0 },    { 'kd', 0 },    { 'kl', 0 },    { 'kr', 0 },
      { 'k1', 0 },    { 'k2', 0 },    { 'k3', 0 },    { 'k4', 0 },
      { 'IC', 0 },    { 'DC', 0 },    { 'IL', 0 },    { 'DL', 0 },
      { 'kh', 0 },    { 'f.', 0 },    { 'f-', 0 },    { 'f,', 0 },
      { 'f0', 0 },    { 'f1', 0 },    { 'f2', 0 },    { 'f3', 0 },
      { 'f4', 0 },    { 'f5', 0 },    { 'f6', 0 },    { 'f7', 0 },
      { 'f8', 0 },    { 'f9', 0 },
      { 0, 0 },
};

/* �������� ������������ ������ �� ����� */
/*
 * ������� hw_set �������� ������, ������� ��������
 * ������� ������������ ���������, ��. termcap(5)
 * �� ������ �������� ���� ������.
 * ��� �������� ������������� ��������� �������� ����� �� ��������
 * (�.�. �� ��������������� ESC-������������������)
 * � ��������� � ���� ����� �������� ������ ������� "hw_set() {}"
 */
char *tcapo[] = {
	"cm", "cl", "cd", "ce", "ks", "ke",
	"cs", "sr", "sf", "al", "dl",
	"so", "se", "us", "ue",
	"md", "mr", "mb", "mk", "me",
	"bl", "vb",
	"lh",   /* ��� ���������� */
	0 };

/* ����� ���������� � ���������� �����, � ����� ���� ������ */
KBL kbl[KBLSIZE] = {
		/*==== �������� ������� */
      { 'k3',   'KO',   "PF3    " },    /* KEYPAD ON */
      { 'ku',   'AU',   "�����  " },
      { 'kd',   'AD',   "����   " },
      { 'kl',   'AL',   "�����  " },
      { 'kr',   'AR',   "������ " },
      { '^M',   'NL',   "RETURN " },    /* ���� (����� ������) */
      { 'de',   'DE',   "DEL    " },    /* ����� */
      { 'k1',   'GL',   "PF1    " },    /* ������� (GOLD) */
      { 'k2',   'HE',   "PF2    " },    /* �����. ���. (HELP) */
      { 'k4',   'PR',   "PF4    " },    /* ������ ������ */
      { '^A',   'RE',   "CTRL-A " },    /* �������� (REFRESH) */
      { '^I',   'TA',   "TAB    " },    /* ��������� */
		/*==== �������������� ������� */
      { 'f0',   'U0',   0         },
      { 'f1',   'U1',   0         },
      { 'f2',   'U2',   0         },
      { 'f3',   'U3',   0         },
      { 'f4',   'U4',   0         },
      { 'f5',   'U5',   0         },
      { 'f6',   'U6',   0         },
      { 'f7',   'U7',   0         },
      { 'f8',   'U8',   0         },
      { 'f9',   'U9',   0         },
      { 'f.',   'U.',   0         },
      { 'f-',   'U-',   0         },
      { 'f,',   'U,',   0         },
      { '^J',   'NL',   0         },    /*==== �������� */
      { '^H',   'HE',   0         },
      { '^E',   'GL',   0         },
      { 'kh',   'GL',   0         },
      { '^R',   'RE',   0         },
      { 0 },
	};


LPA lpaout[LPASIZE] = {
	{       0,      0       },
	{       ' ',    0       },              /* TXT  */
	{       ' ',    0       },              /* HDR  */
	{       '>',    0       },              /* VAR  */
	{       '>',    0       },              /* ALT  */
	{       '-',    0       },              /* MSE  */
	{       ' ',    A_US|A_MD },            /* ERR  */
	{       '!',    A_SO    },              /* ATT  */
};

LPA lpainp[LPASIZE] = {
	{       0,      0       },
	{       ' ',    A_SO    },              /* TXT  */
	{       ' ',    A_SO    },              /* HDR  */
	{       '#',    A_SO    },              /* VAR  */
	{       '?',    A_SO    },              /* ALT  */
	{       '*',    A_SO    },              /* MSE  */
	{       ' ',    A_US|A_MD },            /* ERR  */
	{       ' ',    A_MB    },              /* ATT  */
};
