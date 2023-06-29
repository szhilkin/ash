#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "linebp.h"
#include "assist.h"
#include "astat.h"

extern  char    coprts[];
extern  char   *vexdir;
extern  int     y0_top;

char   *onoff[] = { " [ ]", " [x]", 0 };

extern  char    Cfill[];
extern  char    Crepf[];
extern  char    Coutf[];
extern  char    Csubs[];

/*ARGSUSED*/
int
t_ymax(line, cod)
register LINE *line;
kbcod cod;
{
	int *var;

	var = (int *)line->varl;
	if (*var < 2 || *var > (maxli - 4)) {
		w_emsg("Укажите от 2 до 20");
		return(FALSE);
	}
	return(TRUE);
}

IN_PORTS inport[] = {
	/*NOSTRICT*/
	{ "cvt_lh",     cvt_lh  },
	{ "coprts",     coprts },
	/*NOSTRICT*/
	{ "cvt_hl",     cvt_hl  },
	{ "f",          Cfill },
	{ "i",          Crepf },
	{ "o",          Coutf },
	{ "r",          Csubs },
	/*NOSTRICT*/
	{ "onoff",      onoff },
	/*NOSTRICT*/
	{ "t_ymax",     t_ymax },
	/*NOSTRICT*/
	{ "cvt_a",      cvt_a },
	/*NOSTRICT*/
	{ "pF",         &panelf },
	/*NOSTRICT*/
	{ "sF",         &scrolf },
	/*NOSTRICT*/
	{ "hF",         &histf  },
	/*NOSTRICT*/
	{ "cF",         &clockf },
	/*NOSTRICT*/
	{ "mF",         &cmailf },
	/*NOSTRICT*/
	{ "yy_max",     &yy_max },
	/*NOSTRICT*/
	{ "y0",         &y0     },
	/*NOSTRICT*/
	{ "y0_top",     &y0_top },
	{ 0,            0       },
	};

/*
 * Открыть для чтения служебный файл ash.
 * Просматривается каталоги .:$HOME:/usr/new/lib/ash
 */
FILE *
afopen(filen, cwdlook)
char *filen;
int   cwdlook;     /* 0-не просматривать текущий каталог */
{
	register FILE *fp;
	char fname[200];
	register char *pathp;
	extern char *getenv();

	fp = NULL;
	if (cwdlook)
		fp = fopen(filen, "r");
	if (fp == NULL && (pathp=getenv("HOME")) != (char *)0) {
		/*VARARGS*/
		sprintf(fname, "%s/%s", pathp, filen);
		fp = fopen(fname, "r");
	}
	if (fp == NULL) {
		/*VARARGS*/
		sprintf(fname, "%s%s", vexdir, filen);
		fp = fopen(fname, "r");
	}
#ifdef DEVELOPE
	if (fp == NULL) {
		w_emsg(" "); w_str("Не найден "); w_str(filen);
	}
#endif
	return(fp);
}

/*
 * Получить внешнюю страницу.
 */
LINE *
getvex(pgname)
register char *pgname;   /* имя файла страницы */
{
	register LINE *linep;
	extern LINE *b_page();

	/*NOSTRICT*/
	if((linep=b_page(vexdir, pgname, inport)) == NULL) {
		w_emsg("Не найден ");
		w_str(pgname);
	}
	return(linep);
}

sup()
{
	LINE *supm;

	if ((supm = getvex("sup.lb")) == NULL)
		return(FALSE);
	u_page(supm, "suphlp.lb");
	d_page(supm);

	pre_vf();
	er_pag();
	return(TRUE);
}
