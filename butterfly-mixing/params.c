#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

struct argument
{
  int num;
  char string[30];
};

void *thread1_func( void * );
void *thread2_func( void * );

int main(){
	pthread_t thread1, thread2;
  	void *thread1_return, *thread2_return;
  	struct argument arg1, arg2;
  	
	int i;
  	int wait_thread_end;
  
  	arg1.num = 2011;
  	strcpy( arg1.string, "From Hello" );
  
  	arg2.num = 2017;
  	strcpy( arg2.string, "To World" );
  
  	pthread_create(&thread1, NULL, thread1_func, (void*)&arg1 );
  	pthread_create( &thread2, NULL, thread2_func, (void*)&arg2 );
  
  	for( i = 0; i < 2; i++ ){
    		printf("Ini process！\n");
    		sleep(2);
  	}
  
  	wait_thread_end = pthread_join( thread1, &thread1_return );
  	if( wait_thread_end != 0 ){
    		printf("Call thread 1 fail!\n");
  	}
  	else{
    		printf("Call thread 1 success! Return val is: %d\n", (int)thread1_return);
  	}
  
  	wait_thread_end = pthread_join( thread2, &thread2_return);
  	if( wait_thread_end != 0 ){
		printf("Call thread 2 fail!\n");
  	}
  	else{
		printf("Call thread 2 success! Return val is: %d\n",(int)thread2_return );
  	}

  	return EXIT_SUCCESS;
}

void *thread1_func( void *arg ){
	int i;
 	struct argument *arg_thread1;
	arg_thread1 = ( struct argument * )arg;
  
  	for( i = 0; i < 3; i++){
    		printf( "From thread 1，Params are %d, %s\n", arg_thread1->num, arg_thread1->string);
    		sleep(2);
  	}
  	return (void *)123;
}

void *thread2_func( void *arg ){
	int i;
  	struct argument *arg_thread2;
  	arg_thread2 = ( struct argument * )arg;
  	for( i = 0; i < 3; i++){
    		printf( "From thread 2，Params are %d, %s\n", arg_thread2->num, arg_thread2->string);
    		sleep(2);
  	}
 	return (void *)456;
}
