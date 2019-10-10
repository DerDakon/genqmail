// (C) 2007 Benedikt Boehm <hollow@gentoo.org>
//
// based on authlogger.c (C) 2006 Tomislav Randjic
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

int main(int argc, char *argv[])
{
	char *user = getenv("SMTPAUTHUSER");
	char *meth = getenv("SMTPAUTHMETHOD");
	char *rip  = getenv("TCPREMOTEIP");
	int ppid   = getppid();

	if (user && *user != '\0')
		fprintf(stderr, "smtp-login: Login: <%s>, method=%s, rip=%s, pid=%d\n", user, meth, rip, ppid);

	return 0;
}
