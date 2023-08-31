#include <xinu.h>
#include <prodcons.h>
#include <joinh.h>
#include <prodcons_bb.h>
void producer(int count) {
  // TODO: implement the following:
  // - Iterates from 0 to count (count including)
  //   - setting the value of the global variable 'n' each time
  //   - print produced value (new value of 'n'), e.g.: "produced : 8"
	int i;
	for(i=0;i<=count;i++){
		wait(can_read);
		n=i;
		printf("produced : %d\n",n);
		signal(can_write);
	}
	signal(join_prod);
	
}

void producer_bb(int id,int count){
	int i;
	int j;
	for(i=0;i<id;i++){
		for(j=0;j<count;j++){
			wait(can_read_bb);
			int ind=(head%5);
			arr_q[ind]=j;
			head++;
			printf("name : producer_%d, write : %d\n",i,j);
			signal(can_write_bb);
		}
	}
	signal(join_prod_bb);

}