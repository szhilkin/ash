/*
 *      vcc = video + cc
 *
 *      $Header: vcc.c,v 1.7 90/12/27 16:35:38 vsv Exp $
 *      $Log:	vcc.c,v $
 * Revision 1.7  90/12/27  16:35:38  vsv
 * �������� USRATTR � ������ � ���.
 * 
 * Revision 1.6  89/08/29  15:59:05  vsv
 * ������ LINLIB_3
 * 
 * Revision 1.5  89/02/02  15:10:11  vsv
 * ���������� ������ ������� ����� ������ ---LINES,
 * ����������� � ������ ��������� '=' ������ ���� ����� �����
 * � ���������� ���������� ���������
 * 
 * Revision 1.4  89/01/13  17:55:49  vsv
 * ��������� ������, �� �������� � �����.
 * ��������� ����� ���������� �����
 * �������� ��������.
 * 
 * Revision 1.3  88/06/17  19:23:59  vsv
 * ���������� ������ vlbp
 * 
 * Revision 1.2  88/06/01  09:57:22  vsv
 * ����� ����� ���� ��� � ������, ���
 * � � ������� ��������
 * 
 * Revision 1.1  88/05/14  13:03:12  vsv
 * Initial revision
 * 
 */

#include <ediag.h>
#include <stdio.h>
#include <ctype.h>
#include "line.h"
#include "linebp.h"

#include "vgen.h"

extern  char *malloc();

char    Enomem[] = "No memory (%s)\n";
char    Rnomem[] = "��� ������ (%s)\n";

int     nlin = 0;       /* ����� ��������� �� �����.lav ������ */
int  lbptyp = 0;        /* ������� ��� (�� ����� ������ "���=" */
int  vscan  = 0;        /* ����: ����� ������ ����������� �� ��������� */

char    Esxerr[] = "%s: %d:Syntax err:  %s\n";
char    Rsxerr[] = "%s: %d:�������. ��.: %s\n";



/* ��������� ������������� */
#define LBPOSIZ 200             /* ������ ��������� ������ */

#ifdef VCC
char    pag_id[] = "---PAGE";   /* ������ ����������� �������� */
#endif

char    vis_id[] = "---";
char    com_id[] = "#";
char    por_id[] = "PORTS";
char    tab_id[] = "TABLES";
char    lin_id[] = "LINES";
char    scr_id[] = "SCREEN";
char    hel_id[] = "HELP";
char    end_id[] = "END";
char    markl    = '.';

#define isnamec(c) (isalnum(c) || c == '_')

/*----------------------------*/
/* �������� ���������� ������ */
/*----------------------------*/
typedef struct {
	int     nmt;            /* ��� */
	char   *nms;            /* ������, ��� ��� ����������� */
} NM_DEF;

/* ����������:
 * 1)   ���� ��� �� ����������� � ���� �������, ��
 *      ��� ������������� ������ � ����� ������.
 */

/* ���� ������ ��� ������� ����������� ���� */

#define DPORT   1
#define DTABLE  2
#define DLINE   3

/* ���� ����� � ������������� ���� */
typedef struct {
	char    *top;           /* ������ ���� */
	char    *bot;           /* �����, ����� ��� ����� ������ */
	unsigned siz;           /* ������ ��������� ����� � ������ */
	NM_DEF  *nmd;           /* �������: �� ����� ������ ������ */
	NM_DEF  *nmp;           /* ��������� �� ����� ��������� ���� */
	int     ncnt;           /* ������� ����� */
	int     nmax;           /* ������������ ���-�� ����� � ���� */
	} HEAP;

HEAP    shp  = {0};     /* ���� ����� �������� lbp */

char    *scrp = 0;      /* ����� ������ ������ = malloc(80*24); */

/* ��������� ������������ ������ */
int     cur_li = 0;     /* ������ */
/*int     cur_co = 0;     /* ������� */

char lbpo[LBPOSIZ] = {0};       /* ����� ������������ ������ � ������� lbp */

/*---- �����: ����� � ��������� �� ������ */
char ifn[50];
char ofn[50];
char hvfn[50];

FILE *ifp;
FILE *hvfp;

/*-------  ����������� ------*/
usage(s)
char *s;
{
#ifdef VLBP

	fprintf(stderr,
	"�����:\n %s [-h file.hv] [-o file.la] file.lav\n",
	s);
#endif
#ifdef VCC
	fprintf(stderr,
	"�����:\n %s [-o file.c] file.cv\n",
	s);
#endif
	exit(1);
}

int     cnterr = 0 ;    /* ������� ������ */


main(argc, argv)
/*----------*/
/*   MAIN   */
/*----------*/
int  argc;
char *argv[];
{
	register acnt;
	char *s;

	_setediag();

	if (argc <  2) {
		usage(argv[0]);
	}
	for ( acnt=1; acnt < argc ; acnt++ ) {
		if ( (int)argv[acnt][0] == '-' ) {
			switch( (int)argv[acnt][1] ) {
			case 'h' :
			case 'H' :
				strcpy(hvfn, argv[++acnt]);
				break;
			case 'o' :
			case 'O' :
				acnt += 1;
				strcpy( ofn, argv[acnt]);
				break;
			default:
				usage(argv[0]);
				break;
			};
		} else if ( ifn[0] == '\0') {
			strcpy( ifn, argv[acnt]);
		} else {
			usage(argv[0]);
		}
	}

	/* ������� ����.lav */
	if ((ifp=fopen(ifn, "r")) == NULL) {
		perror(ifn);
		exit(1);
	}
	/* ��������� ������ ��� ������ ������ */
	if ( (scrp=malloc(80*24))==NULL ) {
		fprintf(stderr, ediag(Enomem, Rnomem), "scrp");
		exit(1);
	}
	/* ������� �������� �������� ���� */
	if(*ofn) {
		if ((freopen(ofn, "w", stdout)) == NULL) {
			perror(ofn);
			exit(1);
		}
	}

#ifdef VCC
    /*----------- � ����� ����� �.���� ������ ����� �������� */
    while(fgets(lbpo, LBPOSIZ, ifp), feof(ifp) == 0) {

      if (str_eq(lbpo, pag_id) == 0) {
	printf("%s", lbpo);             /* ����� �� �� */
      }
      else {
	s = lbpo + strlen(pag_id);      /* ������� ����� ��� �������� */
	while(isspace(*s)) s++;
	s[ strlen(s) - 1 ] = '\0';

	printf("LINE %s[] = {\n", s);  /* ������ �������� �������� */

#endif
	/* ���������������� ���� ����� (������_������, ����������_�����) */
	ini_hs( &shp, HSSIZE, 200);

	/* ��������� ���� ����� � ������������� ����;
	 * ��������� ����� ������ �� ������ SCREEN;
	 */
	cur_li = 0;    /* ������������ ������ ������ ������� */
	pass1();
	if ( cnterr ) {
		fprintf(stderr,
ediag("PASS1: Errors detected: %d\n", "PASS1: ���������� ������: %d\n"),
		cnterr);
		exit(1);
	}
	/* ����������� �����, �������� ����.la */
	scan_s();

	/* ���� ���������� ����.hv, �������� ���,
	 * �� ���� ��������� ��� ��� ����� ������� */
/*      do_hv();        */

#ifdef VCC
	des_hs( &shp ); /* ���������� ���� ����� */

	printf( "%s\n\n",   "{ 0 }, };"  );

      } /*---------- ����� �������� */
    } /*--------- ���������� ����� ������ �������� ������� */
#endif

	fclose(ifp);
	exit(0);

}


/*--------- ��������� ������� ������ ----------*/

str_eq(s, pat)
/*-------------------------*/
/* �������� ��� � �������� */
/*-------------------------*/
register char *s;
register char *pat;
{
	/* ������� 0, ���� �� ���������;
	 *         1, ���� ������ ����������;
	 */
	while ( *pat != '\0' ) {
		if ( *pat++ != *s++ )
			return(0);
	}
	return(1);
}

NM_DEF *fnd_nm(s)
char *s;
/*-------------------------*/
/* ����� ����������� ����� */
/*-------------------------*/
{
	register NM_DEF *hpp;
	register unsigned i;

	for (hpp=shp.nmd, i=0; i<shp.ncnt; i++, hpp++) {
		/* ���� ��������� s � "���=..", ������� ��������� */
		if(str_eq(hpp->nms, s))
			return(hpp);
	}
	/* �� ���������: ������� NULL */
	return((NM_DEF *)NULL);
}

des_hs( hp )
/*-----------------------*/
/* ���������� ���� ����� */
/*-----------------------*/
register HEAP *hp;
{
	free( hp->nmd );
	free( hp->top );        /* ���������� ������ � �������� ������� */
}

ini_hs( hp, siz, strnum )
/*-----------------------------*/
/* ���������������� ���� ����� */
/*-----------------------------*/
register HEAP *hp;
unsigned siz;
{
	extern char *malloc();

	if((( hp->top = malloc(siz)) != NULL )
	&& (( hp->nmd =
	  (NM_DEF *)malloc(sizeof(NM_DEF)* strnum)) != NULL )) {
		hp->bot = hp->top;
		hp->siz = siz;
		hp->nmp = hp->nmd;
		hp->nmax = strnum;
		return(1);
	}
	return(0);
}

char *
c_ports(name)
/*---------------------------------------*/
/* �� ����� name ������� ����. �� por_id */
/*---------------------------------------*/
char *name;
{
	static char nm_fnd[82]; /* �������� ����� (������ ����� "���=") */
	register NM_DEF *hpp;   /* ����������� ����� */
	register char *res;     /* ��������� */

	strcpy(nm_fnd, name); strcat(nm_fnd, "=");
	if(((hpp=fnd_nm(nm_fnd)) != (NM_DEF *)NULL)
	&& ((hpp->nmt) == DPORT)) {
		for(res=hpp->nms; *res++ != '='; ); /* ���������� "���=" */
		strcpy(nm_fnd, res);
	} else {
		nm_fnd[ strlen(nm_fnd) - 1 ] = 0;
	}
	return(nm_fnd);
}

char *
c_lines(lbps)
/*---------------------------------------------------------*/
/* ������ � ����. lbp ������������� � ������� ������. LINE */
/*---------------------------------------------------------*/
register char *lbps;
{
	static  nm_ret[100];    /* ������ ���������� */
			/* �������� ������ � ������� lbp: */
	char    fldtyp[20];     /* ��� ����, ���� "ipa" */
	char    susflg[20];     /* ����� �������� */
	char    lpar[20];       /* ��������� �������� ����� */
	int     multp;    /* ����: ���� �������� '|' ����� ����. �����. */
	int     parn;   /* ���������� ���������� ��� �������� ���������� */
	char   *lbpssav;
	register char *s;
	register char *os;

	lbpssav = lbps;
	/* ����� �������� */
	for(s=fldtyp; !isspace(*lbps); lbps++)
		*s++ = (isupper(*lbps)) ? tolower(*lbps) : *lbps;
	*s = '\0';
	while(isspace(*lbps)) lbps++;

	/* ����� ����� */
	for(s=susflg; !isspace(*lbps); lbps++)
		*s++ = (isupper(*lbps)) ? tolower(*lbps) : *lbps;
	*s = '\0';
	while(isspace(*lbps)) lbps++;

	/* ������ ���������� ��������� */
	os = nm_ret;

	/* ���������� ����� � ������� �� */
	for(multp=0,s=susflg; *s; ) {
		if(multp)       *os++ = '|';
		else            multp = 1;
		switch(*s) {
		case  'U':
		case  'u': strcpy(os, "SUSU"); os += 4; break;
		case  'D':
		case  'd': strcpy(os, "SUSD"); os += 4; break;
		case  'L':
		case  'l': strcpy(os, "SUSL"); os += 4; break;
		case  'R':
		case  'r': strcpy(os, "SUSR"); os += 4; break;
		case  'N':
		case  'n': strcpy(os, "SUSNL"); os += 5; break;

		case  '*':
		case  '-': *os++ = '0'; break;
		default:
			fprintf(stderr, "'%c': %s\n", *s,
			ediag("bad flag","������ ����"));
		}
		s++;
	}
	*os++ = ','; *os++ = '\t';

	/* ���������� �������� � ������� �� */
	for(multp=0,s=fldtyp; *s; ) {
		if(multp)       *os++ = '|';
		else            multp = 1;
		switch(*s) {
		case  'I':
		case  'i': strcpy(os, "INP"); os += 3; break;
		case  'P':
		case  'p': strcpy(os, "PMT"); os += 3; break;
		case  'C':
		case  'c': strcpy(os, "LTXT"); os += 4; break;
		case  'H':
		case  'h': strcpy(os, "LHDR"); os += 4; break;
		case  'V':
		case  'v': strcpy(os, "LVAR"); os += 4; break;
		case  'A':
		case  'a': strcpy(os, "LALT"); os += 4; break;
		case  'M':
		case  'm': strcpy(os, "LMSE"); os += 4; break;
		case  '1':
		case  '2':
		case  '3':
		case  '4':
		case  '5':
		case  '6':
		case  '7':
		case  '8':
		case  '9':
		case  '0': strcpy(os, "USRATTR");
			   os[7] = *s;
			   os += 8; break;

		case  '*':
		case  '-':
		case  'G':
		case  'g': *os++ = '0'; break;
		default:
			fprintf(stderr, "'%c': %s\n", *s,
			ediag("bad attribute","������ �������"));
		}
		s++;
	}

	/* ��������� ��������� */
	parn = 0;
	while( *lbps ) {

		parn++;
		/* ����������� ����� ���������� */
		*os++ = ','; *os++ = '\t';

		switch( *lbps ) {
		case '\0':
			goto ret;
		case  '*':
		case  '-':
			lbps++;
			/* �������� ������ */
			*os++ = '0';
			break;
		case  '"':
			/* ����� ������ � ����������� �� */
			do {
				*os++ = *lbps;
			} while( ! ((*lbps != '\\') && (*++lbps == '"') ));

			*os++ = *lbps++;
			break;
		default:

			/* �������� ��� */
			for(s=lpar; *lbps && !isspace(*lbps); )
				*s++ = *lbps++;
			*s = '\0';

			/* ����� ����������� */
			sprintf(os, "%s", c_ports(lpar));
			os += strlen(os);
		}
		/* ���������� �� ����. ��������� */
		while(*lbps && isspace(*lbps))  lbps++;
	}
ret:
	*os = '\0';

	/* ���� �� ��������� ���������� ��������� ����������... */
	/** parn = 1 (c,h); ��� ��������� parn = 5 **/
	if(*fldtyp=='c' || *fldtyp=='h') {
		if(parn == 1 )
			return(nm_ret);
		else
			goto err_exit;
	} else if(parn == 4) {
		return(nm_ret);
	} else { ; }
err_exit:
	fprintf(stderr, "%s: '%s'\n",
	ediag("Bad par count","������ ���������� ����������"), lbpssav);

}


subst(siz)
/*---------------------*/
/* ������� ����������� */
/*---------------------*/
int siz;        /* ������ ���� */
{
	static  char subs[300];         /* ������ � ������������� */
	int     prompted = 0;           /* ���� ����� ��������� */
	register char *from;
	register char *to;
	int     comma;

	prompted = 0;
	to = subs;
	from = lbpo;
#ifdef VLBP
	while(!isspace(*from)) {
		if(*from == 'p' || *from++ == 'P') prompted = 1;
		from++;
	}
#endif VLBP
#ifdef VCC
	comma = 0;
	while(*from) {
		if(*from == ',') comma++;
		if(comma == 4 && *from=='P'
		&& from[1]=='M' && from[2]=='T')  /* attr == PMT ? */
			prompted = 1;
		from++;
	}
#endif VCC
	from = lbpo;
	while(*from) {
		if(*from == '$' && from[1] == '#') {      /* ������ ���� */
			from++, from++;
			sprintf(to, "%d", siz-prompted); /* �������� ������*/
			while(*to) to++;        /* ����� ����� �����.*/
		}
		else {
			*to++ = *from++;
		}
	}
	*to = '\0';

	/* ���� �� ������ ���� -o, ��������� �� ��������� */
	printf("%s\n", subs);

}

#ifdef VCC

mk_vcc(siz, li, co, s, typ)
/*----------------------------------------*/
/* ������������� � ������� ��������� LINE */
/*----------------------------------------*/
int     siz;
int     li;
int     co;
register char *s;       /* ���������� ���� */
int     typ;            /* 'h', 'c', 'k', '?' */
{
	/* ��������� �������� � ������ lbpo;
	 * ������� mk_lbp � mk_vcc ����� ������,
	 * ���� ������� �� ������;
	 * ����������� � ����� � ���� ��������
	 * �������� subst;
	 */
	static  char nm_fnd[82];        /* ���, ������� ����� ����� */
	register NM_DEF *hpp;           /* ����������� ����� */
	register char *os;
	register char *o2s;

	if(typ == '?') {        /* ���� � ������, ���� ������� ����������� */
		strcpy(nm_fnd, s); strcat(nm_fnd, "=");
		if(((hpp=fnd_nm(nm_fnd)) != (NM_DEF *)NULL)
		&& ((hpp->nmt) != DPORT)) {
			os = (hpp->nms);
			while(*os++ != '=');    /* ����� �������� ����� */
			  strcpy(nm_fnd, os); strcat(nm_fnd, "=");
			  if(hpp->nmt == DTABLE) {   /* ������. ����. */
			  sprintf(lbpo,
			  "{ %2d,%2d,%2d, SUST|%s, 0,0,0,0, %s },",
			  siz, li, co, vscan?("SUSL"):("SUSU"),
			  c_ports(os));
			} else
			if(hpp->nmt == DLINE) {
				sprintf(lbpo,
				"{ %2d,%2d,%2d, %s },",
				siz, li, co, c_lines(os));
		       }
		} else {
			/* ���������: ������ ��� ����� */
			sprintf(lbpo,
			"{ %2d,%2d,%2d, 0, LVAR|INP|PMT, 0,0,0, %s },",
			siz, li, co, s);
		}
	} else
	if(typ == 'h' || typ == 'c') {
		sprintf(lbpo, "{ %2d,%2d,%2d, 0, %s, 0,0,0, ",
		siz, li, co, typ=='h' ? ("LHDR"):("LTXT") );

		for(os=lbpo; *os; os++) ;       /* ����� ����� */
		*os++ = '"';
		while(*s) {
			if(*s == '"') *os++ = '\\';
			*os++ = *s++;
		}
		strcpy(os, "\" },");
	} else
	if(typ == 'k') {
		sprintf(lbpo,
   "{ %2d,%2d,%2d, 0, ATT|MID|PAD, 0, cvt_lh, 0, \"%s\" },",
		siz, li, co, s);
	}
	subst(siz);        /* ������� ����������� */
}
#endif VCC

#ifdef VLBP

mk_lbp(siz, li, co, s, typ)
/*-------------------------------*/
/* �������� ������ � ������� lbp */
/*-------------------------------*/
int     siz;
int     li;
int     co;
register char *s;
int     typ;
{
	static  char nm_fnd[82];        /* ���, ������� ����� ����� */
	register NM_DEF *hpp;           /* ����������� ����� */
	register char *os;
	register char *o2s;

	if(typ == '?') {        /* ���� � ������, ���� ������� ����������� */
		strcpy(nm_fnd, s); strcat(nm_fnd, "=");
		if(((hpp=fnd_nm(nm_fnd)) != (NM_DEF *)NULL)
		&& ((hpp->nmt) != DPORT)) {
			os = (hpp->nms);
			while(*os++ != '=');    /* ����� �������� ����� */
			if(hpp->nmt == DTABLE) {
			sprintf(lbpo,
			"g\t%2d %2d %2d\tt%c - - - %s",
			siz, li, co, vscan?('l'):('u'), os);
			} else
			if(hpp->nmt == DLINE) {
				o2s = lbpo;
				while(!isspace(*os))   /* �������� */
					*o2s++ = *os++;
				while( isspace(*os)) os++; /* ��� �������*/
				sprintf(o2s,
				"\t%2d %2d %2d\t%s",
				siz, li, co, os);
		       }
		} else {
			sprintf(lbpo,
			"ip\t%2d %2d %2d\t- - - - %s",
			siz, li, co, s);
		}
	} else
	if(typ == 'h' || typ == 'c') {
		sprintf(lbpo, "%c\t%2d %2d %2d\t", typ, siz, li, co);
		for(os=lbpo; *os; os++) ;       /* ����� ����� */
		*os++ = '"';
		while(*s) {
			if(*s == '"') *os++ = '"';  /* ����� ������ lbp...*/
			*os++ = *s++;
		}
		*os++ = '"'; *os = '\0';
	} else
	if(typ == 'k') {
		sprintf(lbpo, "%c\t%2d %2d %2d\t- - cvt_lh - \"%s\"",
			typ, siz, li, co, s);
	}
	subst(siz);        /* ������� ����������� */
}
#endif VLBP


mk_lin(li, co)
/*----------------------------------------*/
/* ����� ���������� ��� ������������ ���� */
/*----------------------------------------*/
int li;
int co;
{
	/* ����� ����, ��� ������ lbp ��������,
	 * � ������ ������ ������ �� �����������
	 * �������� ����� �� ���� - ��� ������������� ���������.
	 */
	register char *s;
	register char *os;      /* ��������� ��� ����������� */
	char     *fbeg;         /* ������ � ������� ������ ���� */
	unsigned spcnt;         /* ������� �������� � ����� ����������� */
	static  char stro[80];  /* ����� �������� ������ ����������� ���� */
	int  fldsiz;    /* ������ ���� */
	int  li_lbp, co_lbp;    /* ���������� ���� ��� lbp */
	int  typ;               /* ��� ���� (c,h,k,?) */

	spcnt = fldsiz = 0;
	os = &stro[0];
	fbeg = s = &scrp[80 * li + co];
	li_lbp = li; co_lbp = co;       /* ��������� ���������� */
	if(*fbeg == markl) {  /*--------------* ���� � ������ */
		s++; fldsiz = 1; typ = '?';
		while(co++, isnamec(*s)) {      /* �������� ��� ���� */
			fldsiz++; *os++ = *s++; }
		if(fldsiz == 1) {       /* �����, ��� ���� �� ��� ���� ?: */
			if(fbeg[1] == '"') {         /* ��������� ? */
				fldsiz = 2; s= &fbeg[2]; typ = 'h';
				while(co++<80) {
					fldsiz++;
					if (*s=='"' && s[1]==markl)
						{ s++; break; }
					else {        *os++ = *s++;   }
				}
			} else
			if(fbeg[1] == '\'') {       /* ��� �����. ������� ? */
				typ = 'h'; spcnt = 1; s++;
				while(spcnt--) {        /* ��. �� ������� */
					co++; *os++ = *s++; }
				fldsiz = 3;
			} else
			if(fbeg[1] == ':') {       /* ��� ����. ������� ? */
				typ = 'k'; spcnt = 3;
				while(spcnt--) {        /* ��. �� ������� */
					co++; *os++ = *s++; }
				fldsiz = 4;     /* ����� �� 7..8 */
			} else {
				fprintf(stderr,
ediag("Screen scan err:line %d, col %d\n", "��. ������������: ��� %d, ���. %d\n"),
				li, co); cnterr++;
			}
		}
		while(*s++ == markl && (co++)<80) {
			fldsiz++; }       /* �������� ��� ������ */
		*os = '\0';
		goto fill_end;
	} else
	if(*fbeg == '=' && co_lbp == 0) { /*-----* ��������� �� ��� ������ */
		co++; s++;
		while(co++ < 80 ) {
			if(*s==' ') {
				if((spcnt++) >=4)    break;
			} else {
				spcnt = 0;
			}
			*os++ = *s++;
		}
		*os = '\0'; os--;
		while(*os == ' ') *os-- = '\0';
		fldsiz = 80;
		typ = 'h'; goto fill_end;
	}
	else {          /*---------------* ����� ����������� */
/***
		*os++ = *s++; co++;
***/
		while((co++)<80 ) {
			*os++ = *s++; fldsiz++;
			if(*s==' ') {
				if(s[1]   == markl)    break;
				if((spcnt++) >=3)    break;
			} else {
				spcnt = 0;
			}
		}
		*os = '\0';
		while(*--os == ' ') { *os = '\0'; fldsiz--; }
		typ = 'c';
	}
fill_end:

#ifdef VLBP
	/* �������� ������ � ������� lbp */
	mk_lbp(fldsiz, li_lbp, co_lbp, stro, typ);
#endif
#ifdef VCC
	/* �������� ������ ������������� ��������� LINE */
	mk_vcc(fldsiz, li_lbp, co_lbp, stro, typ);

#endif

	/* ������ �� ������ ������ ������ ��� ����� */
	for(s=fbeg; fldsiz > 0; fldsiz--) *s++ = ' ';
}

scan_s()
/*--------------------------*/
/* ����������� ����� ������ */
/*--------------------------*/
{
	register int li;        /* ������ */
	register int co;        /* ������� */
	char    *lbeg;          /* ���. ������ � ������ ������ */
	int     vertli;         /* ���. ������������� ��������� (������)*/

	/* ������� ���� �� ������� */
	for(li=0; vscan=0,li<cur_li; li++) {
		lbeg = &scrp[ 80 * li ];
		if(*lbeg == '+') {      /* �������� �� ��������� */
			vscan=1; vertli = li;
			/* ���� �� �������� ������ */
			for(co=1; co<80; co++) {
				/* ���� �� ������� */
				for(li=vertli;
				lbeg = &scrp[80*li], *lbeg == '+';
				li++) {
					if(lbeg[co] != ' ')
						mk_lin(li, co);
				}
			}
			li--;
		} else {        /* ����. �������� �� ������� */
			/* ���� �� �������� ������ */
			for(co=0; co<80; co++) {
				if(scrp[80*li+co] != ' ')
					mk_lin(li, co);
			}
		}
	}
}

put_hs(s, hp)
/*-----------------*/
/* �������� � ���� */
/*-----------------*/
register char *s;
register HEAP *hp;
{
	unsigned siz;


/*      printf("-(%d)->\t%s\n", lbptyp, s);     *�������*/

	/*-------------------------- ���������, ����� ����� ���... */
	if((hp->ncnt == hp->nmax) ||
	(hp->bot - hp->top + hp->siz)<(siz=strlen(s) + 1)) {
		/* ������ */
		fprintf(stderr,
ediag("%s: %d:\t:No working plase, sorry...\n", "%s: %d:\t:��� ����� � ���. �������..\n"), ifn, nlin);
		exit(1);
	} else
	      { /*------------- ���� �� ������ �������� �� ��������... */
		/*--- �������� ������ � ���� */
		strcpy(hp->bot, s);
		hp->nmp->nms = hp->bot; /* ��������� ���. ������ */
		hp->nmp->nmt = lbptyp;  /* ���. ��� ������ */
		hp->nmp++; hp->ncnt++;  /* ����. ���. � ���� ���� */
		hp->bot += siz;         /* � ���� ������ "���=..." */
	}
}

fil_sc()
/*-------------------------*/
/* ��������� ������ ������ */
/*-------------------------*/
{
	/* ����� ����� ���� �������� ���������
	 * ��������� � ������ ����� �����.
	 */
	int i;
	char *s;
	char *scr;
	char *savescr;
	int   c;

	s = lbpo;
	savescr = scr = &scrp[ 80 * cur_li ];
	for(i=0; i<80; i++) *scr++ = ' ';
	scr = savescr;
	for(i=0;  i<80 ; i++) {
		c = (int)*s++;
		switch(c) {
		case '\t' :     i +=(7 - (i%8));   break;
		case '\0' :
		case '\n' :     goto end_fil; break;
		default:
			scr[i] = (char)c;  break;
		}
	}
end_fil:
	cur_li += 1;
}

ins_hs()
/* ������������ ������ �� ������ � ������������� ����:
 *
 * ����� ������� ���� "��� = ��������� ;"
 * ��������� ��� � ��������� ������ � ����;
 * ������ ���������� ������� �������;
 * ����������� ����� � ������� ������� ����;
 * ����������:
 *      ������ �������� � �������� ����� �� �����������.
 */
{
	register char *from;
	register char *to;
	int     consts;         /* ����: ��������� ��������� */

	static  char    tmps[200];

	from = lbpo;

BEG_OF_SCAN:
	/* � ������ ������ ---LINES �.���� �� ���� �������� */
	if (*from == '\0')
		return;

	consts = 0;
	to = tmps;
	while(isspace(*from))
		from++;
	/* ����� "���=" */
	while(isnamec(*from))   *to++ = *from++;
	while(isspace(*from))   from++;

	if ((*from != '\0') && (*from != '=')) {
		fprintf(stderr, ediag(Esxerr,Rsxerr), ifn, nlin,
		ediag(" the '=' mast be after name.", " '=' ������ ���� ����� �����.") );
		return;
	} else {
		*to++ = *from++;
	}
	while(isspace(*from)) from++;
	while(*from) {
		if ( *from == '\n' ) *from = ';'; /* ����� ��� ���������� */
		if ( *from == ';' && !consts ) {
			if ( to != tmps ) {
				*to = ' ';
				/* ���������� ������� � ����� ������:  */
				while(isspace(*to) && tmps<=to)
					*to-- = '\0';
				/* �������� � ����, ���� �� ����� */
				if(tmps[0])
					put_hs(tmps, &shp);
				from++;
				goto BEG_OF_SCAN;
			} else  {
				from++;
			}
		} else
		if ( *from == '"' && from[1] == '"' ) {
			*to++ = *from++;
		} else
		if ( *from == '"' && from[1] != '"' ) {
			if (consts) consts = 0;
			else        { *to++ = *from++; consts = 1; }
		} else    ;
		*to++ = *from++;
	}
}

pass1()
/* ������ ������ :
 * ��������� � ���� ������ �� ������ PORTS, TABLES, LINES;
 * �������������, �������� � ���� �����;
 * ������������ �� ������ SCREEN;
 */
{
	register char *s;       /* ��������� �� ����. ����� */
	int skpflg;     /* ����: 1=���������� ������, 0=��������� */

	s = &lbpo[ strlen(vis_id) ];
	skpflg = 1;

	for ( nlin=1; fgets(lbpo, LBPOSIZ, ifp), feof(ifp) == 0; nlin++) {

		/* ���� com_id -- ������, ����� ��������� ������ */
		/* ��������� ������ � ������ scr_id,
		 * �� ���� ������������.
		 */
		if ( str_eq(lbpo, vis_id) ) {

			/*--- end_id -- ������������ */
			if      (str_eq( s, end_id )) {
				return;
			}
			/*--- ��� ������ ��� �� �������������� */
			else if (str_eq( s, hel_id )) {
				skpflg = 1; lbptyp = 0;
			}
			/*--- ��� ������ ���� ��������� */
			else if (str_eq( s, scr_id )) {
				skpflg = 0; lbptyp = 0;
			}
			else if (str_eq( s, por_id )) {
				skpflg = 0; lbptyp = DPORT;
			}
			else if (str_eq( s, tab_id )) {
				skpflg = 0; lbptyp = DTABLE;
			}
			else if (str_eq( s, lin_id )) {
				skpflg = 0; lbptyp = DLINE;
			}
			else if (str_eq( s, com_id )) {
				;       /* ������ �� �������� */
			}
			else {
				if (skpflg == 0 && lbptyp) {
					fprintf(stderr,
					"%s: %d: ������ � �����:%20.20s\n",
					ifn, nlin, lbpo);
					cnterr += 1;
					/* ���������� ����� ������ */
					skpflg = 1;
				} else
				if (skpflg == 0 && lbptyp == 0) {
					/* ��, ������. �� ������ ������
					 * ������ � �� �����... */
					fil_sc(); /* ������ ������ */
				} else {
					;
				}
			}
		} else {
			/* ������ ���� ��������� ������,
			 * ���� ������� ����
			 */
			if ( skpflg == 0 ) {
			    if (lbptyp)
				ins_hs();       /* �������� � ���� */
			    else
				fil_sc();       /* ��������� ������ ������ */
			}
		}
	} ;
	/* ��������� ���� ����� �� ����� scr_id */
	fprintf(stderr,
	"%s: %d: �� ������� ������ '%s%s'\n",
	ifn, nlin, vis_id, end_id );
	cnterr++;
}

