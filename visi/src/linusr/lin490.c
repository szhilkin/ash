/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin490.c,v 3.1 89/08/29 14:50:44 vsv Rel $
 *
 *      $Log:	lin490.c,v $
 * Revision 3.1  89/08/29  14:50:44  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.0  88/06/16  18:06:19  vsv
 * ��������������� ������
 * 
 */

#include <ediag.h>
#include <stdio.h>
#include "line.h"

lerror(s)
/*-----------------------*/
/* ������ ������� perror */
/*-----------------------*/
register char *s;
{
	extern  int errno;              /* ��������� ����� ������ */
	extern  int sys_nerr;           /* ����. ����� ��������� */
	extern  char *sys_errlist[];    /* ����. ������ ��������� */
	extern  char *sys_rerrlist[];   /* ����. ������ ��������� */

	char    outstr[STRLEN];

	if (errno <= sys_nerr ) {
		sprintf ( outstr,
		"CLIB: '%s' : %s",      s,
		ediag ( sys_errlist[ errno ], sys_rerrlist[ errno ] ) );
	} else {
		sprintf ( outstr, "CLIB: %d : %s ",
		ediag ( "Unknown error", "����������� ������" ) );
	}
	/* ����� ��������� �� ������ */
	w_emsg ( outstr );
}
