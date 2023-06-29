/*
**      +----------+    βιβμιοτελα χχοδα-χωχοδα
**     (c) linlib  !    δμρ αμζαχιτξο-γιζςοχωθ
**      +----------+    χιδεοτεςνιξαμοχ
**/

/*
 *      $Header: lin490.c,v 3.1 89/08/29 14:50:44 vsv Rel $
 *
 *      $Log:	lin490.c,v $
 * Revision 3.1  89/08/29  14:50:44  vsv
 * χεςσιρ LINLIB_3
 * 
 * Revision 3.0  88/06/16  18:06:19  vsv
 * πςεδχαςιτεμψξωκ χωπυσλ
 * 
 */

#include <ediag.h>
#include <stdio.h>
#include "line.h"

lerror(s)
/*-----------------------*/
/* αξαμοη ζυξλγιι perror */
/*-----------------------*/
register char *s;
{
	extern  int errno;              /* σιστενξωκ ξονες οϋιβλι */
	extern  int sys_nerr;           /* ναλσ. ξονες σοοβύεξιρ */
	extern  char *sys_errlist[];    /* αξημ. σπισολ σοοβύεξικ */
	extern  char *sys_rerrlist[];   /* ςυσσ. σπισολ σοοβύεξικ */

	char    outstr[STRLEN];

	if (errno <= sys_nerr ) {
		sprintf ( outstr,
		"CLIB: '%s' : %s",      s,
		ediag ( sys_errlist[ errno ], sys_rerrlist[ errno ] ) );
	} else {
		sprintf ( outstr, "CLIB: %d : %s ",
		ediag ( "Unknown error", "ξειϊχεστξαρ οϋιβλα" ) );
	}
	/* χωχοδ σοοβύεξιρ οβ Οϋιβλε */
	w_emsg ( outstr );
}
