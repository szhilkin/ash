/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *  New version for POSIX termios.h.
 */

#include <stdio.h>
#include "line.h"

#ifdef USE_TERMIOS
#include <signal.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <termios.h>
/* #ifndef UW_20 */
/* #include <sys/filio.h> */
/* #endif */

/*
 *  ���������� �������� �������� ���������
 */

static struct termios old;
static struct termios new;  /* terminal i/o status */

int osgflg = 0;         /* ����� �������� (old.sg_flags)???????? */

/*
 * ������� �� ������ ����������
 */
static v_off();
static void onintr()
{
	/* ��� ������� ����������� ��� ������� �� SIG_QUIT � ��� exit */
	signal(SIGINT,SIG_IGN);
	v_off(); exit(0);
}

/*------------------------------*/
/* ����������� ����� - �������� */
/*------------------------------*/
static int v_off()
{
	static   void     (*fsig)();

	fflush(vttout);         /* ������� ��������� ����� */

	/* ������� ����������� ������� �� ������ SIG_INT */
	fsig = signal(SIGINT,SIG_IGN);
	if( fsig == &onintr ) {
		fsig = SIG_DFL;
	}
	signal( SIGINT, fsig );

#if 0
	ioctl(vtti, TCSETS, &old);
#endif
	/* tcsetattr(vtti, TCSADRAIN, &old); */
	return (tcsetattr(vtti, TCSANOW, &old));
}

/*----------------------------*/
/* ��������� ��������� ������ */
/*----------------------------*/
static int v_on()
{
	static   void       (*fsig)();
	cc_t i;

#if 0
	ioctl(vtti,TCGETS,&old);        /* get prev tty modes */
#endif
	/* get prev tty modes */
	tcgetattr(vtti, &old);
	new = old;
	new.c_oflag = (new.c_oflag&(~OPOST));
	new.c_lflag = ISIG;

	/* special character processing */
/*        new.c_cc[VINTR  ] = CNUL; */
/*        new.c_cc[VQUIT  ] = CNUL; */
	for ( i = 0; i <= NCCS; i++)
	    new.c_cc[i] = CNUL;
	new.c_cc[VSTART] = old.c_cc[VSTART];
	new.c_cc[VSTOP] = old.c_cc[VSTOP];
	new.c_cc[VMIN ] = 1;    /* check, may be VMIN=VTIME=0 is better ? */
	new.c_cc[VTIME] = 5;

	fsig = signal(SIGINT,SIG_IGN);
	if( fsig == SIG_DFL )   {
		fsig = &onintr;
		signal( SIGINT, fsig );
	}

#if 0
	ioctl(vtti, TCSETS, &new);
#endif
	/* tcsetattr(vtti, TCSADRAIN, &new); */
	fflush(vttout);
	return(tcsetattr(vtti, TCSANOW, &new));
}

/*-----------------------------------------------------*/
/* �������� �� ���������� �������� � ������ ���������� */
/*-----------------------------------------------------*/
ttytst()
{
	static long num_r;
#ifdef FIONREAD
	ioctl(vtti, FIONREAD, &num_r);
#else
#ifndef FIORDCHK
#define FIORDCHK        (('f'<<8)|3)            /* V7 */
#endif
	ioctl(vtti, FIORDCHK, &num_r);
#endif
	return( num_r == 0l );
}

/*--------------*/
/* ���� ������� */
/*--------------*/
/* ��� �������� ����� ��������� ����������� ����,
 * � ���� ������ ������ �������� ��������, ��
 * �� ��� ����������, ��� ���������� ����������� �� ���,
 * ��������� ��������������� �� ���������
 * �� ����� ���������� �������.
 *      P.S.
 *      ��������� ����� ��������� ���� �� ���� �������
 *      ��� ���, �� � �� ��������� ���� �� ������ -
 *      ����� ��������� �������� ������������ ��� �������.
 */

int     cyrflg = 0;     /* ��������� ������ ������ ��������� */

static  int cyrkbd()
{
/* ��������� �������� ��� ������������� */
}

static int kb_nwt = 0;  /* �� ������� ���� � ���������� -
			 * ��� ������ ������� ���������
			 * ����������� �������.
			 * �� ��������� - ���� ��������� */
/*
 * ������� �������� ������, ��� �������
 * ����������� ������� ����������� � ����, ���� �����
 */
ttyinp()
{
	char         cc;
	register int c;

	next_j();       /* ���� ��� ����� ������ ������� ������� */
	cyrkbd();
	fflush(vttout);

	if(kb_nwt) {
		while( ttytst() ) { /* ���� ������� ������ */
			next_j();       /* ���������� ������������ */
			cyrkbd();
			fflush(vttout);
		}
		if (read(vtti, &cc, 1) < 0) exit(2);
		c = cc;
		goto ret_c;     /* ������� ��� */
	} else {

		if (read(vtti, &cc, 1) < 0) exit(1);
		c = cc;
		goto ret_c;
ret_c:
		c &= 0377;
		if(c > 0177)    cyrflg = 1;
		else            cyrflg = 0;
		return( c );
	}
}

/*------------------------------------------------*/
/* ���������� �������� ������������� �����-������ */
/*------------------------------------------------*/

extern int k_pad();     /* ��. lin215.c */

io_set(flags)
register int flags;
{
	/* ������������ keypad ���������� ������ ��-��
	 * ������������� ��������������� termcap ��� vt100, xterm,
	 * � ������� ������� �� ��������� ��������� ��������
	 * ������ ��� ���������� keypad
	 */
	if(flags & IO_VIDEO)    {
		fflush(vttout);
		if (v_on()) {
			perror("io_set(v_on)");
			v_on();
		}
		k_pad(1);
	}
	if(flags & IO_TTYPE)    {
		k_pad(0);
		fflush(vttout);
		if (v_off()) {
			perror("io_set(v_off)");
			v_off();
		}
	}
	if(flags & IO_WAIT)
				kb_nwt = 0;
	if(flags & IO_NOWAIT)
				kb_nwt = 1;
}

#endif  /* USE_TERMIOS */
