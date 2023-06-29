#include <ediag.h>
#include <stdio.h>      /* ��������� ����������� ���������� ��/��� */
#include "line.h"       /* ����-��������� LINLIB */

#ifdef JCLsuspend

#include <signal.h>

#if !defined(SIGTSTP)
v_susp()
{
       w_emsg("JCL are not supported, sorry...");
       return(FALSE);
}

#else /* SIGTSTP */

extern int getpgrp();   /* ������: cc ... -ljobs */

/*
       #ifndef BSD
       #ifdef SYSV
       extern int killpg();
       #endif
       #endif
*/

v_susp()
/*
 * Suspend for ash.
 * Job control (JCL) optionally required.
 *
 *      BSD 2.*, BSD 4.*, ����� 2.*, ISC UNIX (386/ix) V2.2
 */
{
	char *getenv();
	register char *env;
	int     pid;

	/* �������� �� csh ����� ������� � �� ������� */
	if ((env = getenv("SHELL")) != NULL) {
	    if( strcmp("/bin/csh", env) == 0 ) {

		pid = getpid();
		scrlst();       /* ������ � ������ ������ */
		io_set(IO_TTYPE);
		putchar('\n');
#ifdef  INFOSUSPEND
		printf(ediag("To continue use 'fg' or %%\n",
			 " ��� ����������� ����������� ������� fg ��� %%\n"));
#endif
		kill(pid, SIGTSTP);  /* ���������� ������ ash */
		io_set(IO_VIDEO);
		scrlnl();       /* ������ ���������� */
		return( TRUE );
	    }
	}
	w_emsg(ediag("This is NOT a subshell of /bin/csh",
		     "�� ����, ��� �� ��������� � /bin/csh"));
	return(FALSE);
}

#endif /* SIGTSTP */
#endif /* JCLsuspend */

#ifndef JCLsuspend
p_susp()
{
       w_emsg("SUSPEND are NOT implemented...Sorry");
       return(FALSE);
}
#endif
