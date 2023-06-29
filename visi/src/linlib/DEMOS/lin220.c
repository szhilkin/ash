/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin220.c,v 1.1 90/12/27 16:29:01 vsv Rel $
 *
 *      $Log:	lin220.c,v $
 * Revision 1.1  90/12/27  16:29:01  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.2  89/08/29  15:16:00  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  88/06/27  15:19:57  vsv
 * ������� ������ RCS
 * 
 */

#include <ediag.h>
#include <stdio.h>
#include "line.h"
#include "line0.h"

/*
 * ��������� �� ��� ���������
 *
 * ��������� ��� ���������� ��������� �� ��� ���������
 */

#include <ctype.h>

extern char *malloc();

#ifdef DEMOS2
extern  char *getenv();
#endif

#ifdef RT11
static  char ttynm[30] = "VT0:TRMCAP.VT0";

/*-------------------------------*/
/*  �������� ��� ����� ��������� */
/*-------------------------------*/
char *
gttynm()
{
	FILE    *fopen();
	FILE    *i_fp;
	register char *s;

	for( s = &ttynm[2]; *s < '9'; *s += 1 ) {
		if((i_fp=fopen(ttynm, "r")) == NULL )
			continue;
		else {
			fclose(i_fp);
			ttynm[13] = *s;
			break;
		}
	}
	ttynm[0] = 'T';
	ttynm[1] = 'T';
	ttynm[2] = 'Y';
	return( ttynm );
}
#endif

/*  ������� �� ���������� termcap */
extern  int  tgetent(), tgetnum(), tgetflag();
extern  char *tgetstr();

extern  char *UP;       /* ������� ���������� ��� tgoto() */
extern  char *BC;

/* ����� �������� linlib, � ���������
 * ��� ���������� ��������� � �����.
 */
static char *codesp = 0;

/* ������� ������ */
extern  int     maxli;
extern  int     maxco;


/*---------------------*/
/* �������� ���������� */
/*---------------------*/
extern  KBF   kbf[];
extern  char *tcapo[];


static int cant()
/*-------------*/
/* ����������� */
/*-------------*/
{
#ifdef  DEMOS2
	fprintf(stderr, "%s '%s'\n",
	ediag("Bad environ", "�� ����������� ����������"), "TERM");

#endif
#ifdef  RT11
	fprintf(stderr,
"=== ��� ����� ��������� :\n\r\
	TTY:TRMCAP.VT* (.ASSIGN NL: VT*:)\n\r");
#endif
	exit(1);

}

/*----------------------------------------*/
/* ���������� �� ���������� ���� �������� */
/* ������ �� s2, ������ � s1              */
/*----------------------------------------*/
static int stripd(s1, s2)
register char *s1, *s2;
{
	while(isdigit(*s2))
		s2++;
	strcpy(s1, s2);
}


char *
gettcp(key)
/*-----------------------------------*/
/* �������� ESC-��� ��� �������� key */
/*-----------------------------------*/
char *key;
{
	/* ������� ��������� ESC-������������������
	 * � �����, ������ ��������� �� ������
	 * ������ � ������.
	 */
	register char *p, *pp;
	char *ptcap;
	char tbuf[80];          /* ����� ���������� �������� */
	char *x;                /* ��������� ��� ��������� � tgetstr() */

	x = tbuf;
	pp = codesp;    /* ���������, ���� ������ ��������� �������� */
	p = tgetstr(key, &x);          /* ��������� �������� */
	if (p && *p!=0) { ; }
	else {  p = ""; }

	ptcap = pp;
	/* ������� ��������, ��������� �������� � ������: */
	stripd(pp, p);
	pp += (unsigned )(strlen(pp)+1);

	codesp = pp;    /* ���������, ���� ������ ��������� �������� */
	return(ptcap);
}

hw_set()
/*--------------------------------------*/
/* ��������� ��������� �� ��� ��������� */
/*--------------------------------------*/
{
	register char *p;
	register char **pp;
	register KBF *kbfp;
	kbcod    cod;
	char  *buf;             /* ��������� �� ��������� ����� termcap */
	char     keys[4];       /* ������ ��� ������ � termcap ("k1") */
	int     ok;

	/* ������ ��� ��������� linlib */
	codesp = malloc(1024);  /* ����������� ����... */

	/* �������� ������ ��� ���������� ������ */
	if( (buf = malloc(1024)) == 0 ) {
		fprintf(stderr, "\nHWSET:%s\n", ediag("NO Memory\n", "���� ������"));
		exit(1);
	}
	/* �������� �������� ��������� */
#ifdef  DEMOS2
	if( p = getenv("TERM") ) {
#endif
#ifdef  RT11
	if( p = gttynm() ) {
#endif
		ok = tgetent(buf,p);
		if(ok != 1) {
			fprintf(stderr, "%s: '%s'\n",
	   ediag("Unknown terminal type", "�������� ����������"), p);
			exit(1);
		}
	}
	else    {
		cant();
	}

	/*---------------------------------*/
	/* ��������� ��������� ��� tgoto() */
	/*---------------------------------*/
/***    p = tgetstr("bc", &x);
	if (p && *p==0) {       BC = 0; }
	else    {
		stripd(bc_str, p); BC = bc_str; }
	p = tgetstr("up", &x);
	if (p && *p==0) {       UP = 0; }
	else    {
		stripd(up_str, p); UP = up_str; }

****/   /* ����� �����: */
	BC = gettcp("bc");   if(*BC=='\0') BC = 0;
	UP = gettcp("up");   if(*UP=='\0') UP = 0;

	/*-------------------------*/
	/* �������� ������� ������ */
	/*-------------------------*/
	maxli = tgetnum("li");
	maxco = tgetnum("co");

	/*--------------------------------*/
	/* ��������� ��������� ��� linlib */
	/*--------------------------------*/
	for (pp=tcapo; *pp; pp++) {
		*pp = gettcp(*pp);
	}
	for (kbfp= &kbf[0]; cod=(kbfp->t_key); kbfp++) {
		keys[0] = cod0(cod);
		keys[1] = cod1(cod);
		keys[2] = '\0';
		kbfp->t_cap = gettcp(keys);
	}
	do_kbl();       /* ��������� ����. ���. ����� */
	free(buf);
}
