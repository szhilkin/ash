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
{ KBCOD('k','u'), 0 },
{ KBCOD('k','d'), 0 },
{ KBCOD('k','l'), 0 },
{ KBCOD('k','r'), 0 },
{ KBCOD('k','1'), 0 },
{ KBCOD('k','2'), 0 },
{ KBCOD('k','3'), 0 },
{ KBCOD('k','4'), 0 },
{ KBCOD('I','C'), 0 },
{ KBCOD('D','C'), 0 },
{ KBCOD('I','L'), 0 },
{ KBCOD('D','L'), 0 },
{ KBCOD('k','h'), 0 },
{ KBCOD('f','.'), 0 },
{ KBCOD('f','-'), 0 },
{ KBCOD('f',','), 0 },
{ KBCOD('f','0'), 0 },
{ KBCOD('f','1'), 0 },
{ KBCOD('f','2'), 0 },
{ KBCOD('f','3'), 0 },
{ KBCOD('f','4'), 0 },
{ KBCOD('f','5'), 0 },
{ KBCOD('f','6'), 0 },
{ KBCOD('f','7'), 0 },
{ KBCOD('f','8'), 0 },
{ KBCOD('f','9'), 0 },
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
      { KBCOD('k','3'),   KB_KP,   "PF3    " },    /* KEYPAD ON */
      { KBCOD('k','u'),   KB_AU,   "�����  " },
      { KBCOD('k','d'),   KB_AD,   "����   " },
      { KBCOD('k','l'),   KB_AL,   "�����  " },
      { KBCOD('k','r'),   KB_AR,   "������ " },
      { KBCTL(    'M'),   KB_NL,   "RETURN " },    /* ���� (����� ������) */
      { KBCOD('d','e'),   KB_DE,   "DEL    " },    /* ����� */
      { KBCOD('k','1'),   KB_EX,   "PF1    " },    /* ������� (GOLD) */
      { KBCOD('k','2'),   KB_HE,   "PF2    " },    /* �����. ���. (HELP) */
      { KBCOD('k','4'),   KB_PR,   "PF4    " },    /* ������ ������ */
      { KBCTL(    'A'),   KB_RE,   "CTRL-A " },    /* �������� (REFRESH) */
      { KBCTL(    'I'),   KB_TA,   "TAB    " },    /* ��������� */
		/*==== �������������� ������� */
      { KBCOD('f','0'),   KBUSR('0'),   0         },
      { KBCOD('f','1'),   KBUSR('1'),   0         },
      { KBCOD('f','2'),   KBUSR('2'),   0         },
      { KBCOD('f','3'),   KBUSR('3'),   0         },
      { KBCOD('f','4'),   KBUSR('4'),   0         },
      { KBCOD('f','5'),   KBUSR('5'),   0         },
      { KBCOD('f','6'),   KBUSR('6'),   0         },
      { KBCOD('f','7'),   KBUSR('7'),   0         },
      { KBCOD('f','8'),   KBUSR('8'),   0         },
      { KBCOD('f','9'),   KBUSR('9'),   0         },
      { KBCOD('f','.'),   KBUSR('.'),   0         },
      { KBCOD('f','-'),   KBUSR('-'),   0         },
      { KBCOD('f',','),   KBUSR(','),   0         },
      { KBCTL(    'J'),   KB_NL,   0         },    /*==== �������� */
      { KBCTL(    'H'),   KB_HE,   0         },
      { KBCTL(    'E'),   KB_EX,   0         },
      { KBCOD('k','h'),   KB_EX,   0         },
      { KBCTL(    'R'),   KB_RE,   0         },
      { 0 },
	};


LPA lpaout[LPASIZE] = {
	{       ' ',    0       },
	{       ' ',    0       },              /* TXT  */
	{       ' ',    0       },              /* HDR  */
	{       '>',    0       },              /* VAR  */
	{       '>',    0       },              /* ALT  */
	{       '-',    0       },              /* MSE  */
	{       ' ',    A_US|A_MD },            /* ERR  */
	{       '!',    A_SO    },              /* ATT  */
};

LPA lpainp[LPASIZE] = {
	{       ' ',    0       },
	{       ' ',    A_SO    },              /* TXT  */
	{       ' ',    A_SO    },              /* HDR  */
	{       '#',    A_SO    },              /* VAR  */
	{       '?',    A_SO    },              /* ALT  */
	{       '*',    A_SO    },              /* MSE  */
	{       ' ',    A_US|A_MD },            /* ERR  */
	{       ' ',    A_MB    },              /* ATT  */
};
