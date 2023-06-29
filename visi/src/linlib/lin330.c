/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin330.c,v 1.1 90/12/27 16:29:18 vsv Rel $
 *
 *      $Log:	lin330.c,v $
 * Revision 1.1  90/12/27  16:29:18  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  89/08/29  15:17:03  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.0  88/06/16  17:50:23  vsv
 * ������� ��������� ������ �� ������� ��������������
 * 
 */

#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "line0.h"

extern SCREEN scrn;

int     edinsm = 0;     /* ����: ����� ������� */
/* extern  int edinff;  */
int     edinff = 1;     /* ����: ���������� ��������� ��������� ������ */


static edinfo( infflg )
register int infflg;
{
	if ( !edinff )  return;
	if ( infflg ) {
		w_msg(scrn.sc_at, ":���:");
		if ( edinsm )   w_str("<�������>");

	} else {
		w_emsg("");
	}
}

static int
chgstr(s, size, ofs, cod)
/* ��������� ��������� � �������� */
register char *s;
int  size;
int ofs;
kbcod cod;
{
	register int j;
	register int lend;      /* ������� ���������� ��������� ������� */
		 int chg;       /* ������� ��������� */

	/* ����� ���. ����� ������ */
	if(edinsm) {
		for(lend=size; lend>0 && s[--lend]==' ';) ;
		if(lend<ofs) lend=ofs;
	}
	chg = ofs;
	if(cod==KB_DE) {
		cod = ' ';
		ofs = --chg;
		if(edinsm) {
			for(j=chg; j<lend; j++) s[j]=s[j+1];
			s[j++]=' ';
		} else {
			s[lend = chg] = cod;
		}
		/******* <--(KB_DE)--- ******/
		cp_set(scrn.sc_li, scrn.sc_co - 1, scrn.sc_at);
	} else {
		if(edinsm) {
			for(j=size; --j>chg; ) s[j]=s[j-1]; /* �������� */
			lend += (lend<(size-1) ? 1 : 0);
		} else {
			lend = chg;
		}
		s[chg] = cod;
		ofs += 1;
	}

	for(j=chg; j<=lend; j++) w_chr(s[j]);   /* �� �����!!! */
	return (ofs);
}


e_str(str, size, ctst, ofsp)
/*-----------------*/
/* �������� ������ */
/*-----------------*/
register char    *str;  /* ������ ��� ��������������*/
int     size;           /* ������ ���� �������������� */
kbcod   (*ctst)();      /* ���� ��� ����� ���������� ����� */
int     *ofsp;          /* ��������� �� �������� �������� �� ���. ���� */
{
	/* ������ ���� � VIDEO �������� ����� cp_set();
	 * ctst ���������� 0, ���� ��� �� �������� ������;
	 *                -1, ���� ���� ��������� ��������������;
	 *              cod,  ���� ��� �������� ���������� ������.
	 */

	register int i;
	register int j;
	register int  col;      /* ������� � ������ �� ������ */
		 int  lin;
		 int  atr;
		 kbcod    cod;
		 kbcod    ok;

	col = scrn.sc_co;
	lin = scrn.sc_li;
	atr = scrn.sc_at;

	edinfo(1);
	/* ��������� ����� ������ � �������� */
	cp_set(lin, col, atr);
	j = 0;
	while(str[j]) w_chr(str[j++]);
	while(j<size) w_chr(str[j++] = ' ');
	str[size] = 0;

	i = ofsp ? *ofsp : 0;

	/* ���� �������������� */
	for( ;; ) {
		if(i >= size) i = size-1;       /* ������ ���������!!! */
		cp_set(lin, col+i, 0);

		cod = r_cod(0);
		/* ������������ ���� */
		ok = ctst ? (*ctst)(cod, size, i) : cod;
		if(ok == -1) {
			goto ret;
		} else if(ok == 0) {
			bell();
		} else {
			switch(cod = ok) {
			case KB_PR: /* ���. ������� ��������� ������ */
				switch(r_cod(0)) {
				case KB_AR: /* ����� �������� ���. */
					   for(i=size-1; isspace(str[i]);
						i--); i++; break;
				case KB_AL: i = 0; break;
				case ' ':
					   for(j=i; j<size; j++) {
						w_chr(str[j] = ' ');
					   }; break;
				case KB_DE:
					   for(j=i; j<size-1; j++) {
					       w_chr(str[j] = str[j+1]);
					   };
					   w_chr(str[j] = ' ');
					   str[size] = '\0';
					   break;
				case KB_PR: edinsm=(edinsm ? 0 : 1);
					   edinfo(1);
					   break;
				default: bell(); break;
				}; break;

			case KB_AL: if(i>0)  i--;
				   else     goto ret;
				   break;
			case KB_TA: if(i == size-1) goto ret;
				   i += 8-(i%8); break;
			case KB_AR: if(i == size-1) goto ret;
				   i++;
				   break;
			case KB_NL: goto ret;
			case KB_DE:
				   if(i==0)      goto ret;
				   i = chgstr(str, size, i, cod);
				   break;
			default:
				if(cod1(cod))   goto ret;/*�� ����� � �����*/
				else            i=chgstr(str, size, i, cod);

			}
		}
		/* ����� ����� ��������� */
	}
ret:
	str[size] = 0;
	if(ofsp) *ofsp = i;

	/* ���������� ������� � ����� ������ */
	for (i=size; --i>=0 && (str[i]==' ');) ;
	str[++i] = 0;
	edinfo(0);
	return(cod);
}
