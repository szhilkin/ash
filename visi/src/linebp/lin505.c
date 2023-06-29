/*
**      +----------+    ���������� �����-������
**     (c) linlib  !    ��� ���������-��������
**      +----------+    ���������������
**/

/*
 *      $Header: lin505.c,v 1.1 90/12/27 16:25:29 vsv Rel $
 *
 *      $Log:	lin505.c,v $
 * Revision 1.1  90/12/27  16:25:29  vsv
 * ������ V3_3
 * 
 */

#include <stdio.h>
#include "line.h"
#include "linebp.h"

extern  char *phelp0;

extern  IN_PORTS in_help[];

extern char   *vexdir;  /* ������� ���������� �������� ������� */

w_help(page)
/*--------------------------------*/
/* �������� ���������� ���������� */
/*--------------------------------*/
char    *page;
{
    char *pages[3];
    LINE *phelp;
    register int i;

    pages[1] = page;
    pages[2] = phelp0;

    for (i = 1; i> 0; ) {
	if (i > 2)  i = 1;
	er_pag();
	w_msg(INP|MSE, " ���� ���������");
	fprintf(vttout, " #%d ", i);

	if(pages[i]) {
	    if(phelp=b_page(vexdir, pages[i], in_help)) {
		w_page(phelp);
		d_page(phelp);
		cp_set(-1, 0, TXT);
		switch(r_cod(0)) {

		case KB_HE:
			i++;
		case KB_RE:
			continue;
		default:
			i = 0;
		}
	    } else {
		fprintf(vttout, " �� ������ ");
		fflush(vttout);
		if ( ++i == 3 )   return(FALSE);
	    }
	}
    }
    er_pag();
    w_msg(TXT, "");
    return(TRUE);
}
