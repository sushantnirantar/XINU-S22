/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>
#include <shprototypes.h>
#include <joinh.h>
#include <prodcons_bb.h>
#include <stdlib.h>
#include <future_prodcons.h>
#include <tscdfst.h>
#include <fs.h>
/*------------------------------------------------------------------------
 * xsh_run - execute other shell commands
 *------------------------------------------------------------------------
 */
sid32 join_run;
sid32 can_read_bb;
sid32 can_write_bb;
int arr_q[5];
int head;
int tail;
sid32 join_prod_bb;


void future_prodcons(int nargs,char *args[]){

	print_sem=semcreate(1);
	future_t *f_exclusive;
	f_exclusive=future_alloc(FUTURE_EXCLUSIVE,sizeof(int),1);
	char *val;
	int i=2;
	if(nargs<3){
		printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
		exit();
	}
	while(i<nargs){
		if(strcmp(args[i],"g")==0){
			i++;
			continue;
		}
		else if(strcmp(args[i],"s")==0){
				i++;
				if(i>=nargs){
					printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
					exit();

				}
				else if(strcmp(args[i],"")!=0||strcmp(args[i],"\0")!=0||strcmp(args[i]," ")!=0||strcmp(args[i],"g")!=0){
					i++;
					continue;
				}
				else{
				printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
				exit();

			}
		}
		else{
			printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
			exit();
		}
	}
	int num_args=i;
	i=2;
	val=(char *)getmem(num_args);
	while(i<nargs){
		if(strcmp(args[i],"g")==0){
			char id[10];
			sprintf(id,"fcons%d",i);
			resume(create(future_cons,2048,20,id,1,f_exclusive));
		}

		if(strcmp(args[i],"s")==0){
			i++;
			uint8 number=atoi(args[i]);
			val[i]=number;
			resume(create(future_prod,2048,20,"fprod1",2,f_exclusive,&val[i]));
			sleepms(5);
		}
		i++;
	}
	sleepms(100);
	future_free(f_exclusive);
}

void future_prodcons1(int nargs,char *args[]){

	print_sem=semcreate(1);
	future_t *f_q;
	char *val;
	int i=3;
	if(nargs<3){
		printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
		exit();
	}
	if(strncmp(args[2],"a",1)==0||strncmp(args[2],"b",1)==0||strncmp(args[2],"c",1)==0||strncmp(args[2],"d",1)==0||strncmp(args[2],"e",1)==0||strncmp(args[2],"f",1)==0||strncmp(args[2],"g",1)==0||strncmp(args[2],"h",1)==0||strncmp(args[2],"i",1)==0||strncmp(args[2],"j",1)==0||strncmp(args[2],"k",1)==0||strncmp(args[2],"l",1)==0||strncmp(args[2],"m",1)==0||strncmp(args[2],"n",1)==0||strncmp(args[2],"o",1)==0||strncmp(args[2],"p",1)==0||strncmp(args[2],"q",1)==0||strncmp(args[2],"r",1)==0||strncmp(args[2],"s",1)==0||strncmp(args[2],"t",1)==0||strncmp(args[2],"u",1)==0||strncmp(args[2],"v",1)==0||strncmp(args[2],"w",1)==0||strncmp(args[2],"x",1)==0||strncmp(args[2],"y",1)==0||strncmp(args[2],"z",1)==0){
		printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
		exit();

	}
	f_q=future_alloc(FUTURE_QUEUE,sizeof(int),atoi(args[2]));
	while(i<nargs){
		if(strcmp(args[i],"g")==0){
			i++;
			continue;
		}
		else if(strcmp(args[i],"s")==0){
				i++;
				if(i>=nargs){
					printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
					exit();

				}
				else if(strcmp(args[i],"")!=0||strcmp(args[i],"\0")!=0||strcmp(args[i]," ")!=0||strcmp(args[i],"g")!=0){
					i++;
					continue;
				}
				else{
				printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
				exit();

			}
		}
		else{
			printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
			exit();
		}
	}
	int num_args=i;
	i=3;
	val=(char *)getmem(num_args);
	while(i<nargs){
		if(strcmp(args[i],"g")==0){
			char id[10];
			sprintf(id,"fcons%d",i);
			resume(create(future_cons,2048,20,id,1,f_q));
		}

		if(strcmp(args[i],"s")==0){
			i++;
			uint8 number=atoi(args[i]);
			val[i]=number;
			resume(create(future_prod,2048,20,"fprod1",2,f_q,&val[i]));
			sleepms(5);
		}
		i++;
	}
	sleepms(100);
	future_free(f_q);
}

void prodcons_bb(int nargs, char *args[]){
	if(nargs>5){
		printf("Syntax: run prodcons_bb [# of producer processes] [# of consumer processes] [# of iterations the producer runs] [# of iterations the consumer runs]\n");
		signal(join_run);
	}
	if(nargs<5){
		printf("Syntax: run prodcons_bb [# of producer processes] [# of consumer processes] [# of iterations the producer runs] [# of iterations the consumer runs]\n");
		signal(join_run);
		
	}
	if(nargs==5){
		if(strncmp(args[1],"a",1)==0||strncmp(args[1],"b",1)==0||strncmp(args[1],"c",1)==0||strncmp(args[1],"d",1)==0||strncmp(args[1],"e",1)==0||strncmp(args[1],"f",1)==0||strncmp(args[1],"g",1)==0||strncmp(args[1],"h",1)==0||strncmp(args[1],"i",1)==0||strncmp(args[1],"j",1)==0||strncmp(args[1],"k",1)==0||strncmp(args[1],"l",1)==0||strncmp(args[1],"m",1)==0||strncmp(args[1],"n",1)==0||strncmp(args[1],"o",1)==0||strncmp(args[1],"p",1)==0||strncmp(args[1],"q",1)==0||strncmp(args[1],"r",1)==0||strncmp(args[1],"s",1)==0||strncmp(args[1],"t",1)==0||strncmp(args[1],"u",1)==0||strncmp(args[1],"v",1)==0||strncmp(args[1],"w",1)==0||strncmp(args[1],"x",1)==0||strncmp(args[1],"y",1)==0||strncmp(args[1],"z",1)==0
			||strncmp(args[2],"a",1)==0||strncmp(args[2],"b",1)==0||strncmp(args[2],"c",1)==0||strncmp(args[2],"d",1)==0||strncmp(args[2],"e",1)==0||strncmp(args[2],"f",1)==0||strncmp(args[2],"g",1)==0||strncmp(args[2],"h",1)==0||strncmp(args[2],"i",1)==0||strncmp(args[2],"j",1)==0||strncmp(args[2],"k",1)==0||strncmp(args[2],"l",1)==0||strncmp(args[2],"m",1)==0||strncmp(args[2],"n",1)==0||strncmp(args[2],"o",1)==0||strncmp(args[2],"p",1)==0||strncmp(args[2],"q",1)==0||strncmp(args[2],"r",1)==0||strncmp(args[2],"s",1)==0||strncmp(args[2],"t",1)==0||strncmp(args[2],"u",1)==0||strncmp(args[2],"v",1)==0||strncmp(args[2],"w",1)==0||strncmp(args[2],"x",1)==0||strncmp(args[2],"y",1)==0||strncmp(args[2],"z",1)==0
			||strncmp(args[3],"a",1)==0||strncmp(args[3],"b",1)==0||strncmp(args[3],"c",1)==0||strncmp(args[3],"d",1)==0||strncmp(args[3],"e",1)==0||strncmp(args[3],"f",1)==0||strncmp(args[3],"g",1)==0||strncmp(args[3],"h",1)==0||strncmp(args[3],"i",1)==0||strncmp(args[3],"j",1)==0||strncmp(args[3],"k",1)==0||strncmp(args[3],"l",1)==0||strncmp(args[3],"m",1)==0||strncmp(args[3],"n",1)==0||strncmp(args[3],"o",1)==0||strncmp(args[3],"p",1)==0||strncmp(args[3],"q",1)==0||strncmp(args[3],"r",1)==0||strncmp(args[3],"s",1)==0||strncmp(args[3],"t",1)==0||strncmp(args[3],"u",1)==0||strncmp(args[3],"v",1)==0||strncmp(args[3],"w",1)==0||strncmp(args[3],"x",1)==0||strncmp(args[3],"y",1)==0||strncmp(args[3],"z",1)==0
			||strncmp(args[4],"a",1)==0||strncmp(args[4],"b",1)==0||strncmp(args[4],"c",1)==0||strncmp(args[4],"d",1)==0||strncmp(args[4],"e",1)==0||strncmp(args[4],"f",1)==0||strncmp(args[4],"g",1)==0||strncmp(args[4],"h",1)==0||strncmp(args[4],"i",1)==0||strncmp(args[4],"j",1)==0||strncmp(args[4],"k",1)==0||strncmp(args[4],"l",1)==0||strncmp(args[4],"m",1)==0||strncmp(args[4],"n",1)==0||strncmp(args[4],"o",1)==0||strncmp(args[4],"p",1)==0||strncmp(args[4],"q",1)==0||strncmp(args[4],"r",1)==0||strncmp(args[4],"s",1)==0||strncmp(args[4],"t",1)==0||strncmp(args[4],"u",1)==0||strncmp(args[4],"v",1)==0||strncmp(args[4],"w",1)==0||strncmp(args[4],"x",1)==0||strncmp(args[4],"y",1)==0||strncmp(args[4],"z",1)==0){
			printf("Syntax: run prodcons_bb [# of producer processes] [# of consumer processes] [# of iterations the producer runs] [# of iterations the consumer runs]\n");
			signal(join_run);
			
		}
		else{
			join_prod_bb=semcreate(0);
			can_read_bb=semcreate(1);
			can_write_bb=semcreate(0);
			head=0;
			tail=0;
			int prod_bb=atoi(args[1]);
			int cons_bb=atoi(args[2]);
			int pro_th=atoi(args[3]);
			int con_th=atoi(args[4]);
			if((prod_bb*pro_th)!=(cons_bb*con_th)){
				printf("Iteration Mismatch Error: the number of producer(s) iteration does not match the consumer(s) iteration\n");
				signal(join_run);
				
			}
			else{
				resume(create(producer_bb,1024,20,"producer_bb",2,prod_bb,pro_th));
				resume(create(consumer_bb,1024,20,"consumer_bb",2,cons_bb,con_th));
				wait(join_prod_bb);
				wait(join_prod_bb);
				signal(join_run);

			}
			

		}
	
	
}
}
shellcmd xsh_run(int nargs, char *args[]) {



	/* Output info for '--help' argument */

	if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {
		printf("fstest\n");
		printf("futest\n");
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("tscdf\n");
		printf("tscdf_fq\n");
		return 0;
	}
	args++;
	nargs--;

	/* Check argument count */

	if(strcmp(args[0],"futest")==0){
		if(strcmp(args[1],"-pc")==0){
			future_prodcons(nargs,args);
			return 0;
		}
		if(strcmp(args[1],"--free")==0){
			return future_free_test(nargs,args);

		}
		if(strcmp(args[1],"-f")==0){
			return future_fib(nargs,args);

		}
		if(strcmp(args[1],"-pcq")==0){
			future_prodcons1(nargs,args);
			return 0;
		}
		else{
			printf("Syntax: run futest [-pc [g ...] [s VALUE ...]] | [-pcq LENGTH [g ...] [s VALUE ...]] | [-f NUMBER] | [--free]\n");
			return 0;
		}
		
		
	}

	if (strcmp(args[0],"hello")==0) {
		join_run=semcreate(0);
		resume(create((void *)xsh_hello,1024,20,"hello",2,nargs,args));
		wait(join_run);
		return 0;
	}

	if (strcmp(args[0],"fstest")==0) {
		return fstest(nargs,args);
		

	}

	if (strcmp(args[0],"tscdf")==0) {
		join_run=semcreate(0);
		resume(create((void *)stream_proc,4096,20,"tscdf",2,nargs,args));
		wait(join_run);
		return 0;
	}

	if (strcmp(args[0],"tscdf_fq")==0) {
		join_run=semcreate(0);
		resume(create((void *)stream_proc_futures,4096,20,"tscdf_fq",2,nargs,args));
		wait(join_run);
		return 0;
	}

	if (strcmp(args[0],"prodcons")==0) {
		join_run=semcreate(0);
		pid32 pid1=create((void *)xsh_prodcons,1024,20,"prodcons",2,nargs,args);
		resume(pid1);
		wait(join_run);
		return 0;
	}

	if (strcmp(args[0],"prodcons_bb")==0) {
		join_run=semcreate(0);
		pid32 pid1=create((void *)prodcons_bb,1024,20,"prodcons_bb",2,nargs,args);
		resume(pid1);
		wait(join_run);
		return 0;
	}

	if (strcmp(args[0],"prodcons")!=0&&strcmp(args[0],"list")!=0&&strcmp(args[0],"hello")!=0&&strcmp(args[0],"prodcons_bb")!=0) {
		printf("fstest\n");
		printf("futest\n");
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("tscdf\n");
		printf("tscdf_fq\n");
		return 0;
	}

	if (nargs < 0) {
		printf("fstest\n");
		printf("futest\n");
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("tscdf\n");
		printf("tscdf_fq\n");
		return 1;
	}
	if (nargs == 0) {
		printf("fstest\n");
		printf("futest\n");
		printf("hello\n");
		printf("list\n");
		printf("prodcons\n");
		printf("prodcons_bb\n");
		printf("tscdf\n");
		printf("tscdf_fq\n");
		return 0;
	}

}
