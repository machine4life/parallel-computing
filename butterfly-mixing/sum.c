#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4
#define NUM_ITERS 3

int N;
int *X;
int gSum[NUM_THREADS];

void* Summation(void *pArg){
	int tNum= *((int *)pArg);
 	int lSum=0;
 	int start,end;
 	int i;
 	start = (N/NUM_THREADS)*tNum;
  	printf("start is:%d\n",start);
 	end = (N/NUM_THREADS)*(tNum+1);
 	printf("end is:%d\n",end);
 	if(tNum == NUM_THREADS -1)
  		end =N;
 	for(i=start;i<end;i++)
  		lSum+=X[i];
 	gSum[tNum]=lSum;
 	free(pArg);
	return 0;
}

void initArr(){
 	int i;
 	puts("input array length:：");
 	scanf("%d",&N);
 	X=(int*)malloc(N*sizeof(int));
 	for(i=0;i<N;i++){
   		X[i]=i+1;
   		printf("%d\t",X[i]);
 	}
}

int main(){
 	int sum=0;
 	pthread_t tHandles[NUM_THREADS];
 	initArr();
	for(int iter = 0;iter < NUM_ITERS;iter++){
		for(int i=0;i<NUM_THREADS;i++ ){
			int *threadNum =(int* )malloc(4);
			*threadNum=i;
			printf("threadNum is:%d\n",*threadNum);
			pthread_create(&tHandles[i],NULL,Summation,
							(void*)threadNum);
		}
		
		for(int j=0;j<NUM_THREADS;j++){
			pthread_join(tHandles[j],NULL);
			sum+= gSum[j];
		}
		printf("the sum of array elements is %d\n",sum);
	}
 	return 0;
}
