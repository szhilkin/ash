/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: linebp.h,v 3.4 90/01/11 16:20:26 vsv Exp $
 *
 *      $Log:	linebp.h,v $
 * Revision 3.4  90/01/11  16:20:26  vsv
 * ���������� ������ ����������:
 * ���, ��� ������� � �������� ����������, 
 * �������� � ���������� linxhe
 * 
 * Revision 3.3  90/01/11  10:18:26  vsv
 * ������ V32
 * 
 * Revision 3.2  89/08/29  16:21:51  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.1  88/04/27  08:40:30  vsv
 * ���������� ��� ��������� 
 * ������� VISI
 * 
 * Revision 3.0  87/12/21  12:24:06  vsv
 * ��������������� ������.
 * 
 */

/*
** ��������� ������ ��� ������
** � �������� ���������� �������
*/

/*-----------------------------------*/
/* �������� ��� ���������� ��������� */
/*-----------------------------------*/
typedef struct {
	char    *in_name;        /* ��� ��� ������ �� ������� �������� */
	char    *in_addr;        /* ����� � ��������� */
	} IN_PORTS;

/*
 * ��������� ������ �������� �������� ��������
 */

typedef struct {
	int     lh_magic;       /* ������������� �������� �������� */
	int     lh_len;
	char    lh_name[6];
	int     lh_free[16];   /* ���� �� ������������ */
	unsigned lh_lines;
	unsigned lh_heaps;
	unsigned lh_names;
	} LINE_H;               /* ��������� �������� �������� */

#define  LH_MAGIC 000477

extern  char   *v_dir;          /* ������� VISI */
extern  char   *phelp0;         /* ��� ����� �� ����������� ���������� */
extern  IN_PORTS in_help[];     /* ����� ��� ����������. ���� */
