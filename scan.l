%{
#include "y.tab.c"

static int workout_line;
int wk_lex_line = 1;

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

%s DATES FREELINE CARDIO EXERCISE SETS

%%
- { RETURN(DASH); }
to { RETURN(TO); }
: { RETURN(COLON); }
x { RETURN(BY); }
\+ { RETURN(PLUS); }
AM { RETURN(AM); }
PM { RETURN(PM); }

[0-9]+\.[0-9]+ { yylval.dval = atof(yytext); RETURN(DOUBLE); }
[0-9]+ { yylval.ival = atoi(yytext); RETURN(INTEGER); }
<INITIAL,DATES>^[0-9]+/- { workout_line = 1; yylval.ival = atoi(yytext); RETURN(INTEGER); }

<SETS>\([^\)]+\) {
	strncpy(ptext_buf, yytext + 1, PTEXT_BUF_SIZE);
	if (yyleng - 2 < PTEXT_BUF_SIZE)
	 ptext_buf[yyleng - 2] = 0;
	else
	 ptext_buf[PTEXT_BUF_SIZE - 1] = 0;
	yylval.string = ptext_buf;
	RETURN(PTEXT);
}

<SETS>""/[\n\ \t]*[0-9]{4}-[0-9]{2}-[0-9]{2}	{ DBEGIN(DATES); RETURN(EOW); }
<CARDIO>^(None|No\ cardio)[^\n]*/\n { RETURN(NO); }
<FREELINE,CARDIO,EXERCISE>[^\n]+/\n { yylval.string = yytext; RETURN(LINE); } 

<EXERCISE>\n   { wk_lex_line++; DBEGIN(SETS); }
<SETS>\n    { wk_lex_line++; DBEGIN(EXERCISE); }
\n {
	wk_lex_line++;

	switch(workout_line) {
		case 2: DBEGIN(FREELINE); break;
		case 3: DBEGIN(CARDIO); break;
		case 4: DBEGIN(FREELINE); break;
		case 6: DBEGIN(EXERCISE); break;
	}
	workout_line++;
}

[\ \t]+		;
%%
