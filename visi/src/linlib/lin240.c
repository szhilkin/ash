/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin240.c,v 1.1 90/12/27 16:29:07 vsv Rel $
 *
 *      $Log:	lin240.c,v $
 * Revision 1.1  90/12/27  16:29:07  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.9  89/08/29  15:16:14  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.8  89/08/29  10:32:44  vsv
 * ��������� ������� �������������� 
 * �������� ��������� �� ������� (��� uas)
 * ������ #ifdef UASDEBUG
 * 
 * Revision 3.7  89/08/14  16:37:16  vsv
 * ����������� ������ � ������ ����� ���������,
 * ������ ������.
 * 
 * Revision 3.6  89/05/05  11:45:05  vsv
 * vhset :lh: works without :lh: termcap key
 * 
 * Revision 3.5  89/01/16  19:29:12  vsv
 * �������� ��������� ����,
 * ��������� � ��������� ���� ������
 * �� �������� vhset.
 * 
 * Revision 3.4  89/01/05  14:41:58  vsv
 * ���������� �����: ���� ���� � �����
 * ��������� ����.
 * 
 * Revision 3.3  88/08/18  10:36:59  vsv
 * ���������� ������������ ����� cont:
 * 
 * Revision 3.2  88/07/27  16:32:14  vsv
 * ������� ���������� ������ ��������� ������
 * 
 * Revision 3.1  88/06/27  15:20:15  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  88/06/16  17:55:17  vsv
 * ��������� .vhelp �� .vhset
 * 
 */

#include <ediag.h>
#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "line0.h"

/*
 * ��������� ���������� ����� ���������
 */

extern  LPA     lpainp[];
extern  LPA     lpaout[];
extern  KBL     kbl[];      /* �������� ������������ ��������� */
extern  char   *filelh;    /* ��������� �� ��� ����� � ��������� */
extern  int     kpadon;    /* ����: �������������� ���������� �������� */

#ifdef DEMOS2
extern  char *getenv();
#endif DEMOS2


do_kbl()
/*--------------------------------*/
/* ��������� ��������� ���������� */
/*--------------------------------*/
{
	char    name[100]; /* ��� ���������� ��� ������ HELP */
	char    str[100];  /* ������ ��� �������� ����� ������ ��������� */
	FILE    *lhfp;     /* ���� � ���������� */

	kbcod   t_key;     /* ���. ��� ������� */
	kbcod   t_cod;     /* ���. ��� */
	kbcod   t_cods;    /* ���. ��� ������� ��� ������ */

	register LPA *lpap;
	register char *s;
	register int   i;
		 int ibeg;
		 char *os;

#ifndef DEMOS2
	/* ���� ��� ����������, ��� ������ ���������, ����� */
	if ( *t_lh == '\0' ) return;
#endif
#ifdef  DEMOS2
	/* ���������� �����, ��� �� ������ */
	if ( (s=getenv("VHSET")) != NULL ) {
		strcpy(name, s);
	} else {
		strcpy(name, "/usr/local/lib/visi");
	}
	strcat(name, "/.vhset/");
	if(*t_lh) {
		strcat(name, t_lh);
	} else {
		if((s=getenv("TERM")) != NULL)
			strcat(name, s);
	}

	/* ��������� ���� */
	if( (lhfp=fopen(name, "r")) == NULL ) {
		/* ������� �����������... ���� ��������� */
		return;
	}
#endif  DEMOS2

#ifdef  RT11
	/* ���� ����� ���� ���� 'HOM:VT52.VHS' */
	/*              ����    'SY:VT52.VHS' */
	strcpy (name, "VHS:");
	strcat (name, t_lh );
	strcat (name, ".VHS");

	/* ��������� ���� */
	if( (lhfp=fopen(name, "r")) != NULL ) {
		goto cont1;
	}
	strcpy (name, "SY:");
	strcat (name, t_lh );
	strcat (name, ".VHS");

	/* ��������� ���� */
	if( (lhfp=fopen(name, "r")) == NULL ) {
		return;
	}
cont1:
	for(s=name; *s; s++) {
		if(islower(*s)) toupper(*s);
	}
#endif

	/* �������� ��� �����, ���� ���� ��� */
	if (filelh) strcpy(filelh, name);

	ibeg = 0;
	/* ������ ���������... */
	while( fgets(s=str, 98, lhfp), feof(lhfp)==0) {
		if(isspace(*s))       continue;

		/* �������� ���. ���������� ? */
		if     (*s == '+')      { w_raw(t_ks); kpadon = 1;      }
		else if(*s == '-')      { w_raw(t_ke); kpadon = 0;      }

		/* ��� � �������� ������ ? */
		else if(*s == ':') {
			s++;
			/* �������� ��� ���� ������� */
			t_key  = tocod0(*s++);
			t_key |= tocod1(*s++);
			t_cod  = tocod0(*s++);
			t_cod |= tocod1(*s++);

#ifndef UASDEBUG
			/* ���������, ������ ���� ��������� */
			for(i=ibeg, ibeg++; t_cods = kbl[i].t_cod; i++ ) {
				if(t_cods == t_cod) {
					/* ����������� ���. ��� */
					kbl[i].t_key = t_key;
					/* ����������� �������� */
					if(os = kbl[i].t_knm) {
					    while(*s && isprint(*s))
						*os++ = *s++;
					    *os = 0;
					}
					goto cont;
				}
			}
#else
			/* ������� �������� */
			i=ibeg; ibeg++;

			/* ����������� ���. ��� */
			kbl[i].t_key = t_key;
			/* ����������� ���. ��� */
			kbl[i].t_cod = t_cod;
			/* ����������� �������� */
			if(os = kbl[i].t_knm) {
			    while(*s && isprint(*s))
				*os++ = *s++;
			    *os = 0;
			}
			goto cont;
#endif
		} else
		if(isdigit(*s)) {     /* ������� � ��������� */
			lpap = &lpaout[*s++ & 07];  /* ASCII-���7-���8-depend! */
			lpap->lpa_p  =  *s++;
			lpap->lpa_a  = (*s++ & 07) << 6;
			lpap->lpa_a |= (*s++ & 07) << 3;
			lpap->lpa_a |= (*s++ & 07);
			lpap = &lpainp[*str & 07];
			lpap->lpa_p  =  *s++;
			lpap->lpa_a  = (*s++ & 07) << 6;
			lpap->lpa_a |= (*s++ & 07) << 3;
			lpap->lpa_a |= (*s++ & 07);
		}
cont:
		;
	}
	fclose(lhfp);
	return;
}
