#include <sys/types.h>
#if !defined(u_short)
#define         u_short ushort
#endif

#include <sys/stat.h>
#define R_OK    4
#define W_OK    2
#define X_OK    1

#include <string.h>
#include <stdio.h>
#include <ediag.h>

#ifndef SYSV
#ifdef pdp11
#include <ndir.h>       /* BSD 4.2 & DEMOS/P stubs */
#else
#include <dirent.h>
#define SYSV
#endif
#else
#include <dirent.h>
#endif /* SYSV directory maint */

#include "line.h"
#include "assist.h"
#include "astat.h"

extern  char  Crepf[];
extern  char  Cfill[];

extern  int errno;

#define ITMCNM 255      /* must be in "assist.h" */

/*NOXSTR*/
char    itmcnm[ITMCNM+1] =      /* Current item's name (saved) */
			"..";
int     itmci = -1;             /* Current item's index */
/*  YESXSTR*/

struct  stat cwdstat;

char    cwdpath[400];   /* CURRENT working directory full name */
char    lwdpath[400];   /* LAST working directory full name */

int rescan()
{
/*     cwdstat.st_ino = (ino_t)0;   */
    cwdstat.st_mtime = (time_t)0L;
    return(1);
}

/*
 * ������� ������ ���� ������� (rwx)
 */
char *
rwxmode(stp)
struct stat *stp;
{
	static  char rwxs[4];
	u_short umode;  /* �������� ����� ������� */
	unsigned short userid;  /* ������������� ������������ */


	userid = getuid();

	umode = stp->st_mode;
	if (userid) { /* �� �������� ������������������ */
	    if      (stp->st_uid == userid);
/*          else if (stp->st_gid == getgid())   */
	    else if (gidchk(stp->st_gid))
		    umode <<= 3;
	    else    umode <<= 6;
	}
	/* assistant shell, ������ ���������� �� root,
	 * ������� �� �����, ������� �������� user ��� ����.
	 * ������ � ����� �� ����������. (c) by vsv     ;-)
	 */
	rwxs[0] = ((umode & S_IREAD) ? 'r' : '-');
	rwxs[1] = ((umode & S_IWRITE)? 'w' : '-');
	rwxs[2] = ((umode & S_IEXEC) ? 'x' : '-');
	rwxs[3] = 0;
	return(rwxs);
}

/*
 * ������� ������������ ������ ���������
 * ��� ������������� ������.
 *
 * ������ ������:
 * tstat1():
 *      "rwx +" ����� ��������, ������� �������.
 * tstat2():
 *      "rwx f filename"  ����� �����, ��� ���, ��� �����.
 */

#ifdef RT11
#define TSTATS  40      /* �. ���� ���� � ������... */
#else
#define TSTATS  400
#endif
static  char    tstats[TSTATS];         /* ������ ��� ��������� ������� */

char *
tstat1()
{
	register int i;

	strcpy(tstats, rwxmode(&cwdstat));
	strcat(tstats, " -");
	/* �������� ������� ������� */
	for (i=0; i < itmmax; i++)
		if (*itms[i] == MONEY) {
			tstats[4] = '+'; break;
		}
	return(tstats);
}

char *
tstat2(fname)
char *fname;            /* ��� ����� �� ���� */
{
	int   staterr;
	struct stat itmstat;
	u_short     mode;       /* ����� ������� � ��� */
#ifdef RETRO
	char  ftypestr[4];      /* ��������� � ����� ����� */
#endif
	register char ftype;    /* ������ ���� ����� */

#if defined(S_IFLNK)
	if ((staterr = stat(fname, &itmstat)) < 0) {
	       staterr = lstat(fname, &itmstat);
	}
#else
	staterr = stat(fname, &itmstat);
#endif
	/* ���� �� ������� �������� �� ���������� ������... */
	/* ��� ��� � ���� */
	if (staterr == 0) {
	    /* �������� ��� ����� */
	    mode = itmstat.st_mode;
	    switch(mode & S_IFMT) {
#if defined(S_IFLNK)
	    case S_IFLNK:   ftype = 'l'; break;
#endif
#if defined(S_IFIFO)
	    case S_IFIFO:   ftype = 'p'; break;
#endif
	    case S_IFCHR:   ftype = 'c'; break;
	    case S_IFBLK:   ftype = 'b'; break;
	    case S_IFDIR:   ftype = 'd'; break;

	    case S_IFREG:
			    if (itmstat.st_size == (off_t)0L)
				ftype = 'E';
			    else
				ftype = filetype(fname);
			    break;
	    default:
			    ftype = '*';
			    break;
	    }
	    sprintf(tstats, "%3.3s %c ", rwxmode(&itmstat), ftype);
	}
	else {
	    sprintf(tstats, "??? ! ");
	}
	/* ����� ������ ����������� tstats �.���� ������ TSTATS !?... */
	strncat(tstats, fname, TSTATS-7);       /* !?... */
	tstats[TSTATS-1] = '\0';
	return(tstats);
}

itmrestor()
{
	char    itmnnm[ITMCNM+1];   /* ����� ��� */
	register int i;
	int dofs;       /* delta offset increment */

	/*
	 * ������������ ����������� ��������� �������
	 */
	for(itm = 0; itm < itmmax; itm++) {
	    i = itm;
	    cmdsub(itmnnm, "#@", itm);
	    if (strcmp(itmcnm, itmnnm) == 0) {
		goto adjust;
	    }
	}
	if (itmci >= 0) {
	    if (itmci < itmmax)
		itm = itmci;    /* ������������ ������ */
	    else
		itm = itmmax - 1;
	}
	else    {
	    /* ������������ �� ����������� */
	    itmofs = itm = 0;
	    return;
	}
adjust:
	/* ��� �������� � ����, ����
	 * ���������� ��������� ��������� ���� */
	if (itm >= itmofs && itm <  (itmofs + (xx*yy)))
	    return;
	if (yy <= 0 || xx <= 0) {
		w_emsg("yy or xx are bad:");
		printf("yy = %d, yy = %d", yy, xx);
	}
	dofs = (xx == 1 ? yy : (yy*(xx/2)));


	/* ��������� ����� ��������� ���� */
	for(itmofs = 0;    ;itmofs += dofs)
	    if (itm >= itmofs && itm <  (itmofs + (xx*yy)))
		return;
#ifdef RETRO
	/* ������������ �� ����������� */
	itmofs = itm = 0;
#endif
}

itmlwd()
{
	register int cwdlen;     /* ����� ����� �������� �������� */
	register char *p;

	cwdlen = strlen(cwdpath);

	/*
	 * ��������� ������ ������ ��������, ��
	 * �������� ���������.
	 */

	if (prefix(cwdpath, lwdpath)) { /* ���� ����������� ����� */
		if (cwdlen == 1 || lwdpath[cwdlen++] == '/') {
			p = itmcnm;
			while (lwdpath[cwdlen] && lwdpath[cwdlen] != '/')
				*p++ = lwdpath[cwdlen++];
#ifdef RETRO
			*p++ = '/';
#endif
			*p = '\0';
		}
	}
	else
		strcpy(itmcnm, "..");
}

int
scomp(p1, p2)
register char **p1;
register char **p2;
{
	/* ���������� ���� ������ ����� items, ������� ���� ��� ����� */
	return(strcmp( (*p1)+2, (*p2)+2));
}

vls()
/*
 * ���������� ������� ls.
 * ��������� ������, ���������� ����. ����� ������
 */
{
    DIR *dirp;
#ifdef  SYSV
    struct dirent *dp;
#else
    struct direct *dp;
#endif
    register char *itmbp;
    short len;
    int     Lflag;  /* ����: �� ��������� ����. ������ */
    int     Fflag;  /* ����: ���������� ��� ����� */
    int     aflag;  /* ����: ���������� ��� ����� */
    struct  stat sb;
    char    *fname;
    char    ftype;  /* ������ ���� ����� */
    int     ok;

    Lflag = Fflag = aflag = 0;
    if (index(Cfill, 'F')) Fflag = 1;
#if defined(S_IFLNK)
    if (index(Cfill, 'L')) Lflag = 1;
#endif
    if (index(Cfill, 'a')) aflag = 1;

    len = itmlen = itmmax = 0;
    itms[itmmax] = itmbp = itmbuf;
    *itmbp++ = ' ';

    if ((dirp = opendir(Crepf)) != NULL) {
      for (dp = readdir(dirp); dp != NULL; dp = readdir(dirp))
	{
/*              printf("%6ld %s\n", dp->d_ino, dp->d_name);
 */
			/* ����� ���� �� ��� ���������� (� ���), ���
			   ��������� ��������� len */
		len = strlen(dp->d_name);

		if ( !aflag && dp->d_name[0] == '.'
			    && strcmp(dp->d_name,"..") != 0)
			/* ������� ����� */
			continue;

		/* ���� ����� �� ������� */
		if (&itmbuf[itmbsz] <= &itmbp[len]) {
			w_emsg(ediag("No mem for all menu items",
				     "��� ����� ��� ���� ������� ����"));
			break;
		}
		/* ��������� ��� ������� � ������� */
		fname = dp->d_name;

		/* ���������� ��� ����� */
#if defined(S_IFLNK)
		if (Fflag|Lflag) {
			if(Lflag) {
				ok =  stat(fname, &sb);
				if (ok < 0)
				/* ������ �������� ���� �������� ���������� */
				       ok = lstat(fname, &sb);
			}
			else    ok = lstat(fname, &sb);
#else
		if (Fflag) {
			ok =  stat(fname, &sb);
#endif
			if (ok < 0) {
				if (!Lflag) {
#if defined (S_IFLNK)
					if (lstat(fname, &sb) < 0) {
					       w_emsg("Can't stat:");
					       w_str(fname);
					}
#else
					w_emsg("Can't stat:");
					w_str(fname);
#endif
				}
				else    {
#ifdef RETRO
					itmbp -= len;
#endif
					continue;
				}
			}
			switch(sb.st_mode & S_IFMT) {
#if defined(S_IFLNK)
			case S_IFLNK:   ftype = '@'; break;
#endif
#if defined(S_IFIFO)
			case S_IFIFO:   ftype = '='; break;
#endif
			case S_IFCHR:   ftype = '"'; break;
			case S_IFBLK:   ftype = ':'; break;
			case S_IFDIR:   ftype = '/'; break;
			case S_IFREG:
				if ((sb.st_mode & S_IEXEC) == S_IEXEC)
					ftype = '*';
				else    ftype = ' ';
				break;
			default:        ftype = '?'; break;
			}
			*itmbp++ = ftype;
		}
		else    *itmbp++ = ' ';
		/* ��������� ��� ������� � ������� */
		strcpy(itmbp, fname);
		itmbp += len;
		*itmbp++ = '\0';
		if ( len > itmlen ) itmlen = len;
		if (itmmax >= ITMMAX)
			break;  /* �� ����� � ������ �������� */
/*              if ((itmmax % 10) == 0) {
			w_chr('#'); fflush(vttout);
		}
 */
		itmmax++;
		itms[itmmax] = itmbp;
		*itmbp++ = ' ';
	}
        closedir(dirp);
    }
    else {
	return(1); /* ERROR filling main menu */
    }
    *itmbp++ = '\0';
    if (itmmax == 0) {
	    strcpy(itmbuf, " /..");
	    len = 4;
	    itmmax++;
    }
    if ( len > itmlen ) itmlen = len;
    itmlen++;
/*  w_str("sort..."); fflush(vttout);
 */
    qsort(itms, itmmax, sizeof(char *), scomp);
/*  w_str("done"); fflush(vttout);
 */
    return(0);  /* OK */
}

cwdshow()
/* �������� ���. ������� � ��. ��������������� ���������� */
{
	char tmpstr[140];
	register int i;
	int     showli;     /* ������ ������ */
	int     deltco;     /* ���� � ��������� ������ ������, �� == 1 */
	extern char *getwd();
	extern char *getenv();
	extern int mailf2;  /* ��. chckmail */
	int userlen;
	char *ashlbl;


	mailf2 = 1;

	if (panelf) {
	    showli = 0;         deltco = 0;
	} else {
	    showli = maxli-1;   deltco = 1;
	}
	if ((ashlbl=getenv("ASHLBL")) == (char *)0) {
#ifdef SYSV
		ashlbl = getenv("LOGNAME");
#else
		ashlbl = getenv("USER");
#endif
	}
	if (ashlbl == (char *)0)
		ashlbl = "Unknown USER";

	sprintf(tmpstr,
	"%s%s", ashlbl, (getuid() == 0 ? "(su)" : ""));

	cp_set(showli, 0, HDR);      /* ����� ������ */
	w_str(tmpstr); userlen = strlen(tmpstr);

	if (Crepf[0] != '\0') {
		sprintf(tmpstr,
		" %s <%s>", Crepf, rwxmode(&cwdstat));
	} else {
		strcpy(tmpstr, " * ");
	}
	userlen += strlen(tmpstr);
	at_set(ATT);
	w_str(tmpstr);

#ifdef  pdp11
	if (cwdpath[0] == '\0' && getwd(cwdpath) == 0) {
		fprintf(stderr, "getwd() == 0\n");
		fatal();
	}
#endif
#ifdef SYSV
	if (cwdpath[0] == '\0') {
		FILE *wdf;
		char *s;
		if ((wdf=popen("/bin/pwd", "r")) == NULL) {
			fprintf(stderr, "Can't exec /bin/pwd\n");
			fatal();
		}
		fgets(cwdpath, 400, wdf);
		for (s=cwdpath; *s != '\n'; s++) ; *s = '\0';
		pclose(wdf);
	}
#endif
	sprintf(tmpstr, "[ %s ]", cwdpath);
	cp_set(showli, userlen, TXT); er_eol();
	i = strlen(tmpstr);
	i += deltco;
	if (i > (maxco-userlen)) {
		cp_set(showli, userlen + 1, HDR);
		w_str(&tmpstr[i - maxco + userlen + 1 + deltco]);
	}
	else {
		cp_set(showli, maxco - i - deltco, HDR);
		w_str(tmpstr);
	}
#ifdef DEBUG
	sprintf(tmpstr, "0x%07lx", (unsigned long)vf);
	cp_set(-2, 0, TXT); w_str(tmpstr);
#endif
}

/*
 * ��������� ������� ����.
 * ������� ��� ���������� ������� � Cfill,
 * ���� ���. � ':', ������ ���������� ������� ls.
 * ���� �������� ���� �� ������, ������� ������� ����������
 * ����������� ������.
 *
 * ���������� 1, ���� ��������� ����� ����, ����� 0.
 */
fil_vf(newflag)
int newflag;    /* ���� 0, �� ������ �������� ������� */
{
	struct  stat newstat;
	FILE *fpls;
	int samedir;    /* ����: ��� �� ������� */

	if (Crepf[0] == '\0' && Cfill[0] != ':') {
		samedir = 0;
		/*
		 * ���� ��������� �����������
		 * ������ �������� ����
		 */
		cwdstat.st_ino = (ino_t)0;
	}
	else {
		stat(Crepf, &newstat);
		/* ���� �� �������� �������� �� ���������� ������ stat... */

		samedir = (cwdstat.st_dev   == newstat.st_dev
			&& cwdstat.st_ino   == newstat.st_ino);

		if (newflag == 0)
			if (cwdstat.st_mtime == newstat.st_mtime && samedir) {
				cwdstat = newstat;
				return(0);
			}
		cwdstat = newstat;
	}
#ifdef  RETRO
	cwdshow();      /* � �������� ����� ��� ������... */
#endif
	if (samedir) {
		/* ��������� ������� ����� � ���������� ������ ���� */
		itmci = itm;
		strcpy(itmcnm, &itms[itm][2]);
/*                cmdsub(itmcnm, "#@", itm);     */
	}
	else    {
		itmci = -1;
		itmlwd();
	}
	if (itmbuf != (char *)0)
		free(itmbuf);
	/* ����� � ������������� ��������� ������ itmbsz */
	if ((itmbuf=malloc(itmbsz+1)) == (char *)0) {
		w_emsg("��� ������ ��� �������� ������...");
		onintr(1);
	}

	w_msg(TXT, Cfill); fflush(vttout);

	if (Cfill[0] != ':') { /* ���������� ������� */
	/*NOSTRICT*/
		if (Cfill[0] == '\0'
		||  (fpls = popen(Cfill, "r")) == NULL) {
			fprintf(stderr,
"\n������ ��� ���������� �������� ����('%s') : %d\n", Cfill, errno);
			fatal();
		}
		vfread(fpls);   /* ������� ������� */
		pclose(fpls);
	}
	else {
	    if ( vls() ) {        /* ���������� ������� */
		w_emsg("Can't read '.' - use cd manually");
		return(0);
	    }
	}
	w_emsg("");

	itmini();       /* ��������� �������� ���� */
	itmrestor();
	pre_vf();       /* ������� �������� LINLIB ��� ���� */
	return(1);
}

/*
 * ������� �������, ������ �����������, ��������� cwd, lwd;
 */
vchdir(cdarg)
char *cdarg;
{
	char  nwdpath[400];     /* ����� ������� */

	if (*cdarg != '/') {
		strcpy(nwdpath, cwdpath);
		strcat(nwdpath, "/");
		strcat(nwdpath, cdarg);
	}
	else
		strcpy(nwdpath, cdarg);

	dcanon(nwdpath);        /* ������ ����� �� ������ ����� */

	if (access(nwdpath, R_OK|X_OK)) {
		w_emsg("���������� �������:");
		w_str(nwdpath);
		return(-1);
	}
	if (chdir(nwdpath) < 0) {
		w_emsg(
		"�� ���� ������� ������� ��");
		w_str(nwdpath);
		return(-1);
	}
	strcpy(lwdpath, cwdpath);
	strcpy(cwdpath, nwdpath);
	return(0);
}
