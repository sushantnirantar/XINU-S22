/* Global variable for producer consumer */
extern int arr_q[5]; /* this is just declaration */
sid32 can_read_bb;
sid32 can_write_bb;
extern int head;
extern int tail;
/* Function Prototype */
void consumer_bb(int id,int count);
void producer_bb(int id,int count);
