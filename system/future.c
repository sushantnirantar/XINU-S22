#include <future.h>
#include <xinu.h>
future_t* future_alloc(future_mode_t mode, uint size, uint nelem) {
  intmask mask;
  mask = disable();
  future_t *fut;
  fut=(future_t *)getmem((size*nelem)+sizeof(future_t));
  fut->data=getmem(nelem*size);
  fut->size=size;
  fut->mode=mode;
  fut->get_queue=newqueue();
  fut->max_elems=nelem;
  fut->count=0;
  fut->head=0;
  fut->tail=0;
  fut->set_queue=newqueue();
  fut->state=FUTURE_EMPTY;
  restore(mask);
  return fut;
}

syscall future_free(future_t *f){
  intmask mask;
  int statu;
  mask=disable();
  if(f->mode==FUTURE_EXCLUSIVE){
    delqueue(f->get_queue);
    delqueue(f->set_queue);
    statu=freemem(f,sizeof(future_t)+f->size+sizeof(f->get_queue)+sizeof(f->set_queue));
  }
  if(f->mode==FUTURE_SHARED){
    pid32 pid1;
    while((pid1=dequeue(f->get_queue))!=EMPTY){
      kill(pid1);
    }
    delqueue(f->get_queue);
    delqueue(f->set_queue);
    statu=freemem(f,sizeof(future_t)+f->size+sizeof(f->get_queue)+sizeof(f->set_queue));
  }
  if(f->mode==FUTURE_QUEUE){
    pid32 pid1;
    pid32 pid2;
    while((pid1=dequeue(f->get_queue))!=EMPTY){
      kill(pid1);
    }
    while((pid2=dequeue(f->set_queue))!=EMPTY){
      kill(pid2);
    }
    delqueue(f->get_queue);
    delqueue(f->set_queue);
    statu=freemem(f,sizeof(future_t)+f->size+sizeof(f->get_queue)+sizeof(f->set_queue));
  }
  restore(mask);
  if(statu){
    return OK;
  }else{
    return SYSERR;
  }

}

syscall future_get(future_t *f,char *out){
  intmask mask;
  mask=disable();
  if(f->state==FUTURE_EMPTY && f->mode==FUTURE_EXCLUSIVE){
    f->state=FUTURE_WAITING;
    f->pid=getpid();
    suspend(f->pid);
    memcpy(out,f->data,sizeof(f->size));
    restore(mask);
    return OK;
  }
  if(f->state==FUTURE_READY && f->mode==FUTURE_EXCLUSIVE){
    memcpy(out,f->data,sizeof(f->size));
    f->state=FUTURE_EMPTY;
    restore(mask);
    return OK;
  }
  if(f->state==FUTURE_WAITING && f->mode==FUTURE_EXCLUSIVE){
    f->state=FUTURE_WAITING;
    restore(mask);
    return SYSERR;
  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_EMPTY){
    f->state=FUTURE_WAITING;
    pid32 pid1=getpid();
    enqueue(pid1,f->get_queue);
    suspend(pid1);
    restore(mask);
    return OK;

  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_WAITING){
    f->state=FUTURE_WAITING;
    pid32 pid12=getpid();
    enqueue(pid12,f->get_queue);
    suspend(pid12);
    restore(mask);
    return OK;

  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_READY){
    f->state=FUTURE_READY;
    memcpy(out,f->data,sizeof(f->size));
    restore(mask);
    return OK;

  }
  if(f->mode==FUTURE_QUEUE && f->state==FUTURE_EMPTY){
    f->state=FUTURE_WAITING;
    pid32 pid3=getpid();
    enqueue(pid3,f->get_queue);
    suspend(pid3);
    char *headelemptr=f->data+(f->head*f->size);
    memcpy(out,headelemptr,f->size);
    f->head=((f->head)+1)%f->max_elems;
    f->count=(f->count)-1;
    pid32 pid4;
    if((pid4=dequeue(f->set_queue))!=EMPTY){
    resume(pid4);
    }
    
    restore(mask);
    return OK;
  }
  if(f->mode==FUTURE_QUEUE && f->state==FUTURE_WAITING){
    f->state=FUTURE_WAITING;
    pid32 pid5=getpid();
    enqueue(pid5,f->get_queue);
    suspend(pid5);
    char *headelemptr=f->data+(f->head*f->size);
    memcpy(out,headelemptr,f->size);
    f->head=((f->head)+1)%f->max_elems;
    f->count=(f->count)-1;
    
    pid32 pid10;
    if((pid10=dequeue(f->set_queue))!=EMPTY){

    resume(pid10);
    }

    restore(mask);
    return OK;
  }
  if(f->mode==FUTURE_QUEUE && f->state==FUTURE_READY){
    if(f->count==0){
      pid32 pid6=getpid();
      enqueue(pid6,f->get_queue);
      suspend(pid6);
    }
    char *headelemptr=f->data+(f->head*f->size);
    memcpy(out,headelemptr,f->size);
    f->head=((f->head)+1)%f->max_elems;
    f->count=(f->count)-1;
    
    pid32 pid7;
    if((pid7=dequeue(f->set_queue))!=EMPTY){

    resume(pid7);
  }
    
    restore(mask);
    return OK;
  }
  restore(mask);
  return SYSERR;
}

syscall future_set(future_t *f, char *in){
  int mask;
  mask=disable();
  if(f->state==FUTURE_EMPTY && f->mode==FUTURE_EXCLUSIVE){
    memcpy(f->data,in,f->size);
    f->state=FUTURE_READY;
    restore(mask);
    return OK;
  }
  if(f->state==FUTURE_READY && f->mode==FUTURE_EXCLUSIVE){
    restore(mask);
    return SYSERR;
  }
  if(f->state==FUTURE_WAITING && f->mode==FUTURE_EXCLUSIVE){
    memcpy(f->data,in,sizeof(f->size));
    f->state=FUTURE_EMPTY;
    resume(f->pid);
    restore(mask);
    return OK;
  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_EMPTY){
    memcpy(f->data,in,f->size);
    f->state=FUTURE_READY;
    restore(mask);
    return OK;

  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_WAITING){
    f->state=FUTURE_READY;
    memcpy(f->data,in,f->size);
    pid32 pid1;
    while((pid1=dequeue(f->get_queue))!=EMPTY){
      resume(pid1);
    }
    restore(mask);
    return OK;

  }
  if(f->mode==FUTURE_SHARED && f->state==FUTURE_READY){
    restore(mask);
    return SYSERR;

  }
  if(f->mode==FUTURE_QUEUE){
    f->state=FUTURE_READY;
    if(f->count==(f->max_elems)){
      pid32 pid8=getpid();
      enqueue(pid8,f->set_queue);
      suspend(pid8);
    }
    char *tailelemptr=f->data+(f->tail*f->size);
    memcpy(tailelemptr,in,f->size);
    f->tail=((f->tail)+1)%f->max_elems;
    f->count=(f->count)+1;
    pid32 pid9;
    if((pid9=dequeue(f->get_queue))!=EMPTY){

    resume(pid9);
  }
    
    restore(mask);
    return OK;

  }

  
  restore(mask);
  return SYSERR;
}

// TODO: write your code here for future_free, future_get and future_set