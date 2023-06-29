#include <sys/types.h>
#include <sys/stat.h>

#include <ediag.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "assist.h"

/*----------------------*/
/* ����� �������, misc. */
/*----------------------*/

/*------*/
/* ���� */
/*------*/
static time_t prevtime = (time_t)0;
static int nxtjflag;

showtime(on)
/* ���������� ��������� ����� � ��. ����������� ������� */
{
	nxtjflag = on;
	if ( !on && clockf) {
		cp_set(1, maxco-8, TXT);
		fprintf(vttout, "%8.8s", "");
	}
}

showclck()
/*
 * �������� ��������� ����.
 */
{
	time_t curtime;
	struct tm *tp;
	struct tm *localtime();

	if (clockf) {

		time(&curtime);
		if (curtime != prevtime) {
			prevtime = curtime;

			tp = localtime(&prevtime);

#ifndef CP_SAV
			cp_sav();
#endif
			cp_set(1, maxco-8, ATT);
			fprintf(vttout, "%02d:%02d'%02d",
			tp->tm_hour, tp->tm_min, tp->tm_sec);
#ifndef CP_SAV
			cp_fet();
#endif
		}
	}
}

/* #ifndef SVR4         */
/* #define MAIL_DUMB    */
/* #endif               */

#ifndef CP_SAV
#define CURSOR_SAVE     cp_sav()
#define CURSOR_FETCH    cp_fet()
#else
#define CURSOR_SAVE
#define CURSOR_FETCH
#endif

int mailf2 = 0; /* ������ ��������� �������� �����? */
chckmail()
/*
 * ��������� ������� ����� �����
 */
{
    extern char *getenv();
    static time_t mchktime = (time_t)0; /* ��� �����. ���� ������ �����*/
    static time_t mboxtime;             /* ���� ������ ����� �� stat */
    struct stat mailstat;
    char *mailbox;
    kbcod cod;

    if (cmailf) {
	if (!mailf2) return;

	if ((mailbox = getenv("MAIL")) == (char *)0)
		return;

	if (stat(mailbox, &mailstat) < 0)
		return;

	mboxtime = mailstat.st_mtime;

	if (mailstat.st_size != 0L) {
#ifdef MAIL_DUMB
		if (mchktime == (time_t)0) {
			mchktime = mboxtime;    /* ���� ��� ��� ������ */
		}

#else
		/* �������������� �����, ��� �������� */
		if (mailstat.st_mtime > mailstat.st_atime) {
#endif
			if (mchktime != mboxtime) {
			    mchktime = mboxtime;
			    CURSOR_SAVE;
			    cp_set(-2, -11, ERR); bell();
			    w_str(" New Mail ");
			    CURSOR_FETCH;
			}
			else {
			    /* ���� ������������� ����� */
			    CURSOR_SAVE;
			    cp_set(-2, -7, ATT);
			    w_str(" mail ");
			    CURSOR_FETCH;
			}
#ifndef MAIL_DUMB
		}
#endif
	}
    }
    mailf2 = 0;     /* ��������� � 1 ������ cwdshow() */
}

/*
 * ���������� ����������� ��������.
 */
next_j()
{
	if (nxtjflag) {
#ifdef CP_SAV
		cp_sav();
#endif
		showclck();
		chckmail();
#ifdef CP_SAV
		cp_fet();
#endif
	}
}

clritm()
/* ���������� ����� �� ������ ��� ���� */
{
    register int i;
    register LINE *l;

    for (l = vf,i = 0; l->size && i < yy; l++,i++) {
	cp_set(l->line, l->colu, TXT); er_eol();
    }
}

scrlnl()
/*
 * to scroll with newlines
 * ���������� ������ �� ������ ��� ������ ��������� ������
 */
{
	register int i;

	if (scrolf) {
		io_set(IO_TTYPE);
		for (i = y0; i < maxli; i++)
			putc('\n', vttout);
		io_set(IO_VIDEO);
	}
	else    er_pag();
}

fatal()
{
	cp_set(-1, 0, TXT);
	io_set(IO_TTYPE);
	fprintf(stderr, "FATAL ERROR - exit\n");
	exit(1);
}

itmshow()
/* �������� ��������� "����" */
{
	int showco; /* ������� ���. ������ ��������� ���� */
	register int i;

	/* �������� ��������� ���� */
	if (yy*xx < itmmax) {  /* ���� �� ��� ����� �� ������... */
		/* ��������������... */
		showco = (maxco - (itmmax/yy))/2;
		if (showco < 0) showco = 0;

		cp_set(maxli - 2, showco, TXT);
		for (i = 0; i < itmmax; i += yy) {
			if (i >= itmofs && i < itmofs+(yy*xx))
				w_chr('*');
			else    w_chr('-');
		}
	}
}

/*
 * �������� ������ ���� "*.c", "???*", "*.[ch]" � �.�.
 * �� ������� ��������.
 * ��������� ���������� � ������������� �� �������� ������.
 */
int patcmp(pat, str)
register char *pat;
register char *str;
{
	register int whileok;

	if (*pat == 0) return (1);
#ifdef RETRO
	if (*str == 0) return (0);
#endif
	for ( ; *pat; str++) {
		/* ����� ��������� ������ */
		if (*pat == *str) {
			pat++; continue;
		}
		switch (*pat++) {
		case '?':
		/* ���� ������������ ������ */
			if (*str) continue;
			break;
		case '[':
		/* ������ �� ������ ��� ��������� */
			whileok = 0;
			while(*pat && *pat != ']') {
				if (pat[1] == '-') {
				/* �������� ����������� ���� */
				    if (pat[2] && pat[2] != ']'
					&& *str >= *pat
					&& *str <= pat[2])
					whileok = 1;
				    pat++;pat++;pat++;
				}
				else if (*pat++ == *str)
					/* ����� ������ �� ������ */
					whileok = 1;
			}
			if (whileok && *pat == ']') {
				pat++; continue;
			}
			break;
		case '*':
		/* ����� ���������� ����� �������� */
			if (*pat == 0) return(1);
			while(*str)
				if (patcmp(pat, str++))
					return(1);
			break;
		} /* ����� switch */
		return(0);
	}
	if (*str == 0)
		return(1);
	return(0);
}

/*
 * ���� ������ � ����������, � ��������� ������ ������.
 */
kbcod
pmtrstr(pmtstr, str, size)
char *pmtstr;   /* ������ ��������� */
char *str;      /* ������ ��� ����� */
int   size;     /* ������ ������ ��� ����� */
{
	kbcod cod;
	LINE  pattl;
	int   savedf;

	pattl.size = size;
	pattl.colu = strlen(pmtstr) + 3;
	pattl.line = maxli - 1;
	pattl.flag = 0;
	pattl.attr = LVAR|INP;
	/*NOSTRICT*/
	pattl.cvts = (char *)0;
	/*NOSTRICT*/
	pattl.cvtf = (void *)0;  /* ��� ��������� �� �������, �����. int */
	/*NOSTRICT*/
	pattl.test = (void *)0;  /* ��� ��������� �� �������, �����. int */
	pattl.varl = str;

	for ( ;; ) {
		w_msg(TXT, pmtstr);
		savedf = edinff;
		edinff = 0;     /* �� ���������� ��������� ��������� */
		cod = r_line(&pattl, 0);
		edinff = savedf;
		switch (cod) {
		default:
			continue;
		case KB_NL:
		case KB_EX:
			return(cod);
		}
	}
}

/*
 * ������� ��� ����� �� ����������.
 *
 * ��� ����� from ���������� � �������� ������ out.
 * ����� �������� ���������� sufmode:
 * sufmode == 0         ���������� ��� �� ���������� ��������;
 * sufmode == 1         ���������� ������� � ������;
 * sufmode == 2         ���������� ������� ��� �����.
 */
fnsplit(to, from, sufmode)
	 char *to;      /* ���� ���������� */
register char *from;    /* ������ ���������� */
int sufmode;            /* ����� �������� */
{
	register char *s;
	register char *saveds;  /* ��������� �� ��������� ����� � ������ */

	*to = '\0';
	saveds = (char *)0;
	for (s = from; *s; s++)
		if (*s == '.' )
			saveds = s;
	if (saveds == from || saveds[1] == '\0')
		saveds = (char *)0;
	if (sufmode) {
		/* ����������� ������� */
		if (s = saveds) {
			/* ���� ������� ��� ������ */
			if (sufmode == 2) s++;
			strcpy(to, s);
		}
		return;
	}
	else {
		/* ����������� ��� �� ����� �������� */
		for (s = from; *s && s != saveds; )
			*to++ = *s++;
		*to = '\0';
	}
}

/*
 * ���������� ����� ������, ������������ � ��������� -@
 *
 * ���� ����������� ��� ��������:
 * -@3          ������ ����, ����� ���� �������� ������
 * -@20-32      ������� ���� ������ � ������ ����� ���������� ���������
 */

static char out_str[200] = "";
char *nmsubs(inps, s)
char *inps;
char *s;        /* ������ ��� ����������� */
{
	int a,b;
	register int i;
	register char *p;
	char tmps[20];

	while (isspace(*s)) s++;
	if (*s == '\0') goto no_subs;

	/* ������ ����� */
	p = tmps;
	while (isdigit(*s)) *p++ = *s++;
	*p = '\0';
	a = atoi(tmps);

	while (isspace(*s)) s++;
	if (*s == '\0') {
		/* ���������� ����� ����� �� inps */
		while(a>1) {
			while (   isspace(*inps) ) inps++;
			while ( ! isspace(*inps) ) inps++;
			a -= 1;
		}
		while (   isspace(*inps) ) inps++;
		p = out_str;
		while (isspace(*inps) == 0) *p++ = *inps++;
		*p = '\0';
	}
	else if (*s++ == '-') {
		while (isspace(*s)) s++;
		/* ������ ����� */
		p = tmps;
		while (isdigit(*s)) *p++ = *s++;
		*p = '\0';
		b = atoi(tmps);

		/* ���������� �������� (�������) */
		p = out_str;
		for (i=a; i < b && isspace(inps[i]); i++)
			/* ���������� ���������� ������� */
			;
		for (   ; i < b; i++)
			*p++ = inps[i];
		*p = '\0';
	}
	else {

no_subs:
		strcpy(out_str, inps);
	}
	return(out_str);
}
