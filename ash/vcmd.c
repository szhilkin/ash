#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "assist.h"
#include "astat.h"

/* ���������� ������� */
extern  int     cmdset();
extern  int     sup(), v_susp();
extern  int     vshcmd(), menu2();
extern  int     fsh(), fmenu2();
extern  int     ffile(), fmsg(), fmsgerr();
extern  int     rescan();

extern  char  *nmsubs();

#ifdef DEBUG
extern  int     kshow();
#endif

FUNTAB funtab[] = {
	{ "read",       0 },
	{ "sh",         fsh },
	{ "menu2",      fmenu2 },
	{ "cmdset",     cmdset },
	{ "sup",        sup },
	{ "sus",        v_susp },
	{ "file",       ffile },
	{ "ls",         0 },
	{ "help",       w_help },
	{ "msg",        fmsg },
	{ "err",        fmsgerr },
	{ "rescan",     rescan },
#ifdef DEBUG
	{ "kshow",      kshow },
#endif
	{ (char *)0,    0 },
};

extern  char    Cfill[];
extern  char    Crepf[];
extern  char    Coutf[];
extern  char    Csubs[];

extern  PATCMD  pc[];
extern  KEYTAB  kt2[];
extern  KEYTAB  kt1[];

static  char    lastkey[4];     /* ��� ��������� ������� ������� */
/*
 * ���������������� ��������� �������
 *
 * ��� ���� �������������:
 * 1. ����������� ������������� ����� ������� �� ������� kt1.
 *    ������������ ����� ������ ��������, ������� �������.
 * 2. ���� ��� �������� ���������� �� ����� '+',
 *    ���������������� ������� kt2.
 *    ������������ ����� �����, ��� ���, ������ �����.
 * ������������ ��������:
 *      -1      �� ������� ������������, ����� �� ��������.
 *       0      ����� �� ��������, ����� �� ��������.
 *       1      ������� ���������, ����� ��������.
 */
/*ARGSUSED*/
vcmd(j, cod, mainl)
int     j;      /* ������ ������ ����� � �������� */
kbcod   cod;    /* ��� ������� ������� */
LINE    *mainl; /* ��������� �� �������� ���� */
{
	char  file[140];
	char    cods[20];       /* ������ � �����. �������. ������� */
	register KEYTAB *ktp;
	register PATCMD *pcp;
	register int i;
	char   *patp;           /* ������� ��� ��������� */
	char   *cmd;

	/* �������� ������ � ����� �������, ����� ����������. */
	lastkey[0] = cod0(cod);
	lastkey[1] = cod1(cod);
	lastkey[2] = 0;
	for (ktp=kt1; ; ktp++) {
		if (ktp->kt_key == (char *)0)
			return(-1);     /* ������ �� ������� */
		if (patcmp(ktp->kt_key, lastkey))
			break;
	}

	/* �������� ������� ������� �������, ����� ���������� */
	patp = tstat1();
	for (pcp=ktp->kt_tab, i=ktp->kt_ib; i <= ktp->kt_ie; i++) {
		if (pcp[i].pc_pat == (char *)0)
			return(-1);     /* ����� ������� */
		if (patcmp(pcp[i].pc_pat, patp))
			break;
		if (i == ktp->kt_ie)
			return(-1);     /* ����� ������, ������ �� ������� */
	}
	cmd = pcp[i].pc_cmd;    /* ������� �������� */
	/* ��������� ������� */
	/* ���� ������� ���. �� ��. '+' */
	if (*cmd == '+') {
		/* ����� ��������� ��� ������� ������� */
		for (i=0, cmd++; cmd[i] && cmd[i] != ' '; i++)
			cods[i] = cmd[i];
		cods[i] = 0;
		for (ktp=kt2; ; ktp++) {
			if (ktp->kt_key == (char *)0)
				return(-1);     /* ������ �� ������� */
			if (patcmp(ktp->kt_key, cods))
				break;
		}
		/* �������� ������� ������� �������, ����� ���������� */
/*                cmdsub(file, &itms[itm][2]);  */
		cmdsub(file, "#@", itm);
		patp = tstat2(file);
		for (pcp=ktp->kt_tab, i=ktp->kt_ib; i <= ktp->kt_ie; i++) {
			if (pcp[i].pc_pat == (char *)0)
				return(-1);     /* ����� ������� */
			if (patcmp(pcp[i].pc_pat, patp))
				break;
			if (i == ktp->kt_ie)
				/* ����� ������, ������ �� ������� */
				return(-1);
		}
		cmd = pcp[i].pc_cmd;    /* ����� ������� �������� */
	}
	if (*cmd == '_' && cmd++) {
	/* ���������� ������� */
		return(vincmd(cmd));
	}
	return(vexcmd(cmd, (char *)0));   /* ������� ������� */
}

int vincmd(cmd)
register char *cmd;     /* ���������� ������� */
{
	char  keywd[20];        /* �������� ����� ������� */
	char *keywdp;
	int (*keyf)();
	register FUNTAB *ftp;

	keywdp = keywd;
	while (*cmd && *cmd != ' ')
		*keywdp++ = *cmd++;
	*keywdp = 0;
	while (*cmd && *cmd == ' ') cmd++;   /* ������� ������� */

	/* ���������� ������������ */
	for (ftp = funtab; ftp->ft_name; ftp++) {
		if (strcmp(ftp->ft_name, keywd) == 0) {
			if (keyf = ftp->ft_fun)
				return((*keyf)(cmd));
			else    break;
		}
	}
	w_emsg(keywd); w_str(cmd);
	return(-1);     /* ������� �� ������� */
}

/*
 * ��������� ����������� #k,#@,#i,#o,#a,..., ����� ##.
 */

cmdsub(ptmp, p, i)
register char *p;       /* ������ ���������� */
register char *ptmp;    /* ���� ���������� */
register int  i;        /* ����� itm */
{
    extern char *getenv();
    register char *s;
    char *nm_ptr;

    *ptmp = '\0';
    while (*p) {
	if (*p == MONEY) {
	    p++;
	    switch(*p) {

	    default:    /* ���������� ��� ��������� */
		*ptmp++ = MONEY;
		*ptmp++ = *p++;
		continue;
	    case 0:     /* ����� */
		break;
	    case 'k':   /* ��� ��������� ������� */
		strcpy(ptmp, lastkey);
		break;
	    case '@':   /* ��� ����� ����� ������� */
		nm_ptr = nmsubs(&itms[i][2], Csubs);
		strcpy(ptmp, nm_ptr);
		break;
	    case 'i':   /* ��� ��������� �����/�������� �������� */
		strcpy(ptmp, Crepf);
		break;
	    case 'o':   /* ��� �������� (�������) �����/�������� */
		strcpy(ptmp, Coutf);
		break;
	    case 'e':   /* ��� ��������� ������� (EDITOR=) */
		if ((s=getenv("EDITOR")) == NULL)
#ifdef DEMOS2_EDITOR
		    s = "re";   /* �������� ����� */
#else
		    s = "vi";   /* �������� UNIX */
#endif
		strcpy(ptmp, s);
		break;
	    case 'm':   /* ��� ��������� ���������, ������ more */
		if ((s=getenv("PAGER")) == NULL)
		    s = "more";
		strcpy(ptmp, s);
		break;
	    case '*':   /* ��� ����� �� ���������� �������� */
		fnsplit(ptmp, &itms[i][2], 0);
		break;
	    case 's':   /* ��������� ������� ����� �����, � ������ */
		fnsplit(ptmp, &itms[i][2], 1);
		break;
	    case 'S':   /* ��������� ������� ����� �����, ��� ����� */
		fnsplit(ptmp, &itms[i][2], 2);
		break;
#ifdef DEVELOPE
	    case 'n':   /* ����� ��� ��� ��������/�������������� */
	    case 'a':   /* ������ ������������ */
	    case 'b':   /* ������ ������������ */
#endif
	    }
	    p++;
	    while (*ptmp) ptmp++;
	    continue;
	}
	else
	    *ptmp++ = *p++;
    }
    *ptmp = '\0';       /* ����� ����������� */
}

/*ARGSUSED*/
int vexcmd(cmd, cmdlbl)
char *cmd;      /* "�����" �������, ��������� ����������� */
char *cmdlbl;   /* ������� ������ ������� */
{
	char    tmpcmd[140];

	if (cmd) {
	    /* ��������� ����������� */
	    cmdsub(tmpcmd, cmd, itm);
	    cmd = tmpcmd;
	}
	return( vshcmd(cmd, (char *)0) );
}

int
fsh(cmd)        /* ������� sh */
register char *cmd;
{
	if (*cmd == '\0')
		/* vshcmd ���������� ������ ������� */
		cmd = (char *)0;
	return(vexcmd(cmd, (char *)0));
}
int
fmenu2(file)        /* ���� �� ������ .ashmenu */
register char *file;
{
	char    cmdlbl[100];    /* ������� ������ ������� */
	char    tmpcmd[40];

	switch(menu2(tmpcmd, cmdlbl, file)) {
	case 1:
		/* ��������� ������� ������������ */
		vshcmd(tmpcmd, cmdlbl);
		/* �������������... */
	case 0:
		/* ������������ ��. ���� */
		return( 1 );
	}
	return( 0 );
}

int
ffile(cmd)
register char *cmd;
{
	char  file[140];

	cmdsub(file, cmd, itm);
	w_msg(TXT, tstat2(file));
	return( 0 );
}

int
fmsg(s)
register char *s;
{
	w_msg(TXT, s);
	return(0);
}

int
fmsgerr(s)
register char *s;
{
	w_msg(ERR, s);
	return(0);
}

int
kshow(s)
/*
 * �������� ��������� ��������� (��� �������)
 */
register char *s;
{
	char tmps[300];
	register int i, j;

	for (i=0; kt1[i].kt_key; i++) {
		sprintf(tmps, "kt1: %d:%d:'%s'",
		kt1[i].kt_ib,
		kt1[i].kt_ie,
		kt1[i].kt_key);
		cp_set((i%10)+2, 0, TXT); w_str(tmps); er_eol();
		if ( (i+1) % 10 ) ;
		else    { bell(); r_cod(0); er_pag(); }
	}
	for (i=0; kt2[i].kt_key; i++) {
		sprintf(tmps, "kt2: %d:%d:'%s'",
		kt2[i].kt_ib,
		kt2[i].kt_ie,
		kt2[i].kt_key);
		cp_set((i%10)+2, 0, TXT); w_str(tmps);
		if ( (i+1) % 10 ) ;
		else    { bell(); r_cod(0); er_pag(); }
	}
	return(1);
}
