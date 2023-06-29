#include "line.h"

extern int fastmenu;  /* ������� �����: �� ������ �������� ������,
			 �� � ������� ����� ���� */

static
int onmseline(cline)    /* ��������: ������ ����� �� ����� "����� ����"? */
register LINE *cline;
{
	if ((cline->attr & (INP|LMSE)) == (INP|LMSE))
		return( 1 );
	return( 0 );
}

LINE *mseline(linem, cline, cod)
/*-------------------------------------------*/
/* ������� ��������� �� ����� ��������� ���� */
/* � ����������� ������� ������� ������      */
/*-------------------------------------------*/
	 LINE *linem;    /* ��� �������� */
register LINE *cline;    /* ������� ����� �������� */
	 kbcod cod;      /* ��� ������� ������� */
{
	int   i;
	LINE *cline0;


	if ( ((fastmenu && cod >= '0') || ( !fastmenu && cod >= '1'))
	   && cod <= '9' && onmseline(cline)) {

		if (fastmenu && cod == '0') {
			if( !unr_c(KB_EX) )
				bell();
			return(cline);
		}
		w_line(cline0 = cline);
		for (cline = linem, i = '0'; cline->size; cline++) {
			if (onmseline(cline))
				i++;
			if (cod == i) { /* �������! */
				if( fastmenu && !unr_c(' ') )
					w_emsg("LINLIB: unr_c ERROR in mseline");
				return(cline);
			}
		}
		return(cline0);
	}
	return(cline);
}
