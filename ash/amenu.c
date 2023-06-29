#include <stdio.h>
#include "line.h"
#include "line0.h"
#include "assist.h"

/*
 * ������ � ����.
 *
 */

pre_vf()
/*--------------------------------*/
/* ��������� �������� ���� ������ */
/*--------------------------------*/
{
	register int i;
	register int j;
	int yyxx, x0x;
	unsigned msize, pagesize;

	if (vf != (LINE *)0)
	    free((char *)vf);

	msize = ((yy * xx) + 1) * sizeof(LINE);
#ifdef BSDpagesize
	pagesize = getpagesize();
	if (msize < pagesize) {
	    msize = pagesize;
	}
#endif
#ifndef RETRO
	if ((vf=(LINE *)malloc(msize)) == (LINE *)0) {
#else
	if ((vf=(LINE *)calloc((yy * xx)+1, sizeof(LINE))) == (LINE *)0) {
#endif
		/*VARARGS*/
		fprintf(stderr, "No memory for vf[]\n");
		fatal();
	}
	yyxx = yy * xx;
	i = x0x = 0;
	while( i < yyxx && i+itmofs < itmmax) {
		for (j = 0; j < yy && i < yyxx && i+itmofs < itmmax; j++) {

			vf[i] = *ltmpl; /* ����������� ������ */
			vf[i].colu = x0 + x0x; /* ((i / yy) * dx); */
			vf[i].line = y0 + j;   /* (i % yy); */
			vf[i].size = dx;
			/* ���� ������ ������� ��� ���������� ������ */
			if (*itms[i+itmofs] == MONEY) {
				vf[i].attr = ATT|INP|NED|LFASTR;
			} else {
				vf[i].attr = ltmpl->attr; /* TXT|INP|NED|LFASTR; */
			}
			vf[i].varl = (char *)(&itms[i+itmofs]);
			i++;
		}
		x0x += dx;
	}
	vf[i].size = 0;
}

int
itmadj(cod)
/*
 * ��������� ��������� ����, ��������� �������
 * � ������� ������ ����� ��� �������� ����
 */
kbcod cod;
{
	register int i;
	register int itmr;      /* �������� itm */

	itmr = itm;

	/* ������� �������� ������� ������ */

	switch (cod) {

	case KB_AL:
		itmr -= yy;
		if (itmr < 0) itmr = 0;
		break;
	case KB_AU:
		if (itmr > 0) itmr--;
		break;
	case KB_AD:
		if (itmr < itmmax-1) itmr++;
		break;
	case KB_AR:
		itmr += yy;
		if (itmr >= itmmax) itmr = itmmax - 1;
		break;
	}
	itm = itmr;

	/* ��������� � ������������ �������� itm
	 * � ��������� ������� �� ������
	 */
	i = itmr - itmofs;
	if (i >= 0 && i < xx * yy) ;      /* ���� �� ���� ������� */
	else {
		switch (cod) {
		case KB_AL:
			itmofs -= ofsx;
			if (itmofs < 0)
				itmofs = 0;
			break;
		case KB_AU:
			itmofs -= ofsy;   break;
		case KB_AR:
			itmofs += ofsx;
if (xx > 1 && itmofs >= (((itmmax / ofsy) - (ofsx / ofsy)) * ofsy))
	itmofs = ((itmmax / ofsy) - (ofsx / ofsy)) * ofsy;
			break;
		case KB_AD:
		case ' ':
			itmofs += ofsy;   break;
		}
		itm = itmr;     /* ������������ ����. */
		clritm();
		pre_vf();
		itmshow();
		w_page(vf, 0);
	}
	return(itm - itmofs);
}

itmini()
/*-------------------------------*/
/* ��������� ��������� ��������� */
/*-------------------------------*/
{
	int nxx;        /* ���������� �������� �� ������ */

	yy = yy_max;
	xx = xx1;

	if (xx1 != 1) {
		xx = maxco/(itmlen + 1);
		if (xx == 0) xx = 1;
	}
	yy = (itmmax + xx - 1)/xx;        /* �.�. ����� ������ �������... */
	if (yy > yy_max) yy = yy_max;
	nxx = (itmmax + yy - 1)/yy;        /* �.�. ����� ������ �������... */
	if (nxx < xx) xx = nxx;
	/* ���� ������ ������ ���� */
	if (yy == 1) {
		yy = 2;
		xx = (xx + 1)/2;
	}
	x0 = ((maxco - ((itmlen + 1) * xx)) / (xx + 1));
	dx = itmlen + 1 + x0;
/*      dx = itmlen + 1 + ((maxco - ((itmlen + 1) * xx)) / (xx + 1));   */
	if (dx > maxco) dx = maxco;

	y0 = maxli - yy - 2;
/*      x0 = (maxco - (dx*xx))/2;       */

	if (xx == 1) {
		ofsx = yy;
		ofsy = yy/2;
		x0 = (maxco - itmlen) / 2;
	}
	else  {
		ofsx = yy * (xx - 1);
		ofsy = yy;
	}
	if (x0 < 0) x0 = 0;

}
