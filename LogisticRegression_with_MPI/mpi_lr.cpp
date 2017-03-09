/*!
* \brief parallel logistic regression with mpi to deal with classification
* \..... using batch gradient descent and only output final weights vector(m* \..... atrix)
* \author zhpmatrix
* \date 2017-03-09
*/

#include "mpi.h"
#include <omp.h>

#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

#define N 100

#define NRA 100          	/* number of rows in matrix A(training matrix)*/
#define NCA 3            	/* number of columns in matrix A */
#define NCB 1                 	/* number of columns in matrix B(weight matrix) */
#define MASTER 0               	/* taskid of first task */
#define FROM_MASTER 1          	/* setting a message type */
#define FROM_WORKER 2          	/* setting a message type */

// load data from file
void loadData(MatrixXd& X,MatrixXd& y,const string filename){
	ifstream file(filename);string line;
	for(int i = 0;i < N;i++){
		getline(file,line);
		double x0=1.0,x1,x2;int label;
		sscanf(line.c_str(),"%lf  %lf  %d",&x1,&x2,&label);
		X(i,0) = x0;X(i,1) = x1,X(i,2) = x2;
		y(i,0) = label;
	}

}

// sigmoid data from matrix
void sigmoid(MatrixXd& mat){
	for(int i = 0;i < mat.rows();i++){
		for(int j = 0;j < mat.cols();j++){
			double dat = mat(i,j);
			mat(i,j) = exp(dat)/(1+exp(dat));
		}
	}
}


void mat2Arr_3(MatrixXd& X,double arr[][3],int rows,int cols){
	for(int i = 0;i < rows;i++){
		for(int j = 0;j < cols;j++){
			arr[i][j] = X(i,j);
		}
	}
}

void mat2Arr_1(MatrixXd& X,double arr[][1],int rows,int cols){
	for(int i = 0;i < rows;i++){
		for(int j = 0;j < cols;j++){
			arr[i][j] = X(i,j);
		}
	}
}

void arr2Mat_3(double arr[][3],MatrixXd& X,int rows,int cols){
	for(int i = 0;i < rows;i++){
		for(int j = 0;j < 3;j++){
			X(i,j) = arr[i][j];
			//cout << X(i,j) << endl;
		}
	}

}

void arr2Mat_1(double arr[][1],MatrixXd& X,int rows,int cols){
	for(int i = 0;i < rows;i++){
		for(int j = 0;j < 1;j++){
			X(i,j) = arr[i][j];
			//cout << X(i,j) << endl;
		}
	}

}

int main(int argc,char* argv[]){

	int numtasks,         	 	/* number of tasks in partition */
  	taskid,                		/* a task identifier */
  	numworkers,            		/* number of worker tasks */
  	source,                		/* task id of message source */
  	dest,                  		/* task id of message destination */
  	mtype,                 		/* message type */
  	rows,                  		/* rows of matrix A sent to each worker */
	averow,extra,offset,  	        /* used to determine rows sent to each worker */
  	i, j, k,rc;                	/* misc */
	
	double  a[NRA][NCA],   /* training data without label */
  	b[NCA][NCB],           /* weight vector  */
  	c[NRA][NCB],           /* label */
        dw[3][1];	       /* temp weight matrix*/

	string filename="train.txt";  		/* training data file */
	int maxIterNum = 500;          		/* max iter times */
	double learnRate = 0.001;     		/* learning rate for gradient descent*/	
	double w[3][1] = {1.0,1.0,1.0}; 	/* initial weight matrix*/
	
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	if (numtasks < 2 ) {
  		printf("Need at least two MPI tasks. Quitting...\n");
  		MPI_Abort(MPI_COMM_WORLD, rc);
  		exit(1);
  	}
	numworkers = numtasks-1;
	
	for(int iter = 0;iter < maxIterNum;iter++){	
   		// master task
   		if (taskid == MASTER){
       			printf("mpi_mm has started with %d tasks.\n",numtasks);
      			printf("Initializing arrays...\n");
			
			MatrixXd X(N,3),y(N,1);
			loadData(X,y,filename);
	
			mat2Arr_3(X,a,100,3);
			mat2Arr_1(y,c,N,1);
      
      			/* Send matrix data to the worker tasks */
      			averow = NRA/numworkers;
      			extra = NRA%numworkers;
      			offset = 0;
      			mtype = FROM_MASTER;
      			for (dest=1; dest<=numworkers; dest++){
         			rows = (dest <= extra) ? averow+1 : averow;    
         			printf("Sending %d rows to task %d offset=%d\n",rows,dest,offset);
				MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
				MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);
				MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,MPI_COMM_WORLD);
				MPI_Send(&c[offset][0], NRA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
				MPI_Send(&w, 3, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
				offset = offset + rows;
      			}//for

      			/* Receive results from worker tasks */
      			mtype = FROM_WORKER;
      			for (i=1; i<=numworkers; i++){
         			source = i;
         			MPI_Recv(&dw, 3*1, MPI_DOUBLE, source, mtype, MPI_COMM_WORLD, &status);
				// update weight
	 			for(int k = 0;k < 3;k++){w[k][0] += dw[k][0];}
      			}
      			cout << "iter=" << iter << " w=\n" << w[0][0] <<","<< w[1][0] <<","<< w[2][0] <<endl;
   		}//if
   		
		//worker task
		if (taskid > MASTER){
		      mtype = FROM_MASTER;
		      MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		      MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);
		      MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		      MPI_Recv(&c, NRA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		      MPI_Recv(&w, 3*1, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD, &status);
		      
		      MatrixXd deltaW(3,1);arr2Mat_1(dw,deltaW,3,1);
		      MatrixXd ma(rows,3);arr2Mat_3(a,ma,rows,3);
		      
		      //weight
		      MatrixXd mb(3,1);arr2Mat_1(w,mb,3,1);
		      //label
		      MatrixXd mc(rows,NCB);arr2Mat_1(c,mc,rows,NCB);

		      MatrixXd tmp(rows,1);tmp = ma*mb;sigmoid(tmp);

		      // update weight according to data in current process
		      deltaW = learnRate*ma.transpose()*(mc-tmp);
		      
		      mat2Arr_1(deltaW,dw,3,1);
		      
		      mtype = FROM_WORKER;
		      MPI_Send(&dw, 3*1, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
		   }//if
   	}//for
   	
	MPI_Finalize();
   	return 0;
}
