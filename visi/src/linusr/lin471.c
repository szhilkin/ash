/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      ���������������� ������� ���������� LINLIB
 *
 *      u_page(page, phline)    ������ �� ���������;
 */

/*
 *      $Header: lin471.c,v 1.1 89/08/29 14:50:41 vsv Rel $
 *      $Log:	lin471.c,v $
 * Revision 1.1  89/08/29  14:50:41  vsv
 * ������ LINLIB_3
 * 
 */

#include <stdio.h>
#include "line.h"

u_page(page, phline)
/*-----------------------------*/
/* ������� ������ �� ��������� */
/*-----------------------------*/
LINE *page;             /* �������� �������� */
LINE *phline;           /* ��� ����� �������� HELP */
{
    kbcod   cod;        /* ��� ��������� ������� */
    LINE *cline;        /* ��������� �� ������� ����� � �������� */

    cline = page;       /* ���������� � ������ �������� */
    er_pag();
    w_page(page);

    while ( -1 ) {
	cod = r_page( page, &cline, 0);
	switch ( cod ) {
	case ' ':
		/* ������������ ����� ���� */
		if((cline->attr & LMSE) == LMSE) {
			er_pag();
			w_page(page);
		}
		break;
	case KB_EX :
		return;
	case '?':
	case KB_HE:
		/* �������� ���������� ���������� */
		w_help(phline);
		w_page(page);
		break;
	default:   w_emsg("");     /* �������� ��������� �� ������ */
	}
    }
}

