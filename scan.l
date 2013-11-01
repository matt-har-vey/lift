%{
#include "y.tab.c"
static int line = 1;
%}

%s STRUCTURED FREELINE

%%
- return DASH;
to return TO;
: return COLON;
x return BY;
\+ return PLUS;
AM|PM { return AMPM; }
[0-9]+(\.[0-9]+)? { return NUMBER; }
\([^\)]+\) { int len = strlen(yytext);
             yylval.string = malloc((strlen(yytext) - 1) * sizeof(char));
             strncpy(yylval.string, yytext + 1, len-2);
             yylval.string[len-1] = 0;
             return PTEXT; }
<FREELINE>[^\n]+/\n { return LINE; } 
^[^0-9\n][^\n]*/\n { return LINE; }
[\ \t]+    ;
\n         { if (line == 2) { BEGIN FREELINE; } else if (line == 4) { BEGIN STRUCTURED; } line += 1; }
%%
