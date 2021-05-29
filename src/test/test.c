#include <stdio.h>
#include <stdlib.h>
#include"FixPoint.h"
#include <time.h>

fixPoint MAT[MAX_SIZE*MAX_SIZE];
float M1[MAX_SIZE*MAX_SIZE];
float M2[MAX_SIZE*MAX_SIZE];

float ANS[MAX_SIZE*MAX_SIZE];

void printMat(float M[],int r,int c)
{
	for(int i = 0; i< r;i++)
	{
		for(int j = 0 ; j < c; j++)
		{
			printf("%f  ",M[i*c+j]);
		}
		printf("\n");
	}
}

void MulErrorVsR()
{
	FILE * fp = freopen("../../data/ErrorVsFractionBits.txt","w",stdout);
	srand(time(0));
	
	int size = 4;
	for(int r = 0; r < 32; r++)
	{
		
		for(int i = 0; i < size ; i++)
		{
			for(int j = 0; j < size ; j++)
			{
				M1[i*size+j] = ((float)rand()/(float)(RAND_MAX)) * (5.0);
			}
		}
		matMulByFix(M1,M2,size,size,size,MAT,r);
		matMulByFloat(M1,M2,size,size,size,ANS);
		float sum = 0;
		for(int i = 0; i < size ; i++)
		{
			for(int j = 0; j < size ; j++)
			{
				sum += Error(MAT[i*size+j],ANS[i*size+j]);
			}
		}
		float meanError = sum/(float)(size*size);
		printf("%d\t%f\n",r,meanError);
	}
}

void MulTimeVsSize()
{
	srand(time(0));
	freopen("../../data/TimeVsSize.txt","w",stdout);
	clock_t start, end;
    	double timeused_fix,timeused_float;
	
	for(int size = 2; size < MAX_SIZE; size++)
	{
		for(int i = 0; i < size ; i++)
		{
			for(int j = 0; j < size ; j++)
			{
				M1[i*size+j] = ((float)rand()/(float)(RAND_MAX)) * (5.0);
				M2[i*size+j] = ((float)rand()/(float)(RAND_MAX)) * (5.0);
			}
		}
		start = clock();
		matMulByFix(M1,M2,size,size,size,MAT,16);
		end = clock();
		timeused_fix = ((((double) (end - start)) / CLOCKS_PER_SEC))*1000;
		matMulByFloat(M1,M2,size,size,size,ANS);
		end = clock();
		timeused_float = (((double) ((end - start)) / CLOCKS_PER_SEC))*1000;
		printf("%d\t%lf\t%lf\n",size,timeused_fix,timeused_float);
	}
}

int main()
{	
	MulTimeVsSize();
	MulErrorVsR();
	return 0;
}


