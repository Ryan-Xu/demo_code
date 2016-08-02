#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>

#define handle_error_en(en, msg) \
				do { errno = en; perror(msg); exit(EXIT_FAILURE); } while(0);
				
struct thread_info {
	pthread_t thread_id;
	int thread_num;
	char *thread_para;
};

int num = 0;
pthread_mutex_t mutex;

static void *
thread_start(void *arg)
{
	struct thread_info *tinfo = (struct thread_info *)arg;
	
	pthread_mutex_lock(&mutex);
	num++;
	printf("Thread: (%d) get num value: %d\n", tinfo->thread_num, num);
	pthread_mutex_unlock(&mutex);
	
	pthread_exit((void *)"T1");
}

int main(int argc, char **argv)
{
		int s, tnum, opt, num_threads;
		struct thread_info *tinfo;
		pthread_attr_t attr;
		void *res;
		
		num_threads = 1;
    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch (opt) {
        case 'n':
            num_threads = strtoul(optarg, NULL, 0);
            break;

        default:
            fprintf(stderr, "Usage: %s [-n pthread-num] arg...\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
	
    tinfo = calloc(num_threads, sizeof(struct thread_info));
    if (tinfo == NULL)
        perror("calloc");
        
    s = pthread_attr_init(&attr);
    if (s != 0)
        handle_error_en(s, "pthread_attr_init");
        
    s = pthread_mutex_init(&mutex, NULL);
    if(s != 0)
    	 handle_error_en(s, "pthread_mutex_init");
    
    for(tnum = 0; tnum < num_threads; tnum++)
    {
    	s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    	if (s != 0)
      handle_error_en(s, "pthread_attr_setdetachstate");
    	
    	s = pthread_create(&tinfo[tnum].thread_id, &attr, &thread_start, &tinfo[tnum]);
    	if (s != 0)
            handle_error_en(s, "pthread_create");
    	tinfo[tnum].thread_num = tnum + 1;
    }
    
    s = pthread_mutex_destroy(&mutex);
    if(s != 0)
    	 handle_error_en(s, "pthread_mutex_destroy");
    	 
    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error_en(s, "pthread_attr_destroy");
    /*    
    for (tnum = 0; tnum < num_threads; tnum++) {
        s = pthread_join(tinfo[tnum].thread_id, &res);
        if (s != 0) {
        	if(s == EINVAL)
        		printf("thread is not a joinable thread.\n");
        	else
          	handle_error_en(s, "pthread_join");
        }
				else {
        	printf("Joined with thread %d; returned value was %s\n",
                tinfo[tnum].thread_num, (char *) res);
        	free(res);      
        }
    }
		*/
		sleep(1000);
		
    free(tinfo);
    exit(EXIT_SUCCESS);
    
	return 0;
}