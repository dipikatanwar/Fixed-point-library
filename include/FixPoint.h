#include<stdio.h>
#define MAX_SIZE 100

typedef int fixPoint;
int FBITS;

fixPoint MAT1[MAX_SIZE*MAX_SIZE];
fixPoint MAT2[MAX_SIZE*MAX_SIZE];
fixPoint MAT3[MAX_SIZE*MAX_SIZE];

fixPoint floatToFix(float a);
float fixToFloat(fixPoint a);

void matInvByFix(float A[], int n, fixPoint B[], int FIX_BITS);
void matMulByFix(float *A, float *B, int p,int q, int r, fixPoint *C, int FIX_BITS);
fixPoint expByFix(float a,float b,int FIX_BITS);
float Error(fixPoint a,float b);

void matMulByFloat(float M1[],float M2[],int m1row,int m1col,int m2col,float ans[]);
void matInvByFloat(float M[],int size, float ans[]);


