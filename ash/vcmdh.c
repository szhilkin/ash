/*
 * ςαβοτα σ βυζεςον λοναξδξωθ στςολ.
 * ξεώτο ξαποδοβιε ιστοςιι, ϊαπονιξαΰτσρ
 * τομψλο υξιλαμψξωε λοναξδω.
 */

#include <string.h>
#include <stdio.h>
#include "line.h"       /* ζακμ-ϊαηομοχολ LINLIB */
#include "assist.h"

#ifdef TINYSMALL
#define CMDB 512        /* ςαϊνες βυζεςα λοναξδ */
#define CMDP 40         /* λομιώεστχο λοναξδ χ βυζεςε */
#define CMDHL 8         /* λομιώεστχο λοναξδ χ νεξΰ ξα όλςαξε */
#else
#define CMDB 8096       /* ΔΧΑ ΠΟΜΞΩΘ άΛÒΑΞΑΙΣΤΟÒΙΙ ΘΧΑΤΙΤ... */
#define CMDP 60         /* λομιώεστχο λοναξδ χ βυζεςε */
#define CMDHL 8         /* λομιώεστχο λοναξδ χ νεξΰ ξα όλςαξε */
#endif /* TINISMALL */

static  char    cmdb[CMDB+1];   /* βυζες λοναξδ */
static  char   *cmdp[CMDP+1];   /* υλαϊατεμι ξα λοναξδω */
static  int     cmdplast = 0;   /* ιξδελσ ποσμεδξεκ λοναξδω */
static  int     cmdpi = 0;      /* ιξδελσ ποσμεδξεκ χϊρτοκ/πομοφ. λοναξδω */
static  int     cmdbot = 0;     /* ιξδελσ σχοβοδξοηο νεστα χ βυζεςε */

int cmddel(cmd)
/*
 * υβςατψ ιϊ βυζεςα
 * ΧΟΪΧÒΑέΑΕΤΣΡ ΞΟΝΕÒ ΥΔΑΜΡΕΝΟΚ ΛΟΝΑΞΔΩ.
 */
char *cmd;
{
	register int i;
	int saven;      /* ΙΞΔΕΛΣ ΥΛΑΪΑΤΕΜΡ ΣΜΕΔΥΐέΕΚ ΛΟΝΑΞΔΩ */
	int delsize;    /* ÒΑΪΝΕÒ ΥΔΑΜΡΕΝΟΚ ΛΟΝΑΞΔΩ */

/***
	if (*cmd == '\0')
		return(-1);
 ***/
	for (i = 0; cmdp[i]; i++)
		if (strcmp(cmd, cmdp[i]) == 0) {
			delsize = strlen(cmd) + 1;
			/* σλοπιςυεν σοδεςφινοε βυζεςα χ ξοχοε νεστο */
			for (saven = i + 1; saven < cmdplast; i++,saven++) {
				cmdp[i] = cmdp[saven] - delsize;
				strcpy(cmdp[i], cmdp[saven]);
			}
			cmdbot -= delsize;
			saven = i; cmdplast--;
			while (i <= CMDP)
				cmdp[i++] = 0;
			return(saven);
		}
	return(-1);
}

cmdsqz(reqsz)
/*
 * πςοώιστιτψ σταςωε λοναξδω ι σφατψ */
/**/
int     reqsz;  /* ςαϊνες, λοτοςωκ τςεβυετσρ οσταχιτψ σχοβοδξων */
{
	register int i;
	int     realsz; /* ΟΣΧΟΒΟΦΔΑΕΝΩΚ ÒΑΪΝΕÒ */
	int     saven;  /* ΠÒΕΦΞΙΚ ΙΞΔΕΛΣ ΣΑΝΟΚ ΣΤΑÒΟΚ ΣΟΘÒΑΞΡΕΝΟΚ ΛΟΝΑΞΔΩ */

	/* ΕΣΜΙ ΝΕΣΤΟ ΕΣΤΨ, ΞΙήΕΗΟ ΞΕ ΔΕΜΑΤΨ */
	if ((reqsz < (CMDB - cmdbot)) && cmdplast < CMDP)
		return;

	/* ΠΟΣήΙΤΑΕΝ ΪΑΞΡΤΟΕ ΛΟΝΑΞΔΑΝΙ ΝΕΣΤΟ */
	i = realsz = 0;
	do {
		realsz += strlen(cmdp[i++]) + 1;
	} while (*cmdp[i] && ((CMDB - cmdbot) + realsz) < reqsz);
	saven = i;

	/* ΝΟΦΞΟ ΞΕ ΠÒΟΧΕÒΡΤΨ, ΕΣΜΙ ÒΑΪΝΕÒ ΒΥΖΕÒΑ ΞΕ ΝΕΞΨΫΕ ΔΧΥΘ ΣΤÒΟΛ: */
	/* ΠÒΟΧΕÒΙΤΨ, ÒΕΑΜΨΞΟ ΜΙ ΟΣΧΟΒΟΔΙΜΟΣΨ... */
	if (((CMDB - cmdbot) + realsz) < reqsz)
		/* ΥΞΙήΤΟΦΙΝ ΧΣΕ ΣΟΔΕÒΦΙΝΟΕ ΒΥΖΕÒΑ */
		i = cmdbot = cmdpi = 0;
	else {
		/* ΣΛΟΠΙÒΥΕΝ ΣΟΔΕÒΦΙΝΟΕ ΒΥΖΕÒΑ Χ ΞΟΧΟΕ ΝΕΣΤΟ */
		for (i = 0; saven < cmdplast; i++,saven++) {
			cmdp[i] = cmdp[saven] - realsz;
			strcpy(cmdp[i], cmdp[saven]);
		}
		cmdbot -= realsz;
	}
	cmdplast = i;
	if (cmdpi > cmdplast)   /* ΞΕ ΠÒΟΝΑΪΑΤΨ Σ ΤΕΛΥέΕΚ ΙΣΤΟÒΙΕΚ!!! */
		cmdplast = i;
	while (i <= CMDP)       /* ΪΑΞΥΜΙΤΨ ΞΕΪΑΞΡΤΩΕ ΥΛΑΪΑΤΕΜΙ */
		cmdp[i++] = 0;
}

int cmdput(cmd)
/*
 * πομοφιτψ χ βυζες
 */
char *cmd;
{
	int newsize;
	char *p;

	if (*cmd == 0)          /* ξιώεηο ξε δεματψ */
		return(0);
	cmddel(cmd);           /* υβςατψ σταςυΰ λοπιΰ */
	newsize = strlen(cmd) + 1;
	cmdsqz(newsize);

	/* πομοφιτψ ξοχυΰ λοναξδυ */
	p = cmdb;
	p += cmdbot;
	cmdp[cmdplast] = p;
	strcpy(cmdp[cmdplast], cmd);

	cmdplast++;
	cmdpi = cmdplast;       /* νοδιζιγιςοχατψ ιξδελσω */
	cmdbot += newsize;

	return(-1);
}

int cmdprv(cmd)
/*
 * χϊρτψ πςεδωδυύυΰ λοναξδυ ιϊ βυζεςα
 */
char *cmd;
{
	if (cmdpi > 0) cmdpi--;
	else    return(0);
	if (cmdp[cmdpi])
		strcpy(cmd, cmdp[cmdpi]);
	return(1);
}

int cmdnxt(cmd)
/*
 * χϊρτψ σμεδυΰύυΰ λοναξδυ ιϊ βυζεςα
 */
char *cmd;
{
	if (cmdp[cmdpi] && cmdplast > cmdpi)
		cmdpi++;
	if (cmdp[cmdpi]) {
		strcpy(cmd, cmdp[cmdpi]);
		return(1);
	}
	else    *cmd = 0;
	return(0);
}

/*
 * σΟΘÒΑΞΙΤΨ ΙΣΤΟÒΙΐ (ΒΥΖΕÒ ΛΟΝΑΞΔ) Χ ΖΑΚΜ $HOME/.ashhist
 * Α ΤΑΛΦΕ ΠΟΜΟΦΙΤΨ ΕΕ ΤΥΔΑ.
 * όΤΙ ΔΧΕ ΠΟΔΠÒΟΗÒΑΝΝΩ ΧΩΪΩΧΑΐΤΣΡ ΙΣΛΜΐήΙΤΕΜΨΞΟ ΙΪ main()
 */
char *hfile = "/.ashhist";

cmdghist(hdp)
char *hdp;
{
	FILE *fp;
	char filename[200];
	char cmd[140];
	int c;
	register char *p;

	strcpy(filename, hdp);
	strcat(filename, hfile);

	if ((fp = fopen(filename, "r")) == NULL)
		return(0);
	p = cmd;
	while ((c = getc(fp)) != EOF) {
		if ((*p++ = c) == '\n') {
			*(--p) = '\0';
			cmdput(cmd);
			p = cmd;
		}
	}
	fclose(fp);
	return(1);
}

cmdphist(hdp)
char *hdp;
{
	char filename[200];
	FILE *fp;
	register char **pp;

	strcpy(filename, hdp);
	strcat(filename, hfile);

	if ((fp = fopen(filename, "w")) == NULL)
		return(0);

	for (pp = cmdp; *pp != (char *)0; pp++)
		/*VARARGS*/
		fprintf(fp, "%s\n", *pp);

	fclose(fp);
	return(1);
}

static  char *cmdpp;

int t_hist(line, cod)
/*
 * τεστ δμρ στςολ βυζεςα ιστοςιι λοναξδ
 */
register LINE *line;
kbcod cod;
{
	char *cmd;

	cmd = *(char **)(line->varl);

	switch(cod) {
	case ' ':
		/* δοβαχιτψ λοναξδυ χ ςαβοώικ βυζες */
		strcat(cmdpp, cmd);
		break;
	case KB_NL:
		/* σλοπιςοχατψ λοναξδυ χ ςαβοώικ βυζες */
		strcpy(cmdpp, cmd);
		break;
	case KB_DE:
		/* υβςατψ λοναξδυ ιϊ πανρτι */
		if (cmdplast > 2 ) {
			cmddel(cmd);
			clritm();
			itmmax = cmdplast;
/***
			if (itm < cmdplast);
			else    itm = cmdplast - 1;
			if (itm > 1)
				itm -= 1;
 ***/
			pre_vf();
			itmshow();
			w_page(vf, 0);
		}
		else
			bell();
		break;
	}
	return (TRUE);
}

h_menu()
/*
 * ςαβοτα σ νεξΰ βυζεςα ιστοςιι λοναξδ.
 */
{
	register unsigned i;
	kbcod cod;

	/* ΠΕÒΧΟΞΑήΑΜΨΞΩΚ ΠΟΛΑΪ ΞΑ άΛÒΑΞΕ */
	cp_set(y0, 0, TXT);
	er_eop();
	w_cmd(cmdpp);   /* οσταχιτψ λοναξδυ ξα όλςαξε */
	itmshow();
	w_page(vf, 0);

	for ( ;; ) {

		i = itm - itmofs;
		cod = r_line( &vf[i], 0 );

		w_emsg("");

		switch (cod) {

		default:
			break;
		case KB_HE:      /* ΣΠÒΑΧΛΑ */
			cp_set(-1, 0, TXT);
			printf("λΟΝΑΞΔΑ # %2d ΙΪ %2d, %4d ΒΑΚΤ (%2d%%)",
			itm, cmdplast, cmdbot, (100*cmdbot)/CMDB);
			break;
		case KB_EX:      /* ΧΩΘΟΔ */
		case ' ':       /* ΔΟΒΑΧΙΤΨ */
		case KB_NL:      /* ΪΑΝΕΞΙΤΨ */
			return;

		case KB_RE:      /* ΠΕÒΕÒΙΣΟΧΛΑ */
			er_pag();
			cwdshow();
			w_emsg("");
			itmshow(); w_page(vf, 0);
			break;
#ifdef RETRO
		case KB_DE:
			if (itm - itmofs < 0)
				cod = KB_AU;
			else    break;
			/* ΠÒΟΧΑΜΙΧΑΕΝΣΡ... */
#else
		case KB_DE:
			cod = KB_AU;
			/* ΠÒΟΧΑΜΙΧΑΕΝΣΡ... */
#endif
		case KB_AL:
		case KB_AU:
		case KB_AD:
		case KB_AR:
			i = itmadj(cod);
			break;
		}
	}
}

static  LINE tmplate =
/*NOSTRICT*/
{ 16, 0, 0, 0,
       TXT|INP|PMT|NED|LFASTR,
	       0,
		       cvt_sp,
		       t_hist,
			       (char **)0 };

cmdvew(cmd)
char  *cmd;
{
	extern int  y0_top;     /* ΟΠÒΕΔΕΜΕΞΟ Χ vshcmd */
	extern char pmtsh[];    /* --"-- */
	LINEMENU savelm;

	if (cmdplast <= 1) {
		bell(); return;
	}
	savelm = clm;
	cp_set(y0-1, 0, TXT);   /* σοθςαξιτψ σχιτολ, σν. ξιφε */
	er_eop();

	/* ΙΞΙΓΙΑΜΙΪΑΓΙΡ ΝΕΞΐ ΛΟΝΑΞΔ */
	itms   = cmdp;          /* υλαϊατεμι ξα στςολι λοναξδ */
	itmmax = cmdplast;      /* ποσμεδξρρ λοναξδα χ ιστοςιι */
	vf     = (LINE *)0;
	itmlen = maxco - ((strlen(pmtsh)) * 2);
	ltmpl  = &tmplate;

	itm    = cmdpi;         /* τελυύαρ λοναξδα */
	if (cmdpi < cmdplast);
	else         itm -= 1;
	yy_max = 10;
	itmofs = 0;
	while((itm - itmofs) >= yy_max)
		itmofs += yy_max;
	itmini();
	pre_vf();

	/* σοθςαξιτψ σχιτολ */
	if (y0_top > y0) {
		y0_top = y0;
		scrlnl();
	}
	cmdpp = cmd;    /* δμρ λοπιςοχαξιρ ξοχοκ λοναξδω */

	h_menu();

	free((char *)vf); vf = (LINE *)0;
	cp_set(y0_top, 0, TXT); er_eop();

	cmdpi = itm;    /* ξοχοε ϊξαώ. ιξδελσα ιστοςιι */
	clm = savelm;
}
