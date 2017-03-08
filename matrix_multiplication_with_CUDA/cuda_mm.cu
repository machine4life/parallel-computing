/*! 
* \brief Matrix multi with shared memory
*/

#include <stdio.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <time.h>
#include <stdlib.h>
 
__global__ void MatrixMuiOnDevice(int *M,int *N, int *P, int width)
{      
        int x = threadIdx.x;
        int y = threadIdx.y;
         
        float Pervalue = 0;
         
        for (int i = 0; i < width; i++)
        {      
                float Mdlement = M[y * width + i];
                float Ndlement = N[width * i + x];
                 
                Pervalue += Mdlement * Ndlement;
        }
         
        P[y * width + x] = Pervalue;
}
int main()
{      
        int a[30][30],b[30][30],c[30][30];
        int *M, *N, *P;
        int width = 30;
        int NUM = 900;
        dim3 dimBlock(30,30);
        cudaEvent_t start,stop;
        float elapsedTime;
        cudaEventCreate(&start);
        cudaEventCreate(&stop);
 
 
        cudaMalloc((void**)&M, 900*sizeof(int));
        cudaMalloc((void**)&N, 900*sizeof(int));
        cudaMalloc((void**)&P, 900*sizeof(int));
        for(int i = 0; i < 30; i++)
                for(int j = 0; j < 30; j++)
                {
                        a[i][j] = 2;
                        b[i][j] = 3;
                }
 
        cudaMemcpy(M,a,NUM*sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(N,b,NUM*sizeof(int),cudaMemcpyHostToDevice);
        cudaMemcpy(c,P,NUM*sizeof(int),cudaMemcpyDeviceToHost);
        cudaEventRecord(start,0);
        MatrixMuiOnDevice<<<1,dimBlock>>>(M,N,P,width);
        cudaThreadSynchronize();
        cudaEventRecord(stop,0);
        cudaEventSynchronize(stop);
        cudaEventElapsedTime(&elapsedTime,start,stop);
 
        printf("%f\n",elapsedTime);
        for(int i = 0; i < 30; i++)
                for(int j = 0; j < 30; j++)
                {
                        printf("%d \n",c[i][j]);
                }
 
        cudaFree(M);
        cudaFree(N);
        cudaFree(P);
        return 0;
}

