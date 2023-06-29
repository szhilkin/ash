/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin310.c,v 1.1 90/12/27 16:29:12 vsv Rel $
 *
 *      $Log:	lin310.c,v $
 * Revision 1.1  90/12/27  16:29:12  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.3  89/08/29  15:16:44  vsv
 * ������ LINLIB_3
 * 
 * Revision 3.2  88/07/27  16:34:40  vsv
 * ������ � �������� 'DEL' ��������� � ������ ����
 * 
 * Revision 3.1  88/06/27  15:20:48  vsv
 * ������� ������ RCS
 * 
 * Revision 3.0  87/12/21  12:19:24  vsv
 * ��������������� ������.
 * 
 */


#include <stdio.h>
#include "line.h"
#include "line0.h"

extern  int     ttyinp();
extern  KBF     kbf[];


/*------------------------------------------------------*/
/* ������� cod, ���� ������ �� �������, ���� ��� linlib */
/*------------------------------------------------------*/
kbcod  k_pars(cod)
int cod;
{
	register int n;
	register KBF *kbfp;
	register char *esccod;
	int  like;               /* ����: ���� ������� ������� */
	char kbuf[10];

	kbuf[0] = cod & 0177;
	kbuf[1] = 0;
	like = 1;               /* ���� � ����-�� ������ */
	for(n=1; like!=0; ) {

		like = 0;
		for(kbfp = kbf; kbfp->t_key; kbfp++) {
			if(strncmp(kbuf, esccod=kbfp->t_cap, n) == 0) {
				like = 1;
				if(strcmp(kbuf, esccod) == 0)
					return(kbfp->t_key);
				else
					continue;
			}
		}
		if(like == 0) {
			if(n == 1)
				return((kbcod)cod);
			else
				return( 0);     /* ������ �������� */
		}
		else {
			/* �������� ��� ������� ������������������ -
			 * ����� ���� ���������,
			 * ��������� � �������������� �� �����
			 * � �������� � � ����� ����������,
			 * ���� �������� ������� �����.
			 */
/*                      cod=ttyinp();           */
			kbuf[n++] = escseq(   ttyinp() );
			kbuf[n  ] = 0;
		}
	}
	return(KBCOD('O','O'));   /* �� ������ ������ ��� ������� */
}

/*---------------------*/
/* ������� ��� ������� */
/*---------------------*/
r_key()
{
	register kbcod cod ;
	register pars ;         /* k_pars() = */

	cod = ttyinp();
	if(cod == -1)
		return( -1);    /* ����������� ����� - ��� �������� */
	else
		cod &= 0377;    /* �������� �������� ���������� int=char */

	if(cod > 0200 && cod < 0300)
		cod &= 0177;    /* � ����� "�������" #,%,?, � �.�. */

	/* ���������-��������� ��� */
	if(cod != (pars=k_pars(cod)))
		return(pars);

	/* ��������� ������� � ���������� �������� ����� */
	else if(((cod < 0377)&&(cod > 0277))
	|| ((cod > 037)&&(cod < 0177)))
		return(cod);

	/* ASCII DEL */
	else if(cod == 0177)
		return(KBCOD('d','e'));

	else if(cod > 0 && cod < 040)   /* Nonprintable ASCII */
		return( KBCTL(cod + ('A'-'\001'))) ;
	else
		return(cod);   /* ��� ������� */
}
