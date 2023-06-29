#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include "line.h"
#include "assist.h"

char *malloc();

/*
 * ������������ ����������� ����������������� ����.
 *
 * �������� ���� .ashmenu � �������, ���� � ��������,
 * ���� � ��������� ��������. (����� ������� environ PATHM).
 * ���� �������� ����������� ��������� ����� ��� sh, csh � �.�.
 * ������� menu2r ��������� ���� ����, ���������� ������ �������
 * � offset ��� lseek.
 * ����� ��������� ����� ���� �� ������� itm ������������ � �����
 * (����� lseek), ���������� � /tmp, � ����������� ��� ������.
 * ��� ���������� ����������� ���������� �����.
 */

#define LBLS    30      /* ���������� ������� ���� */

char    *lbls[LBLS+1];          /* ������ ���� */
off_t    lblofs[LBLS+1];        /* ��������� �� ���. ����� */
int      lblsize[LBLS+1];       /* ������ ������ */
int      lblsi;                 /* ������ �������� ����� ���� */
int      lblmax;                /* ���������� ������� */


extern   char    *tmpflnm;      /* ���� ��� ������ � ���������� ���� */
static   FILE *fpmenu;

int
menu2r(mfile)
char *mfile;
{
	char str[140];          /* ��������� ������ �� ����� */
	int c;
	register char *p;
	off_t curofs;
	extern FILE *afopen();

	/* ����� ���� ���������� �� �������� �������� ���� */
	if ((fpmenu = afopen(mfile, 1)) == NULL)
		return(0);
	/*
	 * ������ ������ �� �����, ��������� �� ������,
	 * ���� ���� �������� �����������, ����������.
	 */
	p = str; lblsi = 0;
	curofs = (off_t)0;
	while ((c = getc(fpmenu)) != EOF) {
	    curofs += (off_t)1;
	    if ((*p++ = c) == '\n') {
		*(--p) = '\0';
		if (strncmp(str, "#+", 2) == 0) {
		    if (lblsi) {
			/* ������� ���������� �������� */
			lblsize[ lblsi - 1] = (int)( curofs -
			lblofs[ lblsi - 1] - (off_t)(strlen(str) + 1));
		    }
		    lblofs[ lblsi] = curofs;

		    p = &str[2];
		    if ((lbls[ lblsi] = malloc(strlen(p) + 1)) == NULL)
			break;  /* ��� ����� ��� ����� */
		    else
			strcpy(lbls[ lblsi], p);

		    if (lblsi < LBLS) lblsi++;
		    else break; /* ��� ����� ��� ���������� */
		}
		p = str;        /* ������������ ��������� */
	    }
	}
	/* ��������� �������� ����������� �� EOF */
	lblmax = lblsi;
	if (lblsi) {
	    lblsi -= 1;
	    lblsize[ lblsi] = (int)(curofs - lblofs[ lblsi]);
	}
	lblsi = 0;
	return(1);
}

menu2u(cmdlbl)
/*
 * ������ ����, ������ ���������� �����.
 */
char *cmdlbl;   /* ������� ��� ������ ������ ����������� ������� */
{
	register int count;
	register LINE *vfp;
	unsigned i;
	kbcod cod;
	FILE *fptmp;
	int c;

	/* �������������� ����� �� ������ */
	cp_set(y0, 0, TXT);
	er_eop();
	itmshow();
	w_page(vf, 0);

	for ( ;; ) {

		i = itm - itmofs;
		vfp = vf;
		vfp += i;

		cod = r_line( vfp, 0 );

		w_emsg("");

		switch (cod) {

		default:
			break;
		case KB_HE:      /* ������� */
			break;

		case KB_NL:
		case ' ':
			/* �������� ��������� ���� */
			if ((fptmp = fopen(tmpflnm, "w")) != NULL) {
				if (fseek(fpmenu, lblofs[ itm], 0) < 0)
					return(0);
				count = lblsize[ itm];
				while(--count >= 0
				&& (c = getc(fpmenu)) != EOF) {
					putc(c, fptmp);

				}
				fclose(fptmp);
				/*VARARGS*/
				sprintf(cmdlbl, "#+%s", lbls[itm]);
				/* ����� ��������� ���.���� */
				return(1);
			}
			/* �������������... */
		case KB_EX:
			return(0);      /* ����� ��� ���������� */

		case KB_RE:      /* ����������� */
			er_pag();
			cwdshow();
			w_emsg("");
			itmshow(); w_page(vf, 0);
			break;
		case KB_AL:
		case KB_AU:
		case KB_AD:
		case KB_AR:
			i = itmadj(cod);
			break;
		}
	}
}

static  LINE tmplate =
/*NOSTRICT*/
{ 16, 0, 0, 0, MSE|INP|PMT|NED|LFASTR, 0, cvt_sp, 0, 0 };

int
menu2(tmps, cmdlbl, mfile)
char *tmps;     /* ������ ��� ������� */
char *cmdlbl;   /* ������� ��� ������ ������ ������� */
char *mfile;    /* ��� ����� ��������� ���� */
{
	extern int  y0_top;     /* ���������� � vshcmd */
	extern char pmtsh[];    /* --"-- */
	LINEMENU savelm;
	int ok;

	if ( !menu2r(mfile) ) {
		w_emsg("��� ������"); w_str(mfile);
		return(-1);
	}

	savelm = clm;
	cp_set(y0-1, 0, TXT);   /* ��������� ������, ��. ���� */
	er_eop();

	/* ������������� ���� */
	itms   = lbls;          /* ��������� �� ������ ���� */
	itmmax = lblmax;        /* ���������� ������� */
	vf     = (LINE *)0;
	itmlen = maxco/2;
	ltmpl  = &tmplate;

	itm    = lblsi;         /* ������� ����� ���� */
	yy_max = 10;
	itmofs = 0;
	while((itm - itmofs) >= yy_max)
		itmofs += yy_max;
	itmini();
	yy = 10;
	x0 = itmlen/2 - 1;
	pre_vf();

	/* ��������� ������ */
	if (y0_top > y0) {
		y0_top = y0;
		scrlnl();
	}

	ok = menu2u(cmdlbl);       /* ����� �� ���� */

	/*
	free((char *)vf); vf = (LINE *)0;
*/
	cp_set(y0_top, 0, TXT); er_eop();
	for (; lblmax >= 0; lblmax--)
	    if (lbls[ lblmax] != (char *)0) {
		free(lbls[ lblmax]); lbls[ lblmax] = (char *)0;
	}
/*      lblsi = itm;       ����� �������� �������� ������ ���� */

	free((char *)vf); vf = (LINE *)0;
	fclose(fpmenu);
	clm = savelm;

	if (ok)
		sprintf(tmps, ":sh %s %c%c", tmpflnm, MONEY, MONEY);
	return(ok);
}
