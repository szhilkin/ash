/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin460.c,v 1.1 90/08/24 08:08:17 vsv Exp $
 *
 *      $Log:	lin460.c,v $
 * Revision 1.1  90/08/24  08:08:17  vsv
 * Initial revision
 * 
 * Revision 3.2  89/08/29  14:50:26  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  88/06/27  15:31:01  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  88/06/16  18:04:44  vsv
 * ��������������� ������
 * 
 */

#include <stdio.h>
#include "line.h"

/*---------------------------*/
/* ����� ��������� �� ������ */
/*---------------------------*/

static  int msgflg ;    /* �� ������ ���� ��������� */

int     ok_msg()
{
	return(msgflg);
}

w_msg(vamode, str)
register int     vamode;        /* ������������ */
register char    *str ;         /* ����� ��������� �� ������ */
{
	if (msgflg) {                   /* ���� ������ �� ������ ������ */
		msgflg = 0;             /* �������� �� */
		cp_set(-1, 0, TXT);
		er_eol();
	}
	if (*str) {    /* ���� �������� ������ �������� ������ */
		msgflg = 1;
		cp_set(-1, 0, vamode);
		if ((vamode & VIDEO) == ERR) {
			bell();
			w_str("������: ");
		}
		w_str(str); w_str(" ");
		at_set(TXT); er_eol();
	}
}

/* ��� ��������� ������������� �� ������� �������� */
w_emsg(s)
register char *s;
{
	w_msg(ERR, s);
}
