#include <string.h>
#include <stdio.h>
#include "line.h"       /* ����-��������� LINLIB */
#include "assist.h"

extern  char *getenv();

extern  int     y0_top; /* defined in main.c */

/*NOXSTR*/
char    pmtsh[20] = "sh>" ;
/*YESXSTR*/
int pmtshsz;            /* ������ ��������� */

static  char cmd0[140] = "";    /* ������� ������� �������� */
static  int  cmdsize = 0;       /* ����. ������ ������ */
static  int  pos = 0;           /* ������� � ������ */

/*
 * ������� w_cmd ���������� � ���� ������
 * � � ������ ������ � ��������
 */
w_cmd(cmd)
register char *cmd;
{
	register int i;

	cp_set(-1, 0, TXT); er_eol();
	at_set(TXT|INP);
	w_str(pmtsh);
	cp_set(-1, pmtshsz, TXT);
	for(i = 0; cmd[i] && i < cmdsize; i++)
		w_chr(cmd[i]);
}

scrlst()        /* ������ � ������ ������ */
{
	cp_set(y0_top, 0, TXT); er_eop();
	cp_set(y0_top - 1, 0, TXT);
	fflush(vttout);
}

vshcmd(cmd, cmdlbl)
/*
 * ��������� ������� /bin/sh
 */
char *cmd;      /* ������� ��� ���������� */
char *cmdlbl;   /* ������� ��� ������ ������ ������� */
{
	kbcod cod;
	int syscod;             /* ��� �������� system */
	int cmdrun;             /* ����: ������� ����������� */
	int justrun;            /* ����: ���������, �� ������������� */

	cmdrun = 0;
	pmtshsz = strlen(pmtsh) + 1;
	cmdsize = maxco - 1 - pmtshsz;
	justrun = 0;
	cod = 0;

	if (cmd == (char *)0) {
	    /* ������������ ��������� ������� */
	    ;
	}
	else {
		if (*cmd == ':') {
			justrun = 1;
			cod = KB_NL;
			/* �������� ������� ��������� */
			cmd++;
			strcpy(cmd0, cmd);
		}
		else {
			/* �����. ������ ��, ��� �� ������ */
			strncpy(cmd0, cmd, cmdsize);
			cmd0[cmdsize] = 0;
		}
		pos = strlen(cmd0);
	}

	for (;;) {
	    if ( !justrun ) {
		/* �������������� ��������� ������ */
		switch(cod) {
		case KB_HE:
		case KB_AU:
		case KB_AD:
			pos = strlen(cmd0);     /* ������ � ����� */
		default:
			w_cmd(cmd0);
		}
		cod = re_str((char *)&cmd0[0], cmdsize, 0, &pos);
	    }
		switch(cod) {
		default:
			bell();
			continue;
		case KB_HE:
			cmdvew(cmd0);
			/* ��� �������������� �������� ����: */
			cmdrun = 1;
			break;
		case KB_AU:
			/* ����. ������� */
			if ( !cmdprv(cmd0) )
				bell();
			break;
		case KB_AD:
			/* ��������� ������� */
			if ( !cmdnxt(cmd0) )
				bell();
			break;
		case KB_NL :
			if (cmd == (char *)0) {
				/* ��� strncmp ���������� */
				cmd = "";
			}
			/* ��������� �������, ���� �� ������������� */
			if (strncmp(cmd, cmd0, strlen(cmd0)) != 0) {
				cmdput(cmd0);
				/* ��� ����.������� ����� ���������: */
				cmd = "";
			}
			/* ����� ������? */
			if(strcmp(cmd0, "exit")==0)
				ashexit(0);
			/* �������� ������� */
			if (strcmp(cmd0, "cd") == 0) {
				char    *homedir;
				if ((homedir=getenv("HOME")) == NULL) {
					w_emsg(
					"HOME=... �� ������");
					return(0);
				}
				if (vchdir(homedir) < 0) {
					return(0);
				}
				goto fil_cd;
			}

			/* ������� � �������? */
			if (strncmp(cmd0, "cd ", 3) == 0) {
				if (index(cmd0, ';'))
					goto std_shell;

				if (vchdir(&cmd0[3]) < 0) {
					return(0);
				}
				else    {
fil_cd:
					if (y0 < y0_top)
					     y0_top = y0;
					/* ��� ���� ���������� ���� */
					return(1);
				}
			}
std_shell:
			/*-----------------*/
			/* ������� ������� */
			/*-----------------*/

			cmdrun = 1;

			syscod = vsystem(cmd0, cmdlbl);
			justrun = 0;
			if(syscod) {
				at_set(TXT|INP);
				printf(" [ Exit (%d/%d) ]\r",
				cod1(syscod), cod0(syscod));
			}
			else {
				at_set(TXT|INP);
				printf(" [ ok ]\r");
			}
			at_set(TXT);
			do {
				cod = r_cod(0);
				at_set(ATT|INP);
				/*VARARGS*/
				if (cod == KB_NL)
				 printf(" press SPACE bar or type command ");
				at_set(TXT);
				er_eol();
				/*VARARGS*/
				printf("\r");
			} while (cod == KB_NL);

			/* �������� ���������� ������� sh */
			er_eol(); fflush(vttout);
			scrlnl();
			showtime( 1 );          /* ���� �������� */

			switch (cod) {
			case KB_AL:
			case KB_AR:
			case KB_AD:
			case KB_AU:
					continue;
			/* ������� � ���� ���� ������ */
			case ' ':       return(1);
			case KB_EX:      return(0);
			default:
				if (cod1(cod) == 0) {
					/* ���� ����� ������� */
					cmd0[0] = cod;
					cmd0[1] = '\0';
					pos = 1;
					continue;
				}
				goto fil_cd;
			}       /* ����� �������� ���������� ������� sh */

		case KB_EX:
			w_msg(TXT, " ");
			w_msg(TXT, "");
			if (cmdrun) {
				scrlst(); return(1);
			}
			return(0);      /* ������ �� ������� */
		}
	}
}
