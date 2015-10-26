#include "cstack.h"
void cinit(cstack *s) {
	s->i = 0;
}
void cpush(cstack *s, char x) {
	s->a[s->i] = x;
	(s->i)++;
}
/* the caller must check for empty before calling */
char cpop(cstack *s) {
	(s->i)--;	
	return s->a[s->i];
}
int cempty(cstack *s) {
	return s->i == 0;
}
int cfull(cstack *s) {
	return s->i == SIZE - 1;
}
char cview(cstack *s) {
	char t;	
	s->i -= 1;
	t = s->a[s->i];
	s->i += 1;
	return t; 
}

int celements(cstack *s) {
	return s->i;
}
