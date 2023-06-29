/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin460.c,v 3.2 89/08/29 14:50:26 vsv Rel $
 *
 *      $Log:	lin460.c,v $
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
static  int errflg ;    /* ����: ��������� �� ������ �� ���� */

w_emsg ( str )
char    *str ;          /* ����� ��������� �� ������ */
{
	if( *str ) {    /* ���� �������� ������ �������� ������ */
		errflg = 1;
		cp_set(-1, 0, ERR);
		bell();
		printf(" ?!! %s ", str);
	} else {
		if(errflg) {
			errflg = 0;
			cp_set(-1, 0, TXT ) ;
			er_eol();
		}
	}
}

w_msg(attr, str)
int     attr;
char *str;
{
	w_emsg(str);
}
