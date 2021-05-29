%{
#include<stdio.h>

void yyerror(char *s);
int yylex();
float MATRIX1[100];
float MATRIX2[100];
float *MATRIX;
int m_index;
int m_row;
typedef int fixPoint;
fixPoint MATRIX3[100];
float expo_base;
float expo_pow;

extern fixPoint floatToFix(float a);
extern void matInvByFix(float A[], int n, fixPoint B[], int FIX_BITS);
extern void matMulByFix(float *A, float *B, int p,int q, int r, fixPoint *C, int FIX_BITS);
extern fixPoint expByFix(float a,float b,int FIX_BITS);
extern float fixToFloat(fixPoint p);
extern void printMatByFix(fixPoint M[],int p,int q);
%}
%start S
%union {float fnum;}
%token <fnum> NUMBER

%%

S	:matrix		{;}
  	|line		{;}
	;

line	:line NUMBER	{expo_pow = $2;}
     	|NUMBER		{expo_base = $1;}
	;

matrix	:'['arrays']'	{printf("\n matrix found\n");}
       	;

arrays	:array		{;}
	|arrays','array	{;}
	;
array	:'['list']'	{m_row++;}
      	;

list	:list','NUMBER	{ MATRIX[m_index++]=$3;}
	|NUMBER		{ MATRIX[m_index++]=$1;}
	;

%%
extern FILE* yyin;

void printMATRIX(float *mat, int r, int c)
{
//	printf(" row %d col %d",r, c);
	for(int i=0; i< r; i++)
	{
		for(int j=0; j< c; j++)
			printf("%f ", mat[i*c+j]);
		printf("\n");
	}
}

int mstrcmp(char*s, char*p)
{
  int i = 0;
  while(s[i] && s[i]==p[i]) i++;
  if(s[i]==p[i]) return 1;
  return 0;
  
}

int main(int argc, char** argv)
{
        int op = argv[1][0] - '0';
	if(op==1)
    	{
		printf("\nMATRIX MULTIPLICATION \n");
    		yyin = fopen(argv[2], "r");
		m_index = 0;
       		m_row = 0;
		MATRIX = MATRIX1;
		yyparse();
		int m_col1 =m_index/m_row;
		int m_row1 = m_row;
		printf("Matrix 1\n");
		printMATRIX(MATRIX1,m_row1,m_col1);
	        yyin = fopen(argv[3], "r");
		m_index = 0;
	        m_row = 0;
		MATRIX = MATRIX2;
		yyparse();
		int m_col2 = m_index/m_row;
		printf("Matrix 2\n");
		printMATRIX(MATRIX2,m_row,m_col2);
		if(m_row != m_col1)
			printf("In Compatible Dimensions"); 
		else
		{
			matMulByFix(MATRIX1, MATRIX2, m_row1,m_col1,m_col2, MATRIX3,8);
			printf("\nResult\n");
			printMatByFix(MATRIX3,m_col1,m_col2);
		}		
	}
        else if(op==2)
	{
		printf("MATRIX INVERSE \n");
    		yyin = fopen(argv[2], "r");
		m_index = 0;
		m_row = 0;
		MATRIX = MATRIX1;
		yyparse();
		printf("Matrix\n");
		printMATRIX(MATRIX1,m_row,m_row);
		matInvByFix(MATRIX1, m_row, MATRIX3, 8);
		printf("\nResult\n");
		printMatByFix(MATRIX3,m_row,m_row);
	}
	else if(op==3)
	{
		printf("\nEXPONENTIATION \n");	
	    	yyin = fopen(argv[2], "r");
		yyparse();
 		fixPoint p = expByFix(expo_base, expo_pow,8);
		printf("FixPoint base %f pow %f result %f\n", expo_base, expo_pow, fixToFloat(p));	
	}
    	else
     	{
       		printf("InValid Command\n");
     	}
}


void yyerror(char *s)
{
printf("\nInvalid%s\n",s);
}
