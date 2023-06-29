
/* �������� ��������� ���������� ������ ���� */

#ifdef TINYSMALL
	/* pdp11 & xenix286 small model */
#define ITMMAX  400     /* ����. ���������� ������� ���� */
#define ITMLEN  16      /* ����� ������ ������ ���� */
#define ITMBUF  2048    /* ������ ������ ������ ���� �� ��������� */

#else
	/* normal 32-bit commputers */
#define ITMMAX  1200     /* ����. ���������� ������� ���� */
#define ITMLEN  16      /* ����� ������ ������ ���� */
#define ITMBUF  16384   /* ������ ������ ������ ���� �� ��������� */

#endif
#define BEGLI 11        /* ������ ������ ���� �� ������ (???) */

#define MONEY '#'       /* ���� ��� ����������� � �������� */

extern  char *malloc();
extern  char *calloc();
extern  char *getenv();

/*
 * ��������� ������ ���� ���� ������ � ��. ����.
 *
 */
typedef struct {

char   **_itms;         /* ����� ������� ���������� �� ������ ���� */
char    *_itmbuf;       /* ����� ����� ������� ���� */
int      _itmbsz;       /* ������ ������ ����� */

LINE    *_vf;            /* ��������� �� �������� ���� */
LINE    *_ltmpl;         /* ������ ��� ���������� �������� */
int     _xx1;            /* ��������� ���������� �������� (xx1 >= 1) */

int     _itm;            /* ������ ���������� �������� ������ ���� */
int     _itmofs;         /* �������� ���� (������) */
/* ��������� ��� ��������� ������������ ��� ������ ������ ������� ���� */
int     _itmlen;         /* ����. ����� ������ ������ ���� */
int     _itmmax;         /* ���������� ������� ���� */

int     _yy_max;         /* ����������� ����������� ���������� ����� ���� */
int     _yy;    /* ���������� ������� ���� � ������� */
int     _xx;    /* ���������� �������� */
int     _dx;    /* ���������� ����� ������� ��������� �������� �������� */
int     _x0;             /* ������� ������� ������� */
int     _y0;             /* ������� ������ ������ */
		/* �������� ��� �������� ����� ����: */
int     _ofsx;           /* �����/������ */
int     _ofsy;           /* �����/���� */
} LINEMENU;

extern LINEMENU clm;    /* ������� ���� */

#define itms        clm._itms
#define itmbuf      clm._itmbuf
#define itmbsz      clm._itmbsz
#define vf          clm._vf
#define ltmpl       clm._ltmpl
#define xx1         clm._xx1
#define itm         clm._itm
#define itmofs      clm._itmofs
#define itmlen      clm._itmlen
#define itmmax      clm._itmmax
#define yy_max      clm._yy_max
#define yy          clm._yy
#define xx          clm._xx
#define dx          clm._dx
#define x0          clm._x0
#define y0          clm._y0
#define ofsx        clm._ofsx
#define ofsy        clm._ofsy

extern  char   *yesno[];
extern  int     scrolf, oneitm, panelf, histf, clockf, cmailf, loginf;
extern  char   *homedir;
extern  char   *cwd;
extern  int     t_file();

extern  int     maxli, maxco;
