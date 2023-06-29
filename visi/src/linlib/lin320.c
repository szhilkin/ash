/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin320.c,v 1.1 90/12/27 16:29:15 vsv Rel $
 *
 *      $Log:	lin320.c,v $
 * Revision 1.1  90/12/27  16:29:15  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.2  89/08/29  15:16:53  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  88/06/16  17:52:06  vsv
 * ���������� ������ ��������� ���������,
 * ��� ������� ����������� ��� ������:
 * ������ short ��� char (bool).
 * 
 * Revision 3.0  88/06/06  08:54:18  vsv
 * ���������� ������ � ������� er_scr,
 * ������� ��������� � ������������.
 * 
 * Revision 3.0  87/12/21  12:18:32  vsv
 * ��������������� ������.
 * 
 */


#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "line0.h"
#include "lineva.h"

extern  char *UP;
extern  char *BC;
SCREEN scrn = { 0 };

extern  LPA lpaout[];

extern  LPA lpainp[];

#define AWOLDS (A_SO|A_US|A_MD)

/* ������ ����� ���������: */
static  awolds = AWOLDS;        /* ��� ������ ��� �������� */

at_set(awi)
/*--------------------------*/
/* ���������� ������������� */
/*--------------------------*/
register int awi;        /* ������ � ����� ��������� */
{
	extern SCREEN scrn;

	register int awold;     /* ����� ������ ��������� */
	register int aw;        /* ����� �������� */

	if(awi == 0) return;    /* ������ �� ������ */

	awold = awolds;
	scrn.sc_at = (awi &= VIDEOM);
	/* VCOLOR ���� �� �������������� ... */
	if(awi & INP) aw = lpainp[awi & VIDEO].lpa_a;
	else          aw = lpaout[awi & VIDEO].lpa_a;

	if(aw == awold) return;         /* ���� ����� �� ���������� */
	awolds = aw;                    /* ��������� �� ����. ��� */

	/* ������� ��� ��������� */
	if(awold & A_SO) w_raw(t_se);
	if(awold & A_US) w_raw(t_ue);
	if(awold & (A_MD|A_MR|A_MB|A_MK)) w_raw(t_me);

	/* ������ �������� */
	if(aw & A_SO) w_raw(t_so);
	if(aw & A_US) w_raw(t_us);
	if(aw & A_MD) w_raw(t_md);
	if(aw & A_MR) w_raw(t_mr);
	if(aw & A_MB) w_raw(t_mb);
	if(aw & A_MK) w_raw(t_mk);

}

/*---------------------------*/
/* ��������� ������� ������� */
/* � ��������� ������        */
/*---------------------------*/
cp_set(li, co, at)
int li, co, at;
{
	register char *p;
	register int aa;
	extern char *tgoto();

	if(li < 0) li = (maxli+li);
	if(co < 0) co = (maxco+co);

	scrn.sc_li = li; scrn.sc_co = co;       /* ������� ���������� */
	p = tgoto(t_cm, co, li);
	w_raw( p);

	aa = (at & VIDEOM);
	at_set(aa);
}
/*-----------------------------------------------------*/
/* ���������/������������ ��������� � �������� ������� */
/*-----------------------------------------------------*/
static int s_colu, s_line, s_attr;
cp_sav()
{
	s_colu = scrn.sc_co;
	s_line = scrn.sc_li;
	s_attr = scrn.sc_at;
}
cp_fet()
{
	cp_set(s_line, s_colu, s_attr);
}

/*-------------------------------------------------*/
/* ������� �����, �� ����� ������, �� ����� ������ */
/*-------------------------------------------------*/
er_pag()
{
	awolds = AWOLDS;
	at_set(TXT);    /* ����� ������� ��������� ������ (������ ���) */
	w_raw(t_cl);

}

er_eop()
{
	w_raw(t_cd);
}

er_eol()
{
	w_raw(t_ce);
}

/*------------------------*/
/* ������� ������� ������ */
/*------------------------*/
er_scr(from, to)
register int from, to;
{
	while ( from <= to ) {
		cp_set(from++, 0, TXT);
		er_eol();
	}
}

/*----------------------------*/
/* ������ ��� �������� ������ */
/*----------------------------*/
bell()
{
	if     ( t_vb[0] )    { w_raw(t_vb); }  /* ��������: ������ ���� */
	else if( t_bl[0] )    { w_raw(t_bl); }
	else                  { return;      }
}
