/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin465.c,v 3.1 89/08/29 14:50:32 vsv Rel $
 *
 *      $Log:	lin465.c,v $
 * Revision 3.1  89/08/29  14:50:32  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.0  88/06/16  18:05:20  vsv
 * ��������������� ������
 * 
 */

#include <stdio.h>
#include "line.h"

/*-----------------------------*/
/* ����� ���������� ���������� */
/* � ��������������� ��������� */
/* ������� ������              */
/*-----------------------------*/
help ( from, to, text )
int     from;           /* ������� ������� (����� ������ �� ������) */
int     to;             /* ������ ������� */
char    **text;         /* ���������� ����� */
{
	register i ;

	er_scr(from, to);

	if(text) {
		for( i=from; *text; i++) {
			cp_set( i, 0, TXT);
			w_str( *text++ );   /* �������� ����� �� ������ */
		}
	}
}
