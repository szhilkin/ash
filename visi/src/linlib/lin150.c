/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin150.c,v 1.1 90/12/27 16:28:49 vsv Rel $
 *
 *      $Log:	lin150.c,v $
 * Revision 1.1  90/12/27  16:28:49  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.3  89/08/29  15:14:37  vsv
 * ������ LINLIB_3
 *
 * Revision 3.2  88/07/28  09:25:47  vsv
 * ��������� ������� w_strn
 *
 * Revision 3.1  88/05/23  10:24:45  vsv
 * ���������� ������ ����� �������� ��� RT-11
 *
 * Revision 3.0  87/12/21  12:16:07  vsv
 * ��������������� ������.
 *
 */

#include <stdio.h>
#include "line.h"

/*
**      ����� �� ����� ����� � ����� ���������
**      � � �������������� (�� ������� ��������)
**/

#ifdef RT11
/*
 * ��� RT-11 �� ����� ������ ������ ��� ��� ���������,
 * ����� ������ ��������.
 */

w_raw(s)
register char *s;
{
	fprintf(vttout, "%s", s);
}

w_str(s)
register char *s;
{
	fprintf(vttout, "%s", s);
}

w_strn(s, n)
register char *s;
register int n;
{
	n -= strlen(s);
	fprintf(vttout, "%s", s);
	while(--n>=0) w_chr(' ');
}

w_chr(c)
register int c;
{
	putc(c, vttout);
}


int escseq(c)
register int c;
{
	return(c);
}

#endif /* RT11 */
#ifdef DEMOS2
#include <sgtty.h>

extern  int osgflg;             /* ����� �������� (old.sg_flags) */

/*--------------------------------*/
/* ����� ������ � ����� ��������� */
/*--------------------------------*/
/* �� ����� ���� � ����������������,
 * ���, ����� ������� ����� �������������, a
 * �������� ����� ���������...
 */
w_raw(s)
register char *s;
{
	register int c;
	for(; *s; s++) {
		c = *s;
		/* ����� ����������, ������ ���
		 * UCASE ��� ����� �������� :
		 */
		if(((osgflg & LCASE)!=0) && (c >= 0140) && (c < 0177))
			c |= 0200;
		putc( c, vttout );
	}
}

/*---------------------------------*/
/* ������ ������ �������� �� ����� */
/* � ��������������                */
/*---------------------------------*/
/*
 * ������������ �������, ������� ��� ��
 * ���������� � ����� ����������,
 * ���, ����� ��� �������� ���� �������,
 * � ���� �� ���-�� ������.
 */
w_str(s)
register char *s;
{

	for(; *s; s++) {
		w_chr(*s);
	}
}

w_strn(s, n)
/*---------------------------------------------------*/
/* ������ ������ �������� �����, ��������� ��������� */
/*---------------------------------------------------*/
register char *s;
register int n;
{
	n -= strlen(s);
	for(; *s; s++) {
		w_chr(*s);
	}
	while(--n>=0) w_chr(' ');
}

/*------------------------------------------*/
/* ������ ������ � ��������� �������������� */
/*------------------------------------------*/
w_chr(c)
register int c;
{
	extern int osgflg;
	register int oc;        /* ������ ��� ������ */

	if(osgflg & LCASE) {
		switch(c) {
		case '`' :
			oc = '\'';
			break;
		case '{' :
			oc = '(';
			break;
		case '|' :
			oc = '!';
			break;
		case '}' :
			oc = ')';
			break;
		case '~' :
			oc = '^';
			break;
		default :
			oc = c;
			break;
		}
	} else {
		oc = c;
	}
#ifdef  ASCII7
	oc &= 0177;
#endif
	putc(oc, vttout);
}
/*---------------------------------------------*/
/* ������������� ������ ESC-������������������ */
/*---------------------------------------------*/
/*
 * ����������:
 *      ����� �������� �������, ������� �� ��������
 *      ����������, ��� ������� ������ �
 *      ������������������ ��������������
 *      �������, ������� ������������� �
 *      ���� �������.
 */

int escseq(cod)
int cod;
{
	register int c;

	if((c=cod) <= 0177)
		return( c );

	c = 0177 & c;  /* ���������� �� ��������� ������� */

#ifndef DEMOS2CYR
	return( c );
#else
	if(c >= 0100 && c <= 0176) {
		if(osgflg & LCASE)  goto rev_cod;
		if(osgflg & CYRILL) {
		    switch(osgflg & CSTYLE) {
					/* ��� �� ���������������� */
		    case CBITS8   :
		    case CS_TYPE0 :
		    case CS_TYPE3 :
			break;
					/* �������� ������������ ��� */
		    case CS_TYPE2 :
		    case CS_TYPE1 :
			goto rev_cod;
			break;
		    };
		goto return_cod;
		}
	}
rev_cod:        /* ����������� ��� */
	if(c >= 0100 && c <= 0137)
		c += 040;
	else
		c -= 040;
return_cod:
	return( c );

#endif /* DEMOS2CYR */
}
#endif /* DEMOS2 */
