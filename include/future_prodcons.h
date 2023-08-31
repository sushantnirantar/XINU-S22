#include <future.h>
#include <xinu.h>
uint future_prod(future_t* fut, char* value);
uint future_cons(future_t* fut);
void future_prodcons(int nargs, char *args[]);
void future_prodcons1(int nargs, char *args[]);
int future_fib(int nargs, char *args[]);
int future_free_test(int nargs, char *args[]);

extern char *val;
extern sid32 print_sem;