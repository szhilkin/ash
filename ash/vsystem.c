#include <sys/ioctl.h>
#include <sys/param.h>
#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "assist.h"

#define ARGVMAX 500
#ifndef NCARGS 
#define NCARGS 4096
#endif 

extern  int     y0_top;
extern  char    pmtsh[];
extern  char  **environ;
extern  char    Csubs[];

extern  char  *nmsubs();


#define ASH_NOWAIT 001  /* �� ����� ���������� ����������� �������� */
#define ASH_NOFORK 010  /* �������� ��������� �������� */
#define ASH_NOSH   020  /* �� ��������� �������������� ������� sh */

/*
 * ��������� ���������.
 *
 * ������ ������ ��������, ��������.
 */
int vexec(argv0, argv, execmode)
char  *argv0;
char **argv;
{
	extern int errno;
	int pid, piddone;       /* ������ ��������� */
	int forked;             /* ����: ���� ����������� ������� */
	int syscod;
	void (*sigint)();
	/* ������� �� ������ ��� ���� ����������� SIG_IGN,
	   ����� ����������� ��� ����������, ����� �������������
	   ������� ���� ������ */
	sigint = signal( SIGINT, SIG_IGN);

	if (execmode & ASH_NOFORK) {
		forked = 0;
		pid = 0;
	}
	else {
		forked = 1;
		pid = fork();  /* vfork ������� ����� ������� exec */
	}
	if (pid < 0) {
		fprintf(stderr, "Can't fork\n");
		return(-1);
	}
	if (pid == 0) {
		/* �������-������� */
		if (execmode & ASH_NOWAIT) {
			signal( SIGINT, SIG_IGN);
			signal( SIGQUIT, SIG_DFL);
		}
		else {
			signal( SIGINT, SIG_DFL );
			signal( SIGQUIT, SIG_DFL );
		}
		/* ����� �������������� �����. ����� */
		/*...*/

		execvp(argv0, argv, environ);
		fprintf(stderr, "ASH Can't exec %s\n", argv0);
		if (forked)
			exit(1);
		else    {
			signal( SIGINT, sigint);
			return(-1);
		}
	}
	if (pid > 0) {
		/* �������-�������� */
		syscod = 0;
		if (execmode & ASH_NOWAIT)
			/*VARARGS*/
			printf("[%d] Started\n", pid);
		else
			do {
				piddone = wait( &syscod );
				if ( pid != piddone)
					printf("[%d] done\n", piddone);
			} while ( pid != piddone);

		signal( SIGINT, sigint );
		signal( SIGQUIT, SIG_IGN );
		return(syscod);
	}
#ifdef lint
	/*NOTREACHED*/
	return(0);
#endif
}

/*
 * �������� ��������� ������� �������.
 */
shstart()
{
	showtime( 0 );                  /* �������� ���� */
	cp_set(0, 0, TXT); er_eol();    /* �������� cwd */

	if (y0 > y0_top)
		cp_set(y0_top-1, 0, TXT);
	else
		cp_set(y0-1, 0, TXT);

	y0_top = y0;    /* ���������� ������� ������ */
	er_eop();

	io_set(IO_TTYPE);
}

extern  int     mark_i;
extern  int     mark_o;

/*ARGSUSED*/
int avexec(cmd, cmdlbl, execmode)
char *cmd;      /* ���������� �������, ������� ���� ��������� */
char *cmdlbl;   /* ������ ��� ���������, ��� ������� == cmd */
int  execmode;
{
	char *argv[ARGVMAX+1];
	register char *p;
	register int i;

	p = cmd; while(isspace(*p)) p++;
	i = 0;
	argv[i] = p;
	for (; *p; p++) {
		if(isspace(*p)) {
			/* ������� ��������� �������� */
			*p++ = '\0';
			while(*p && isspace(*p)) p++;
			i++; argv[i] = p;
		}
		if (i < ARGVMAX) continue;
		else break;
	}
	i++; argv[i] = (char *)0;
	return(vexec(argv[0], argv, execmode));
}

/*
 * ������������ ������� ��� /bin/sh.
 * ����������� ���������� ������.
 * ����� ������� ����� �����������.
 */
int shexec(cmd, cmdlbl, execmode, execapnd)
char *cmd;      /* ���������� �������, ������� ���� ��������� */
char *cmdlbl;   /* ������ ��� ���������, ��� ������� == cmd */
int  execmode;
int  execapnd;
{
	/* ������ ��� �����������:
	 * ����. ����� ���������� + ����� �������� "exec "
	 */
	char cmd2[NCARGS + 6];
	char *argv[5];
	int syscod;         /* ��� �������� ������� */
	int i;
	register int j;
	register char *p;
	char *nm_ptr;

	p = cmd;
	i = 0;
	if ( execapnd ) {
		/*VARARGS*/
		sprintf(cmd2, "exec ");
		i += 5;
	}
	/* ���������� ������� �����. ����� */
	if (mark_i >= 0) {
		strcat(&cmd2[i], "<");
		while(cmd2[++i]);
		cmdsub(&cmd2[i], "#@", mark_i);
		strcat(&cmd2[i], " ");
		while(cmd2[++i]);
		itms[mark_i][0] = ' '; mark_i = -1;
/*                while(cmd2[++i]);     */
	}
	/* ���������� ��������� */
	while( *p ) {
		if (*p == MONEY && p[1] == MONEY) {
			p++; p++;
			/* ���������� */
			for (j = 0; j < itmmax; j++) {
	  /* �������� ������������ ����� ��������������� */
				if (i >= (NCARGS-80)) {
					fprintf(stderr, " *** %s ***\n",
					"������������ ����������� ����");
					return(-1);
				}
				if(itms[j][0] == MONEY) {
				   /* �������� ��������� ��� */
				   nm_ptr = nmsubs(&itms[j][2], Csubs);
				   sprintf(&cmd2[i], "%s ", nm_ptr);
				   while(cmd2[++i]);
#ifdef RETRO
				   i--; cmd2[i++] = ' ';
#endif
				}
			}
			/* ������ ��������� ������ - �� ������ */
			cmd2[--i] = '\0';
		}
		else    cmd2[i++] = *p++;
	}
	cmd2[i] = 0;
	/* ���������� ������� �����. ������ */
	if (mark_o >= 0) {
/*                sprintf(&cmd2[i], " >%s", &itms[mark_o][2]);  */
		strcat(&cmd2[i], " >");
		while(cmd2[++i]);
		cmdsub(&cmd2[i], "#@", mark_o);

		itms[mark_o][0] = ' '; mark_o = -1;
	}

	/* �������� ����������� �������: */
	if (cmdlbl == (char *)0) {
		cmdlbl = cmd2;  /* ���� ������� � �������� */
	}
	printf("%s %s", pmtsh, cmdlbl);
	if (execmode & ASH_NOWAIT)
		putchar('&');
	putchar('\n');

	fflush(stdout);

	/* ��������� ������� */
	if (execmode & ASH_NOSH)
		syscod = avexec(cmd2, cmdlbl, execmode);
	else {
		argv[0] = "sh";
		argv[1] = "-c";
		argv[2] = cmd2;
		argv[3] = (char *)0;

		syscod = vexec(argv[0], argv, execmode);
	}
	return(syscod);
}

/*
 * ������� ���������� ������.
 *
 * ������ ��������� ������, ����� ������ � ������� �������.
 */
int vsystem(cmd, cmdlbl)
char *cmd;      /* ���������� �������, ������� ���� ��������� */
char *cmdlbl;   /* ������ ��� ���������, ��� ������� == cmd */
{
	int execmode;   /* ����� ������� ������� */
	int execapnd;   /* ����: �������� ������� ������� "exec " */
	int execargv;   /* ����: ��������� ������� ��� sh */
	register char *p;
	register char *cmdp;
	int syscod;

	execmode = 0;

	execargv = execapnd = 1;
	if (strncmp(cmd, "exec ", 5) == 0) {
		execmode |= ASH_NOFORK;
		p = cmdp = &cmd[5];
	}
	else
		p = cmdp = cmd;

	for (; *p; p++) {
		switch(*p) {
		default: break;
		case '&':
			if (p[1] == '\0') {
				/* ������� ������, ���������� ������� */
				execmode |= ASH_NOWAIT;
				*p = '\0'; --p;
				while (p > cmdp && isspace(*p)) {
					*p = '\0'; --p;
				}
				continue;
			}
		/* ����������� ��������� */
		case '|': case ';':
		case '(': case ')':
		case '{': case '}':
			execapnd = 0;
			/* �������������... */

		/* ������ ����������� sh */
		case '[': case ']':
		case '<': case '>':
		case '*': case '@': case '?':
		case '!': case '$': case '^':
		case '\'': case '"': case '`':
			execargv = 0;
			break;
		case MONEY:
			if (p[1] == MONEY) {
				p++; p++;
				continue;
			}
			execargv = 0;
			break;
		}
	}
	if (mark_i >= 0 || mark_o >= 0)
		execargv = 0;
	if (execargv)
		execapnd = 0;
	execmode |= (execargv ? ASH_NOSH : 0);
	shstart();
	syscod = shexec(cmdp, cmdlbl, execmode, execapnd);
	fprintf(vttout, " "); fflush(vttout);
	io_set(IO_VIDEO);
	return(syscod);
}
