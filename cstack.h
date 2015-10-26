#define SIZE 128
typedef struct cstack{
	char a[SIZE];
	int i;
}cstack;
void cinit(cstack *s);
void cpush(cstack *s, char x);
char cpop(cstack *s);
int cempty(cstack *s);
int cfull(cstack *s);
char cview(cstack *s);
int celements(cstack *s);
