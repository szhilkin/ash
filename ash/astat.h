/*
 * ������� ������ ��� ������������� ������.
 */
#ifndef TINYSMALL

#define PCMAX   600
#define KT2MAX  80
#define KT1MAX  200
#define BUFSMAX 4000

#else

#define PCMAX   200
#define KT2MAX  30
#define KT1MAX  60
#define BUFSMAX 2000

#endif /* SYSV */

/*
 * ��������� ������ ��� �������������� ��������� ������.
 */

/* ������� ������/�������� */
typedef struct {
	char *pc_pat;   /* ������ ������� */
	char *pc_cmd;   /* ����������� ������� */
} PATCMD;

/* ������� �������� ��������� */
typedef struct {
	char   *kt_key; /* ���� ������ */
	PATCMD *kt_tab; /* ����� ������� ������������� */
	int     kt_ib;  /* ������ ���. */
	int     kt_ie;  /* ������ ����� */
} KEYTAB;

/* ������� ���� ���������� ������� */
typedef struct {
	char   *ft_name;        /* ������ � ������ */
	int    (*ft_fun)();    /* ��������� �� ������� */
} FUNTAB;

extern  char *tstat1();
extern  char *tstat2();
