
#include<stdio.h>
//#include"matrixMath.c"

void m_fillCoFactor(float M[], float A[],int currRow,int currCol,int size)
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

float m_determinent(float M[],int size)
{
	if(size == 1) return M[0];
	float d = 0;
	int sign = 1;
	float temp[size*size];
	for( int i = 0 ; i < size ; i++)
	{
		m_fillCoFactor(M,temp,0,i,size);
		d += sign*M[0*size+i]*m_determinent(temp,size-1);
		sign = -1*sign;
	}
	return d;
}

void m_fillAdjoint(float M[],float A[],int size)
{
	if(size == 1) { A[0] = 1; return;}
	int sign = 1;
	float temp[size*size];

	for(int i =0; i < size;i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			m_fillCoFactor(M,temp,i,j,size);
			sign = (i+j)%2 == 0?1:-1;
			A[j*size+i] = (sign)*m_determinent(temp,size-1);
		}
	}

}

void matInvByFloat(float M[],int size, float ans[])
{
	float d = m_determinent(M,size);
	//printf("\ndeternimate %f \n",d);
	if(d == 0) { printf("\nSingular Matrix\n"); return;}

	float adj[size*size];
	m_fillAdjoint(M,adj,size);

	for(int i = 0; i < size ;i++)
	{
		for(int j = 0 ; j < size ; j++)
		{
			ans[i*size+j] = adj[i*size+j]/d;
		}
	}
}

void  matMulByFloat(float M1[],float M2[],int m1row,int m1col,int m2col,float ans[])
{
	float res;
	float temp;

	for(int i = 0 ; i < m1row; i++)
	{
		for(int k = 0 ; k < m2col; k++)
		{
			res = 0;
			for(int j = 0; j < m1col; j++)
			{
				temp = (M1[i*m1col+j])*(M2[j*m2col+k]);
				res += temp;
			}
			ans[i*m2col+k] = res;
		}
	}
}

void printMatByFloat(float M[],int row,int col)
{
	for(int i = 0 ; i < row; i++)
	{
		for(int j = 0 ; j < col; j++)
		{
			printf("%f  ",M[i*col+j]);
		}
		printf("\n");
	}
}
