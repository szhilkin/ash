#include <stdio.h>
#include "line.h"

static  char    devtty[] = "/dev/tty";

vtty()
{
	if((vttout = fopen(devtty, "w")) == NULL) {
		fprintf(stderr, "%s: ξΕΔΟΣΤΥΠΕΞ\n", devtty);
		exit(1);
	}
	vtti = open(devtty, 0);
}
