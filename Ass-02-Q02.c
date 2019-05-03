#include "Ass-02.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <time.h>

int isOperator(char );
int isFunction(char );
void mathParser(void);
float evaluatePostfix(char** queue, int qlen);
int associative(char token);
int greaterPrecedence(char first, char second);

#define MAX_MATH_STR 30				//This is the maximum number of characters that an expression may have in calculator.
#define MAX_DISP_STR 13				//This is the maximum number of characters that may be displayed on the screen at any time.

// ================ MATH PARSING SECTION =======================
#define qpush(x) queue[qlen++] = x
#define spush(x) stack[slen++] = x
#define spop() stack[--slen]

// SHUNTING-YARD ALGORITHM
//This function will parse the math expression and return the result OR an error if the expression made no sense.
void mathParser(void)
{
	char* stack[100];//malloc
	char* queue[100];//malloc
	int qlen = 0;
	int slen = 0;

	printf("math string is : ");
	for (int j = 0; j != '0'; j++)
	printf("%c", mathString[j]);
	printf("-end\n");

	/* 	while there are tokens to be read: */
	for (int j = 0; j < strlen(mathString); j++)
	{
/*     read a token. */
		char* token = (char*)malloc(sizeof(char)*10);
		token[0] = mathString[j];
		token[1] = '\0';
		printf("%dReads token '%s'", j, token);
		printf("	Action: ");
/*     if the token is a number, then: */
		if (isdigit(token[0]))
		{
			int len = 0;
			token[len++] = token[0];
			// read til end of number
			while (isdigit(mathString[j+1]) || mathString[j+1] == '.')
			{
			#define qpush(x) queue[qlen++] = x
				j++;
				token[len++] = mathString[j];
			}
			token[len] = '\0';
			printf("num is %s\n", token);
			printf("Push it to the output queue\n");
			/*         push it to the output queue. */
			qpush(token);

		}
		/*     if the token is a function then: */
		if (isFunction(token[0]))
		{
			printf("Push it to the operator stack\n");
			/*         push it onto the operator stack  */
			spush(token);
		}
		/*     if the token is an operator, then: */
		if (isOperator(token[0]))
		{
			/*         while ((there is a function at the top of the operator stack) */
			while ((slen > 0 && isFunction(stack[slen-1][0]))

					/*                or (there is an operator at the top of the operator stack with greater precedence) */
					|| (slen > 0 && isOperator(stack[slen-1][0]) && greaterPrecedence(stack[slen-1][0], token[0]) == -1)

					/*                or (the operator at the top of the operator stack has equal precedence and is left associative)) */
					|| ((slen > 0 && isOperator(stack[slen-1][0]) && greaterPrecedence(stack[slen-1][0], token[0]) == 0 && associative(stack[slen-1][0]) == 0)

						/*               and (the operator at the top of the operator stack is not a left parenthesis): */
						&& stack[slen-1][0] != '('))
			{
				printf("ENTER?\n");
				/*             pop operators from the operator stack onto the output queue. */
				printf("Pop stack to output\n");
				qpush(spop());
			}
			/*         push it onto the operator stack. */
			printf("Push it to the operator stack\n");
			spush(token);
		}
		/*     if the token is a left paren (i.e. "("), then: */
		if (token[0] == '(')
			/*         push it onto the operator stack. */
		{
			printf("Push it to the operator stack\n");
			spush(token);
		}
		/*     if the token is a right paren (i.e. ")"), then: */
		if (token[0] == ')')
		{
			/*         while the operator at the top of the operator stack is not a left paren: */
			while (slen > 0 && stack[slen-1][0] != '(')
			{
				printf("Pop stack to output\n");

				/*             pop the operator from the operator stack onto the output queue. */
				qpush(spop());
				/*         #<{(| if the stack runs out without finding a left paren, then there are mismatched parentheses. |)}># */
			}

			/*         if there is a left paren at the top of the operator stack, then: */
			if (stack[slen-1][0] == '(')
			{
				printf("Pop stack\n");
				/*             pop the operator from the operator stack and discard it */
				spop();
			}
		}
	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
		printf("%s ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)

		printf("%s ", stack[a]);
	printf("\n\n");
	}
	// end



	// after loop, if operator stack not null, pop everything to output queue
	// if there are no more tokens to read:
	//     while there are still operator tokens on the stack:
	while (slen > 0)
	{
		/*         #<{(| if the operator token on the top of the stack is a paren, then there are mismatched parentheses. |)}># */
		if (stack[slen-1][0] == '(' || stack[slen-1][0] == ')')
			printf("mismatched parenthesis\n");
		/*         pop the operator from the operator stack onto the output queue. */
		qpush(spop();)

	}
	/* exit. */

	// print final contents of stack and queue
	printf("Output queue: ");
	for (int a = 0; a < qlen; a++)
			printf("%s ", queue[a]);
	printf("	Operator stack: ");
	for (int a = 0; a < slen; a++)
			printf("%s ", stack[a]);
	printf("\n");


	//Getting the solution.
	result = evaluatePostfix(queue, qlen);	// evaluate postfix value read from stack.
	sprintf(&mathString, "%.2f", result);		//Store the reult in the string. Null terminate that badboi
	sprintf(&ans, "%.2f", result);				//Store the reult in the string. Null terminate that badboi
	printf("Result: %g\n", result);
	printf("amount is %s\n", mathString);
	printf("ans is: %s\n",ans);
}



void equalsPressed(){
	int diff = 0; 								//For calculating cursor position.
	mathParser();								//This has replaced the mathString with the float.
	mathStrLen = strlen(mathString);			//Update length of the string.

	diff = mathStrLen - MAX_DISP_STR;			//Get difference.

	BSP_LCD_SetTextColor (0xFFFF);				//Text Color: White.
	BSP_LCD_FillRect (81, 0, 239, 46);			//Write over screen.
	BSP_LCD_SetTextColor (LCD_COLOR_BLACK);		//Text Color: Black.

	if(diff <= 0){								//String length is less than screen.
		cursorPos = (mathStrLen-1);				//-1 because strLen is not zero based.
	}else{										//String is too long for screen so...
		cursorPos = 12;
	}

	printCursor(cursorPos);

	char showString[MAX_DISP_STR + 1];
	//Performs the assignment for the string to be displayed.
	for(int j = 0; j<MAX_DISP_STR; j++){
		showString[j] = *(mathString+j);
	}

	showString[MAX_DISP_STR] = '\0';

	BSP_LCD_DisplayStringAt(85,13,showString,LEFT_MODE);	//Display result on screen.
}

/* Insert ANS into equation. */
void ansPressed(){
	int i = 0;
	char* num = ans[0];
	*(num+1) = '\0';

	while(num != '\0'){
		updateDisplay(&num);
		i++;
		*(num) = ans[i];
		*(num+1) = '\0';
	}
}
