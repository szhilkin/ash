/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin440.c,v 3.4 89/08/29 14:50:05 vsv Rel $
 *
 *      $Log:	lin440.c,v $
 * Revision 3.4  89/08/29  14:50:05  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.3  88/12/28  12:04:35  vsv
 * ����� ������������ ������.
 * ��������, ��� ���� ��������.
 * 
 * Revision 3.2  88/12/25  13:48:30  vsv
 * ������� �������� ������� ������������
 * malloc() & free() ��� � ���� fopen() & fclose(),
 * �� ����� ������� � �����, ���� ������� ����� ������
 * lbp & b_page().
 * 
 * Revision 3.1  88/06/27  15:30:39  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  87/12/21  12:22:20  vsv
 * ��������������� ������.
 * 
 */

/*
 * ������ � �������� ���������� �������
 */

#include <stdio.h>
#include "line.h"
#include "linebp.h"

LINE_H  lhd = { 0 };    /* ����� ��� ���������� ��������� */
static  char **atabp = NULL;   /* ������� ������� ��� ��������� line[] */
static  LINE *lineb = NULL;     /* ��������� �� ��������, ���-��� ������ */
	char exname[8] = { 0 };       /* ��� �� �������� �������� */
static  unsigned maxid = 0;     /* ����. ����� ����� ��� ����������� */

static int errflg = 0;     /* ���� ������ ��������� ������ */
static err_l(line)
LINE *line;
{
	if( errflg ) {
		line->flag = 0;
		line->attr = LHDR;
		line->cvts = 0;
		line->cvtf = 0;
		line->test = 0;
		line->varl = "<?>";
	} else {
		return ;
	}
}

d_page(llll)
/*---------------------*/
/* ���������� �������� */
/*---------------------*/
LINE *llll;
{
	free( llll );
}


LINE *
b_page(libnam, fnam, ports)
/*-----------------------------------------*/
/* ��������� �������� �� �������� �������� */
/*-----------------------------------------*/
char  *libnam;          /* ��� ���������� � ���������� */
char  *fnam;            /* ��� ����� (��������) */
IN_PORTS *ports;        /* ������� ���� ���������� */
{
	char    *malloc();
	register LINE *line;
	int      j;
	unsigned i;
	register char *s;
	int      c;
	unsigned count;
	FILE   *fp;
	IN_PORTS *in_pp;
	char    b_pgnm[100];
		/* ��� ������������ ����� �������� �������� */

	/* ����� ����� � ���������� */
	if( libnam == NULL )
		strcpy(b_pgnm, "");
	else
		strcpy(b_pgnm, libnam);
	strcat(b_pgnm, fnam);

	/* ������� ���� �������� �� ������ */
	if((fp=fopen(b_pgnm, "rn")) == NULL) {
		return(NULL);
	}

	/* ��������� ��������� */
	s = (char *) &lhd;
	for(i=0; i<sizeof(lhd); i++) {
		if ((c=getc(fp)) == EOF)  break;
		else                      *s++ = c;
	}
	fclose(fp);     /* ����� ������������� */

	/*  ������ ��� �� ������� (��������� ��������� malloc & free) */

	/* ��������� ������ ��� line[] � heaps */
	count = lhd.lh_lines + lhd.lh_heaps;
	lineb = (LINE *)malloc(count);
	if ( lineb == NULL ) {
		fclose(fp);
		return(NULL);
	}

	/* ����� ������� ���� �������� �� ������ */
	if((fp=fopen(b_pgnm, "rn")) == NULL) {
		free((char *)lineb);
		return(NULL);
	}
	/* ���������� ��������� */
	for(i=0; i<sizeof(lhd); i++) {
		if ((c=getc(fp)) == EOF)  break;
	}

	/* ��������� line � heaps */
	s = (char *)lineb;
	for(i=0; i<count; i++) {
		if ((c=getc(fp)) == EOF)  break;
		else                      *s++ = c;
	}

	/* ��������� ������ �� ������� ������� */
	atabp = (unsigned *)malloc( lhd.lh_names);
	if ( atabp == NULL ) {
		fclose(fp);
		free(lineb);
		return(NULL);
	}

	/* ��������� ������� ������� */
		/* ���������� 8 - ����� ����� � ������ �� ����. �������� */
	maxid = ((unsigned)(lhd.lh_names)/8);
	for(i=0; i < maxid; i++) {

		/* ������ ��������� ��� */
		s = exname;
		for(j=0; j<8; j++) {
			if ((c=getc(fp)) == EOF)  break;
			else                      *s++ = c;
		}
		/* ����� ��� ���� ����� �� ����-������� */
		atabp[i] = (char *)(~NULL);
		for(in_pp=ports; in_pp->in_name; in_pp++) {
			if(strcmp(in_pp->in_name, exname) == 0) {
				atabp[i] = in_pp->in_addr;
				break;
			}
		}
	}
	/* ��������� ��������� � �������� lineb[] */

	for(line=lineb; line->size; line++) {
		errflg = 0;
		adj_lin(line);
		err_l(line);
	}

	/* ������� ������� ������ �� ������� ������� */
	free( atabp );

	/* --- ������� ���� --- */
	fclose(fp);

	/* --- ������� ��������� �� �������� --- */
	return(lineb);
}

/*---------------------------------------*/
/* ��������� ���������� � �������� ����� */
/*---------------------------------------*/
adj_lin(line)
LINE *line;
{
	char    *adj_ptr();

	line->cvts = adj_ptr( (char *)line->cvts );
	line->cvtf = adj_ptr( (char *)line->cvtf );
	line->test = adj_ptr( (char *)line->test );
	line->varl = adj_ptr( (char *)line->varl );
}

char *
adj_ptr( s )
/*---------------------*/
/* ��������� ��������� */
/*---------------------*/
char  *s;
{
	register unsigned u;
	char   *rets;

	u = (unsigned)s;

	if(u == 0)
		return( NULL );
	if(u<maxid) {
		if( atabp[u] == (char *)(~NULL) ) {
			errflg = 1;
		} else {
			/* ����� ������������� �� ������� */
			rets = atabp[u];
		}
	} else {
		/* ����� ������ �� �������� �������� */
		u -= 2048;             /* ������ ����������� */
		u += (lhd.lh_lines);   /* ����. �� line */
		u += (unsigned)(lineb); /* ���� */
		rets = (char *)u;
	}
	return( rets );
}
