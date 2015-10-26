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


/*void iinit (istack *a) {
	a->i = 0;
	return;
}
void ipush(istack *a, int n) {
	a->arr[a->i] = n;
	a->i += 1;	
	return;
}
int ipop(istack *a) {
	a->i -= 1;
	return a->arr[a->i];
}
//int ielements(istack *a) {
//	return a->i;
//}
int iempty(istack *a) {
	return a->i == 0;
}
//int iview(istack *a) {
//	int t;	
//	a->i -= 1;
//	t = a->arr[a->i];
//	a->i += 1;	
//	return t;
//}
int ifull(istack *a) {
	return a->i == SIZE;
}
char cview(cstack *a) {
	char t;	
	a->i -= 1;
	t = a->arr[a->i];
	a->i += 1;
	return t; 
}
int celements(cstack *a) {
	return a->i;
}
void cinit(cstack *a) {
	a->i = 0;
	return;
}
void cpush(cstack *a, char n) {
	a->arr[a->i] = n;
	a->i += 1;	
	return;
}
char cpop(cstack *a) {
	a->i -= 1;
	return a->arr[a->i];
}
int cempty(cstack *a) {
	return a->i == 0;
}
int cfull(cstack *a) {
	return a->i == SIZE;
}
void dinit (dstack *a) {
	a->i = 0;
	return;
}
double dview(dstack *a) {
	double t;
	a->i -= 1;
	t = a->arr[a->i];
	a->i += 1;
	return t;
}
void dpush(dstack *a, double n) {
	a->arr[(a->i)++] = n;
	return;
}
int delements(dstack *a) {
	return a->i;
}
double dpop(dstack *a) {
	a->i -= 1;
	return a->arr[a->i];
}
int dempty(dstack *a) {
	return a->i == 0;
}
int dfull(dstack *a) {
	return a->i == SIZE;
} */
