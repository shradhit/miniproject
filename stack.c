#include "stack.h"

void init(stack *s) {
	s->i = 0;
}
void push(stack *s, int x) {
	s->a[s->i] = x;
	(s->i)++;
}
/* the caller must check for empty before calling */
int pop(stack *s) {
	(s->i)--;	
	return s->a[s->i];
}
int empty(stack *s) {
	return s->i == 0;
}
int full(stack *s) {
	return s->i == SIZE - 1;
}
int elements(stack*s){
	return s->i;
}

int view(stack *s) {
	int t;	
	s->i -= 1;
	t = s->a[s->i];
	s->i += 1;	
	return t;
}


