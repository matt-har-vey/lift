%{
#include "y.tab.c"
static int line = 1;

#define PTEXT_BUF_SIZE   4096
static char ptext_buf[PTEXT_BUF_SIZE];

#ifdef SCANDEBUG
  #define RETURN(x) printf("%s: %s\n", #x, yytext); return x;
  #define DBEGIN(x) { printf("BEGIN <%s>\n", #x); BEGIN x; }
#else
  #define RETURN(x) return x;
  #define DBEGIN(x) BEGIN x;
#endif
%}

%s DATES FREELINE CARDIO ENAME SETS

%%
- { RETURN(DASH); }
to { RETURN(TO); }
: { RETURN(COLON); }
x { RETURN(BY); }
\+ { RETURN(PLUS); }
AM|PM { yylval.string = yytext; RETURN(AMPM); }

[0-9]+\.[0-9]+ { yylval.dval = atof(yytext); RETURN(DOUBLE); }
<DATES>^[0-9]+/- { line = 1; yylval.ival = atoi(yytext); RETURN(INTEGER); }
[0-9]+ { yylval.ival = atoi(yytext); RETURN(INTEGER); }

<SETS>\([^\)]+\) {
	strncpy(ptext_buf, yytext + 1, PTEXT_BUF_SIZE);
	if (yyleng - 2 < PTEXT_BUF_SIZE)
	 ptext_buf[yyleng - 2] = 0;
	else
	 ptext_buf[PTEXT_BUF_SIZE - 1] = 0;
	yylval.string = ptext_buf;
	RETURN(PTEXT);
}

<ENAME>\n           DBEGIN(SETS);
<SETS>\n/[\n\ \t]*[0-9]{4}-[0-9]{2}-[0-9]{2}	{ DBEGIN(DATES); RETURN(EOW); }
<SETS>\n            DBEGIN(ENAME);

<CARDIO>^(None|No\ cardio)[^\n]*/\n { RETURN(NO); }
<FREELINE,CARDIO,ENAME>[^\n]+/\n { yylval.string = yytext; RETURN(LINE); } 

^[^0-9\n][^\n]*/\n { yylval.string = yytext; RETURN(LINE); }

[\ \t]+    ;
\n         { switch(line) {
						   case 2: DBEGIN(FREELINE); break;
               case 3: DBEGIN(CARDIO); break;
               case 4: DBEGIN(FREELINE); break;
               case 6: DBEGIN(ENAME); break;
             }
             line += 1; }
%%
