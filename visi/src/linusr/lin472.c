/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      ���������������� ������� ���������� LINLIB
 *
 *      cvt_sp()   ���� ������ �� ���������;
 *
 */

/*
 *      $Header: lin472.c,v 1.1 89/08/29 14:50:42 vsv Rel $
 *      $Log:	lin472.c,v $
 * Revision 1.1  89/08/29  14:50:42  vsv
 * ������ LINLIB_3
 * 
 */

#include <stdio.h>
#include "line.h"


int     cvt_sp(line, cod, mod, str)
/*-----------------------------------*/
/* ���� ������ �� �������� ��������� */
/*-----------------------------------*/
LINE *line;
kbcod cod;
char *mod;
char *str;
{
	register char **spp;

	spp = (char **)line->varl;
	if(*mod == 'r') strcpy(*spp, str);
	else            strcpy(str, *spp);
	return(TRUE);
}

