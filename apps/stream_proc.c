#include <xinu.h>
#include <stdio.h>
#include <tscdfst.h>
#include <joinh.h>
#include "tscdf.h"

uint pport;

//declarinng global variables to hold num_streams,time_window and output_time
int g_num_streams,g_work_queue_depth,g_time_window,g_output_time;

void stream_consumer(int32 id, struct stream *str) {

  // TODO: Print the current id and pid
	struct tscdf *tc;
	int tail;
	int count=1;
	kprintf("stream_consumer id:%d (pid:%d)\n",id,getpid());
	tc=tscdf_init(g_time_window);
	while(1){
		wait(str->spaces);
		wait(str->mutex);
		tail=str->tail;
		//checking if it's the endpoint of stream ts=0 and v=0
		if(str->queue[tail].time==0 && str->queue[tail].value==0){
			kprintf("stream_consumer exiting\n");
			break;
		}

		tscdf_update(tc,str->queue[tail].time,str->queue[tail].value);
		if(count==g_output_time){
			char output[10];
			int * qarray;
			qarray = tscdf_quartiles(tc);

			if (qarray == NULL) {
  				kprintf("tscdf_quartiles returned NULL\n");
  				continue;
			}

			sprintf(output, "s%d: %d %d %d %d %d", id, qarray[0], qarray[1], qarray[2], qarray[3], qarray[4]);
			kprintf("%s\n", output);
			freemem((char *) qarray, (6*sizeof(int32)));
			count=0; //resetting count for the next tscdf calculation after output_time
		}
		count++;
		tail++;
		tail=tail%g_work_queue_depth;
		str->tail=tail;
		signal(str->mutex);
		signal(str->items);
	}
	tscdf_free(tc);
	ptsend(pport,getpid());


}

int stream_proc(int nargs, char* args[]) {
	//for timing the stream_proc
	ulong secs,msecs,time;
	secs=clktime;
	msecs=clkticks;
	//argument parser code
	char usage[] = "Usage: run tscdf -s <num_streams> -w <work_queue_depth> -t <time_window> -o <output_time>\n";
	int num_streams,work_queue_depth,time_window,output_time;
	int i;
	char *ch, c;
	if (nargs != 9) {
  		printf("%s", usage);
  		return SYSERR;
	} else {
  		i = nargs - 1;
  		while (i > 0) {
    		ch = args[i - 1];
    		c = *(++ch);

    		switch (c) {
      		case 's':
        	num_streams = atoi(args[i]);
        	break;

      		case 'w':
        	work_queue_depth = atoi(args[i]);
        	break;

      		case 't':
        	time_window = atoi(args[i]);
        	break;

      		case 'o':
        	output_time = atoi(args[i]);
        	break;

      		default:
        	printf("%s", usage);
        	return SYSERR;
    }

    i -= 2;
  }
}

	//initializing global variables g_num_streams,g_time_window,g_output_time
	g_num_streams=num_streams;
	g_output_time=output_time;
	g_time_window=time_window;
	g_work_queue_depth=work_queue_depth;
	struct stream **s;
	//allocating memory for stream and buffer
	//pointer to pointer for all streams
	s=(struct stream **)getmem(sizeof(struct stream *)*(num_streams));
	if(s==(struct stream **)SYSERR){
		kprintf("get_mem_failed_pointers\n");
		signal(join_run);
		return SYSERR;
		}
	for(i=0;i<num_streams;i++){
		s[i]=(struct stream *)getmem(sizeof(struct stream)+(sizeof(de)*work_queue_depth));
		if(s[i]==(struct stream *)SYSERR){
			kprintf("get_mem_failed_stream\n");
			signal(join_run);
			return SYSERR;
		}
		s[i]->items=semcreate(work_queue_depth);
		s[i]->mutex=semcreate(1);
		s[i]->spaces=semcreate(0);
		s[i]->head=0;
		s[i]->tail=0;
		s[i]->queue=(sizeof(struct stream)+(char *)s[i]); //include offset
}
//spawning consumer processes and initializing port to hold 'num_streams' messages

	pport=ptcreate(num_streams);
	if(pport==SYSERR){
		signal(join_run);
		return SYSERR;
	}
	for(i=0;i<num_streams;i++){
		resume(create((void *)stream_consumer,4096,20,"stream_consumer",2,i,s[i]));
	}

	int st,ts,v;
	char *a;
	for(i=0;i<n_input;i++){
		a = (char *) stream_input[i];
		st = atoi(a); //stream 'id'
		while (*a++ != '\t');
		ts = atoi(a);
		while (*a++ != '\t');
		v = atoi(a);
		wait(s[st]->items);
		wait(s[st]->mutex);
		int head=s[st]->head;
		s[st]->queue[head].time=ts;
		s[st]->queue[head].value=v;
		head++;
		head=head%work_queue_depth;
		s[st]->head=head;
		signal(s[st]->mutex);
		signal(s[st]->spaces);
	}

	//getting proc_id from port messages
	uint proc_id;
	for(i=0;i<num_streams;i++){
		proc_id=ptrecv(pport);
		kprintf("process %d exited\n",proc_id);
	}
	ptdelete(pport,0);

	time = (((clktime * 1000) + clkticks) - ((secs * 1000) + msecs));
  	printf("time in ms: %u\n", time);
  	signal(join_run);
  	return OK;
}


