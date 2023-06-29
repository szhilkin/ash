/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: line0.h,v 3.4 90/01/11 10:18:16 vsv Rel $
 *
 *      $Log:	line0.h,v $
 * Revision 3.4  90/01/11  10:18:16  vsv
 * ������ V32
 * 
 * Revision 3.3  89/08/29  16:21:42  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.2  88/04/27  11:10:28  vsv
 * maxli, maxco � ����� line.h, ��� ����� ������.
 * 
 * Revision 3.1  88/04/27  08:40:08  vsv
 * ���������� ��� ��������� 
 * ������� VISI
 * 
 */

/*
 * �������� ������������ ���������
 */

/*#define  TTY_FULL       /* ������ ����������� TERMCAP (������ ��� RT-11) */
			/* ��. ����� TRMCAP.C */

/* �������� ����� �������:
 */
typedef struct {
	kbcod   t_key;  /* ���. ��� ('ku') */
	char   *t_cap;  /* ESC-��� (:ku=\EA:) */
} KBF;  /* ���������� - ��� ������� � termcap */

typedef struct {
	kbcod   t_key;  /* ���. ��� ('ku') */
	kbcod   t_cod;  /* ���. ��� ('AU') */
	char   *t_knm;  /* �������� ������� ("�����") */
} KBL;  /* ���������� - ��������� ��� � �������� */
#define KBLSIZE 40

/* ��������� � ������������� */
typedef struct {
	int     lpa_p;  /* ��������� */
	int     lpa_a;  /* ����� ��������� */
} LPA;
#define LPASIZE 8

/* ������� ��������� ������
 */
typedef struct {
	bool    sc_li;  /* LIne - �����. ������ */
	bool    sc_co;  /* COlumn - �����. ������� � ������ */
	short   sc_at;  /* ATtributes - ������� ����� ��������� */
} SCREEN;

/* ����������� ������ �� �������� �� /etc/termcap;
 * ��� ���������� ��� ��������� ������ ���� ����� (lh= ... )
 */
extern char *tcapo[];

#define t_cm  tcapo[0]  /* ������ ��������� ������� */
#define t_cl  tcapo[1]  /* ����. ����� */
#define t_cd  tcapo[2]  /* ����� ������ */
#define t_ce  tcapo[3]  /* ����� ������ */
#define t_ks  tcapo[4]  /* ���/����. ���. ����. */
#define t_ke  tcapo[5]
#define t_cs  tcapo[6]  /* ��������� */
#define t_sr  tcapo[7]
#define t_sf  tcapo[8]
#define t_al  tcapo[9]  /* �����/����. ������ */
#define t_dl  tcapo[10]
#define t_so  tcapo[11] /* ��������� */
#define t_se  tcapo[12]
#define t_us  tcapo[13] /* ������������� */
#define t_ue  tcapo[14]
#define t_md  tcapo[15] /* ������� */
#define t_mr  tcapo[16] /* ������ */
#define t_mb  tcapo[17] /* ������� */
#define t_mk  tcapo[18] /* ����� */
#define t_me  tcapo[19] /* ��� ������ */
#define t_bl  tcapo[20] /* ������ */
#define t_vb  tcapo[21] /* ����������� */
#define t_lh  tcapo[22] /* ��� ���������� (���������� LINLIB) */

