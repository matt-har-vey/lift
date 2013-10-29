%{
#include "stdio.h"
extern int yylval;
%}
%%
20[0-9]+-[0-9]{2}-[0-9]{2} printf("DATE: %s\n", yytext);
([0-9]{1,2}:[0-9]{2}(A|P)M)\ to\ ([0-9]{1,2}:[0-9]{2}(A|P)M)  printf("TIMES: %s\n", yytext);
([1-9][0-9]*\+)?([1-9][0-9]*)x([0-9]+(\.[0-9]+)?) printf("SET: %s\n", yytext);
\((.+)\) printf("COMMENT: %s\n", yytext);
[A-z].*$ printf("EXERCISE: %s\n", yytext);
\ |\n ;
%%
