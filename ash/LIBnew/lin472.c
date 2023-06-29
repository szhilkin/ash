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
#ifndef OLD_CVT_SP
	register int max_co;
	int size;

	spp = (char **)line->varl;
	size = line->size;
	if (line->attr & PMT) size -= 1;
	max_co = maxco-2;
	if(*mod == 'r') {
		strcpy(*spp, str);
	}
	else    {
		strncpy(str, *spp, size);
		if (strlen(*spp) >= size) str[size] = '\0';
	}
#else

	spp = (char **)line->varl;
	if(*mod == 'r') {
		strcpy(*spp, str);
	}
	else    {
		strcpy(str, *spp);
	}
#endif
	return(TRUE);
}

