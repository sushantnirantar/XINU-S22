/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <prodcons.h>
#include <stdlib.h>
#include <joinh.h>
/*------------------------------------------------------------------------
 * xsh_prodcons - Producer/Consumer problem
 *------------------------------------------------------------------------
 */
int n;
sid32 can_read;
sid32 can_write;
sid32 join_prod;

shellcmd xsh_prodcons(int nargs, char *args[]) {
	int count=200;
	
	
	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Syntax: run prodcons [counter]");
		signal(join_run);
		return 0;
	}

	/* Check argument count */

	if (nargs > 2) {
		printf("Syntax: run prodcons [counter]");
		signal(join_run);
		return 0;
	}

	if (nargs < 0) {
		return 0;
	}
	if (nargs == 0) {
		printf("Syntax: run prodcons [counter]");
		signal(join_run);
		return 0;
	}
	if (nargs == 1) {
		can_read=semcreate(1);
		can_write=semcreate(0);
		join_prod=semcreate(0);
		resume(create(producer, 1024, 20, "producer", 1, count));
  		resume(create(consumer, 1024, 20, "consumer", 1, count));
  		wait(join_prod);
  		wait(join_prod);
  		signal(join_run);
  		return 0;
		
	}


	if (nargs == 2) {
		if(strncmp(args[1],"a",1)==0||strncmp(args[1],"b",1)==0||strncmp(args[1],"c",1)==0||strncmp(args[1],"d",1)==0||strncmp(args[1],"e",1)==0||strncmp(args[1],"f",1)==0||strncmp(args[1],"g",1)==0||strncmp(args[1],"h",1)==0||strncmp(args[1],"i",1)==0||strncmp(args[1],"j",1)==0||strncmp(args[1],"k",1)==0||strncmp(args[1],"l",1)==0||strncmp(args[1],"m",1)==0||strncmp(args[1],"n",1)==0||strncmp(args[1],"o",1)==0||strncmp(args[1],"p",1)==0||strncmp(args[1],"q",1)==0||strncmp(args[1],"r",1)==0||strncmp(args[1],"s",1)==0||strncmp(args[1],"t",1)==0||strncmp(args[1],"u",1)==0||strncmp(args[1],"v",1)==0||strncmp(args[1],"w",1)==0||strncmp(args[1],"x",1)==0||strncmp(args[1],"y",1)==0||strncmp(args[1],"z",1)==0){
			printf("Syntax: run prodcons [counter]");
			signal(join_run);
			return 0;
		}
		else{
		int k=atoi(args[1]);
		count=k;
		can_read=semcreate(1);
		can_write=semcreate(0);
		join_prod=semcreate(0);
		resume(create(producer, 1024, 20, "producer", 1, count));
  		resume(create(consumer, 1024, 20, "consumer", 1, count));
  		wait(join_prod);
  		wait(join_prod);
  		signal(join_run);
  		return 0;
  	}

	}
	semdelete(can_write);
	semdelete(can_read);
	semdelete(join_prod);


}
