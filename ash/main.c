#include <signal.h>
#include <ctype.h>
#include <stdio.h>
#include "line.h"
#include "assist.h"

/*NOXSTR*/
char    *tmpflnm = "/tmp/ashXXXXXX";
/*YESXSTR*/
extern  char *mktemp();

extern  char *getenv();

int     y0_top = 0;   /* Начало свитка на экране */

int     scrolf = 1;     /* флаг: продвигать рулон, а не гасить экран */
int     oneitm = 0;     /* флаг: разрешено указать только один пункт меню */
int     panelf = 1;     /* флаг: показывать панель подсказки */
int     histf  = 0;     /* флаг: сохранять историю команд */
int     clockf = 1;     /* флаг: показывать часы */
int     cmailf = 1;     /* флаг: проверять почту */
int     loginf = 0;     /* флаг: главная оболочка, ppid() == 1 */

char   *homedir;        /* домашний каталог */
char   *cwd;            /* текущий (рабочий) каталог */

usage()
{
	fprintf(stderr, "Usage: ash [-1] [-bN]\n");
	exit(1);
}

int     allcod = 1;

ashexit(ok)
{
#ifdef RETRO
	cp_set(-1, 0, TXT);
#else
	cp_set(y0, 0, TXT);
	er_eop();
	cp_set(y0, 0, TXT);
#endif
	io_set(IO_TTYPE);
#ifdef RETRO
	putchar('\n');
#endif
	if (ok == 0 && histf && homedir != (char *)0)
		cmdphist(homedir);

	unlink(tmpflnm);

	exit(ok);
}

/*ARGSUSED*/
void onintr(signo)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	ashexit(1);
}

static  LINE tmplate =
/*NOSTRICT*/
       { 16, 0, 0, 0,
	       TXT|INP|NED|LFASTR,
		       (char *)0,
			       cvt_sp,
			       t_file,
				       (char*)0 };

char    *itms1[ITMMAX+1];       /* УКАЗАТЕЛИ НА ПУНКТЫ ГЛАВНОГО МЕНЮ */

main(argc, argv)
char **argv;
{
       char *envsup;   /* строка флагов из окружения */
       char *ashstd;   /* имя файла настройки команд */
       int c;

       /* инициализация главного меню */
       itms   = itms1;
       ltmpl  = &tmplate;
       itmbsz = ITMBUF;
       yy_max = 10;

       ashstd = ".ashstd";

       for (argc--, argv++; argc > 0; argc--, argv++) {
	       if (*argv[0] == '-') {
		       switch(argv[0][1]) {
		       default:
			       usage();
		       case 'b':
				itmbsz = atoi( &argv[0][2] );
				if (itmbsz <= 0) {
				       fprintf(stderr,
				       "-b flag bad used...");
				       usage();
			       }
			       continue;
		       case '1':
			       xx1 = 1;
			       continue;
		       }
	       }
	       else {
		       /* имя файла для интерпретации cmdset() */
		       ashstd = *argv;
	       }
       }

       if ((envsup=getenv("ASH")) != (char *)0) {
	       char linenoa[4];
	       char *p;

	       scrolf = histf = panelf = clockf = cmailf = 0;
	       /*** yy_max = 10; */

	       while (c = *envsup++) {
		       switch (c) {
		       case 'l':
				 p = linenoa;
				 while (isdigit(*envsup)) {
				       *p++ = *envsup++;
				 }
				 *p = '\0';
				 yy_max = atoi(linenoa);
				 if (yy_max < 2 )
				       yy_max = 2;
				 if (yy_max > (maxli-4))
				       yy_max = maxli-4;
				 break;
		       case 'p':
				 panelf++;
				 break;
		       case 's':
				 scrolf++;
				 break;
		       case 'h':
				 histf++;
				 break;
		       case 'c':
				 clockf++;
				 break;
		       case 'm':
				 cmailf++;
				 break;
		       }
	       }
       }
       if ((homedir=getenv("HOME")) == (char *)0)
	       histf = 0;

       if (homedir != (char *)0)
	       cmdghist(homedir);

       tmpflnm = mktemp(tmpflnm);      /* получить имя временного файла */
/*         tmpflnm = "/tmp/ash.tmp";        /* получить имя временного файла */

#ifdef  VTTY
	vtty();
#endif
	hw_set();
	/* Настроить нач. состояние области свитка */
	y0_top = maxli - 1;
	io_set(IO_VIDEO);
	signal(SIGINT, onintr);

/*NOXSTR*/
	if ( cmdset(ashstd) && fil_vf(1) ) {
		scrlnl();

		signal( SIGINT, SIG_IGN );
		signal( SIGQUIT, SIG_IGN );

		u_menu(vf, "mainh.lb");
		ashexit(0);
	}
	/* ошибки (плохо установлен ash, не читается реперный файл */
	exit(1);
}
