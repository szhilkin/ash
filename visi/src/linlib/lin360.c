/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin360.c,v 1.1 90/12/27 16:29:26 vsv Rel $
 *
 *      $Log:	lin360.c,v $
 * Revision 1.1  90/12/27  16:29:26  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.2  89/08/29  15:17:36  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  88/06/27  15:21:45  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  87/12/21  12:20:49  vsv
 * ��������������� ������.
 * 
 */

#include <stdio.h>
#include "line.h"
#include "line0.h"

/*----------------------------------------------------------------------*/
/*
 * �������� ! � �������� fnd_ar(), fnd_al(), ...
 *      ������������ ��������� �����, ���������� ��
 *      ���������� ������������ �� ��� ���-11 (DECUS, UNIX,
 *      �� ��������� ��� Whitesmith) :
 *      ��������� ����� ���������� ����� ����������� ��������
 *      � ���������� ������� (����� �� ������ ����).
 *      � ���� ������ ��������� ������� ������� �������� �� ���������,
 *      �� �� �������� �� ��� ��������.
 *      �������� ��������� � ���������, �������� ����� ���������
 *      ���������� ����������� ������������ �� ����� ����������� �
 *      ��������� ������� ���� ������� ��� ����������� ����������.
 *      =========================================================
 *      ���, � ���������, �������� linlib v2.0 -- ��� ���� �����
 *      �������, ��� "����� �� ������� �� �����" (����������), ��
 *      �������� �� �������� �������� ����������
 *      ��������� ������������ -- ����� ��������� ��������
 *      ����� �� �����������.
 */

/*-------------------------*/
/* ����� �� ������� ������ */
/*-------------------------*/
LINE *
fnd_ar (lni, line)
	register LINE *lni;
	register LINE *line ;
{
	register LINE *lnj;

	for(lnj=lni; lnj->size != 0; lnj++) {
		if( (INP & ~(lnj->attr)) )
			continue;
		if(((int)lnj->line == (int)lni->line)
		&& ((int)lnj->colu > (int)lni->colu))
			return(lnj);
	}
	/* ��������� � ���� �� ������ */
	for(lnj=line; lnj->size != 0; lnj++) {
		if( (INP & ~(lnj->attr)) )
			continue;
		if(((int)lnj->line == (int)lni->line))
			return(lnj);
	}
	return(lni);
}
/*------------------------*/
/* ����� �� ������� ����� */
/*------------------------*/
LINE *
fnd_al (lni, line)
	register LINE *lni;
	register LINE *line ;
{
	register LINE *lnj;

	for(lnj=lni; lnj>=line; lnj--) {
		if( (INP & ~(lnj->attr)) )
			continue;
		if((lnj->line == lni->line)
		&& (lnj->colu < lni->colu))
			return(lnj);
	}
	/* ��������� � ���� �� ������ : ���� � ����� */
	for(lnj=lni; lnj->size != 0; lnj++ )       ;
	for(   ; lnj>=line; lnj--) {
		if( (INP & ~(lnj->attr)) )
			continue;
		if((lnj->line == lni->line))
			return(lnj);
	}
	return(lni);
}
/*------------------------*/
/* ����� �� ������� ����� */
/*------------------------*/
LINE *
fnd_au (lni, line)
	register LINE *lni;
	register LINE *line ;
{
	register LINE *lnj;
	int     nxt_line ;      /* ����� ��������� ������ */

	if((nxt_line = lni->line) > 0) {
		for(lnj=lni; lnj>=line; lnj--) {
			if((lnj->line == lni->line))    /*���. ������*/
				continue;
			else if( (INP & ~(lnj->attr)) )
				continue;               /*�� ��� �����*/
			else if((lnj->line <  lni->line)
			      &&(lnj->colu >  lni->colu))
				continue;               /*���. ����, �� ���*/
			else if((lnj->line <  lni->line)
			      &&(lnj->colu <= lni->colu))
				return(lnj);    /*���. ����, ���. �� ������*/
		}
	}
	/* ������� ����� � �������� ����������� */
	for(lnj=lni; lnj->size != 0; lnj++) ; /*������� �����*/
	for(   ; lnj>=line; lnj--) {          /*���� � ������*/
		if( (INP & ~(lnj->attr)) )    /*�� ��� �����*/
			continue;
		else if(lnj->colu <= lni->colu) /*�� ������*/
			return(lnj);
	}
	return(lni);      /* ��������� �� ������ ������ */
}
/*-----------------------*/
/* ����� �� ������� ���� */
/*-----------------------*/
LINE *
fnd_ad (lni, line)
	register LINE *lni;
	register LINE *line ;
{
	register LINE *lnj;
	int     nxt_line ;

	/* ����� ���� �� ����������(��������� �.�.) :
	 *       24 ������ �� ��� ���� ���������� ������...
	 */
	if((nxt_line = (int)lni->line ) < 24 ) { /*������, ���� ���� ����*/
		for(lnj=lni; lnj->size!=0; lnj++) {     /* ����� ? */
			if((lnj->line <= lni->line))    /*���. �� ���� */
				continue;
			else if( (INP & ~(lnj->attr)) ) /*�� ��� �����*/
				continue;
			else {                          /*���. ���� */
			      if((lnj->colu + lnj->size) >= lni->colu)
				  return(lnj);  /*���� ��� ��������*/
			      else
				  continue;     /*��� ���� �����*/
			}
		}
	}
	/* ������� ����� �� ������ �������� */
	for(lnj=line; lnj->size!=0; lnj++) {
		if( (INP & ~(lnj->attr)) )
			continue;
		else if(lnj->colu >= lni->colu)
			return(lnj);
	}
	return(lni);      /* ��������� �� ������ ������ */
}
/*---------------------------------------------------*/
/* ����� ������� �����: � ��������� ����� �� ������� */
/*---------------------------------------------------*/
LINE *
fnd_nxt(lni, line)
	register LINE *lni;
	register LINE *line ;
{
/*      register LINE *lnj;     */

	lni++;
	if(lni->size == 0)
		lni = line;          /* ��������� �� ������ */
	return(lni);
}

aj_tbl(scnd, line_e)
/*-----------------------------------*/
/* ��������� ������� ������� �� ���� */
/*-----------------------------------*/
register LINE *scnd;
LINE *line_e;
{
	register LINE *base;
	int     isleft;
	LINE saved;

	saved = *scnd;
	isleft = (saved.flag & SUSL) ? 1 : 0;
	base = scnd;
	while(base != line_e) {
		base--;
		if(isleft == 1) {
			if (base->colu <  saved.colu) {
				if (base->line == saved.line) {
					goto on_base;
				}
			}
		} else
		if(isleft == 0) {
			if (base->line <  saved.line) {
				if (base->colu == saved.colu) {
					goto on_base;
				}
			}
		} else {
			continue;
		}
	}
	/* ���� ������ ���, ���� �������� ��������� ���������� */
	w_chr('\007');
	return;

on_base:
	*scnd = *base;
	scnd->line = saved.line;
	scnd->colu = saved.colu;
	scnd->varl += (unsigned)(saved.varl);
}

/*---------------------*/
/* ���������� �������� */
/*---------------------*/
w_page ( line_e, cod )
LINE    *line_e;
register kbcod cod;
{
	register LINE *line;

	/* ������� ���, ��� �� ��� ����� */
	for(line=line_e; line->size != 0; line++) {
		/* ��������� ������� ������� ���� ������� ���������... */
		if(line->flag & SUST){
			aj_tbl(line, line_e);
		}
		if( (INP & ~(line->attr)) )
			w_line(line, cod);
	}
	/* ����� ��� ����� � ��������� ������� */
	for(line=line_e; line->size != 0; line++) {
		if( (INP & ~(line->attr)) == 0 )
			w_line(line, cod);
	}
}

kbcod
r_page(line_e, page, posp)
/*-----------------*/
/* ������ �������� */
/*-----------------*/
LINE    *line_e;                /* �������� ��� ��������������  */
LINE   **page;                  /* ������� ����� (������)       */
int    *posp;                   /* ������� ������� ��� �������������� */
{
	int     cod;            /* ���, ������������ r_line()   */

	register LINE *lni;             /* ��������� �� ������� ����� */
	register LINE *line ;           /* ��������� �� ��� �������� */

	line = line_e;
	if(*page != (LINE *)NULL)
		lni = *page;
	else
		lni = line_e;

	/* ���������� ��, ��� ������ ������������� */
	while( INP & ~(lni->attr)) {
		lni++;
		/* ����������� ����� ������ */
		if(lni->size == 0)
			lni = line_e;
	}

	cod = r_line(lni, posp);

	/* ����� ��������� ��� ���������� ������ */
	switch( cod ) {
	case KB_AR :
		if ( (lni->flag & SUSR) == FALSE )
			lni = fnd_ar(lni, line) ;
		break ;
	case KB_AL :
		if ( (lni->flag & SUSL) == FALSE )
			lni = fnd_al(lni, line) ;
		break ;
	case KB_AU :
		if ( (lni->flag & SUSU) == FALSE )
			lni = fnd_au(lni, line) ;
		break ;
	case KB_AD :
		if ( (lni->flag & SUSD) == FALSE )
			lni = fnd_ad(lni, line) ;
		break ;
	case KB_NL :
		if ( (lni->flag & SUSNL) == FALSE )
			lni = fnd_nxt(lni, line) ;
		break ;
	case KB_RE :     /* �������� ����������� */
		er_pag();
		w_page(line, 0);
		break ;
	default :
		break;
	}

	*page = lni;    /* ��������� ��������� �� ���. �����!... */
	return(cod);
}
