// (C) 2007 Benedikt Boehm <hollow@gentoo.org>
//
// based on tarpit.c (C) 2003-2004 Perolo Silantico <per.sil@gmx.it>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>

#define MAX_DELAY 50

int main(int argc, char *argv[])
{
	char *_tarpitcnt = getenv("TARPITCOUNT");
	char *_rcpttocnt = getenv("SMTPRCPTCOUNT");
	char *_tarpitdel = getenv("TARPITDELAY");
	char *sender     = getenv("SMTPMAILFROM");

	/* do not block - invalid values  */
	if (!_tarpitcnt || !_rcpttocnt)
		return 0;

	int tarpitcnt = atoi(_tarpitcnt);
	int rcpttocnt = atoi(_rcpttocnt);
	int tarpitdel = _tarpitdel ? atoi(_tarpitdel) : 0;

	/* within limits - nothing to block */
	if (rcpttocnt <= tarpitcnt)
		return 0;

	/* determine tarpitdel time */
	if (tarpitdel <= 0) {
		if (!_tarpitdel)
			_tarpitdel = "RAND";

		if (strcasecmp(_tarpitdel, "NORMAL") == 0)
			tarpitdel = (rcpttocnt - tarpitcnt + 1) * 2;

		else if (strcasecmp(_tarpitcnt, "MEDIUM") == 0)
			tarpitdel = (rcpttocnt - tarpitcnt + 1) * 5;

		else if (strcasecmp(_tarpitcnt, "HARD") == 0) {
			tarpitdel = (rcpttocnt - tarpitcnt + 1);
			tarpitdel = tarpitdel * tarpitdel;
		}

		else {
			srand(time(NULL));
			tarpitdel = rand() % MAX_DELAY;
		}
	}

	if (tarpitdel > 0) {
		fprintf(stderr, "tarpit: delaying for %u seconds. sender: %s\n", tarpitdel, (sender ? sender : "(unkown)"));
		sleep(tarpitdel);
	}

	return 0;
}
