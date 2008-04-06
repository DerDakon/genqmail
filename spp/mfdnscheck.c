// (C) 2007 Benedikt Boehm <hollow@gentoo.org>
//
// based on mfdnscheck (C) 2003-2004 Perolo Silantico <per.sil@gmx.it>
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
#include <errno.h>

#include <string.h>
#include <resolv.h>
#include <netdb.h>

#define OK             0
#define ERR_TEMPORARY -1
#define ERR_NOEXIST   -2

void nack(const char *msg)
{
	printf("E550 sorry, %s (#5.1.8)\n", msg);
	fprintf(stderr, "mfdnscheck: blocked with: %s\n", msg);
	exit(1);
}

void fail(const char *mse)
{
	printf("E451 %s (#4.4.0)\n", mse);
	fprintf(stderr, "mfdnscheck: temporary failure: %s\n", mse);
	exit(1);
}

int checkdns(char *hostname, int type)
{
	unsigned char buf[PACKETSZ];

	int len = res_query(hostname, C_IN, type, buf, sizeof(buf));

	if (len < 0) {
		if (errno == ECONNREFUSED || errno == TRY_AGAIN)
			return ERR_TEMPORARY;
		else
			return ERR_NOEXIST;
	}

	return OK;
}

int main(int argc, char *argv[])
{
	if (getenv("NOMFDNSCHECK"))
		return 0;

	char *from  = getenv("SMTPMAILFROM");
	char *nobnc = getenv("NOMFDNSBOUNCE");

	if (!(nobnc && strcmp(nobnc, "1") == 0))
		nobnc = NULL;

	if (!from || *from == '\0') {
		/* reject bounce if NOMFDNSBOUNCE is set to 1 */
		if (nobnc)
			nack("no envelope sender address.");
		return 0;
	}

	char *domain = strrchr(from, '@');

	if (!domain++ || *domain == '\0')
		nack("invalid envelope sender address.");

	int ret;
	if ((ret = checkdns(domain, T_MX)) != OK
	&&  (ret = checkdns(domain, T_A)) != OK) {
		switch (ret) {
		case ERR_TEMPORARY:
			fail("DNS temporary failure.");
			break;
		case ERR_NOEXIST:
			nack("envelope sender domain must exist.");
			break;
		}
	}

	return 0;
}
