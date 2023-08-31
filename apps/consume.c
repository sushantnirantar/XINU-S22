#include <xinu.h>
#include <prodcons.h>
#include <joinh.h>
#include <prodcons_bb.h>
void consumer(int count) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - reading the value of the global variable 'n' each time
  //   - print consumed value (the value of 'n'), e.g. "consumed : 8"
	int i;
	for(i=0;i<=count;i++){
		wait(can_write);
		printf("consumed : %d\n",n);
		signal(can_read);
	}
	signal(join_prod);
}

void consumer_bb(int id, int count){
	int i;
	int j;
	for(i=0;i<id;i++){
		for(j=0;j<count;j++){
			wait(can_write_bb);
			int ind=(tail%5);
			int rea=arr_q[ind];
			tail++;
			printf("name : consumer_%d, read : %d\n",i,rea);
			signal(can_read_bb);
		}
	}
	signal(join_prod_bb);
	
}


