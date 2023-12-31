/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin340.c,v 3.5 89/08/29 15:17:13 vsv Rel $
 *
 *      $Log:	lin340.c,v $
 * Revision 3.5  89/08/29  15:17:13  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.4  89/08/29  10:36:03  vsv
 * ���������. ��. rcsdiff
 * 
 * Revision 3.3  88/08/10  10:40:28  vsv
 * �������� ��������� ��������������
 * ��� ����� � ���������������
 * 
 * Revision 3.2  88/08/02  10:48:55  vsv
 * �������� ���� ������ �������� � �������������� ���� (allcod).
 * 
 * Revision 3.1  88/06/27  15:21:26  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  87/12/21  12:19:56  vsv
 * ��������������� ������.
 * 
 */

#include <stdio.h>
#include "line.h"
#include "line0.h"

extern LPA lpainp[];
extern LPA lpaout[];

extern int allcod;


/*----------------------------*/
/* ������� �� �������� printf */
/*----------------------------*/
static int
docvts(ou, fo, va)
register char *ou;      /* ������, ���� ��������� ����� */
register char *fo;      /* ������ printf */
register char *va;      /* ��������� �� ���������� */
{
#ifdef DOUBLE
	if(index(fo, 'f') || index(fo, 'e') || index(fo, 'g'))
		/* ������� � ��������� ������ */
		return( sprintf(ou, fo, *(double *)va) );
	else
		/* ��� ��������� ���������� � ��� */
#endif
		return( sprintf(ou, fo, *(long *)va) );
}

w_line(line)
register LINE *line;
{
/*      next_j();     */

	r_line(line, (int *)(-1));
}

kbcod
r_line(line, posp)
/*-------------------------------------------------*/
/* �����/���� �����, ������� ��� ��������� ������� */
/*-------------------------------------------------*/
register LINE    *line; /* ��������� �� ����� */
	 int     *posp; /* �������, � ������� ������ ������������� */
{
	register char *s;
	register int  i;
	kbcod   cod;            /* ��� ��������� ������� */
	int     attr;           /* ����� ��������� */
	int     cvterror;       /* ���� ������ ������� */
	int     tsterror;       /* ���� ������ ����� */
	int     midcnt;         /* ������� ��� ������������ �� ������ */
	int     filch;          /* ���� ��� ���������� */
	int     wksl;           /* ����� ������ ����� ������������� */
	int     size;           /* ������ ���� */
	int     flo;            /* ����: ������ ������������ */
	int     onexit;         /* ����: ����� ������ */
	char   *eds;            /* ������ ��� ��������� */
	int     base;           /* ������ �������� ����������:
				**   �������� �� ��������� */
	char    wks[STRLEN];    /* ������� ������ */

	attr = line->attr;

	/* ����� ����� r_line ? */
	if(onexit = (posp == (int *)(-1)) ? 1 : 0)
		attr &= ~INP;  /* ���, w_line */
	cod = 0;

inp_retry:
out_string:
	cvterror = tsterror = 0;
	s = eds = wks;
	base = 0; size = line->size;

	i = attr & VIDEO;

	/*==== ����� ���������� ��� ���������� ��������� */
	if (((line->attr & INP) == 0 ) && (lpaout[i].lpa_a == 0))
			filch = '_';
	else            filch = ' ';

	/*==== ��������� */
	if(attr & PMT) {
		eds++; base++; size--;
		if(attr & INP)  *s++ = lpainp[i].lpa_p;
		else            *s++ = lpaout[i].lpa_p;
	}
	/*==== ������ �� ����� */
	if( !(flo = ((attr & FLO) == FLO))) { /*=== ���� �� ���������� */
		if(line->cvtf) {                /* ����� ������� */
			(*(line->cvtf))(line, cod, "w", s);
		}
		else if(line->cvts) {           /* � ����� printf */
			docvts(s, line->cvts, line->varl);
		}
		else {                          /* ������ ������ */
			eds = line->varl;
			goto string_symple; }
	} else {
string_symple:
		strncpy(s, line->varl, size); s[size] = 0;
	}

	wksl = strlen(s);

	/*==== ������������ */
	if(attr & MID) {
		if((midcnt=((size-wksl)/2)) > 0) {
			for(i= wksl+= midcnt; i>=midcnt; i--)
				s[i] = s[i-midcnt];
			while(i >= 0) s[i--] = filch;
		}
	}
	/*==== ���������� */
	if(attr & PAD) {
		for(i=wksl; i<size;)    s[i++] = filch; s[size] = 0;
	}
	/*==== ������, ����� (�� ������ ���������) */
	cp_set(line->line, line->colu, attr);

	/*==== �������� */
	if ( (posp != (int *)(-1)) && (attr & (LFASTR|NED)) == (LFASTR|NED) )
		w_chr(*wks);    /* ������ ��� r_line */
	else
		w_str(wks);

	if(onexit) return(cod);    /* ����� ��� ������ ����� w_line */

	/*==== ������ ����� ������ ������ ������� */
	cp_set(line->line, line->colu, attr);

	/*==== ������ ��� ������ ������� */
	switch(cod = r_cod(0)) {
	case KB_DE: *eds = '\0';
	case  ' ': if(posp) *posp=0;
		   break;
	default:
		/* ���� �������� ��� ����� � ���������������:
		 * ������ �� ����� ����� ������
		 * ����� ��������� ������ � ���� KB_NL;
		 */
		if((attr & NED) == 0) {
			if(allcod && cod1(cod) == 0) {
				unr_c(cod);     /* "���������" ����� */
				*eds = '\0';    /* �������� ������ */
			} else
	/*                if(cod1(cod) != 0)    *******/
				goto inp_test;
		} else
			goto inp_format;
		break;
	}

edit_retry:
	/*==== �� ������������� ? */
	if(attr & NED) goto inp_format;

	cp_set(line->line, base + line->colu, attr);

	/*==== ��������, ������� ��� */
	cod = e_str(eds, size,
		     /*==== ���� ��� ��������� ? */
		    ((attr & EDT) ? (unsigned)(line->test) : 0), posp);
	/*
	 * ���� �������� � �������� ����� ���������:
	 *    ���� ����������� ������ �� KB_NL
	 */
	if(cod != KB_NL) goto inp_test;

inp_format:
	/*==== ������ �� ����� */
	if(flo) goto inp_test;  /* �.���� ����������... */

	if(line->cvtf) {
		if(cvterror = !(*line->cvtf)(line, cod, "r", eds))
			bell();
	} else if(line->cvts) {
#ifdef DURA
		/*
		 * �� ������ �������� �������������� �� ����� :
		 * � ����������� ����� �������� ���� ������ - %f
		 * (� �� ������ � DECUS  -- vsv, 15/02/87)
		 */
		if((index(line->cvts, 'f') != 0)
		    || (index(line->cvts, 'g') != 0)
		    || (index(line->cvts, 'e') != 0))       {
			if(cvterror= (!sscanf(eds,"%f",line->varl)))
				bell();
		}
		/* ��, ��� ����, ����� ������, ���� ���� ������ */
		else    {
			if(cvterror= (!sscanf(eds,line->cvts,line->varl)))
				bell();
		}
#endif DURA
		if(cvterror= (!sscanf(eds,line->cvts,line->varl)))
			bell();

	}
	/*==== ������ ������� ? */
	if(cvterror && onexit == 0 ) goto edit_retry;

inp_test:
	/*==== ���� ����� ��������� ? */
	if( flo || !(attr & EDT) ) {
		tsterror = !(line->test ? (*line->test)(line, cod) : TRUE);
	}
	/*==== ���� ������ ? */
	if(tsterror)           goto inp_retry;

	switch(cod) {
	case KB_AU:      /* ����� ���� ����� ������� � ��. ����� */
	case KB_AD:
	case KB_AL:
	case KB_AR:
	case KB_NL:      break;
	default :       return(cod);
	}

	onexit = 1; attr &= ~INP;       /* ������� ��� ������ */
	goto    out_string;
}
