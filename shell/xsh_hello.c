/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <joinh.h>
/*------------------------------------------------------------------------
 * xsh_hello - print "Hello <Name>, Welcome to the world of Xinu!!"
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {



	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Syntax: run hello name\n");
		return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
		printf("Syntax: run hello name\n");
		return 0;
	}

	if (nargs < 0) {
		printf("Syntax: run hello name\n");
		return 0;
	}
	if (nargs == 0) {
		printf("Syntax: run hello name\n");
		return 0;
	}
	if (nargs == 1) {
		printf("Syntax: run hello name\n");
		return 0;
	}


	if (nargs == 2) {
		printf("Hello %s, Welcome to the world of Xinu!!\n", args[1]);
		signal(join_run);
		return 0;
	}


}
