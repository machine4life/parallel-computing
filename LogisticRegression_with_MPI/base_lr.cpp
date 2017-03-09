/*!
* \brief Logistic Regression For Classification
* \author zhpmatrix
* \date 2017-03-08
*/

#include <fstream>
#include <sstream>
#include <iostream>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

#define N 100
#define X_1_MIN -1
#define X_1_MAX 1
#define X_2_MIN -1
#define X_2_MAX 1

void loadData(MatrixXd& X,MatrixXd& y){
	for(int i = 0;i < N;i++){
		X(i,0) = 1.0;
		X(i,1) = double(X_1_MAX - X_1_MIN) * rand()/RAND_MAX - (X_1_MAX - X_1_MIN)/2.0;
		X(i,2) = double(X_2_MAX - X_2_MIN) * rand()/RAND_MAX - (X_2_MAX - X_2_MIN)/2.0;
		y(i,0) = rand() % 2;
	}
}

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

void sigmoid(MatrixXd& mat){
	for(int i = 0;i < mat.rows();i++){
		for(int j = 0;j < mat.cols();j++){
			double dat = mat(i,j);
			mat(i,j) = exp(dat)/(1+exp(dat));
		}
	}
}

void gradAscent(MatrixXd& X,MatrixXd& y,MatrixXd& w){
	int maxIterNum = 500;
	double learnRate = 0.001;
	for(int i = 0;i < maxIterNum;i++){
		MatrixXd tmp(N,1);tmp = X*w;sigmoid(tmp);
		w += learnRate*X.transpose()*(y - tmp);
	}
}

int main(){
	//srand( (unsigned)time(NULL) );
	string filename="train.txt";
	MatrixXd X(N,3),y(N,1);
	loadData(X,y);
	MatrixXd w(3,1);w << 1.0,1.0,1.0;
	gradAscent(X,y,w);
	return 0;
}
