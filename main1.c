#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"
#include "cstack.h"
#include <string.h>

#define OPERAND 	1
#define OPERATOR 	2
#define INVALID	3
#define BRACKET 	10

typedef struct token {
	int type; 
	union data {
		int num;
		char operator;
	}d;
}token;

enum states {SPC, NUM, OPR, END, ERROR, BRACKO, BRACKC};

token *getnext(char *string, int *reset) {
	static int currstate, nextstate;
	static int i;
	int sum, done = 0;	
	token *p = (token *)malloc(sizeof(token));
	if(p == NULL)
		return NULL;
	if(*reset == 1) {
		*reset = 0;
		currstate = SPC;
		i = 0;
	}
	while(1) {
		switch(string[i]) {
			case '1':
			case '2': 		
			case '3': 
			case '4':
			case '5': 
			case '6': 
			case '7': 
			case '8':
			case '9': 
			case '0': 
				nextstate = NUM;
				break;
			case '+': 	
			case '-': 
			case '*': 
			case '/':
			case '%':
				nextstate = OPR;
				break;
			case '(':
				nextstate = BRACKO;
				break;
			case ')':
				nextstate = BRACKC;
				break;
			case '\0':
				nextstate = END;
				break;
			case ' ':
				nextstate = SPC;
				break;
			default:
				nextstate = ERROR;
				break;
		}
		switch(currstate) {
			case SPC:
				switch(nextstate){
					case SPC:
						break;
					case NUM:
						sum = string[i] - '0';	
						break;
					case OPR:
						break;
					case BRACKO:
						break;
					case BRACKC:
						nextstate = END;
						break;
					case END:
						break;
					case ERROR:
						break;
					default:
						break;
				}
				break;
			case NUM:
				switch(nextstate){
					case SPC:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case NUM:
						sum = sum * 10 + string[i] - '0';
						break;
					case OPR:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case BRACKO:
						nextstate = ERROR;
						break;
					case BRACKC:
						p->type = OPERAND;
						p->d.num = sum;
						nextstate = END;
						done = 1;
						break;
					case END:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					case ERROR:
						p->type = OPERAND;
						p->d.num = sum;
						done = 1;
						break;
					default:
						break;
				}
				break;
			case OPR:
				switch(nextstate){
					case SPC:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case BRACKO:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case BRACKC:
						nextstate = ERROR;
						break;
					case NUM:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						sum = string[i] - '0';
						done = 1;
						break;
					case OPR:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case END:
						p->type = OPERATOR;
						p->d.operator = string[i - 1];
						done = 1;
						break;
					case ERROR:
						p->type = OPERATOR;
						p->d.operator = string[i];
						done = 1;
						break;
					default:
						break;
						
				}
				break;
			case END:
				nextstate = END;
				done = 1;
				p = NULL;
				break;
			case ERROR:
				nextstate = ERROR;
				p->type = INVALID;
				done = 1;
				break;
			default:
				break;
		}
		currstate = nextstate;
		i++;
		if(done == 1)
			return p;
	}
	return NULL;	
}


int postfix(char *string) {
	stack s;
	init(&s);
	token *token;
	int reset = 1;
	int x, y, z;
	while((token = getnext(string, &reset))) {
		if(token->type == OPERAND) {
			push(&s, token->d.num); 
		}
		else if (token->type == OPERATOR) { 
			if(!empty(&s))
				x = pop(&s);
			else {
				fprintf(stderr, "Less operands\n");
				return INT_MIN;	
			}
			if(!empty(&s))
				y = pop(&s);
			else {
				fprintf(stderr, "Less operands\n");
				return INT_MIN;	
			}
			switch(token->d.operator) {
				case '+':
					z = y + x;
					break;
				case '-':
					z = y - x;
					break;
				case '*':
					z = y * x;
					break;
				case '/':
					z = y / x;
					break;
				case '%':
					z = y % x;
					break;
				default:
					return INT_MIN;
					break;
			} 
			push(&s, z);
		}
		else if(token->type == INVALID)
			return INT_MIN;
		free(token);	
	}
	if(!empty(&s)) {
		x = pop(&s);
		if(empty(&s))	
			return x;	
		else {
			fprintf(stderr, "More operands\n");
			return INT_MIN;
		}
	}
	else {
		fprintf(stderr, "Less operands xyz\n");
		return INT_MIN;
	}
} 


char ctop(cstack *cs) {
	char x ;
	x = cpop(cs);
	cpush(cs, x);
	return x;
}

int precedence(char op) {
	if(op == '*' || op == '/')
		return 30;
	if(op == '+' || op == '-')
		return 20;
	if(op == '%')
		return 10;
	return 0;
}

char *intopost(char *infix) {
	char *post = malloc(128);
	char temp[16];
	char x, prevop;
	int reset = 1, i = 0;
	token *t;
	cstack cs;
	cinit(&cs);
	strcpy(post, "");
	while (t = getnext(infix, &reset)) {
		printf("%d\n", t->type);
		if(t->type == OPERAND) {
			sprintf(temp, "%d", t->d.num);
			strcat(post, temp);
			strcat(post, " ");
		} 
		else if(t->type == OPERATOR) {
			prevop = ctop(&cs);
			while(precedence(t->d.operator) < precedence(prevop)) { 			
				if(!cempty(&cs))				
				x = cpop(&cs);
				temp[0] = x; 	
				temp[1] = ' '; 
				temp[2] = '\0';
				strcat(post, temp);
			}
			cpush(&cs, t->d.operator);	
		} 	
		else {
			printf("infix: error\n");
			return NULL;
		}
		
	}
	while(!cempty(&cs)) {
		x = cpop(&cs);
		temp[0] = x; 
		temp[1] = ' '; 
		temp[2] = '\0';
		strcat(post, temp);
	}
	return post;
}

int main() {
	int i = 0, size = 1024,y= 0, m= 0;
	char *p;
	char *str ="";
		while(1) {
		i = 0;
		size = 1024;
		printf("For infix evaluation select 1\n");
		printf("For postfix evaluation select 2\n");
		scanf("%d" ,&m);
		if(m == 1){ 
			printf("Enter the infix string\n");
			str = (char *)malloc(size * sizeof(char));
			scanf(" %[^\n]s",str);
			p= intopost(str);
			if(p) {
				printf("postfix: %s\n" , p);
				y = postfix(p);
			}
			printf("%d\n", y);
		}
		if(m == 2) {
			printf("Enter the postfix string\n");
			str = (char *)malloc(size * sizeof(char));
			scanf(" %[^\n]s",str);
			printf("%s\n", str);
			y = postfix(str);
			printf("%d\n", y);
		
		}
	
	}	return 0;
}	
