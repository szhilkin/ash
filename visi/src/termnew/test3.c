/*
 * test [linlib]
 *
 * 19.05.87     ���������������� �������
 * 20.05.87     �������� ������������ ������� �����-������
 *
 */

#include <stdio.h>
#include "line.h"

#include "line0.h"

extern TCAP tcap;

main()
{
	register int cod;
	register i;

	hw_set();
	io_set();
	er_pag();
	diag();
/*      for(i=0; i<5; i++) {
		cod = r_key();
		printf("\007-------------->%c%c<-------------\n",
		cod & 0377, (cod >> 8)& 0377 );
	}
*/      io_off();

	exit(0);
}

/*
**      diagonal with '*'
**
*/

diag()
{
	int cod;
	register i, j, k;

	for(j=0; j<80-24; j++) {   /* column */
		for(k=j,i=0; i<24; i++,j++) {   /* row */
			switch(cod=r_key()) {
			case PF2 :
				cp_set(1,1,TXT); er_eop();
				printf("PF2");
				break;
			case PF3 :
				cp_set(1,10,TXT); er_eol();
				printf("PF3");
				break;
			case -1 :
				cp_set(1,20,TXT); printf("  -1       ");
				break;
			default :
				cp_set(1,20,INP);
				printf("default %c%c",
				cod & 0377, (cod >> 8)& 0377);
				break;
			}
			cp_set(j, i, TXT);
			putchar('*');
		}
		j=k;
	}
}
