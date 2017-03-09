/*!
 * \file sum.c
 * \brief sum function named foo with MPI
 * \author zhpmatrix
 * \date 2017-03-09
 */

#include <stdio.h>
#include "mpi.h"
int foo(int i){return i;}

int main(int argc,char* argv[]){
	int i,tmp,sum=0,group_size,my_rank,N;

	// make test to send and recv single data
	int d_1,d_2;
	
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&group_size);
	MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	if(my_rank == 0){
		printf("Enter N:");scanf("%d",&N);
		d_2 = 100;
		for(i = 1;i < group_size;i++){
			MPI_Send(&N,1,MPI_INT,i,i,MPI_COMM_WORLD);
			MPI_Send(&d_2,1,MPI_INT,i,i,MPI_COMM_WORLD);
		}

		/************************************/
		
		// comment this code line to make workers do all things
		// need to change worker code as follows
		//for(i = my_rank;i < N;i += group_size){sum +=  foo(i);}
		
		/************************************/
		for(i = 1;i < group_size;i++){
			MPI_Recv(&tmp,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			MPI_Recv(&d_1,1,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			sum += tmp;
			printf("The rank=%d,The d_1=%d\n",i,d_1);
		}
		printf("The result=%d\n",sum);

	}else{
		MPI_Recv(&N,1,MPI_INT,0,my_rank,MPI_COMM_WORLD,&status);
		MPI_Recv(&d_2,1,MPI_INT,0,my_rank,MPI_COMM_WORLD,&status);
		printf("The d_2=%d",d_2);
		
		/************************************/
		// make groupsize -1 to alternate group_size
		for(i = my_rank;i < N;i += (group_size - 1)){sum +=  foo(i);}
		/************************************/

		MPI_Send(&sum,1,MPI_INT,0,my_rank,MPI_COMM_WORLD);
		d_1 = 9;
		MPI_Send(&d_1,1,MPI_INT,0,my_rank,MPI_COMM_WORLD);

	
	}
	MPI_Finalize();
	return 0;
}
