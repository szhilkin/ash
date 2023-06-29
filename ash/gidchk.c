/*
 * by vsv@st.simbirsk.su
 *
 * �������� �������������� ������������ � ������
 *
 * ������������ getgroups(3p)
 */

#ifdef MGROUPS

/* ����� ������ ������� ��� ������, �������������� POSIX ��� BSD */

#include <grp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/* ����. ���������� ������������� ��������������� ������ */
#ifdef NGROUPS_MAX
#define MAXGID_ASH NGROUPS_MAX
#else
#define MAXGID_ASH 17
#endif

#ifdef Fbsd
#define __386BSD__
#endif

gid_t gid;
#ifdef __386BSD__
int   gidset[MAXGID_ASH];  /* ������, ����� ���� ����, @#%$ */
#else
gid_t gidset[MAXGID_ASH];  /* ��� ������ ���� */
#endif

int
gidchk(fgid)
int fgid;
{
/*  int group_ok;       /* ���� ��������� ���������� ������ */
    int maxgid;         /* ��, ��� ���������� getgroups() */
/*  int rc;             /* return code */
    int g;

    maxgid = getgroups(MAXGID_ASH, gidset);

/* ��� ����� ���������� ��� ��������, ���� ��������� ��� ���� � ssx.c
    if (maxgid < 0) {
	perror("ERR: NGROUPS_MAX limit exsided\n");
	exit(-1);
    }
    else {
*/
	/* � ����� ������ ��������� �� ������ ����� */
	for (g = 0; g < maxgid; g++) {
	    if (gidset[g] == fgid)
		return(1);
	}
/*
    }
*/
    return(0);
}

#else

int gidchk(fgid)
int fgid;
{
    if (fgid == getgid()) return(1);
    else                  return(0);
}
#endif /* MGROUPS */
