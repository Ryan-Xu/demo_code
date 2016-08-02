#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
               

static void *
thread_func(void *ignored_argument)
{
	int s;
	s = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
	if(s != 0)
		handle_error_en(s, "pthread_setcancelstate");
		
		
	printf("thread_func(): started; cancellation disabled\n");
  sleep(30);
  printf("thread_func(): about to enable cancellation\n");
  
  s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  if(s != 0)
  	handle_error_en(s, "pthread_setcancelstate");
  	
  sleep(1000);
  
  printf("thread_func(): not canceled!\n");
  return NULL;
}


int
main(void)
{
	pthread_t thr;
	void *res;
	int s;
	
	s = pthread_create(&thr, NULL, &thread_func, NULL);
	if(s != 0)
		handle_error_en(s, "pthread_create");
		
	sleep(2);
	
	printf("main(): sending cancellation request\n");
	s = pthread_cancel(thr);
	if(s != 0)
		handle_error_en(s, "pthread_cancel");
		
	s = pthread_join(thr, &res);
	if(s != 0)
		 handle_error_en(s, "pthread_join");
		 
	if(res == PTHREAD_CANCELED)
		 printf("main(): thread was canceled\n");
	else
		printf("main(): thread wasn't canceled (shouldn't happen!)\n");
  exit(EXIT_SUCCESS);
}
