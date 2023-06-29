/* tgoto [termcap]
**
** ��� ������������� ��������� � LINLIB v3.0
** ���� �� ��������� ��������� ������������� ������� %.
**
**/

#define	CTRL(c)	(('c') & 037)

char	*UP;
char	*BC;

/*
 * ��������� ��� ���������� ��������� �������
 * CM �������� ������� ��� ��������� ��������� �������.
 * The following escapes are defined for substituting row/column:
 *
 *	%d	as in printf
 *	%2	like %2d
 *	%3	like %3d
 *	%.	gives %c hacking special case characters
 *	%+x	like %c but adding x first
 *
 *	The codes below affect the state but don't use up a value.
 *
 *	%>xy	if value > x add y
 *	%r	reverses row/column
 *	%i	increments row/column (for one origin indexing)
 *	%%	gives %
 *	%B	BCD (2 decimal digits encoded in one byte)
 *	%D	Delta Data (backwards bcd)
 *
 * all other characters are ``self-inserting''.
 */
char *
tgoto(CM, destcol, destline)
	char *CM;
	int destcol, destline;
{
	static char bcount;             /* ������� ��������� ��� %. */
	static char result[64];
	static char added[10];
	char *cp = CM;
	register char *dp = result;
	register int c;
	int oncol = 0;
	register int which = destline;

	if (cp == 0) {
toohard:
		/*
		 * ``We don't do that under BOZO's big top''
		 */
		return ("OOPS");
	}
	added[0] = 0;
	while (c = *cp++) {
		if (c != '%') {
			*dp++ = c;
			continue;
		}
		switch (c = *cp++) {

#ifdef CM_N
		case 'n':
			destcol ^= 0140;
			destline ^= 0140;
			goto setwhich;
#endif

		case 'd':
			if (which < 10)
				goto one;
			if (which < 100)
				goto two;
			/* fall into... */

		case '3':
			*dp++ = (which / 100) | '0';
			which %= 100;
			/* fall into... */

		case '2':
two:	
			*dp++ = which / 10 | '0';
one:
			*dp++ = which % 10 | '0';
swap:
			oncol = 1 - oncol;
setwhich:
			which = oncol ? destcol : destline;
			continue;

#ifdef CM_GT
		case '>':
			if (which > *cp++)
				which += *cp++;
			else
				cp++;
			continue;
#endif

		case '+':
			which += *cp++;
			/* fall into... */

		case '.':
casedot:
			/*
			 * This code is worth scratching your head at for a
			 * while.  The idea is that various weird things can
			 * happen to nulls, EOT's, tabs, and newlines by the
			 * tty driver, arpanet, and so on, so we don't send
			 * them if we can help it.
			 *
			 * Tab is taken out to get Ann Arbors to work, otherwise
			 * when they go to column 9 we increment which is wrong
			 * because bcd isn't continuous.  We should take out
			 * the rest too, or run the thing through more than
			 * once until it doesn't make any of these, but that
			 * would make termlib (and hence pdp-11 ex) bigger,
			 * and also somewhat slower.  This requires all
			 * programs which use termlib to stty tabs so they
			 * don't get expanded.  They should do this anyway
			 * because some terminals use ^I for other things,
			 * like nondestructive space.
			 */
			bcount = 0;             /* ������� ��������� */
			switch( which )  {      /* ������� �������� �����... */
				case '\t'    :
				case 'n'&037 : bcount++;
				case 0       :
				case 'd'&037 :
				case '\n'    :
				case 'o'&037 : bcount++; break;
				default      : break;
			}
			while(bcount--) {       /* ���������� �������.. */
				strcat(added, oncol ? (BC ? BC : "\b") : UP);
				which++;
			}
			*dp++ = which;
			goto swap;

		case 'r':
			oncol = 1;
			goto setwhich;

		case 'i':
			destcol++;
			destline++;
			which++;
			continue;

		case '%':
			*dp++ = c;
			continue;

#ifdef CM_B
		case 'B':
			which = (which/10 << 4) + which%10;
			continue;
#endif

#ifdef CM_D
		case 'D':
			which = which - 2 * (which%16);
			continue;
#endif

		default:
			goto toohard;
		}
	}
	strcpy(dp, added);
	return (result);
}
