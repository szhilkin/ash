#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "linebp.h"

char    sbbs[100]={0};   /* ��������� ��� ������ ������ */
char     ss[300]={0};   /* ��������� ������ ��� ����� �� ����� */
char    *ssp  =0;       /* ������� ��������� � ���� ������ */

LINE_H  lhd = {0};      /* ��������� �������� �������� */
LINE    *linep=0;       /* ����� ��� ���������� ���� �������� */

char    *hps = 0;       /* ���� ����� */
char    *hpsp= 0;       /* ������� ��������� � ���� ��� ��������� */

unsigned nmi = 0;      /* ������� ������ ���� */
char    *nms = 0;      /* ������� ���� ������� ��� ����-������� */

char i_fnam[100];       /* ��� �������� ����� */
char o_fnam[100];       /* ��� ��������� ����� */

/*------------*/
/*   main     */
/*------------*/
main(argc, argv)
char    *argv[];
{
	char *malloc(), *calloc();
	register unsigned i;
	register char *s;
	register int   c;
	unsigned     *ip;
	FILE *i_fp, *o_fp;
	FILE *fopen();

	/* �������� ����� ������ */
	if( argc != 3 ) {
		printf("Usage : bl ifile ofile\n");
		exit(1);
	}
	strcpy(i_fnam, argv[1]);
	strcpy(o_fnam, argv[2]);

	/* ������� ���� �� ������ */
	if((i_fp = fopen(i_fnam, "r")) == NULL ) {
		printf("%s : Can't read\n", i_fnam);
		exit(1);
	}

	/* ������� ���� �� ������ */
	if((o_fp = fopen(o_fnam, "wn")) == NULL ) {
		printf("%s : Can't write\n", o_fnam);
		exit(1);
	}

	/* ��������� ������ ��� �������� �������� */
	if((linep = calloc(1000, sizeof(LINE))) == NULL ) {
		printf("Memory for LINE absent...\n");
		exit(1);
	}
	/* ��������� ������ ��� ���� ����� */
	if((hps = malloc(4000)) == NULL ) {
		printf("Memory for HEAP absent...\n");
		exit(1);
	}
	hpsp = hps;

	/* ��������� ������ ��� ������� ���� */
	if((nms = calloc(200, 8)) == NULL ) {
		printf("Memory for NAMTAB absent...\n");
		exit(1);
	}
	put_nm("**--**");

	/* ��������� �������� �������� */
	linebp(i_fp);

	/* ��������� �� ������� 16byte ������� � ��������� */
	i = lhd.lh_lines;
	i += (16 - (i % 16));
	lhd.lh_lines = i;

	i = lhd.lh_heaps;
	i += (16 - (i % 16));
	lhd.lh_heaps = i;

	i = lhd.lh_names;
	i += (2 - (i % 2));
	lhd.lh_names = i;

	/* �������� ��������� �������� �������� */
	s = (char *)&lhd;
	for(i=0; i<sizeof(LINE_H); i++ ) {
		c = *s++;
		putc( c, o_fp);
	}

	/* �������� line[]  � �������� ���� */
	s = (char *)linep;
	for(i=0; i<lhd.lh_lines; i++ ) {
		c = *s++;
		putc( c, o_fp);
	}

	/* �������� ���� ����� */
	s = hps;
	for(i=0; i<lhd.lh_heaps; i++ ) {
		c = *s++;
		putc( c, o_fp);
	}

	/* �������� ������� ���� */
	s = nms;
	for(i=0; i<lhd.lh_names; i++ ) {
		c = *s++;
		putc( c, o_fp);
	}

	/* ������� �������� ���� */
	fclose( o_fp );

	exit(0);

}

static  int     s_count;        /* ������� ����� */
/*-----------------------------*/
/* ��������� �������� �������� */
/*-----------------------------*/
linebp(i_fp)
FILE   *i_fp;
{
	register int c;
	register unsigned i;
	LINE    *line;          /* ������� ��������� */

	s_count = 0;
	line=linep;     /* �������� ������� �������� �������� */

	/* ���� ������ ����� �������� �������� ����� ����� */
	while( fgets( ss, 300, i_fp ) != NULL) {

		s_count += 1;

		/* ���������� ����������� */
		if(ss[0] == '#')        continue;

		ssp = ss;

	    c = *ssp;
	    line->attr = 0;
	    for( ; !isspace(*ssp); ssp++ ) {
		switch( *ssp ) {
		case 'G' :                  /* ����� ������ */
		case 'g' :
		    break;
		case 'I' :                  /* ����� ��� ����� */
		case 'i' :
		    line->attr |= INP;
		    break;
		case 'H' :                  /* ��������� */
		case 'h' :
		    line->attr |= LHDR;
		    break;
		case 'C' :                  /* ����������� */
		case 'c' :
		    line->attr |= LTXT;
		    break;
		case 'P' :                  /* � ���������� */
		case 'p' :
		    line->attr |= PMT;
		    break;
		case 'V' :                  /* ���������� */
		case 'v' :
		    line->attr |= LVAR;
		    break;
		case 'A' :                  /* ������������� */
		case 'a' :
		    line->attr |= LALT;
		    break;
		case 'M' :                  /* ���� */
		case 'm' :
		    line->attr |= LMSE;
		    break;
		case 'K' :                  /* �������� ������� */
		case 'k' :
		    line->attr |= (ATT|MID|PAD);
		    break;
		default :
		    printf("%s: %3d: ���������. ������� '%c'\n",
		    i_fnam, s_count, *ssp );
		    goto cont;
		    break;
		}
	    }

	    /* ������ ��������� ������ ��� ����� */
	    switch( c ) {
	    case 'C':
	    case 'c':
	    case 'H':
	    case 'h':
		    bld_pos(line);  /* ������ 4 ��������� */
		    bld_c(line);        /* ��������� ��������� */
		    break;
	    default:
		    bld_pos(line);  /* ������ 4 ��������� */
		    bld_g(line);        /* ��������� ��������� */
		    break;
	    }
	    line++;
	    lhd.lh_lines += sizeof(LINE);

cont:   ;
	}
	line->size = 0;
	lhd.lh_lines += sizeof(LINE);

}

/*---------------------------------*/
/* ��������� ����������� ��������� */
/*---------------------------------*/

bld_pos(line)
LINE    *line;
{
	char *f_sbbs();
	static  int iii;

	ssp = f_sbbs( ssp );
	sscanf(sbbs, "%d", &iii);
	line->size = (char)iii;

	ssp = f_sbbs( ssp );
	sscanf(sbbs, "%d", &iii);
	line->line = (char)iii;

	ssp = f_sbbs( ssp );
	sscanf(sbbs, "%d", &iii);
	line->colu = (char)iii;

}

/*-------------------------------------*/
/* ��������� ��������� ������ �������� */
/*-------------------------------------*/

char
bld_flg()
{
	char    *f_sbbs();
	int     flg;
	register c;
	register char *s;

	ssp = f_sbbs( ssp );
	flg = 0;
	for( s=sbbs; *s; s++ ) {
		c = *s;
		switch( c ) {
		case '-' :
		case '*' :
			flg = 0;
			break;
		case 'U' :
		case 'u' :
			flg |= SUSU;
			break;
		case 'D' :
		case 'd' :
			flg |= SUSD;
			break;
		case 'L' :
		case 'l' :
			flg |= SUSL;
			break;
		case 'R' :
		case 'r' :
			flg |= SUSR;
			break;
		case 'N' :
		case 'n' :
			flg |= SUSNL;
			break;
		case 'T' :
		case 't' :
			flg |= SUST;
			break;
		default :
		    printf("%s: %3d: ���������. ��� �������� '%c'\n",
		    i_fnam, s_count, c );
		    break;
		}
	}
	return(flg);
}

/*--------------------------*/
/* ��������� ��������-����� */
/*--------------------------*/
char *
bld_par(argstr)
char    *argstr;
{
	register i;
	static char *argp;
	char    *p;
	char    *sav_hpsp;
	char    *ret_s;
	char    *f_sbbs();

	if( *argstr == '\0' )
		argp = ssp;     /* �� �������� �������� */
	else
		argp = argstr;  /* ����������� �������� : tmp, cvt_a */

	while( isspace(*argp) )
		argp++;
	if( *argp == '-' ) {
		argp++;
		ret_s = 0;
		goto ret_ret;
	}

	/* ���� ������ ���������� � " */
	if( *argp == '"' ) {
		/* �������� ������ � ���� */
		argp++;
		for( sav_hpsp=hpsp; *argp; hpsp++, argp++) {
			if(*argp == '"') {
				if(*(argp+1) != '"') {
					/* ��������� ������ */
					lhd.lh_heaps += 2;
					*hpsp++ = '\0';
					argp++;
					break;
				} else {
					/* "" � ���� " */
					*hpsp = *argp++;
				}
			} else  {
				*hpsp = *argp;
			}
			lhd.lh_heaps += 1;
		}
		/* ������� �������� �� ������ ���� + 2048 */
		ret_s = (char *)(sav_hpsp - hps + (unsigned)2048);

	}
	/* ����� ��� ��� ������ ��� ����-������� */
	else {
		argp = f_sbbs( argp );
		/* ���� ��� ���������� */
		if((ret_s=(char *)tst_nm(sbbs)) == 0 ) {
			/* ���������������� ��� */
			ret_s = (char *)put_nm(sbbs);
		}
	}
ret_ret:
	if( *argstr == '\0' )
		ssp = argp;

	return( ret_s );
}

/*----------------------------*/
/* ���������, �������� �� ��� */
/*----------------------------*/
tst_nm( s )
char  *s;
{
	register int i;
	register char *p;

	for(p=nms, i=0; i<nmi; p += 8, i++) {
			if(strncmp(p, s, 6) == 0)
				return( i );
		}
	return( 0 );
}

/*-------------------------*/
/* ��������� ��� � ������� */
/*-------------------------*/
int
put_nm( s )
char  *s;
{
	register char *p;

	p = nms + (nmi * 8);
	strncpy(p, s, 6);
	lhd.lh_names += 8;
	nmi += 1;
	return( nmi-1 );
}

/*---------------------*/
/* ��������� ��������� */
/*---------------------*/
char *
f_sbbs( ispp )
register char *ispp;
{

	register int i;

	while( isspace(*ispp) ) {
		ispp++;
	}
	i = 0;
	while( (isspace(*ispp) == 0) && (i < 6) ) {
		sbbs[i++] = *ispp++;
	}
	while( i<8 )
		sbbs[i++] = 0;

	return( ispp );
}

/*------------------------------*/
/* ��������� �������� ��������� */
/*------------------------------*/

bld_c(line)             /* ����������� */
LINE  *line;
{
	char   bld_flg();
	char  *bld_par();

	line->flag = 0;
	line->cvts = 0;
	line->cvtf = 0;
	line->test = 0;
	line->varl = bld_par("");
}


bld_g(line)             /* ����� ������ */
LINE  *line;
{
	char   bld_flg();
	char  *bld_par();

	line->flag = bld_flg();
	line->cvts = bld_par("");
	line->cvtf = bld_par("");
	line->test = bld_par("");
	line->varl = bld_par("");
}
