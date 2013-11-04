int yylex(void);

int main(int argc, const char** argv) {
	while (yylex() != 0) {}
}
