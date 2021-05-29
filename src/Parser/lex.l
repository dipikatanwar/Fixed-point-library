%{
#include<stdio.h>
#include "y.tab.h"
%}

%%
[+-]?[0-9]*[.]?[0-9]+	{yylval.fnum=atof(yytext); return NUMBER;}
[,\[\]]			{return yytext[0];}
[ \t]			;
[\n]			return 0;
.			printf(" invalid \n");
%%
int yywrap()
{
return 1;
}
