#include<stdio.h>
#include"FixPoint.h"

fixPoint floatToFix(float a){return (fixPoint)(a*(float)(1 << FBITS));}
float fixToFloat(fixPoint a){return (float)(a)/(float)(1 << FBITS);}

int q_part(fixPoint a) {return a >> FBITS;}
int r_part(fixPoint a) {return a/(1<<FBITS);}

fixPoint add(fixPoint a,fixPoint b){ return a+b; }
fixPoint sub(fixPoint a,fixPoint b){ return a-b; }
fixPoint mul(fixPoint a,fixPoint b) { return (fixPoint)((long)((long)a*(long)b) >> FBITS); }
fixPoint m_div(fixPoint a,fixPoint b) { return (fixPoint)((long)(((long)a << FBITS)/(long)b)); }
fixPoint expo(fixPoint a, fixPoint b);

float Error(fixPoint a,float b)
{
	float d = fixToFloat(a) - b;
	if(d < 0) return (float)(-1)*d;
	return d;
}

fixPoint root(fixPoint a,fixPoint b)
{
	fixPoint x[8];
	for(int i = 0 ; i < 8 ; i++)
		x[i] = floatToFix(1);

	for(int i = 0 ; i < 7 ; i++)
	{
		fixPoint bs = add(b,floatToFix(-1));// b-1
		x[i+1] = mul(m_div(floatToFix(1),b),add(mul(bs,x[i]),m_div(a,expo(x[i],bs))));
	}
	return x[7];
}

fixPoint expo(fixPoint a, fixPoint b){
	if( a == floatToFix(0)) return floatToFix(0); // zero raise to power something
	if( b == floatToFix(0)) return floatToFix(1); // anything raise to zero is one
	if( fixToFloat(b) == (float)1) return a; //anything raise to power 1

	if(b & (1 << 31)){

		fixPoint reci = expo(a,mul(b,floatToFix(-1)));
	     	fixPoint res = m_div(floatToFix(1),reci);// negative power is reciprocal
		return res;
	}
	if( fixToFloat(b) < 1) return root(a,m_div(floatToFix(1),b));

	if( (int)fixToFloat(b)%2 == 0) // if power 2 factor
	{
		fixPoint temp = expo(a,m_div(b,floatToFix(2)));
		return mul(temp,temp);
	}
	return mul(a,expo(a,add(b,floatToFix(-1))));
}

void fillCoFactor(fixPoint M[],fixPoint A[],int currRow,int currCol,int size)
{
	int ai = 0;
	for(int i= 0; i<size ;i++)
	{
		for(int j = 0 ; j < size ;j++)
		{
			if( currRow != i && currCol !=  j)
			{
				A[ai++] = M[i*size+j];
			}
		}
	}
}
fixPoint determinent(fixPoint M[], int size)
{
	if(size == 1) return M[0];
	fixPoint d = floatToFix(0);
	fixPoint sign = floatToFix(1);
	fixPoint temp[size*size];
	for( int i = 0 ; i < size ; i++)
	{
		fillCoFactor(M,temp,0,i,size);
		fixPoint t1 = mul(sign,mul(M[0*size+i],determinent(temp,size-1)));
		d = add(d,t1);
		sign = mul(floatToFix(-1),sign);
	}
	return d;
}

void fillAdjoint(fixPoint M[],fixPoint A[],int size)
{
	if(size == 1) { A[0] = floatToFix(1);}
	int sign = floatToFix(1);
	fixPoint temp[size*size];

	for(int i =0; i < size;i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			fillCoFactor(M,temp,i,j,size);
			sign = (i+j)%2 == 0?floatToFix(1):floatToFix(-1);
			A[j*size+i] = mul((sign),determinent(temp,size-1));
		}
	}
}

void matInvFix(fixPoint M[],int size, fixPoint ans[])
{
	fixPoint d = determinent(M,size);
	if(d == floatToFix(0)) { printf("\n Singular Matrix\n"); return;}

	fixPoint adj[size*size];// = (float*)malloc(size*size*sizeof(float));
	fillAdjoint(M,adj,size);

	for(int i = 0; i < size ;i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			ans[i*size+j] = m_div(adj[i*size+j],d);
		}
	}
}

void matMulFix(fixPoint A[], fixPoint B[], int p,int q, int r, fixPoint C[])
{
	fixPoint res;
	fixPoint temp;

	for(int i = 0 ; i < p; i++)
	{
		for(int k = 0 ; k < r; k++)
		{
			res = floatToFix(0);
			for(int j = 0; j < q; j++)
			{
				temp = mul((A[i*q+j]),(B[j*r+k]));
				res = add(res,temp);
			}
			C[i*r+k] = res;
		}
	}
}

void printMatByFix(fixPoint M[],int row,int col)
{
	for(int i = 0 ; i < row; i++)
	{
		for(int j = 0 ; j < col; j++)
		{
			float ans = fixToFloat(M[i*col+j]);				
			printf("%f  ",ans);
		}
		printf("\n");
	}
}

fixPoint expByFix(float a,float b,int FIX_BITS)
{
	FBITS = FIX_BITS;
	return expo(floatToFix(a),floatToFix(b));
}

void matMulByFix(float *A, float *B, int p,int q, int r, fixPoint *C, int FIX_BITS)
{
	FBITS = FIX_BITS;
	for(int i=0; i < p; i++)for(int j=0; j< q; j++)MAT1[i*q+j] = floatToFix(A[i*q+j]);
	for(int i=0; i < q; i++)for(int j=0; j< r; j++)MAT2[i*r+j] = floatToFix(B[i*r+j]);
	matMulFix(MAT1, MAT2, p,q, r, MAT3);
	for(int i=0; i < p; i++)for(int j=0; j< r; j++) C[i*r+j] = MAT3[i*r+j];
}

void matInvByFix(float A[], int n, fixPoint B[], int FIX_BITS)
{
	FBITS = FIX_BITS;
	for(int i=0; i < n; i++)for(int j=0; j< n; j++)MAT1[i*n+j] = floatToFix(A[i*n+j]);
	matInvFix(MAT1,n,MAT3);
	for(int i=0; i < n; i++)for(int j=0; j< n; j++)B[i*n + j] = MAT3[i*n+j];
}
