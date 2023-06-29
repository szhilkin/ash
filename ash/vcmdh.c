/*
 * ������ � ������� ��������� �����.
 * ����� ��������� �������, ������������
 * ������ ���������� �������.
 */

#include <string.h>
#include <stdio.h>
#include "line.h"       /* ����-��������� LINLIB */
#include "assist.h"

#ifdef TINYSMALL
#define CMDB 512        /* ������ ������ ������ */
#define CMDP 40         /* ���������� ������ � ������ */
#define CMDHL 8         /* ���������� ������ � ���� �� ������ */
#else
#define CMDB 8096       /* ��� ������ ������������� ������... */
#define CMDP 60         /* ���������� ������ � ������ */
#define CMDHL 8         /* ���������� ������ � ���� �� ������ */
#endif /* TINISMALL */

static  char    cmdb[CMDB+1];   /* ����� ������ */
static  char   *cmdp[CMDP+1];   /* ��������� �� ������� */
static  int     cmdplast = 0;   /* ������ ��������� ������� */
static  int     cmdpi = 0;      /* ������ ��������� ������/�����. ������� */
static  int     cmdbot = 0;     /* ������ ���������� ����� � ������ */

int cmddel(cmd)
/*
 * ������ �� ������
 * ������������ ����� ��������� �������.
 */
char *cmd;
{
	register int i;
	int saven;      /* ������ ��������� ��������� ������� */
	int delsize;    /* ������ ��������� ������� */

/***
	if (*cmd == '\0')
		return(-1);
 ***/
	for (i = 0; cmdp[i]; i++)
		if (strcmp(cmd, cmdp[i]) == 0) {
			delsize = strlen(cmd) + 1;
			/* ��������� ���������� ������ � ����� ����� */
			for (saven = i + 1; saven < cmdplast; i++,saven++) {
				cmdp[i] = cmdp[saven] - delsize;
				strcpy(cmdp[i], cmdp[saven]);
			}
			cmdbot -= delsize;
			saven = i; cmdplast--;
			while (i <= CMDP)
				cmdp[i++] = 0;
			return(saven);
		}
	return(-1);
}

cmdsqz(reqsz)
/*
 * ���������� ������ ������� � ����� */
/**/
int     reqsz;  /* ������, ������� ��������� �������� ��������� */
{
	register int i;
	int     realsz; /* ������������� ������ */
	int     saven;  /* ������� ������ ����� ������ ����������� ������� */

	/* ���� ����� ����, ������ �� ������ */
	if ((reqsz < (CMDB - cmdbot)) && cmdplast < CMDP)
		return;

	/* ��������� ������� ��������� ����� */
	i = realsz = 0;
	do {
		realsz += strlen(cmdp[i++]) + 1;
	} while (*cmdp[i] && ((CMDB - cmdbot) + realsz) < reqsz);
	saven = i;

	/* ����� �� ���������, ���� ������ ������ �� ������ ���� �����: */
	/* ���������, ������� �� ������������... */
	if (((CMDB - cmdbot) + realsz) < reqsz)
		/* ��������� ��� ���������� ������ */
		i = cmdbot = cmdpi = 0;
	else {
		/* ��������� ���������� ������ � ����� ����� */
		for (i = 0; saven < cmdplast; i++,saven++) {
			cmdp[i] = cmdp[saven] - realsz;
			strcpy(cmdp[i], cmdp[saven]);
		}
		cmdbot -= realsz;
	}
	cmdplast = i;
	if (cmdpi > cmdplast)   /* �� ��������� � ������� ��������!!! */
		cmdplast = i;
	while (i <= CMDP)       /* �������� ��������� ��������� */
		cmdp[i++] = 0;
}

int cmdput(cmd)
/*
 * �������� � �����
 */
char *cmd;
{
	int newsize;
	char *p;

	if (*cmd == 0)          /* ������ �� ������ */
		return(0);
	cmddel(cmd);           /* ������ ������ ����� */
	newsize = strlen(cmd) + 1;
	cmdsqz(newsize);

	/* �������� ����� ������� */
	p = cmdb;
	p += cmdbot;
	cmdp[cmdplast] = p;
	strcpy(cmdp[cmdplast], cmd);

	cmdplast++;
	cmdpi = cmdplast;       /* �������������� ������� */
	cmdbot += newsize;

	return(-1);
}

int cmdprv(cmd)
/*
 * ����� ���������� ������� �� ������
 */
char *cmd;
{
	if (cmdpi > 0) cmdpi--;
	else    return(0);
	if (cmdp[cmdpi])
		strcpy(cmd, cmdp[cmdpi]);
	return(1);
}

int cmdnxt(cmd)
/*
 * ����� ��������� ������� �� ������
 */
char *cmd;
{
	if (cmdp[cmdpi] && cmdplast > cmdpi)
		cmdpi++;
	if (cmdp[cmdpi]) {
		strcpy(cmd, cmdp[cmdpi]);
		return(1);
	}
	else    *cmd = 0;
	return(0);
}

/*
 * ��������� ������� (����� ������) � ���� $HOME/.ashhist
 * � ����� �������� �� ����.
 * ��� ��� ������������ ���������� ������������� �� main()
 */
char *hfile = "/.ashhist";

cmdghist(hdp)
char *hdp;
{
	FILE *fp;
	char filename[200];
	char cmd[140];
	int c;
	register char *p;

	strcpy(filename, hdp);
	strcat(filename, hfile);

	if ((fp = fopen(filename, "r")) == NULL)
		return(0);
	p = cmd;
	while ((c = getc(fp)) != EOF) {
		if ((*p++ = c) == '\n') {
			*(--p) = '\0';
			cmdput(cmd);
			p = cmd;
		}
	}
	fclose(fp);
	return(1);
}

cmdphist(hdp)
char *hdp;
{
	char filename[200];
	FILE *fp;
	register char **pp;

	strcpy(filename, hdp);
	strcat(filename, hfile);

	if ((fp = fopen(filename, "w")) == NULL)
		return(0);

	for (pp = cmdp; *pp != (char *)0; pp++)
		/*VARARGS*/
		fprintf(fp, "%s\n", *pp);

	fclose(fp);
	return(1);
}

static  char *cmdpp;

int t_hist(line, cod)
/*
 * ���� ��� ����� ������ ������� ������
 */
register LINE *line;
kbcod cod;
{
	char *cmd;

	cmd = *(char **)(line->varl);

	switch(cod) {
	case ' ':
		/* �������� ������� � ������� ����� */
		strcat(cmdpp, cmd);
		break;
	case KB_NL:
		/* ����������� ������� � ������� ����� */
		strcpy(cmdpp, cmd);
		break;
	case KB_DE:
		/* ������ ������� �� ������ */
		if (cmdplast > 2 ) {
			cmddel(cmd);
			clritm();
			itmmax = cmdplast;
/***
			if (itm < cmdplast);
			else    itm = cmdplast - 1;
			if (itm > 1)
				itm -= 1;
 ***/
			pre_vf();
			itmshow();
			w_page(vf, 0);
		}
		else
			bell();
		break;
	}
	return (TRUE);
}

h_menu()
/*
 * ������ � ���� ������ ������� ������.
 */
{
	register unsigned i;
	kbcod cod;

	/* �������������� ����� �� ������ */
	cp_set(y0, 0, TXT);
	er_eop();
	w_cmd(cmdpp);   /* �������� ������� �� ������ */
	itmshow();
	w_page(vf, 0);

	for ( ;; ) {

		i = itm - itmofs;
		cod = r_line( &vf[i], 0 );

		w_emsg("");

		switch (cod) {

		default:
			break;
		case KB_HE:      /* ������� */
			cp_set(-1, 0, TXT);
			printf("������� # %2d �� %2d, %4d ���� (%2d%%)",
			itm, cmdplast, cmdbot, (100*cmdbot)/CMDB);
			break;
		case KB_EX:      /* ����� */
		case ' ':       /* �������� */
		case KB_NL:      /* �������� */
			return;

		case KB_RE:      /* ����������� */
			er_pag();
			cwdshow();
			w_emsg("");
			itmshow(); w_page(vf, 0);
			break;
#ifdef RETRO
		case KB_DE:
			if (itm - itmofs < 0)
				cod = KB_AU;
			else    break;
			/* �������������... */
#else
		case KB_DE:
			cod = KB_AU;
			/* �������������... */
#endif
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
{ 16, 0, 0, 0,
       TXT|INP|PMT|NED|LFASTR,
	       0,
		       cvt_sp,
		       t_hist,
			       (char **)0 };

cmdvew(cmd)
char  *cmd;
{
	extern int  y0_top;     /* ���������� � vshcmd */
	extern char pmtsh[];    /* --"-- */
	LINEMENU savelm;

	if (cmdplast <= 1) {
		bell(); return;
	}
	savelm = clm;
	cp_set(y0-1, 0, TXT);   /* ��������� ������, ��. ���� */
	er_eop();

	/* ������������� ���� ������ */
	itms   = cmdp;          /* ��������� �� ������ ������ */
	itmmax = cmdplast;      /* ��������� ������� � ������� */
	vf     = (LINE *)0;
	itmlen = maxco - ((strlen(pmtsh)) * 2);
	ltmpl  = &tmplate;

	itm    = cmdpi;         /* ������� ������� */
	if (cmdpi < cmdplast);
	else         itm -= 1;
	yy_max = 10;
	itmofs = 0;
	while((itm - itmofs) >= yy_max)
		itmofs += yy_max;
	itmini();
	pre_vf();

	/* ��������� ������ */
	if (y0_top > y0) {
		y0_top = y0;
		scrlnl();
	}
	cmdpp = cmd;    /* ��� ����������� ����� ������� */

	h_menu();

	free((char *)vf); vf = (LINE *)0;
	cp_set(y0_top, 0, TXT); er_eop();

	cmdpi = itm;    /* ����� ����. ������� ������� */
	clm = savelm;
}
