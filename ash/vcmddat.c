#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "assist.h"
#include "astat.h"

#define WSMAX 300

char    Cfill[40];
char    Crepf[40];
char    Coutf[40];
char    Csubs[40];

/*NOXSTR*/
static  char dummy[] = "";
/*YESXSTR*/

struct  keycaps {
	kbcod   kc_key;         /* ��� ������� */
	char   *kc_cap;         /* �������� �������� ��� ������ */
	}
	keys0[11] = {
		{ '1', dummy },
		{ '2', dummy },
		{ '3', dummy },
		{ '4', dummy },
		{ '5', dummy },
		{ '6', dummy },
		{ '7', dummy },
		{ '8', dummy },
		{ '9', dummy },
		{ '0', dummy },
		{ 0,   (char *)0 },
	};

struct  keycaps *keyonp = keys0;        /* "�������" ������� */

keyshow(yes)
{
    if (yes) {
	register struct keycaps *k;
	char    tmpstr[20];
	int i, base, ofs;

	ofs = maxco/10;
	base = 0;

	for (i = 0, k = keyonp;
	i < 10 && k->kc_key;
	base += ofs, i++, k++) {
	    if ( i )                    /* ������ ������ ���� - ������ */
		w_chr(' ');
	    cp_set(-1, base, TXT);
	    w_chr(k->kc_key);
	    sprintf(tmpstr, "%-6.6s", k->kc_cap);
	    at_set(HDR);
	    w_str(tmpstr);
	    at_set(TXT);
	}
    }
}

/*
 * �������� �������.
 * ������� ������������, ������ ��� ����� ����������� (����).
 */
PATCMD  pc[PCMAX];      /* �������:�������� */
KEYTAB  kt2[KT2MAX];    /* ��������:����(pc):ixbeg:ixend */
KEYTAB  kt1[KT1MAX];    /* �������:����(pc):ixbeg:ixend */
static  char    bufs[BUFSMAX];

/*
 * ���������� ��� ������������ ����� ���������
 * � ���������� ������.
 */

	/* ������ ������ � ������ �����:*/
int     xchr;
	/* ������� ������� � ��������: */
int     kt2_ix;
int     kt1_ix;
int     pc_ix;
	/* ������� ��������� �� ����� �� ��������: */
char   *bufsp;

/*
 * �������� ������ � �����,
 * ������� ��������� �� ����� ����. ����� ������.
 * ������ �� ������ n ����, ����� ��� ��������, ���
 * ���������� �� ����� n-1 ��������.
 */
char *
apndstr(s, n)
register char *s;       /* ������, �� ������� ��������� */
int     n;              /* ����. ���-�� ����, ����. ��������� */
{
	register char *p;

	p = bufsp;
	while (*s) {
		if (*s == '\n') break;
		if (isspace(*s) && (--n <= 0)) break;
		*p++ = *s++;
	}
	*p++ = '\0'; bufsp = p;
	return(s);
}

/*
 * ���������� ���������.
 */
int
setdef(s)
register char *s;
{
	register int i;
	register char *p;

	switch(*s) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		/* �������� ������� ��� ���� */
		i = (int)*s++ - '0'; i--; if (i < 0) i = 9;
		keys0[i].kc_cap = bufsp;
		apndstr(s, 1);
		break;
	case 'f': case 'i': case 'o': case '@':
		switch (*s++) {
		case 'f': p = Cfill; break;
		case 'i': p = Crepf; break;
		case 'o': p = Coutf; break;
		case '@': p = Csubs; break;
		}
		if (*s) {
			/* �������� '\n' � ����� ������ */
			strcpy(p, s); p[strlen(s)-1] = '\0';
		} else  *p = '\0';
		break;
	default:
		return(0);
	}
	return(1);
}

kt1tune(kt1x)
register int kt1x;
{
	register int i;
	for (i = kt1x - 1; i >= 0; i--, kt1x--) {
		if (kt1[kt1x].kt_ib == kt1[i].kt_ib)
			kt1[i].kt_ie = kt1[kt1x].kt_ie;
	}
}

int
cmdset(file)
char *file;
{
    FILE *fp;
    char ws[WSMAX];     /* ������� ������ ��� ������ �� ����� */
    register char *p;
    extern FILE *afopen();

    /* ��������� ������ �� ������� �� �������� �������� */
    /* � ������? - ������ �� ������� */
    if ((fp=afopen(file, 1)) == NULL) {
	w_emsg("�� �������� ���� ���������");
	return(-1);
    }
    /* ������������� ���������� ������������ */
    pc_ix = 0;
    kt2_ix = kt1_ix = -1;
    bufsp = bufs;
    xchr = '\0';

    while (fgets(ws, WSMAX, fp) != NULL) {
	p = ws;

	switch (*p) {
	/* ������ ������ ���������� ��������� ������� */
	case '-':
		/* ������ ��������� */
		p++;
		setdef(p);
	case '#':
	case '\n':
		/* ������ ���������� � ������ ������������� */
		continue;
	case ' ':
	case '\t':
		/* ������ ������ (� ��������) */
		while (isspace(*p)) p++;
		pc[ pc_ix].pc_pat = bufsp;
		switch (xchr) {
		case ':':       /* kt1 */
			p = apndstr(p, 2);
			kt1[ kt1_ix].kt_ie  = pc_ix;
			break;
		case '+':       /* kt2 */
			p = apndstr(p, 3);
			kt2[ kt2_ix].kt_ie  = pc_ix;
			break;
		default:
			/* ��������� ����� ������ */
			continue;
		}
		while (isspace(*p)) p++;
		pc[ pc_ix].pc_cmd = bufsp;
		p = apndstr(p, 999);
		pc_ix++;
		continue;
	case '+':       /* ������ ������� �������� */
		xchr = *p++; kt2_ix++;
		kt2[ kt2_ix].kt_tab = pc;
		kt2[ kt2_ix].kt_ib  = pc_ix;
		kt2[ kt2_ix].kt_ie  = pc_ix;
		kt2[ kt2_ix].kt_key = bufsp;
		p = apndstr(p, 999);    /* ��� ������ �� ����� */
		continue;
	case ':':       /* ������ ������� ������� */
		xchr = *p++; kt1_ix++;
		kt1[ kt1_ix].kt_tab = pc;
		kt1[ kt1_ix].kt_ib  = pc_ix;
		kt1[ kt1_ix].kt_ie  = pc_ix;
		kt1[ kt1_ix].kt_key = bufsp;
		p = apndstr(p, 999);    /* ��� ������ �� ����� */
		continue;
	}
	/* ����� �.�. ������ ������... */
    }
    kt2_ix++; kt1_ix++; /* ������� �������: */
    kt2[ kt2_ix].kt_key = (char *)0;
    kt1[ kt1_ix].kt_key = (char *)0;
    /* ��������� ������� ��� ������� ������, ����� ����
     * ��������� �������������� �������� */
    kt1tune(kt1_ix);
    fclose(fp);
    return(1);
}
