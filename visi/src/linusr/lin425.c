/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin425.c,v 3.2 89/08/29 14:45:19 vsv Rel $
 *
 *      $Log:	lin425.c,v $
 * Revision 3.2  89/08/29  14:45:19  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  89/01/13  19:12:04  vsv
 * �������� ������� ������ ����� ��������� ������.
 * ����� ����, ��������� ������� tocod �� line.h
 * 
 * Revision 3.0  88/06/16  18:02:25  vsv
 * ��������������� ������
 * 
 * Revision 3.0  87/12/21  12:21:20  vsv
 * ��������������� ������.
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "line0.h"

cvt_lh(line, cod, mod, str)
/*------------------------*/
/* ����� �������� ������� */
/*------------------------*/
LINE    *line;
int       cod;
char     *mod;
char     *str;
{
	/* ����� � ���������� �������� �������
	 * �� �� ���������� (�����������) ����
	 * �������� ":NL" ���������� �� "RETURN"
	 */
	extern   unsigned strlen();
	register char   *s;
	extern   KBL    kbl[];  /* ������� ����� ����� � �������� ������ */
	register KBL   *kblp;
		kbcod   codh;   /* ���, ��� �������� ����� �������� */

	if( *mod == 'w' ) {
		s = line->varl;
		/* ��� ����� ���� ������� � "line.h" */
		codh = tocod0(s[1]);
		codh |= tocod1(s[2]);

		for(kblp=kbl; kblp->t_key!=0; kblp++) {
			if(kblp->t_cod == codh) {
				strcpy(str, kblp->t_knm);
				goto ret;
			}
		}
		strcpy(str, &line->varl[1]);
ret:
/***** ����� �����...
		for(s=str+strlen(str), s--; s > str && *s; s--) {
			if(isspace(*s)) *s = '\0';
		}
 *****/
		;
	}
	return( TRUE ) ;        /* ������ ���� �� �����... */
}
