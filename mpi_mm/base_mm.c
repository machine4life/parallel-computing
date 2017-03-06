#include <stdio.h>
#include <time.h>

#define NRA 1000
#define NCA 250
#define NCB 200

void show(double c[NRA][NCB]){
      /* Print results */
      printf("******************************************************\n");
      printf("Result Matrix:\n");
      int i,j;
      for (i=0; i<NRA; i++)
      {
         printf("\n"); 
         for (j=0; j<NCB; j++) 
            printf("%6.2f   ", c[i][j]);
      }
      printf("\n******************************************************\n");
      printf ("Done.\n");
}
void mm(double a[NRA][NCA],double b[NCA][NCB],double c[NRA][NCB]){
	int i,j,k;
	double tmp;
	for(i = 0;i < NRA;i++){
		for(j = 0;j < NCB;j++){
			tmp = 0.0;
			for(k = 0;k < NCA;k++){
				tmp += a[i][k] * b[k][j];
			}
			c[i][j] = tmp;
		}
	}	
}
int main(){
	int i,j;
	int startTime,endTime; /* get running time */
	double  a[NRA][NCA],   /* matrix A to be multiplied */
  	b[NCA][NCB],           /* matrix B to be multiplied */
  	c[NRA][NCB];           /* result matrix C */
        printf("Initializing arrays...\n");
        for (i=0; i<NRA; i++){
          for (j=0; j<NCA; j++){
             a[i][j]= i+j;
	  }
        }
        for (i=0; i<NCA; i++){
          for (j=0; j<NCB; j++){
            b[i][j]= i*j+1;
	 }
       }
       startTime = clock();
       mm(a,b,c);
       endTime = clock();
       printf("time is %d\n",endTime - startTime);
       //show(c);
       return 0;
}
