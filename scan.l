%{
#include "y.tab.c"
static int line = 1;

#ifdef SCANDEBUG
  #define RETURN(x) printf("%s: %s\n", #x, yytext); return x;
  #define DBEGIN(x) printf("<%s>\n", #x); BEGIN x;
#else
  #define RETURN(x) return x;
  #define DBEGIN(x) BEGIN x;
#endif

char* wkStringReturn(char* buf) {
  return buf;
}
%}

%s DATES FREELINE CARDIO STRUCTURED

%%
- { RETURN(DASH); }
to { RETURN(TO); }
: { RETURN(COLON); }
x { RETURN(BY); }
\+ { RETURN(PLUS); }
AM|PM { yylval.string = wkStringReturn(yytext); RETURN(AMPM); }
[0-9]+\.[0-9]+ { yylval.dval = atof(yytext); RETURN(DOUBLE); }
[0-9]+ { yylval.ival = atoi(yytext); RETURN(INTEGER); }
\([^\)]+\) { yylval.string = wkStringReturn(yytext + 1);
             yylval.string[yyleng - 2] = 0;
             RETURN(PTEXT); }
<STRUCTURED>^\n   { line = 1; DBEGIN(DATES); }
<CARDIO>^(None|No\ cardio)[^\n]*/\n { RETURN(NO); }
<FREELINE,CARDIO>[^\n]+/\n { yylval.string = wkStringReturn(yytext); RETURN(LINE); } 
^[^0-9\n][^\n]*/\n { yylval.string = wkStringReturn(yytext); RETURN(LINE); }
[\ \t]+    ;
\n         { switch(line) {
						   case 2: DBEGIN(FREELINE); break;
               case 3: DBEGIN(CARDIO); break;
               case 4: DBEGIN(FREELINE); break;
               case 6: DBEGIN(STRUCTURED); break;
             }
             line += 1; }
%%
