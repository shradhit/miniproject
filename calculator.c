#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"
#include "cstack.h"
#include <string.h>
#include <math.h>

#define OPERAND 	1
#define OPERATOR 	2
#define INVALID	3
#define BRACKET 	10
#define PI 3.14159265

void add();                                          //function declarations which are to be used
void subtract();
void multiply();
void linearequation();
void determinant();
void inverse();
void eigenvalue();
void matrix();
int fact(int z);

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

char *pretopost(char *prefix) {
	char *post = malloc(128);
	char temp[16];
	char x, prevop;
	int reset = 1, i = 0;
	token *t;
	cstack cs;
	cinit(&cs);
	strcpy(post, "");
	while (t = getnext(prefix, &reset)) {
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
			printf("prefix: error\n");
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

void bc(void)
{
   char base_digits[16] =
	 {'0', '1', '2', '3', '4', '5', '6', '7',
	  '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

   int converted_number[64];
   long int number_to_convert;
   int next_digit, base, index=0;

   /* get the number and base */
   printf("Enter number and desired base: ");
   scanf("%ld %i", &number_to_convert, &base);

   /* convert to the indicated base */
   while (number_to_convert != 0)
   {
	 converted_number[index] = number_to_convert % base;
	 number_to_convert = number_to_convert / base;
	 ++index;
   }

   /* now print the result in reverse order */
   --index;  /* back up to last entry in the array */
   printf("\n\nConverted Number = ");
   for(  ; index>=0; index--) /* go backward through array */
   {
	 printf("%c", base_digits[converted_number[index]]);
   }
   printf("\n");
}

void matrix() {
	float A[100][100],B[100][100],C[100][100];      //Variables declarations
	int row1,column1,row2,column2,k,i,j;
	char choice[1],choose[1];
    		//LABEL for GOTO STATEMENT
    MAT:{
	AD1: printf("\n Welcome to advanced calculation\n");                  //MENU of OPERATIONS
	printf(" U CAN PERFORM:\n");
	printf("1:ADDITION\t");
	printf("2:SUBTRACTION\t");
	printf("3:MULTIPLICATION\t");
	printf("4:LINEAR EQUATION\n");
	printf("5:DETERMINANT\t");
	printf("6:EIGEN VALUE\t");
	printf("\n Enter your choice");
	scanf("%s", choice);                                  //ENTER YOUR CHOICE
	if(choice[0]=='1') {                         //depending upon your choice respective functions will be called
		add();
	}                                 //function call for add
	if(choice[0]=='2') {
		subtract();
	}                              //function call for subtract
	if(choice[0]=='3') {
		multiply();
	}                              //function call for multiply
	if(choice[0]=='4') {
		linearequation();
	}                         //function call for linear equations
	if(choice[0]=='5') {
		determinant();
	}
	if(choice[0]=='6') {
		eigenvalue();
	}
	printf("\n");

	//Convert this label-goto into a do while loop
 	MAT1:{
		printf("CONTINUE(Y/N)\n");
		scanf("%s", choose);
		if(choose[0]=='Y'){
	    	system("clear");
	    	goto MAT;
		}
		if(choose[0]=='N')
    		return ;
		else {
		printf("ERR\n");
		goto MAT1;
		}
    }	
	}
return;
}


void add() {      //enter the rowa and column of the matrix 1 and 2
	float A[100][100],B[100][100],C[100][100];      //Variables declarations
	int row1,column1,row2,column2,k,i,j;
	char choice[1],choose[1];
	printf("\n order 1st matrix\n");
	scanf("%d", &row1);
	scanf("%d", &column1);
	printf("\norder 2 matrix\n");
	scanf("%d", &row2);
	scanf("%d", &column2);
        	if((row1==row2)&&(column1==column2)) {    //checking condition for addition
			printf("\nEnter matrix1\n");
			for( i=0;i<row1;i++) {
           			for( j=0;j<column1;j++) {
               				scanf("%f\t",&A[i][j]);
              		     		printf("\n");
              	 		}
               			printf("\n\n");
           		}          //printing matrix 1
	           	for( i=0;i<row1;i++) {
           			for( j=0;j<column1;j++) {
              				printf("%f\t",A[i][j]);
				}
           	    		printf("\n");
           		}              //enter values of matrix 2
           	printf("\nEnter matrix 2\n");
           	for( i=0;i<row1;i++) {
               		for( j=0;j<column1;j++) {
                		   	scanf("%f\t",&B[i][j]);
                   			printf("\n");
               		}
               		printf("\n\n");
          	}//printing matrix 2
           printf("\n");
           for(i=0;i<row1;i++){
               	for(j=0;j<column1;j++){
                   		printf("%f\t",B[i][j]);
		}
               	printf("\n");
           }//calculating addition of matrix
           for(i=0;i<row1;i++){
               	for(j=0;j<column1;j++){
                		C[i][j]=A[i][j]+B[i][j];
		}
           }//printing the result
           printf("\n\n RESULT\n");
           for(i=0;i<row1;i++) {
               		for(j=0;j<column1;j++){
                   			printf("%f",C[i][j]);
			}
               printf("\n");
           }
       }//if the condition is not satisfied then print an error message
           else
        		printf("ERROR");
	 return;            //return

}
void subtract(){//enering oder for matrix 1 and 2
   	float A[100][100],B[100][100],C[100][100];      //Variables declarations
    	int row1,column1,row2,column2,k,i,j;
   	printf("\norder 1st matrix\n");
    	scanf("%d", &row1);
    	scanf("%d", &column1);
    	printf("\n order 2nd matrix\n");
    	scanf("%d %d", &row2 ,&column2);
       	//checking condition for subtraction
       	if((row1 == row2) && (column1 == column2 )) {
           	printf("\nEnter matrix 1\n");
           //entering matrix 1
          for( i=0; i < row1; i++){
               	for(j=0; j < column1 ; j++){
                   		scanf("%f", &A[i][j]);
                   		printf("\n");
               	}
           }//printing matrix 1
          for(i=0; i < row1; i++){
               	for(j=0;j<column1;j++){
                   		printf("%f", A[i][j]);
		}
          	printf("\n");
          }//enter matrix 2
          printf("\nEnter matrix 2\n");
          for(i=0;i<row1;i++){
               	for(j=0;j<column1;j++){
                   		scanf("%f", &B[i][j]);
                   		printf("\n");
               	}
               	printf("\n\n");
           }//printing matrix 2
           for(i=0;i<row1;i++){
               	for(j=0;j<column1;j++){
                   		printf("%f", B[i][j]);
		}
           	printf("\n");
           }//calculating subtraction
           for(i=0;i<row1;i++){
               	for(j=0;j<column1;j++){
                		C[i][j]=A[i][j] - B[i][j];
		}
           }//printing result of subtraction
           printf("Result\n");
           for( i=0;i<row1;i++){
               	for( j=0;j<column1;j++){
                   		printf("%f", C[i][j]);
		}
           	printf("\n");
           }
      }//if the condition fails then printing an error message
           else
           	 printf("ERROR");

}
void multiply(){  //enter the order of matrix 1 and matrix 2
    	float A[100][100],B[100][100],C[100][100];      //Variables declarations
    	int row1,column1,row2,column2,k,i,j;
    	char choice[1],choose[1];
    	printf("\n order 1st matrix\n");
    	scanf("%d", &row1);
    	scanf("%d", &column1);
    	printf("\norder 2nd matrix\n");
    	scanf("%d", &row2);
   	scanf("%d", &column2);
    	printf("\n");
    	//entering matrix 1
    	printf("Enter Matrix 1:\n");
    	for(i=0;i<row1;i++){
        		for(j=0;j<column1;j++){
		            scanf("%f",&A[i][j]);
		}
        		printf("\n");
    	}
    	//entering matrix 2
    	printf("Enter matrix 2:");
    	printf("\n");
    	for(i=0;i<row2;i++)	{
        		for(j=0;j<column2;j++){
            		scanf("%f", &B[i][j]);
		}
        		printf("\n");
    	}
    	//printing matrix 1

    	printf("\n");
    	printf("Matrix 1 is");
    	printf("\n");
    	for(i=0;i<row1;i++) {
        		for(j=0;j<column1;j++){
         		       printf("%f\t",A[i][j]);
         		}
     	   	printf("\n");
    	}
//printing matrix 2

  	printf("\n");
   	printf("Matrix 2 is:");
  	printf("\n");

    	for(i=0;i<row2;i++){
        		for(j=0;j<column2;j++){
            		printf("%f\t",B[i][j]);
     		   }
      	  	printf("\n");
    	}
    //performing multiplication
     	for(i=0;i<row1;i++){
        		for(j=0;j<column2;j++){   
			C[i][j]=0;
          for(k=0;k<row2;k++){
                	C[i][j]+=A[i][k]*B[k][i];
            }
        }
    }

//printing the result
    	printf("RESULT:");
    	printf("\n");
    	for(i=0;i<row1;i++){
        		for(j=0;j<column2;j++){
           		 printf("%f\t",C[i][j]);
        		}
        	printf("\n");
    	}
}

void linearequation() {                            //function definition for linear equation
	int i, j, k, n;
	float MatA[100][100], MatB[100], X[100];
	float Divisor, Factor, sum;
	printf("Enter variables?\n");
	scanf("%d", &n);
	//reading matrix A
	printf("ENTER COFFICIENTS OF  EQ1 2 &3\n");
	for(i=0; i< n; i++){
		for(j=0; j < n; j++){
			scanf("%f", &MatA[i][j]);
		}
		printf("\n");
	}
	//reading matrix B
	printf("\nENTER VALUES OF EQ 1 2 & 3\n");
	for(i=0; i< n; i++){
		scanf("%f", &MatB[i]);
	}
	//Gauss elimination
	for (i=0; i< n; i++){
		Divisor = MatA[i][i];
		MatA[i][i] = 1.0;
	// divide all values in the row by the divisor
	// to recalculate all coefficients in that row
	for (j = i+1; j < n; j++){
		MatA[i][j] = MatA[i][j]/Divisor;
		}
	//Also divide the corresponding RHS element
	MatB[i] = MatB[i]/Divisor;
	// now replace subsequent rows, by subtracting the
	// appropriate portion of the ithequation from it
	if (i+1 < n) {
		for (k=i+1; k<n; k++){
		Factor = MatA[k][i];
		MatA[k][i] = 0.0;
		for (j = i+1; j < n; j++){
			MatA[k][j] = MatA[k][j] -Factor * MatA[i][j];
			}
		MatB[k] = MatB[k] -Factor * MatB[i];
		}
	}
}
	// back substitution starting with last variable
	X[n-1] = MatB[n-1];
	for (i= n-2; i>=0; i--){
	// Sum up ithrow using values of X already determined
	sum = 0.0;
	for (j = i+1; j < n; j++){
		sum = sum + MatA[i][j] * X[j];
	}
	X[i] = MatB[i] -sum;
	}

	for (i=0; i<n; i++){
		printf("X[%d] is %f\n",i,X[i]) ;
	}
	return;
}

void determinant(){
	int A[100][100],n =0 ,result,i,j;
	printf("\nENTER ORDER\n");
    	scanf("%d", &n);
    	printf("\nENTER ELEMENTS\n");
    	for(i = 0; i < n; i++){
        		for(j = 0;j < n; j++){
          	 	scanf("%d\t", &A[i][j]);
		}
    	    	printf("\n");
    	}
   	printf("MATRIX\n");
   	for(i = 0; i < n; i++){
       		for(j = 0; j < n; j++){
 			printf("%d\t", A[i][j]);
		}
        		printf("\n");
    	}
    	result   = det( A , n);
    	printf("ANSWER == %d",result);
	return;
 }
int det(int A[100][100],int n){
     	int sum=0,C[100][100];
     	if(n=2){
         		sum = A[0][0] * A[1][1] - A[1][0] * A[0][1];
         		return sum;
     	}
     	int i,j,m,h,k;
     	 for(m = 0 ; m < n ; m++){
         		h = 0;k = 0;
         		for(i = 1;j < n;j++){
          		for(j = 0;k < n; k++){
         			     	if(k = m){
              				continue;
				}
         			 C[h][k] = A[i][j];
              		 k++;
          		 if(k == n-1){
                   			h++;
                  			k=0;
           		}
         		 }
          }
         	sum=sum+A[0][m]*pow(-1,m)*det(C,n-1);
      	}
      return sum;
}	

void eigenvalue() {
    	float A[2][2], b, c, Z1, Y1, Y2, Z11;
   	int i, j, k, n;
    	printf("ENTER ORDER\n");
    	printf("2X2\n");
	printf("ENTER MATRIX\n");
    	for(i=0;i<2;i++){
        		for(j=0;j<2;j++){
            		scanf("%f", &A[i][j]);
        		}
        		printf("\n");
    	}
   	printf("MATRIX IS:\n");
    	for(i=0;i<2;i++){
        		for(j=0;j<2;j++){
            		printf("%f\t",A[i][j]);
        		}
        		printf("\n");
    	}
    	b = ( A[0][0] + A[1][1]);
    	c = ( A[0][0] * A[1][1] - A[0][1] * A[1][0] );
    	Z1 = (b * b) - (4 * c);
	Z11 = pow(Z1,(1/2));
	printf("\n");
    	if(Z1 >= 0) {
   		Y1 = ( -b + Z11) /2;
        		Y2 = ( -b - Z11) /2;
        		printf(" = %f \t %f ",Y1,Y2);
    	}
    	if(Z1<0){
        		printf("\nIMAGINARY ROOTS\n");
        		printf("ERR TRY AGAIN");
        		printf("\n");
    	}
	return;
}

void ncr(){
	int n, r, ncr;
	printf("\n Enter the value for N and R \n");
	scanf("%d%d", &n, &r);
	ncr = fact(n) / (fact(r) * fact(n - r));
	printf("\n nCr = %d", ncr);
}

int fact(int z){
	int f = 1, i;
	if (z == 0){
		return(f);
	}

        	else{
		for (i = 1; i <= z; i++){
			f = f * i;
		}
	}
	return(f);
}

void ap(){
	int a, d, n, i, tn;
	int sum = 0;
	printf("Enter the first term value of the A.P. series: ");
	scanf("%d", &a);
	printf("Enter the total numbers in the A.P. series: ");
	scanf("%d", &n);
     	printf("Enter the common difference of A.P. series: ");
	scanf("%d", &d);
	sum = (n * (2 * a + (n - 1)* d ))/ 2;
	tn = a + (n - 1) * d;
	printf("Sum of the A.P series is: ");
	for (i = a; i <= tn; i = i + d ){
		 if (i != tn)
			printf("%d + ", i);

          	 else
			printf("%d = %d ", i, sum);
	}
	return;
}

void hp(){
	int n;
	float i, sum, term;
	printf("1 + 1 / 2 + 1 / 3 +......+1 / n \n");
	printf("Enter the value of n \n");
	scanf("%d", &n);
	sum = 0;
	for (i = 1; i <= n; i++){
		term = 1 / i;
		sum = sum + term;
		}
	printf("the Sum of H.P Series is = %f", sum);
}

int arithmetic() {
	int i;
	printf("\n1)nCR 2) SUM OF AP 3) SUM OF HP\n");
	scanf("%d",&i);
	if(i == 1){
		ncr();
	}
	if(i == 2) {
		ap();
	}
	if(i == 3) {
		hp();
	}
}
		
int main() {
	int i = 0, size = 1024,y= 0, m= 0, choice = 0;
	char *p;
	char *str ="";
		while(1) {
		i = 0;
		size = 1024;
		system("clear");
		printf("\nWELCOME TO ADVANCED CALCULATION\n");
    		printf("\nOPERATIONS CAN BR PERFORMED\n\n");
		printf("\n1)INFIX/ POSTFIX CALCULATOR\n");
		printf("\n2)BASE CONVERSION CALCULATOR\n");
		printf("\n3)MATRIX CALCULATOR\n");
		printf("\n4) ARITHMATIC CALCULATOR");
		printf("\nENTER YOUR CHOICE:\n");
		scanf("%d" ,&choice);
		if(choice == 1) {
			printf("1) FOR INFIX\n");
			printf("2) FOR POSTFIX\n");
			scanf("%d", &m);
			if(m == 1){ 
				printf("\nENTER INFIX STRING\n");
				str = (char *)malloc(size * sizeof(char));
				scanf(" %[^\n]s",str);
				p= intopost(str);
				if(p) {
					y = postfix(p);
				}
				printf("%d\n", y);
			}
			if(m == 2) {
				printf("ENTER POSTFIX STRING\n");
				str = (char *)malloc(size * sizeof(char));
				scanf(" %[^\n]s",str);
				printf("%s\n", str);
				y = postfix(str);
				printf("%d\n", y);
				}
			}
		if(choice == 2) {
			printf("\n -------->>>>>>>>BASE CONVERSION CALCULATOR<<<<<<<-------\n");
			bc();
		}			
                    if(choice == 3) {
			printf("\n------->>>>>>MATRIX<<<<<<-------\n");
			matrix();
		}
		if(choice == 4) {
			printf("\n------->>>>>>ARTHMETIC <<<<<<-------\n");
			arithmetic();
				
		}
	}	
	return 0;
}	
