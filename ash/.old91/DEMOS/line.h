/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: line.h,v 3.7 90/01/11 16:19:10 vsv Exp $
 *
 *      $Log:	line.h,v $
 * Revision 3.7  90/01/11  16:19:10  vsv
 * ��������� �������� ��������� ������� linusr
 * 
 * Revision 3.6  90/01/11  10:18:00  vsv
 * ������ V32
 * 
 * Revision 3.5  89/08/29  16:21:26  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.4  88/08/02  10:50:28  vsv
 * �������� ���� ������ �������� � �������������� ���� (allcod).
 * 
 * Revision 3.3  88/05/17  11:09:34  vsv
 * ���������� ��������� ������ ��������.
 * 
 * Revision 3.2  88/04/27  11:08:55  vsv
 * maxli, maxco � ����� line.h, ��� ����� ������.
 * 
 * Revision 3.1  88/04/27  08:38:28  vsv
 * ���������� ��� ��������� 
 * ������� VISI
 * 
 * Revision 3.0  87/12/21  12:23:14  vsv
 * ��������������� ������.
 * 
 */
#ifndef line_h_def
#define line_h_def
#include <stdio.h>
/**** ������������ ������� (��������� ������ ���� �� ������ !!!)
****/

/*#define DEMOS2          /* DEMOS 2.0 (BSD 2.9 compatible) */
/*#define RT11            /* RT-11 V04.00 � ����� ������ ������ */

/* ����������� ����� � ���� ��� ������ */
#if defined(FILE)
extern  FILE *vttout;
extern  int   vtti;      /* ���������� ����� ��� ����� */
#endif

typedef int   kbcod;    /* ��� �������, �������� '^M', 'P1', 'k1' */
typedef char  bool;     /* �������� ������ ��� ����� �����, ������ */

/* ����������� � ����� ������� (���������� ���� kbcod) */
/* ��������� ��� pdp11, i8086, i80286, m68000 */
#define cod0(c) ((c) & 0xff)            /* �������� ������ ������ */
#define cod1(c) (((c)>>8) & 0xff)       /* --"---   ������ --"--- */
#define tocod0(c) ((c) & 0xff)          /* ������������ ������ --"--- */
#define tocod1(c) (((c)<<8) & 0xff00)   /* ---"---- ������ --"--- */
/* ������������ ��� ������� (kbcod �� ���� ���� char) */
#define KBCOD(c0, c1) (tocod0(c0)|tocod1(c1))

#define STRLEN 82       /* ����� ������ ��� ����� � ���������� */
#ifndef FALSE           /* ����������� �.���� � ��. ����� */
#define TRUE    1
#define FALSE   0
#endif


/*--------------------------------------*/
/* ������������� �������� ����� (�����) */
/*--------------------------------------*/
typedef struct  {
	bool    size;           /* ����� ������ ����, ���. ��������     */
	bool    line;           /* ����� ������ �� ������               */
	bool    colu;           /* ����� ������� (������� ���������)    */
	bool    flag;           /* ����� ��������                       */
	short   attr;           /* ����� ��������� ����� (���,�����,..) */
	int     *cvts;          /* ������ ������. ������. (sprintf)     */
	char    (*cvtf)();      /* ������� ����������. �������. ������. */
	int     (*test)();      /* ����� ������� ��������               */
	char    *varl;          /* ����� ����������, ��������� � ������ */
} LINE ;

/*---------------------------------------*/
/* ����������� ����� ��� ����� ��������� */
/*---------------------------------------*/

/*---- �������� ���������� ���� �������������� */
#define TXT 1       /* ����� ����������� (TEXT) */
#define HDR 2       /* ��������� (HEADER)  */
#define VAR 3       /* ���� ��� ����� (VARIABLE) */
#define ALT 4       /* ������������� (ALTERNATE) */
#define MSE 5       /* �������� ���� (MENU SELECTOR) */
#define ERR 6       /* ��������� �� ������ (ERROR) */
#define ATT 7       /* "�������� !"   (ATTENTION !) */

/*---- ����� ������� ����� ��������� */
#define PMT  0100000    /* ������� ��������� (PROMPTER) */
#define PAD   040000    /* ���� ��� ������ ����������� ������ ��������� */
#define MID   020000    /* ����� � ���� ������������� �� ������ */
#define INP   010000    /* ���� �������� ��� ����� */
#define NED    04000    /* �� ������������� ������ ����� */
#define EDT    02000    /* ����-������� �����. ��� �������������� */
#define FLO    06000    /* ����� ����. ������. � �������� ����������� */
#define VCOLOR 01000    /* ?!!... ������������ ������� ������� ��������� */
#define LFASTR  0400    /* r_line �������������� ������ ������ ������ ���� */

/* ����� ������� ������� �������������� � ���������: */
#define VIDEO  00177
#define VIDEOM (VIDEO|INP|VCOLOR)       /* ����� �������������� */

/*---- �������� ����� ������������ ����� �����: */
#define LTXT TXT
#define LHDR (HDR|MID|PAD)
#define LVAR (VAR|PAD)
#define LALT (ALT|MID|PAD|NED)
#define LMSE (MSE|FLO)

/*----------------------*/
/* ���� ������ �������� */
/*----------------------*/
#define SUST    0100    /* ������� �������: ���� ������ �� ����� */
	/* ��� ��������: */
#define SUSNL   0003    /* � ���������� ���� �� <CR><LF>   */
#define SUSAR   0034    /* �� ��������                     */
#define SUSUD   0014    /* ����� � ����                    */
#define SUSLR   0060    /* ������ � �����                  */
#define SUSU    0004    /* �����                           */
#define SUSD    0010    /* ����                            */
#define SUSL    0020    /* �����                           */
#define SUSR    0040    /* ������                          */

/*---------------------*/
/* ����������� ������� */
/*---------------------*/
#define KB_AU     KBCOD('A','U')
#define KB_AD     KBCOD('A','D')
#define KB_AL     KBCOD('A','L')
#define KB_AR     KBCOD('A','R')
#define KB_GL     KBCOD('G','L')
#define KB_HE     KBCOD('H','E')
#define KB_KP     KBCOD('K','P')
#define KB_PR     KBCOD('P','R')
#define KB_NL     KBCOD('N','L')
#define KB_DE     KBCOD('D','E')
#define KB_RE     KBCOD('R','E')

/*----------------------*/
/* ����� ������� io_set */
/*----------------------*/
#define IO_VIDEO  0000001       /* ��� ���������, ����� � ����� ��������� */
#define IO_TTYPE  0000002       /* ������� ����������� ����� */
#define IO_WAIT   0000004       /* next_j() ������� ������� ������� */
#define IO_NOWAIT 0000010       /* next_j() � ����������� ����� */

/*--------------------*/
/* ������� ���������� */
/*--------------------*/

/* lin(1) - ��������� ��������� � ������������ �������� */
extern  int     io_set(), ttyinp(), w_chr(), w_raw(), w_str(), w_strn();

/* lin(2) - ���������� ����������: ���������� ������� */
extern  int     at_set();
extern  kbcod   r_cod(), r_key();
extern  int     hw_set(), do_kbl();

/* lin(3) - ���������� ����������: ���������� ������� */

extern  int    maxli;   /* ������ ������ �� ����������� */
extern  int    maxco;   /* ������ �� ��������� */

extern  kbcod   k_pars();
extern  int     bell(), cp_fet(), cp_sav(), cp_set();
extern  int     er_eol(), er_eop(), er_pag(), er_scr();
extern  kbcod   e_str(), r_line(), r_page();
extern  int     allcod, edinff;
extern  int     w_line(), w_page();

/* lin(4) - ������� ������ ������������ */
extern  int     cvt_a(), cvt_sp(), cvt_lh(), cvt_hl(), tst_m();

extern  int     w_msg(), w_emsg(), w_help(), u_page();

#define er_page er_pag
#define r_str   e_str

#endif line_h_def
